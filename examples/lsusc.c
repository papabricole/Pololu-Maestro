/*
 * libusc example program to list Pololu Maestro devices on the bus
 * Copyright (C) 2010 Morgan Leborgne <morganleborgne@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <sys/types.h>

#include <libusb.h>
#include "libusc.h"

static void print_devs(libusc_device **devs)
{
    libusc_device *dev;
    int i = 0;

    while ((dev = devs[i++]) != NULL) {
        /*
        printf("%04x:%04x (bus %d, device %d)\n",
               desc.idVendor, desc.idProduct,
               libusb_get_bus_number(dev), libusb_get_device_address(dev));
               */
        printf("device %d\n", dev->servocnt);
        libusc_set_target(dev, 0, 7000);
    }

}

int main(void)
{
    libusc_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init(NULL);
    if (r < 0)
        return r;

    cnt = libusc_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int) cnt;

    print_devs(devs);
    libusc_free_device_list(devs, 1);

    libusb_exit(NULL);
    return 0;
}

