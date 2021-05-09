#ifndef __USB_COMM__H
#define __USB_COMM__H

#include "usb_serial.h"

static usb_serial_cfg_t usb_serial_cfg =
{
    .read_timeout_us = 1,
    .write_timeout_us = 1,
};

static usb_serial_t *usb_serial;


#endif

