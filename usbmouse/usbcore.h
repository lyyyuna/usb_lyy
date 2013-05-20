#ifndef __USBCORE_H
#define __USBCORE_H

#define GET_STATUS         0
#define CLEAR_FEATURE      1
#define SET_FEATURE        3
#define SET_ADDRESS        5
#define GET_DESCRIPTOR     6
#define SET_DESCRIPTOR     7
#define GET_CONFIGURATION  8
#define SET_CONFIGURATION  9
#define GET_INTERFACE      10
#define SET_INTERFACE      11
#define SYNCH_FRAME        12

#define DEVICE_DESCRIPTOR         1
#define CONFIGURATION_DESCRIPTOR  2
#define STRING_DESCRIPTOR         3
#define INTERFACE_DESCRIPTOR      4
#define ENDPOINT_DESCRIPTOR       5
#define REPORT_DESCRIPTOR         0x22

#define SET_IDLE 0x0A


extern void usb_disconnect(void);   /* disconnect the usb */
extern void usb_connect(void);      /* connect the usb */
extern void usb_bussuspend(void);   /* suspend the bus */
extern void usb_busreset(void);     /* reset the usb bus */
extern void usb_ep0out(void);       /* ep0 out */
extern void usb_ep0in(void);        /* ep0 in */
extern void usb_ep1out(void);       /* ep1 out */
extern void usb_ep1in(void);        /* ep1 in */
extern void usb_ep2out(void);       /* ep2 out */
extern void usb_ep2in(void);        /* ep2 in */

extern uint8 configvalue;           /* current config value */
extern uint8 ep1in_isbusy;          /* flag, if ep1 is busy */

#endif
