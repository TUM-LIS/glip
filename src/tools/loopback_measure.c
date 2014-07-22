/* Copyright (c) 2014 by the author(s)
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
 * a PC.
 *
 * Author(s):
 *   Philipp Wagner <philipp.wagner@tum.de>
 *   Stefan Wallentowitz <stefan.wallentowitz@tum.de>
 */

#include <getopt.h>
#include <inttypes.h>
#include <libglip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* total number of bytes to be transferred during the loopback test */
static const size_t TRANSFER_SIZE_BYTES = 300*1024*1024; /* 300 MB */

/* write block size in bytes */
#define WRITE_BLOCK_SIZE 6
/* read block size in bytes */
#define READ_BLOCK_SIZE 2048

extern void parse_options(char* str, struct glip_option* options[],
                          size_t *num_options);

void display_help(void);

void display_help(void)
{
    printf("Usage: glip_loopback_measure -b BACKEND [OPTIONAL_ARGS]\n\n"
           "Write data to the target and read it back, measuring the \n"
           "transfer data rate.\n"
           "\n"
           "REQUIRED ARGUMENTS\n"
           "-b|--backend BACKEND\n"
           "  the backend to be used. See below for available backends.\n"
           "\n"
           "OPTIONAL ARGUMENTS\n"
           "-o|--backend-options\n"
           "  options passed to the backend. Options are key=value pairs \n"
           "  separated by a comma.\n"
           "-h|--help\n"
           "  print this help message\n"
           "-v|--version\n"
           "  print the GLIP library version information\n"
           "\n"
           "EXAMPLES\n"
           "Run the program with the cypressfx2 backend, connecting to the \n"
           "USB device on bus 1 with address 2:\n"
           "$> glip_loopback_measure -b cypressfx2 -ousb_dev_bus=1,"
              "usb_dev_addr=2\n"
           "\n");

    printf("AVAILABLE BACKENDS\n");
    const char** name;
    size_t count;
    glip_get_backends(&name, &count);
    for (size_t i = 0; i < count; i++) {
        printf("- %s\n", name[i]);
    }

    free(name);

    printf("\nConsult the documentation for a list of available options for "
            "each backend.\n");
    printf("\n");
}
void display_version(void);
void display_version(void)
{
    const struct glip_version *v = glip_get_version();
    printf("libglip version: %d.%d.%d%s\n", v->major, v->minor, v->micro,
           v->suffix);
}

size_t current_sent;
size_t current_received;
pthread_mutex_t done_mutex;
pthread_cond_t  done;

pthread_t read_thread;
pthread_attr_t read_thread_attr;

void* read_from_target(void* ctx_void);

int main(int argc, char *argv[])
{
    int c, rv;

    int glip_backend_set = 0;
    char* backend_name;

    char *backend_optionstring;
    struct glip_option* backend_options;
    size_t num_backend_options = 0;

    while (1) {
        static struct option long_options[] = {
            {"help",            no_argument,       0, 'h'},
            {"version",         no_argument,       0, 'v'},
            {"backend",         required_argument, 0, 'b'},
            {"backend-options", required_argument, 0, 'o'},
            {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long(argc, argv, "is:vhb:o:", long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 0:
            /* If this option set a flag, do nothing else now.   */
            if (long_options[option_index].flag != 0) {
                break;
            }
            break;
        case 'b':
            backend_name = strdup(optarg);
            glip_backend_set = 1;
            break;
        case 'o':
            backend_optionstring = optarg;
            parse_options(backend_optionstring, &backend_options,
                          &num_backend_options);
            break;
        case 'v':
            display_version();
            return 0;
        case 'h':
            display_help();
            return 0;
        default:
            display_help();
            return -1;
        }
    }

    if (glip_backend_set == 0) {
        fprintf(stderr, "ERROR: You need to set a backend\n\n");
        display_help();
        return -1;
    }

    pthread_mutex_init(&done_mutex, 0);
    pthread_cond_init(&done, 0);

    struct glip_ctx *ctx;
    glip_new(&ctx, backend_name, backend_options, num_backend_options);

    rv = glip_open(ctx, 1);
    if (rv < 0) {
        fprintf(stderr, "ERROR: Cannot open backend\n");
        return -1;
    }

    rv = glip_logic_reset(ctx);
    if (rv < 0) {
        fprintf(stderr, "Cannot reset device\n");
        return -1;
    }

    /* create a dumb read thread, just discarding the data */
    pthread_attr_init(&read_thread_attr);
    pthread_attr_setdetachstate(&read_thread_attr,
                                PTHREAD_CREATE_DETACHED);
    rv = pthread_create(&read_thread, &read_thread_attr,
                        read_from_target, (void*)ctx);

    struct timespec start, end;

    pthread_mutex_lock(&done_mutex);
    current_sent = 0;
    current_received = 0;
    pthread_mutex_unlock(&done_mutex);

    printf("Running loopback test with %zu bytes of data. This may take a "
           "while ...\n", TRANSFER_SIZE_BYTES);

    clock_gettime(CLOCK_MONOTONIC, &start);

    uint8_t data[WRITE_BLOCK_SIZE];
    for (size_t i = 0; i < WRITE_BLOCK_SIZE; i++) {
        data[i] = i % UINT8_MAX;
    }
    while (current_sent < TRANSFER_SIZE_BYTES) {
        size_t size_written;
        glip_write_b(ctx, 0, WRITE_BLOCK_SIZE, data, &size_written, 0);
        current_sent += size_written;
    }
    if (current_received != current_sent) {
        pthread_cond_wait(&done, &done_mutex);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double diff = (end.tv_sec * 1.0 + end.tv_nsec / 1000000000.0) -
                  (start.tv_sec * 1.0 + start.tv_nsec / 1000000000.0);

    printf("Sent and received %zu bytes in %.03f seconds = %.03lf MByte/s "
           "(bidirectional)\n",
           current_sent, diff, (current_sent)/diff/1024/1024);
    printf("Write block size: %u bytes, read block size: %u bytes\n",
           WRITE_BLOCK_SIZE, READ_BLOCK_SIZE);
    pthread_mutex_unlock(&done_mutex);
    return 0;
}

void* read_from_target(void* ctx_void)
{
    struct glip_ctx *ctx = ctx_void;

    uint8_t data_read[READ_BLOCK_SIZE] = {0};
    size_t size_read;
    uint8_t data_exp = 0;

    pthread_mutex_lock(&done_mutex);
    int byte = 0;
    while (1) {
        glip_read_b(ctx, 0, READ_BLOCK_SIZE, data_read, &size_read, 100);
        current_received += size_read;

        /* verify received data */
        for (size_t i = 0; i < size_read; i++) {
            if (data_read[i] != data_exp) {
                fprintf(stderr, "Data verification failed: expected 0x%x, "
                        "got 0x%x at byte %d\n", data_exp, data_read[i], byte);
                exit(1);
            }
            byte++;
            data_exp = (data_exp + 1) % (UINT8_MAX > WRITE_BLOCK_SIZE ?
                                         WRITE_BLOCK_SIZE : UINT8_MAX);
        }

        if (current_received == current_sent) {
            pthread_cond_signal(&done);
        }
    }
    pthread_mutex_unlock(&done_mutex);

    return NULL;
}
