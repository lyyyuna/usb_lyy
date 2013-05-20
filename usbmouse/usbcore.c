#include "config.h"
#include "pdiusbd12.h"
#include "uart.h"
#include "usbcore.h"

idata uint8 edp0_buffer[16];    /* buffer of endpoint 0 */

uint8  bmRequestType;
uint8  bRequest;
uint16 wValue;
uint16 wIndex;
uint16 wLength;

uint8 * pSendData;

uint16 SendLength;

/* if the length of the sending data is shorter than
   the max length of bMaxPacketSize0, then the host
   would consider the process reaches the end */
uint8 NeedZeroPacket;

uint8 configvalue;           /* current config value */

uint8 ep1in_isbusy;          /* flag, if ep1 is busy */


/* the device descriptor */
code uint8 DeviceDescriptor[0x12] = {
/* bLength */
0x12,
/* bDescriptorType */
0x01,
/* bcdUSB */
0x10,
0x01,
/* bDeviceClass */
0x00,
/* bDeviceSubClass */
0x00,
/* bDeviceProtocol */
0x00,
/* bMaxPacketSize0, the pdiusbd12 chip has a maximize size of 16 */
0x10,

/* idVender, just for test, for commercial use, you should apply for
   it from the usb association */
0x88,
0x88,
/* idProduct */
0x04,
0x00,

/* bcdDevice, version number of product */
0x00,
0x01,
/* iManufacturer */
0x01,
/* iProduct */
0x02,
/* iSerialNumber */
0x03,
/* bNumConfigurations, the number of configurations of a device,
   most device have only one configuration */
0x01
};

/* configuration descriptor */
/* the total length is 9 + 9 + 9 + 7 */
code uint8 ConfigurationDescriptor[9+9+9+7] = {
/* configuration */
/* bLength */
0x09,
/* bDescriptorType */
0x02,
/* wTotalLength */
};
