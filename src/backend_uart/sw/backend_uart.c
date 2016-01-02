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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include <sys/time.h>

static int speed_lookup(int speed);

/**
 * GLIP backend context for the JTAG backend
 */
struct glip_backend_ctx {
	char *device;
	int fd;
	uint32_t speed;
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
	tty.c_lflag = 0;
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	tty.c_iflag &= ~IGNBRK;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    tty.c_oflag = 0;

	if (tcsetattr (bctx->fd, TCSANOW, &tty) != 0)
	{
		err(ctx, "Cannot set attributes");
		return -1;
	}

	// Drain the interface
	int rv;
	uint8_t buffer[16];
	do {
		rv = read(bctx->fd, buffer, 16);
	} while (rv > 0);

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
	struct glip_backend_ctx *bctx = ctx->backend_ctx;

	close(bctx->fd);

	return 0;
}

/**
 * Reset the logic on the target
 *
 * Not supported as no out-of-band availability
 *
 * @see glip_logic_reset()
 * @todo Implement in-band?
 */
int gb_uart_logic_reset(struct glip_ctx *ctx)
{
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
	int rv;

	if (channel != 0) {
		err(ctx, "Only channel 0 supported");
		return -1;
	}

	struct glip_backend_ctx* bctx = ctx->backend_ctx;

	do {
		rv = read(bctx->fd, data, size);
	} while ((rv == -1) && (errno == EAGAIN));

	if (rv >= 0) {
		*size_read = rv;
		return 0;
	} else {
		*size_read = 0;
		return errno;
	}
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
	//return gb_uart_read(ctx, channel, size, data, size_read);
	struct timeval tval_start, tval_current, tval_diff;
	int rv;

	struct glip_backend_ctx* bctx = ctx->backend_ctx;

	*size_read = 0;

	if (timeout > 0) {
		gettimeofday(&tval_start, NULL);
	}

	do {
		rv = read(bctx->fd, &data[*size_read], size - *size_read);

		if (rv >= 0) {
			*size_read += rv;
		}

		if ((rv == -1) && (errno != EAGAIN)) {
			return errno;
		}

		if (timeout > 0) {
			gettimeofday(&tval_current, NULL);
			tval_diff.tv_sec = tval_current.tv_sec - tval_start.tv_sec;
			tval_diff.tv_usec = tval_current.tv_usec - tval_start.tv_usec;
			if (tval_current.tv_usec < tval_start.tv_usec) {
				tval_diff.tv_sec -= 1;
			}
			if (tval_current.tv_usec < 0) {
				tval_current.tv_usec += 1000000;
			}

			if (tval_diff.tv_usec > (timeout * 1000)) {
				return -ETIMEDOUT;
			}
		}
	} while(*size_read != size);

	return 0;
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
	int rv;

	if (channel != 0) {
		err(ctx, "Only channel 0 supported");
		return -1;
	}

	struct glip_backend_ctx* bctx = ctx->backend_ctx;

	do {
		rv = write(bctx->fd, data, size);
	} while((rv == -1) && (errno = EAGAIN));

	if (rv >= 0) {
		*size_written = rv;
		return 0;
	} else {
		*size_written = 0;
		return errno;
	}
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
	struct timeval tval_start, tval_current, tval_diff;
	int rv;

	struct glip_backend_ctx* bctx = ctx->backend_ctx;

	*size_written = 0;

	if (timeout > 0) {
		gettimeofday(&tval_start, NULL);
	}

	do {
		rv = write(bctx->fd, &data[*size_written], size - *size_written);

		if (rv >= 0) {
			*size_written += rv;
		}

		if ((rv == -1) && (errno != EAGAIN)) {
			return errno;
		}

		if (timeout > 0) {
			gettimeofday(&tval_current, NULL);
			tval_diff.tv_sec = tval_current.tv_sec - tval_start.tv_sec;
			tval_diff.tv_usec = tval_current.tv_usec - tval_start.tv_usec;
			if (tval_current.tv_usec < tval_start.tv_usec) {
				tval_diff.tv_sec -= 1;
			}
			if (tval_current.tv_usec < 0) {
				tval_current.tv_usec += 1000000;
			}

			if (tval_diff.tv_usec > (timeout * 1000)) {
				printf("Timeout\n");
				return -ETIMEDOUT;
			}
		}
	} while(*size_written != size);

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
