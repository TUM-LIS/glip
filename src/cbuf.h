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
 * Author(s):
 *   Philipp Wagner <philipp.wagner@tum.de>
 */
#ifndef CBUF_H_
#define CBUF_H_

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * @addtogroup development-cbuf
 * @{
 */

/**
 * The circular buffer structure
 */
struct cbuf {
    /** mutex protecting the fill/free level */
    pthread_mutex_t level_mutex;
    /**condition variable: the fill/free level changed */
    pthread_cond_t level_changed;

    /** position to write the next entry to */
    size_t wr_ptr;
    /** position to read the next entry from */
    size_t rd_ptr;

    /** number of entries in the buffer */
    size_t size;
    /** the buffer itself */
    uint8_t *data;

    size_t hint_max_read_size;
    size_t read_data_tmp_size;
    uint8_t *read_data_tmp;

    size_t hint_max_write_size;
    size_t write_data_tmp_size;
    uint8_t *write_data_tmp;
};

int cbuf_init(struct cbuf *buf, size_t size);
int cbuf_free(struct cbuf *buf);

int cbuf_write(struct cbuf *buf, const uint8_t *data, size_t size);
int cbuf_reserve(struct cbuf *buf, uint8_t **data, size_t size);
int cbuf_commit(struct cbuf *buf, uint8_t *data, size_t size);

int cbuf_read(struct cbuf *buf, uint8_t *data, size_t size);
int cbuf_peek(struct cbuf *buf, uint8_t **data, size_t size);
int cbuf_discard(struct cbuf *buf, size_t size);

bool cbuf_is_empty(struct cbuf *buf);
bool cbuf_is_full(struct cbuf *buf);

size_t cbuf_fill_level(struct cbuf *buf);
size_t cbuf_free_level(struct cbuf *buf);

int cbuf_set_hint_max_read_size(struct cbuf *buf, size_t hint_max_read_size);
int cbuf_set_hint_max_write_size(struct cbuf *buf, size_t hint_max_write_size);

#endif /* CBUF_H_ */

/**@}*/
