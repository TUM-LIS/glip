#include "interface.h"

#include <string.h>

static const int USB_TX_TIMEOUT_MS = 1000; /* 1 second */

static uint8_t optimsoc_get_fifo_width(struct libusb_device_descriptor * desc ) {
    return desc->iSerialNumber;
}

static int optimsoc_reset_communication(struct glip_ctx * ctx,
        struct libusb_device_handle * dev) {
    int rv;
    rv = libusb_control_transfer(dev, 0x40, 0x60,
                                 (1 << 0), 0, 0, 0, USB_TX_TIMEOUT_MS);
    if (rv < 0) {
        err(ctx, "Unable to send USB control message to reset system "
            "(r=1). Error %d: %s\n", rv, libusb_error_name(rv));
        return -1;
    }

    rv = libusb_control_transfer(dev, 0x40, 0x60,
                                 (0 << 0), 0, 0, 0, USB_TX_TIMEOUT_MS);
    if (rv < 0) {
        err(ctx, "Unable to send USB control message to reset system (r=0). "
            "Error %d: %s\n", rv, libusb_error_name(rv));
        return -1;
    }

    return 0;
}


static int optimsoc_reset_logic(struct glip_ctx * ctx,
        struct libusb_device_handle * dev) {
    int rv;

    /* set reset signal */
    rv = libusb_control_transfer(dev, 0x40, 0x60,
                                 (1 << 1), 0, 0, 0, USB_TX_TIMEOUT_MS);
    if (rv < 0) {
        err(ctx, "Unable to send USB control message to reset system "
                 "(r=1). Error %d: %s\n", rv, libusb_error_name(rv));
        return -1;
    }

    /* unset reset signal */
    rv = libusb_control_transfer(dev, 0x40, 0x60,
                                 (0 << 1), 0, 0, 0, USB_TX_TIMEOUT_MS);

    if (rv < 0) {
        err(ctx, "Unable to send USB control message to reset system (r=0). "
            "Error %d: %s\n", rv, libusb_error_name(rv));
        return -1;
    }

    return 0;
}

static uint8_t ztex_get_fifo_width(struct libusb_device_descriptor * desc) {
    (void) desc;
    return 2;
}

static int ztex_reset_communication(struct glip_ctx * ctx,
        struct libusb_device_handle * dev) {
    return 0;
}

static int ztex_reset_logic(struct glip_ctx * ctx,
        struct libusb_device_handle * dev) {
    return 0;
}

static struct firmware_interface interfaces[] = {
    {
        .name = "OpTiMSoC",
        .get_fifo_width = optimsoc_get_fifo_width,
        .reset_communication = optimsoc_reset_communication,
        .reset_logic = optimsoc_reset_logic,
        .wr_ep = 0x01 | LIBUSB_ENDPOINT_OUT,
        .rd_ep = 0x01 | LIBUSB_ENDPOINT_IN
    },
    {
        .name = "ZTEX",
        .get_fifo_width = ztex_get_fifo_width,
        .reset_communication = ztex_reset_communication,
        .reset_logic = ztex_reset_logic,
        .wr_ep = 0x02 | LIBUSB_ENDPOINT_OUT,
        .rd_ep = 0x04 | LIBUSB_ENDPOINT_IN
    },
    {}
};

struct firmware_interface *get_firmware_interface(const char* name) {
    for (int idx = 0; interfaces[idx].name != 0; idx++) {
        struct firmware_interface *fwif = &interfaces[idx];
        if (strncmp(name, fwif->name, strlen(fwif->name)) == 0) {
            return fwif;
        }
    }

    return 0;
}
