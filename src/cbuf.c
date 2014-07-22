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

#include "cbuf.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/**
 * @defgroup development-cbuf Circular Buffer
 * @ingroup development
 * @{
 */

/**
 * Add two position pointers aware of the wrap-around
 *
 * @private
 */
static inline size_t _cbuf_ptr_add(struct cbuf *buf, size_t val1, size_t val2)
{
    return (val1 + val2) & (2 * buf->size - 1);
}

/**
 * Initialize the buffer
 *
 * @param buf the buffer structure to be initialized
 * @param size the size of the buffer in bytes. The size needs to be a power of
 *             two.
 * @return 0 on success
 * @return -ENOMEM if the memory allocation for the data buffer failed
 * @return any other value indicates an error
 */
int cbuf_init(struct cbuf *buf, size_t size)
{
    /* size needs to be a power of two */
    assert((size != 0) && !(size & (size - 1)));

    buf->data = calloc(size, sizeof(uint8_t));
    if (buf->data == NULL) {
        return -ENOMEM;
    }
    buf->size = size;

    buf->rd_ptr = 0;
    buf->wr_ptr = 0;

    buf->hint_max_read_size = 0;
    buf->read_data_tmp = NULL;
    buf->read_data_tmp_size = 0;

    buf->hint_max_write_size = 0;
    buf->write_data_tmp = NULL;
    buf->write_data_tmp_size = 0;

    pthread_mutex_init(&buf->level_mutex, NULL);
    pthread_cond_init(&buf->level_changed, NULL);

    return 0;
}

/**
 * Free all resources of the buffer
 *
 * This function frees all resources inside the buffer, but not the buffer
 * struct @p buf itself.
 *
 * @param buf the buffer
 * @return 0 on success
 * @return any other value indicates an error
 */
int cbuf_free(struct cbuf *buf)
{
    pthread_mutex_destroy(&buf->level_mutex);
    pthread_cond_destroy(&buf->level_changed);

    free(buf->data);
    free(buf->read_data_tmp);
    free(buf->write_data_tmp);
    return 0;
}

/**
 * Give a hint on the maximum read size
 *
 * Reading from the circular buffer requires under some circumstances the use
 * of a temporary buffer. This buffer needs to be as large as a read size and
 * automatically adjusts its size. If you know already how large your maximum
 * read will be (e.g. your maximum packet size), you can set this value as a
 * hint and this way prevent unnecessary memory allocations.
 *
 * Note that this hint is strictly an optimization, it's not necessary for the
 * correct functioning of this buffer!
 *
 * @param buf the buffer
 * @param hint_max_read_size a hint on the maximum read size
 * @return 0 on success
 * @return any other value indicates an error
 *
 * @see cbuf_set_hint_max_write_size()
 */
int cbuf_set_hint_max_read_size(struct cbuf *buf, size_t hint_max_read_size)
{
    buf->hint_max_read_size = hint_max_read_size;
    return 0;
}

/**
 * Give a hint on the maximum write size
 *
 * This function sets a hint on the maximum write size, similar to the
 * maximum read size set in cbuf_set_hint_max_read_size().
 *
 * @param buf the buffer
 * @param hint_max_write_size a hint on the maximum write size
 * @return 0 on success
 * @return any other value indicates an error
 *
 * @see cbuf_set_hint_max_read_size()
 */
int cbuf_set_hint_max_write_size(struct cbuf *buf, size_t hint_max_write_size)
{
    buf->hint_max_write_size = hint_max_write_size;
    return 0;
}

/**
 * Copy data into the buffer
 *
 * @p size bytes of data are taken from @p data and copied into the buffer. You
 * need to check if the buffer has enough space available by calling
 * cbuf_free_level() before; this function call fails with a return value of
 * -ENOMEM if not enough space is available.
 *
 * @param buf the buffer
 * @param data the data to be copied into the buffer
 * @param size the number of bytes copied from @p data into the buffer
 * @return 0 on success
 * @return -EINVAL if the buffer does not have enough space available. No data
 *                 has been copied.
 * @return any other value indicates an error
 *
 * @see cbuf_reserve()
 */
int cbuf_write(struct cbuf *buf, const uint8_t *data, size_t size)
{
    size_t free_level = cbuf_free_level(buf);

    if (size > free_level) {
        return -EINVAL;
    }

    if ((buf->wr_ptr & (buf->size - 1)) + size <= buf->size) {
        /* we can continuously write to the end of the array */
        memcpy(&buf->data[buf->wr_ptr & (buf->size - 1)], data, size);
    } else {
        /* we need to split between the end of the array and the start */
        size_t size_at_end = buf->size - (buf->wr_ptr & (buf->size - 1));
        size_t size_at_start = size - size_at_end;
        memcpy(&buf->data[buf->wr_ptr & (buf->size - 1)], data, size_at_end);
        memcpy(buf->data, &data[size_at_end], size_at_start);
    }

    pthread_mutex_lock(&buf->level_mutex);
    buf->wr_ptr = _cbuf_ptr_add(buf, buf->wr_ptr, size);
    pthread_cond_broadcast(&buf->level_changed);
    pthread_mutex_unlock(&buf->level_mutex);

    return 0;
}

/**
 * Reserve space in the buffer and return a pointer to it
 *
 * This function reserves @p size bytes of memory inside the buffer and returns
 * the pointer @p data to it. This pointer can then be used to write up to
 * @p size bytes of data to it. After the data has been written, the data needs
 * to be committed by calling cbuf_commit(). You may commit less bytes than you
 * reserved (or even nothing at all).
 *
 * You need to check if enough free spaces are available by calling
 * cbuf_free_level() beforehand, otherwise this function will fail with a
 * -ENOMEM return value.
 *
 * The main use case for this API are other APIs: many libraries provide
 * functions which can be passed a pointer to a preallocated chunk of memory
 * and a maximum size and then fill this memory. Using the cbuf_write() function
 * would require another memory copy (once from the API to a temporary buffer,
 * then from the temporary buffer to this buffer), which can be avoided by using
 * the reserve/commit API.
 *
 * @param[in]  buf the buffer
 * @param[out] data pointer which can be filled
 * @param[in]  size maximum number of bytes which can be written to @p data
 * @return 0 on success
 * @return -EINVAL if the buffer does not have enough space available, the
 *                 reservation is not possible.
 * @return -EINVAL the allocation of a temporary buffer failed
 * @return any other value indicates an error
 *
 * @see cbuf_commit()
 * @see cbuf_write()
 */
int cbuf_reserve(struct cbuf *buf, uint8_t **data, size_t size)
{
    size_t free_level = cbuf_free_level(buf);

    if (size > free_level) {
        return -EINVAL;
    }

    if ((buf->wr_ptr & (buf->size - 1)) + size <= buf->size) {
        /* we can hand out a pointer to the actual buffer */
        *data = &buf->data[buf->wr_ptr & (buf->size - 1)];
    } else {
        /* a buffer wrap will occur; we need to hand out a temporary buffer */
        if (buf->write_data_tmp_size < size) {
            if (size <= buf->hint_max_write_size) {
                /* we can use the hint for the temporary buffer size */
                buf->write_data_tmp_size = buf->hint_max_write_size;
            } else {
                /*
                 * temporary buffer growth strategy: use with what the user
                 * requested in |size|, rounded up to the next power of two, up
                 * to |buf->size|
                 */
                int bsr = 0;
                while (size >>= 1) {
                    bsr++;
                }
                size_t new_write_data_tmp_size = (1 << (bsr + 1));
                if (new_write_data_tmp_size > buf->size) {
                    buf->write_data_tmp_size = buf->size;
                } else {
                    buf->write_data_tmp_size = new_write_data_tmp_size;
                }
            }

            free(buf->write_data_tmp);
            buf->write_data_tmp = malloc(buf->write_data_tmp_size *
                                         sizeof(uint8_t));
            if (buf->write_data_tmp == NULL) {
                return -ENOMEM;
            }
        }

        *data = buf->write_data_tmp;
    }

    return 0;
}

/**
 * Commit data written into the buffer
 *
 * After you have reserved space in the buffer by calling cbuf_reserve() and
 * written data to it, use this function to commit the @p size actually
 * written bytes to the buffer. @p data must be a pointer returned by
 * cbuf_reserve(), @p size must be equal or less than the size you reserved in
 * cbuf_reserve().
 *
 * @param buf the buffer
 * @param data the data to commit
 * @param size the number of bytes to commit from @p data
 * @return 0 on success
 * @return any other value indicates an error
 *
 * @see cbuf_reserve()
 */
int cbuf_commit(struct cbuf *buf, uint8_t *data, size_t size)
{
    if (size == 0) {
        return 0;
    }

    if (data == buf->write_data_tmp) {
        /*
         * we handed out a temporary buffer in cbuf_reserve() because a split
         * write between the end of the array and the beginning will occur;
         * we need to copy this temporary buffer to the final location now.
         */
        size_t size_at_end = buf->size - (buf->wr_ptr & (buf->size - 1));
        size_t size_at_start = size - size_at_end;
        memcpy(&buf->data[buf->wr_ptr & (buf->size - 1)], data, size_at_end);
        memcpy(buf->data, &data[size_at_end], size_at_start);
    }

    pthread_mutex_lock(&buf->level_mutex);
    buf->wr_ptr = _cbuf_ptr_add(buf, buf->wr_ptr, size);
    pthread_cond_broadcast(&buf->level_changed);
    pthread_mutex_unlock(&buf->level_mutex);

    return 0;
}

/**
 * Read data from the buffer
 *
 * This copies @p size bytes of data from the buffer into @p data. @p data needs
 * to be large enough to hold @p size bytes of data.
 *
 * You need to check the buffer fill level if @p size bytes of data are
 * available by calling cbuf_fill_level() beforehand; otherwise this function
 * will fail with a -EINVAL return value.
 *
 * @param buf the buffer
 * @param data the data location to copy to
 * @param size the number of bytes to copy
 * @return 0 on success
 * @return -EINVAL if not enough data is available for reading
 * @return any other value indicates an error
 *
 * @see cbuf_peek()
 */
int cbuf_read(struct cbuf *buf, uint8_t *data, size_t size)
{
    if (size == 0) {
        return 0;
    }

    size_t fill_level = cbuf_fill_level(buf);
    if (fill_level < size) {
        return -EINVAL;
    }

    if ((buf->rd_ptr & (buf->size - 1)) + size <= buf->size) {
        /* we can do a continuous read */
        memcpy(data, &buf->data[buf->rd_ptr & (buf->size - 1)], size);
    } else {
        /* we have a split read from the end and the beginning of the array */
        /* from rd_ptr to end of array */
        int size_to_end = buf->size - (buf->rd_ptr & (buf->size - 1));
        memcpy(data, &buf->data[buf->rd_ptr & (buf->size - 1)], size_to_end);
        /* from the beginning of the array until we have enough */
        memcpy(&data[size_to_end], buf->data, size - size_to_end);
    }

    pthread_mutex_lock(&buf->level_mutex);
    buf->rd_ptr = _cbuf_ptr_add(buf, buf->rd_ptr, size);
    pthread_cond_broadcast(&buf->level_changed);
    pthread_mutex_unlock(&buf->level_mutex);

    return 0;
}

/**
 * Return a pointer to data without reading it yet
 *
 * This function returns a pointer to the buffer from which up to @p size
 * bytes of data can be read. The data is not yet removed from the buffer, you
 * need to call cbuf_discard() for that.
 *
 * The buffer needs to have @p size bytes of data available, check this
 * requirement by calling cbuf_fill_level() beforehand; otherwise -EINVAL is
 * returned.
 *
 * The main reason for this the peek/discard API is compatibility with 3rd-party
 * APIs. Call cbuf_peek() to get a pointer to some data, call another API's
 * write function for example and pass this pointer, and finally call
 * cbuf_discard() with the number of bytes actually written by the 3rd-party
 * API.
 *
 * @param[in]  buf the buffer
 * @param[out] data a pointer to the next @p size bytes of data
 * @param[in]  size the number of bytes to look ahead
 * @return 0 on success
 * @return -EINVAL not enough data is available to be read, check @p size
 * @return -ENOMEM the allocation of a temporary buffer failed
 * @return any other value indicates an error
 */
int cbuf_peek(struct cbuf *buf, uint8_t **data, size_t size)
{
    if (size == 0) {
        return 0;
    }

    size_t fill_level = cbuf_fill_level(buf);
    if (fill_level < size) {
        return -EINVAL;
    }

    /* we can do a continuous read, use the existing buffer and be done */
    if ((buf->rd_ptr & (buf->size - 1)) + size <= buf->size) {
        *data = &buf->data[buf->rd_ptr & (buf->size - 1)];
        return 0;
    }

    /* otherwise, we need to copy the two data segments into a continuous buffer */
    /* first, allocate a sufficiently large temporary buffer if necessary */
    if (buf->read_data_tmp_size < size) {
        if (size <= buf->hint_max_read_size) {
            /* we can use the hint for the temporary buffer size */
            buf->read_data_tmp_size = buf->hint_max_read_size;
        } else {
            /*
             * temporary buffer growth strategy: use with what the user
             * requested in |size|, rounded up to the next power of two, up to
             * |buf->size|
             */
            int bsr = 0;
            while (size >>= 1) {
                bsr++;
            }
            size_t new_read_data_tmp_size = (1 << (bsr + 1));
            if (new_read_data_tmp_size > buf->size) {
                buf->read_data_tmp_size = buf->size;
            } else {
                buf->read_data_tmp_size = new_read_data_tmp_size;
            }
        }

        free(buf->read_data_tmp);
        buf->read_data_tmp = malloc(buf->read_data_tmp_size * sizeof(uint8_t));
        if (buf->read_data_tmp == NULL) {
            return -ENOMEM;
        }
    }

    /* from rd_ptr to end of array */
    int size_to_end = buf->size - (buf->rd_ptr & (buf->size - 1));
    memcpy(buf->read_data_tmp, &buf->data[buf->rd_ptr & (buf->size - 1)], size_to_end);
    /* from the beginning of the array until we have enough */
    memcpy(&buf->read_data_tmp[size_to_end], buf->data, size - size_to_end);

    *data = buf->read_data_tmp;

    return 0;
}

/**
 * Discard data in the buffer
 *
 * This function removes (discards) @p size bytes of data from the buffer. See
 * cbuf_peek() for further information on how to use this API.
 *
 * @param buf the buffer
 * @param size the number of bytes to discard
 * @return 0 on success
 * @return -EINVAL not enough data is available to discard, check @p size
 * @return any other value indicates an error
 *
 * @see cbuf_peek()
 */
int cbuf_discard(struct cbuf *buf, size_t size)
{
    if (cbuf_fill_level(buf) < size) {
        return -EINVAL;
    }

    pthread_mutex_lock(&buf->level_mutex);
    buf->rd_ptr = _cbuf_ptr_add(buf, buf->rd_ptr, size);
    pthread_cond_broadcast(&buf->level_changed);
    pthread_mutex_unlock(&buf->level_mutex);

    return 0;
}

/**
 * Is the buffer empty?
 *
 * @param buf the buffer
 * @return true if the buffer is empty, false otherwise
 *
 * @see cbuf_is_full()
 * @see cbuf_free_level()
 */
bool cbuf_is_empty(struct cbuf *buf)
{
    return (buf->wr_ptr == buf->rd_ptr);
}

/**
 * Is the buffer full?
 *
 * @param buf the buffer
 * @return true if the buffer is full, false otherwise
 *
 * @see cbuf_is_empty()
 * @see cbuf_fill_level()
 */
bool cbuf_is_full(struct cbuf *buf)
{
    return (buf->wr_ptr == (buf->rd_ptr ^ buf->size));
}

/**
 * Get the fill level of the buffer
 *
 * The fill level is equal to the number of bytes of valid data stored in the
 * buffer.
 *
 * @param buf the buffer
 * @return the number of valid bytes in the buffer
 *
 * @see cbuf_free_level()
 * @see cbuf_is_full()
 */
size_t cbuf_fill_level(struct cbuf *buf)
{
    if (cbuf_is_full(buf)) {
        return buf->size;
    }

    ssize_t fill_level = (buf->wr_ptr & (buf->size - 1)) -
                         (buf->rd_ptr & (buf->size - 1));
    if (fill_level < 0) {
        fill_level += buf->size;
    }
    return fill_level;
}

/**
 * Get the number of available spaces in the buffer
 *
 * @param buf the buffer
 * @return the number of bytes that can still be written into the buffer
 *
 * @see cbuf_fill_level()
 * @see cbuf_is_empty()
 */
size_t cbuf_free_level(struct cbuf *buf)
{
    return buf->size - cbuf_fill_level(buf);
}

/**@}*/ /* end of doxygen group development-cbuf */
