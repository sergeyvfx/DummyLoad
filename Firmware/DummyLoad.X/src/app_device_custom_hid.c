/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#include "app_device_custom_hid.h"

#include <string.h>
#include <system.h>
#include <usb/usb.h>
#include <usb/usb_device_hid.h>

#include "app_dummy_load.h"

/* Some processors have a limited range of RAM addresses where the USB module
 * is able to access.  The following section is for those devices.  This section
 * assigns the buffers that need to be used by the USB module into those
 * specific areas.
 */
#if defined(FIXED_ADDRESS_MEMORY)
#  if defined(COMPILER_MPLAB_C18)
#    pragma udata HID_CUSTOM_OUT_DATA_BUFFER = HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS
unsigned char ReceivedDataBuffer[64];
#    pragma udata HID_CUSTOM_IN_DATA_BUFFER = HID_CUSTOM_IN_DATA_BUFFER_ADDRESS
unsigned char ToSendDataBuffer[64];
#    pragma udata
#  else defined(__XC8)
unsigned char ReceivedDataBuffer[64] @ HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS;
unsigned char ToSendDataBuffer[64] @ HID_CUSTOM_IN_DATA_BUFFER_ADDRESS;
#  endif
#else
unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];
#endif

volatile USB_HANDLE USBOutHandle;
volatile USB_HANDLE USBInHandle;

typedef enum {
   COMMAND_SET_DIGITS = 0x80,
} CUSTOM_HID_COMMANDS;

/* Initializes the Custom HID code. */
void APP_DeviceCustomHIDInitialize(void) {
  /* Initialize the variable holding the handle for the last transmission. */
  USBInHandle = 0;

  /* Enable the HID endpoint. */
  USBEnableEndpoint(CUSTOM_DEVICE_HID_EP,
                    USB_IN_ENABLED |
                    USB_OUT_ENABLED |
                    USB_HANDSHAKE_ENABLED |
                    USB_DISALLOW_SETUP);

  /* Re-arm the OUT endpoint for the next packet */
  USBOutHandle =
    (volatile USB_HANDLE) HIDRxPacket(CUSTOM_DEVICE_HID_EP,
                                      (uint8_t*)&ReceivedDataBuffer, 64);
}

/* Keep the Custom HID running.
*
* The application should have been initialized and started via
* the APP_DeviceCustomHIDInitialize() function.
*
********************************************************************/
void APP_DeviceCustomHIDTasks(void) {
  /* Check if we have received an OUT data packet from the host. */
  if (HIDRxHandleBusy(USBOutHandle) == false) {
    /* We just received a packet of data from the USB host.
     * Check the first uint8_t of the packet to see what command the host
     * application software wants us to fulfill.
     */

     /* Look at the data the host sent, to see what kind of application specific
      * command it sent. */
    switch (ReceivedDataBuffer[0]) {
      default:
        break;
    }
    /* Re-arm the OUT endpoint, so we can receive the next OUT data packet
     * that the host may try to send us.
     */
    USBOutHandle = HIDRxPacket(CUSTOM_DEVICE_HID_EP,
                               (uint8_t*)&ReceivedDataBuffer,
                               64);
  }
}
