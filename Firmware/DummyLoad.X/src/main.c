/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2014 Sergey Sharybin <sergey.vfx@gmail.com>
 * All rights reserved.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <system.h>
#include <system_config.h>
#include <usb/usb.h>
#include <usb/usb_device_hid.h>

#include "app_device_custom_hid.h"

void main(void) {
  SYSTEM_Initialize();

  USBDeviceInit();
  USBDeviceAttach();

  while (1) {
    SYSTEM_Tasks();

#if defined(USB_POLLING)
    /* Interrupt or polling method.  If using polling, must call
     * this function periodically.  This function will take care
     * of processing and responding to SETUP transactions
     * (such as during the enumeration process when you first
     * plug in).  USB hosts require that USB devices should accept
     * and process SETUP packets in a timely fashion.  Therefore,
     * when using polling, this function should be called
     * regularly (such as once every 1.8ms or faster** [see
     * inline code comments in usb_device.c for explanation when
     * "or faster" applies])  In most cases, the USBDeviceTasks()
     * function does not take very long to execute (ex: <100
     * instruction cycles) before it returns.
     */
    USBDeviceTasks();
#endif

    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop.
     */
    if (USBGetDeviceState() < CONFIGURED_STATE) {
      continue;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop.
     */
    if (USBIsDeviceSuspended() == true) {
      continue;
    }

    /* Application specific USB tasks. */
    APP_DeviceCustomHIDTasks();
  }
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
  switch ((int)event) {
    case EVENT_TRANSFER:
      break;

    case EVENT_SOF:
      break;

    case EVENT_SUSPEND:
      break;

    case EVENT_RESUME:
      break;

    case EVENT_CONFIGURED:
      /* When the device is configured, we can (re)initialize the app code. */
      APP_DeviceCustomHIDInitialize();
      break;

    case EVENT_SET_DESCRIPTOR:
      break;

    case EVENT_EP0_REQUEST:
        /* We have received a non-standard USB request.  The HID driver
         * needs to check to see if the request was for it.
         */
        USBCheckHIDRequest();
        break;

    case EVENT_BUS_ERROR:
        break;

    case EVENT_TRANSFER_TERMINATED:
        break;

    default:
        break;
  }
  return true;
}
