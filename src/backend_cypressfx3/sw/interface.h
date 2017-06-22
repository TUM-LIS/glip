#pragma once

#include "glip-protected.h"

#include <stdint.h>
#include <libusb.h>

struct firmware_interface {
    const char *name;
    uint8_t (*get_fifo_width)(struct libusb_device_descriptor * /* desc */);
    int (*reset_communication)(struct glip_ctx * /* ctx */,
            struct libusb_device_handle * /* dev */);
    int (*reset_logic)(struct glip_ctx * /* ctx */,
            struct libusb_device_handle * /* dev */);
    int wr_ep;
    int rd_ep;
};

struct firmware_interface *get_firmware_interface(const char* name);
