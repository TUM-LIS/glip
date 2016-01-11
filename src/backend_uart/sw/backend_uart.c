/* Copyright (c) 2015-2016 by the author(s)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ============================================================================
 *
 * libglip is a flexible FIFO-based communication library between an FPGA and
 * a PC. This is the UART backend.
 *
 * Author(s):
 *   Stefan Wallentowitz <stefan@wallentowitz.de>
 */

#include "glip-protected.h"

#include "backend_uart.h"
#include <cbuf.h>
#include <util.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>

#include <sys/time.h>
#include <stdio.h>

static int speed_lookup(int speed);
static int check_timeout(struct timeval *start, unsigned long ms);
static int read_blocking(int fd, uint8_t *buffer, size_t size, size_t *size_read, unsigned int timeout);
static int write_blocking(int fd, uint8_t *buffer, size_t size, size_t *size_written, unsigned int timeout);
static int reset_logic(int fd, uint8_t state);

/**
 * Reset the communication logic
 *
 * Sets the communication logic register
 *
 * @param fd Terminal to use
 * @param state Value (binary) to set the register to
 * @return Always returns 0
 */
static int reset_com(int fd, uint8_t state);

static void update_debt(struct glip_backend_ctx* ctx, uint8_t first, uint8_t second);
static int send_credit(struct glip_backend_ctx* ctx, uint16_t tranche);

static void reset(struct glip_backend_ctx* ctx);

void* thread_func(void *arg);

/**
 * Parses an incoming buffer and filters credits
 *
 * Filter and process credits, write data otherwise
 *
 * @param ctx Context
 * @param buffer Buffer to parse
 * @param size Size of the buffer
 */
void parse_buffer(struct glip_backend_ctx *ctx, uint8_t *buffer,
		size_t size);

static const uint16_t UART_MAX_TRANCHE = 0x3fff;

/*! Temporary buffer size */
static const uint16_t TMP_BUFFER_SIZE = 256;

/**
 * GLIP backend context for the JTAG backend
 */
struct glip_backend_ctx {
	char *device;
	int fd;
	uint32_t speed;
	size_t debt;
	size_t credit;

	pthread_t thread;

	size_t buffer_size;

	struct cbuf *input_buffer;
	struct cbuf *output_buffer;

	// Communication for the reset
	volatile int reset_request;
};

/**
 * Initialize the backend (constructor)
 *
 * @param[in]  ctx the library context
 *
 * @return 0 if initializing the backend was successful
 * @return any other value indicates failure
 *
 * @see glip_new()
 */
int gb_uart_new(struct glip_ctx *ctx)
{
	struct glip_backend_ctx *c = calloc(1, sizeof(struct glip_backend_ctx));
	if (!c) {
		return -1;
	}

	ctx->backend_functions.open = gb_uart_open;
	ctx->backend_functions.close = gb_uart_close;
	ctx->backend_functions.logic_reset = gb_uart_logic_reset;
	ctx->backend_functions.read = gb_uart_read;
	ctx->backend_functions.read_b = gb_uart_read_b;
	ctx->backend_functions.write = gb_uart_write;
	ctx->backend_functions.write_b = gb_uart_write_b;
	ctx->backend_functions.get_fifo_width = gb_uart_get_fifo_width;
	ctx->backend_functions.get_channel_count = gb_uart_get_channel_count;

	ctx->backend_ctx = c;

	c->buffer_size = 32768;
	cbuf_init(&c->input_buffer, c->buffer_size);
	cbuf_init(&c->output_buffer, c->buffer_size);

	c->credit = c->buffer_size - 1;
	c->debt = 0;

	return 0;
}

/**
 * Open a target connection
 *
 * @param[in]  ctx the library context
 * @param[in]  num_channels the number of channels. Must be 1 for this backend.
 *
 * @return 0 if opening the connection was successful
 * @return any other value indicates failure
 *
 * @see glip_open()
 */
int gb_uart_open(struct glip_ctx *ctx, unsigned int num_channels)
{
	struct glip_backend_ctx *bctx = ctx->backend_ctx;

	if (num_channels != 1) {
		err(ctx, "Channel number must be 1!\n");
		return -1;
	}

	if (glip_option_get_char(ctx, "device", (const char**) &bctx->device) != 0) {
		bctx->device = "/dev/ttyUSB0";
	}

	if (glip_option_get_uint32(ctx, "speed", &bctx->speed) != 0) {
		bctx->speed = 115200;
	}

	bctx->fd = open (bctx->device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (bctx->fd < 0)
	{
		err(ctx, "Cannot open device %s\n", bctx->device);
		return -1;
	}

	int baud = speed_lookup(bctx->speed);
	if (baud == -1)
	{
		err(ctx, "Speed not known: %d. Custom baud rates are not supported currently\n", baud);
		return -1;
	}

	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (bctx->fd, &tty) != 0)
	{
		err(ctx, "Cannot get device attributes\n");
		return -1;
	}

	cfsetospeed (&tty, baud);
	cfsetispeed (&tty, baud);

	// 8N1
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag &= ~CSTOPB;
	// Hardware flow control
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag |= CRTSCTS;

	tty.c_cc[VMIN]  = 0; // read doesn't block
	tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

	// Raw
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	tty.c_iflag = 0;

	tty.c_oflag = 0;

	if (tcsetattr (bctx->fd, TCSANOW, &tty) != 0)
	{
		err(ctx, "Cannot set attributes\n");
		return -1;
	}

	int autodetect = 0;
	int autodetect_candidates[] = { 9600, 19200, 38400, 57600, 115200,
			230400, 500000, 1000000, 2000000, 3000000, 4000000, 0};
	int *autodetect_try;
	int success = 0;

	do {
		if (autodetect) {
			err(ctx, "Try speed: %d\n", *autodetect_try);

			memset (&tty, 0, sizeof tty);
			if (tcgetattr (bctx->fd, &tty) != 0)
			{
				err(ctx, "Cannot get device attributes\n");
				return -1;
			}

			bctx->speed = *autodetect_try;

			baud = speed_lookup(bctx->speed);
			cfsetospeed (&tty, baud);
			cfsetispeed (&tty, baud);

			if (tcsetattr (bctx->fd, TCSANOW, &tty) != 0)
			{
				err(ctx, "Cannot set attributes\n");
				return -1;
			}
		}

		int rv;
		rv = reset_com(bctx->fd, 1);

		usleep(1000);
		do {
			uint8_t buffer[128];
			rv = read(bctx->fd, buffer, 128);
		} while (rv > 0);

		rv = reset_com(bctx->fd, 0);

		// Read the credit (=debt), just to check read, will reset below
		uint8_t credit[2];
		size_t size_read;

		rv = read_blocking(bctx->fd, credit, 2, &size_read, 1000);
		if (rv == -ETIMEDOUT) {
			success = 0;
		} else if ((credit[0] != 0xfe) && (credit[1] != 0xfe)) {
			success = 0;
		} else {
			success = 1;
		}

		if (!success) {
			if (!autodetect) {
				err(ctx, "Given speed %d did not work with device. Try autodetect..\n", bctx->speed);
//				return -1;
				autodetect = 1;
				autodetect_try = autodetect_candidates;
				continue;
			} else {
				autodetect_try++;
				if (*autodetect_try == 0) {
					err(ctx, "Could not autodetect baud rate.\n");
					return -1;
				}
			}
		}
	} while (!success);

	reset(bctx);

	pthread_create(&bctx->thread, 0, thread_func, bctx);

	return 0;
}

/**
 * Close connection to the target
 *
 * @param[in]  ctx the library context
 *
 * @return 0 if closing the connection was successful
 * @return any other value indicates failure
 *
 * @see glip_close()
 */
int gb_uart_close(struct glip_ctx *ctx)
{
	int rv;
	struct glip_backend_ctx *bctx = ctx->backend_ctx;

	pthread_cancel(bctx->thread);
	pthread_join(bctx->thread, (void**)&rv);

	close(bctx->fd);

	return 0;
}

/**
 * Reset the logic on the target
 * *
 * @see glip_logic_reset()
 */
int gb_uart_logic_reset(struct glip_ctx *ctx)
{
	int rv;

	struct glip_backend_ctx *bctx = ctx->backend_ctx;

	assert(bctx->reset_request == 0);

	bctx->reset_request = 1;

	while (bctx->reset_request == 1) {}

	return 0;
}

/**
 * Read from the target device
 *
 * @param[in]  ctx the library context
 * @param[in]  channel the descriptor of the channel
 * @param[in]  size how much data is supposed to be read
 * @param[out] data the read data
 * @param[out] size_read how much data has been read
 *
 * @return 0 if reading was successful
 * @return any other value indicates failure
 *
 * @see glip_read()
 */
int gb_uart_read(struct glip_ctx *ctx, uint32_t channel, size_t size,
		uint8_t *data, size_t *size_read)
{
	assert(channel == 0);

    struct glip_backend_ctx* bctx = ctx->backend_ctx;

    size_t fill_level = cbuf_fill_level(bctx->input_buffer);
    size_t size_read_req = min(fill_level, size);

    int rv = cbuf_read(bctx->input_buffer, data, size_read_req);
    if (rv < 0) {
        err(ctx, "Unable to get data from read buffer, rv = %d\n", rv);
        return -1;
    }

    *size_read = size_read_req;

    return 0;
}

/**
 * Blocking read from the device
 *
 * @param[in]  ctx the library context
 * @param[in]  channel the descriptor of the channel
 * @param[in]  size how much data is supposed to be read
 * @param[out] data the read data
 * @param[out] size_read how much data has been read
 * @param[in]  timeout the maximum duration the read operation can take
 *
 * @return 0 if reading was successful
 * @return -ETIMEDOUT if the read timeout was hit
 * @return any other value indicates failure
 *
 * @see glip_read_b()
 */
int gb_uart_read_b(struct glip_ctx *ctx, uint32_t channel, size_t size,
		uint8_t *data, size_t *size_read, unsigned int timeout)
{
    int rv;
    struct glip_backend_ctx *bctx = ctx->backend_ctx;
    struct timespec ts;

    if (size > bctx->buffer_size) {
        /*
         * This is not a problem for non-blocking reads, but blocking reads will
         * block forever in this case as the maximum amount of data ever
         * available is limited by the buffer size.
         */
        err(ctx, "The read size cannot be larger than %u bytes.", bctx->buffer_size);
        return -1;
    }

    if (timeout != 0) {
        clock_gettime(CLOCK_REALTIME, &ts);
        timespec_add_ns(&ts, timeout * 1000 * 1000);
    }

    /*
     * Wait until sufficient data is available to be read.
     */
    if (timeout != 0) {
        clock_gettime(CLOCK_REALTIME, &ts);
        timespec_add_ns(&ts, timeout * 1000 * 1000);
    }
    while (cbuf_fill_level(bctx->input_buffer) < size) {
        if (timeout == 0) {
            rv = cbuf_wait_for_level_change(bctx->input_buffer);
        } else {
            rv = cbuf_timedwait_for_level_change(bctx->input_buffer, &ts);
        }

        if (rv != 0) {
            break;
        }
    }

    /*
     * We read whatever data is available, and assume a timeout if the available
     * amount of data does not match the requested amount.
     */
    *size_read = 0;
    rv = gb_uart_read(ctx, channel, size, data, size_read);
    if (rv == 0 && size != *size_read) {
        return -ETIMEDOUT;
    }
    return rv;
}

/**
 * Write to the target
 *
 * @param[in]  ctx the library context
 * @param[in]  channel the descriptor of the channel
 * @param[in]  size how much data is supposed to be written
 * @param[in]  data that is supposed to be written
 * @param[out] size_written how much data has been written
 *
 * @return 0 if writing was successful
 * @return any other value indicates failure
 *
 * @see glip_write()
 */
int gb_uart_write(struct glip_ctx *ctx, uint32_t channel, size_t size,
		uint8_t *data, size_t *size_written)
{
	assert(channel == 0);
    struct glip_backend_ctx* bctx = ctx->backend_ctx;

    unsigned int buf_size_free = cbuf_free_level(bctx->output_buffer);
    *size_written = (size > buf_size_free ? buf_size_free : size);

    cbuf_write(bctx->output_buffer, data, *size_written);

    return 0;
}

/**
 * Blocking write to the target
 *
 * @param[in]  ctx the library context
 * @param[in]  channel the descriptor of the channel
 * @param[in]  size how much data is supposed to be written
 * @param[in]  data that is supposed to be written
 * @param[out] size_written how much data has been written
 * @param[in]  timeout the maximum duration the write operation can take
 *
 * @return 0 if writing was successful
 * @return -ETIMEDOUT if the read timeout was hit
 * @return any other value indicates failure
 *
 * @see glip_write_b()
 */
int gb_uart_write_b(struct glip_ctx *ctx, uint32_t channel, size_t size,
		uint8_t *data, size_t *size_written, unsigned int timeout)
{
	int rv;

	assert(channel == 0);

    struct glip_backend_ctx* bctx = ctx->backend_ctx;
    struct timespec ts;

    if (timeout != 0) {
        clock_gettime(CLOCK_REALTIME, &ts);
        timespec_add_ns(&ts, timeout * 1000 * 1000);
    }

    size_t size_done = 0;
    while (1) {
        size_t size_done_tmp = 0;
        gb_uart_write(ctx, channel, size - size_done, &data[size_done],
                            &size_done_tmp);
        size_done += size_done_tmp;

        if (size_done == size) {
            break;
        }

        if (cbuf_free_level(bctx->output_buffer) == 0) {
            if (timeout == 0) {
                cbuf_wait_for_level_change(bctx->output_buffer);
            } else {
                cbuf_timedwait_for_level_change(bctx->output_buffer, &ts);
            }
        }
    }

    *size_written = size_done;
    if (size != *size_written) {
        return -ETIMEDOUT;
    }

    return 0;
}

/**
 * Get the number of supported channels by this backend
 *
 * @param  ctx the library context
 * @return always 1
 *
 * @see glip_get_channel_count()
 */
unsigned int gb_uart_get_channel_count(struct glip_ctx *ctx)
{
	return 1;
}

/**
 * Get the width of the FIFO
 *
 * @param  ctx the library context
 * @return the word width on the target, in bytes
 *
 * @see glip_get_fifo_width()
 */
unsigned int gb_uart_get_fifo_width(struct glip_ctx *ctx)
{
	return 1;
}

#define KNOWN(x) case x: return B##x

static int speed_lookup(int speed) {
	switch(speed) {
	KNOWN(50);
	KNOWN(75);
	KNOWN(110);
	KNOWN(134);
	KNOWN(150);
	KNOWN(200);
	KNOWN(300);
	KNOWN(600);
	KNOWN(1200);
	KNOWN(1800);
	KNOWN(2400);
	KNOWN(4800);
	KNOWN(9600);
	KNOWN(19200);
	KNOWN(38400);
	KNOWN(57600);
	KNOWN(115200);
	KNOWN(230400);
	KNOWN(460800);
	KNOWN(500000);
	KNOWN(576000);
	KNOWN(921600);
	KNOWN(1000000);
	KNOWN(1152000);
	KNOWN(1500000);
	KNOWN(2000000);
	KNOWN(2500000);
	KNOWN(3000000);
	KNOWN(3500000);
	KNOWN(4000000);
	default: return -1;
	}
}

static int reset_logic(int fd, uint8_t state) {
	uint8_t reset[2];
	size_t written;
	int rv;

	reset[0] = 0xfe;
	reset[1] = ((state & 0x1) << 1) | 0x81;

	return write_blocking(fd, reset, 2, &written, 0);
}

static int reset_com(int fd, uint8_t state) {
	uint8_t reset[2];
	size_t written;

	reset[0] = 0xfe;
	reset[1] = ((state & 0x1) << 1) | 0x85;

	return write_blocking(fd, reset, 2, &written, 0);
}

void parse_buffer(struct glip_backend_ctx *ctx, uint8_t *buffer, size_t size) {
	size_t actual, i;
	int rv;

	for (i = 0; i < size; i++) {
		if (buffer[i] == 0xfe) {
			// Check if next item is already in buffer
			if ((i + 1) < size) {
				if (buffer[i+1] == 0xfe) {
					// If this is the data word, write it
					assert(ctx->credit > 0);
					rv = cbuf_write(ctx->input_buffer, &buffer[i], 1);
					assert(rv == 0);
					ctx->credit--;
				} else {
					// This is the first of the credit message
					// Check if the next is also in buffer
					if ((i + 2) < size) {
						update_debt(ctx, buffer[i+1], buffer[i+2]);
						// Increment the counter for the second extra word
						i++;
					} else {
						// We have reached the end of the buffer, it is
						// safe to use the begin of buffer for the credit
						rv = read_blocking(ctx->fd, buffer, 1, &actual, 0);
						assert(rv == 0);
						update_debt(ctx, buffer[i+1], buffer[0]);
					}
				}
				// Increment the counter for the first extra word
				i++;
			} else {
				// If the next item was not in buffer, read another one,
				// we can now reuse the buffer
				rv = read_blocking(ctx->fd, buffer, 1, &actual, 0);
				assert(rv == 0);

				if (buffer[0] == 0xfe) {
					// That was the data word, write it
					assert(ctx->credit > 0);
					rv = cbuf_write(ctx->input_buffer, &buffer[0], 1);
					assert(rv == 0);
					ctx->credit--;
				} else {
					// We received a credit, read the next
					rv = read_blocking(ctx->fd, &buffer[1], 1, &actual, 0);
					assert(rv == 0);
					update_debt(ctx, buffer[0], buffer[1]);
				}
			}
		} else {
			// This is a data word
			assert(ctx->credit > 0);
			rv = cbuf_write(ctx->input_buffer, &buffer[i], 1);
			assert(rv == 0);
			ctx->credit--;
		}
	}
}

void* thread_func(void *arg) {
	uint8_t buffer[TMP_BUFFER_SIZE];
	size_t avail, actual;
	int rv;

	struct glip_backend_ctx *ctx = (struct glip_backend_ctx*) arg;

	while (1) {
		// Check for reset
		if (ctx->reset_request == 1) {
			rv = reset_logic(ctx->fd, 1);
			assert(rv == 0);

			rv = reset_logic(ctx->fd, 0);
			assert(rv == 0);
			ctx->reset_request = 0;
		}

		// Read
		avail = cbuf_free_level(ctx->input_buffer);
		if (avail == 0) {
			// We are only expecting a credit message now
			// Check if there is one
			rv = read(ctx->fd, buffer, 1);
			if (rv == 1) {
				assert(buffer[0] == 0xfe);
				rv = read_blocking(ctx->fd, buffer, 2, &actual, 0);
				assert(rv == 0);
				assert(actual == 2);
				update_debt(ctx, buffer[0], buffer[1]);
			}
		} else {
			// Read a chunk of data if the circular buffer can accept it
			size_t size = min(avail, TMP_BUFFER_SIZE);
			rv = read(ctx->fd, buffer, size);
			if (rv > 0) {
				parse_buffer(ctx, buffer, rv);
			}
		}

		// Write
		if (ctx->debt > 0) {
			size_t size = min(ctx->debt, TMP_BUFFER_SIZE);
			avail = cbuf_fill_level(ctx->output_buffer);
			if (avail > 0) {
				size = min(avail, size);

				assert(cbuf_read(ctx->output_buffer, buffer, size) != -EINVAL);
				rv = write_blocking(ctx->fd, buffer, size, &actual, 0);
				assert(rv == 0);
				assert(actual == size);
				ctx->debt -= size;
			}
		}

		// Update credit if necessary
		if (ctx->credit < ctx->buffer_size-UART_MAX_TRANCHE) {
			// Give new credit
			ctx->credit += UART_MAX_TRANCHE;

			send_credit(ctx, UART_MAX_TRANCHE);
		}
	}

	return 0;
}

static int read_blocking(int fd, uint8_t *buffer, size_t size,
		size_t *size_read, unsigned int timeout) {
	struct timeval tval_start;
	int rv;

	*size_read = 0;

	if (timeout > 0) {
		gettimeofday(&tval_start, NULL);
	}

	do {
		rv = read(fd, &buffer[*size_read], size - *size_read);

		if (rv >= 0) {
			*size_read += rv;
		}

		if ((rv == -1) && (errno != EAGAIN)) {
			return errno;
		}

		if (timeout > 0) {
			if (check_timeout(&tval_start, timeout)) {
				return -ETIMEDOUT;
			}
		}
	} while(*size_read != size);

	return 0;
}

static int write_blocking(int fd, uint8_t *buffer, size_t size,
		size_t *size_written, unsigned int timeout) {
	struct timeval tval_start;
	int rv;

	*size_written = 0;

	if (timeout > 0) {
		gettimeofday(&tval_start, NULL);
	}

	do {
		rv = write(fd, &buffer[*size_written], size - *size_written);

		if (rv >= 0) {
			*size_written += rv;
		}

		if ((rv == -1) && (errno != EAGAIN)) {
			return errno;
		}

		if (timeout > 0) {
			if (check_timeout(&tval_start, timeout)) {
				return -ETIMEDOUT;
			}
		}
	} while(*size_written != size);

	return 0;
}

static int check_timeout(struct timeval *start, unsigned long ms) {
	struct timeval tval_current, tval_diff;
	gettimeofday(&tval_current, NULL);

	tval_diff.tv_sec = tval_current.tv_sec - start->tv_sec;
	tval_diff.tv_usec = tval_current.tv_usec - start->tv_usec;

	if (tval_current.tv_usec < start->tv_usec) {
		tval_diff.tv_sec += 1;
		tval_diff.tv_usec += 1000000;
	}

	if ((tval_diff.tv_sec * 1000000 + tval_diff.tv_usec)
			> (ms * 1000)) {
		return -ETIMEDOUT;
	}

	return 0;
}

static void update_debt(struct glip_backend_ctx* ctx, uint8_t first,
		uint8_t second) {
	ctx->debt += (((first >> 1) & 0x7f) << 8) | second;
}

static int send_credit(struct glip_backend_ctx* ctx, uint16_t tranche) {
	uint8_t credit[3];
	size_t written;

	credit[0] = 0xfe;
	credit[1] = 0x1 | ((tranche >> 8) << 1);
	credit[2] = tranche & 0xff;

	if (write_blocking(ctx->fd, credit, 3, &written, 0) != 0) {
		return -1;
	}

	return 0;
}

static int reset(struct glip_backend_ctx* ctx) {
	int rv;

	rv = reset_com(ctx->fd, 1);
	if (rv != 0) {
		return -1;
	}

	// Drain the interface
	uint8_t buffer[16];
	do {
		rv = read(ctx->fd, buffer, 16);
	} while (rv > 0);

	rv = reset_com(ctx->fd, 0);

	rv = cbuf_discard(ctx->input_buffer, cbuf_fill_level(ctx->input_buffer));
	assert(rv == 0);

	rv = cbuf_discard(ctx->output_buffer, cbuf_fill_level(ctx->output_buffer));
	assert(rv == 0);

	ctx->credit = UART_MAX_TRANCHE;
	send_credit(ctx, UART_MAX_TRANCHE);

	ctx->debt = 0;
	uint8_t credit[3];
	size_t read;
	rv = read_blocking(ctx->fd, credit, 3, &read, 0);
	assert(rv == 0);

	// Strangely, the UART sometimes returns a trash byte on the first
	// access after the bitstream was loaded. Ignore this.
	if (credit[0] != 0xfe) {
		credit[0] = credit[1];
		credit[1] = credit[2];

		rv = read_blocking(ctx->fd, &credit[2], 1, &read, 0);
		assert(rv == 0);
	}

	assert(credit[0] == 0xfe);

	update_debt(ctx, credit[1], credit[2]);

	return 0;
}
