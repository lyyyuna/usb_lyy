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
/* the length includes configuration, interface, class and endpoint */
sizeof(ConfigurationDescriptor)&0xff,       /* the low byte */
(sizeof(ConfigurationDescriptor)>>8)&0xff,  /* high byte */
/* bNumInterfaces */
0x01,
/* bConfiguration */
0x01,
/* iConfiguration */
0x00,
/* bmAttributes */
0x80,
/* bMaxPower */
0x32,

/* interface descriptor */
/* bLength */
0x09,
/* bDescriptorType */
0x04,
/* bInterfaceNumber */
0x00,
/* bAlternateSetting */
0x00,
/* bNumEndpoints */
/* the number of endp excludes 0 */
0x01,
/* bInterfaceClass */
0x03,   /* HID */
/* bInterfaceSubClass */
0x01,
/* bInterfaceProtocol */
0x02,
/* iInterface */
0x00,

/* HID descriptor */
/* bLength */
0x09,
/* bDescriptorType */
0x21,
/* bcdHID */
0x10,
0x01,
/* bCountyCode */
0x21,
/* bNumDescriptor */
0x01,
/* bDescriptorType */
0x22,
/* bDescriptorLength */
sizeof(ReportDescriptor)&0xff,
(sizeof(ReportDescriptor)>>8)&0xff,

/* endpoint descriptor */
/* bLength */
0x07,
/* bDescriptorType */
0x05,
/* bEndpointAddress */
0x81,
/* bmAttributes */
0x03,
/* wMaxPacketSize */
0x10,
0x00,
/* bInterval */
0x0a    /* the time interval, here, it is 10ms */
};

/************************语言ID的定义********************/
code uint8 LanguageId[4]=
{
 0x04, //本描述符的长度
 0x03, //字符串描述符
 //0x0409为美式英语的ID
 0x09,
 0x04
};
////////////////////////语言ID完毕//////////////////////////////////

/**************************************************/
/*********        本转换结果来自         **********/
/********* Http://computer00.21ic.org    **********/
/*********        作者: 电脑圈圈         **********/
/*********         欢迎大家使用          **********/
/*********    版权所有，盗版请写明出处   **********/
/**************************************************/

//http://computer00.21ic.org/user1/2198/archives/2007/42769.html
//字符串“电脑圈圈的USB专区 Http://group.ednchina.com/93/”的Unicode编码
//8位小端格式
code uint8 ManufacturerStringDescriptor[82]={
82,         //该描述符的长度为82字节
0x03,       //字符串描述符的类型编码为0x03
0x35, 0x75, //电
0x11, 0x81, //脑
0x08, 0x57, //圈
0x08, 0x57, //圈
0x84, 0x76, //的
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x13, 0x4e, //专
0x3a, 0x53, //区
0x20, 0x00, //
0x48, 0x00, //H
0x74, 0x00, //t
0x74, 0x00, //t
0x70, 0x00, //p
0x3a, 0x00, //:
0x2f, 0x00, ///
0x2f, 0x00, ///
0x67, 0x00, //g
0x72, 0x00, //r
0x6f, 0x00, //o
0x75, 0x00, //u
0x70, 0x00, //p
0x2e, 0x00, //.
0x65, 0x00, //e
0x64, 0x00, //d
0x6e, 0x00, //n
0x63, 0x00, //c
0x68, 0x00, //h
0x69, 0x00, //i
0x6e, 0x00, //n
0x61, 0x00, //a
0x2e, 0x00, //.
0x63, 0x00, //c
0x6f, 0x00, //o
0x6d, 0x00, //m
0x2f, 0x00, ///
0x39, 0x00, //9
0x33, 0x00, //3
0x2f, 0x00  ///
};
/////////////////////////厂商字符串结束/////////////////////////////

//字符串“《圈圈教你玩USB》之USB鼠标”的Unicode编码
//8位小端格式
code uint8 ProductStringDescriptor[34]={
34,         //该描述符的长度为34字节
0x03,       //字符串描述符的类型编码为0x03
0x0a, 0x30, //《
0x08, 0x57, //圈
0x08, 0x57, //圈
0x59, 0x65, //教
0x60, 0x4f, //你
0xa9, 0x73, //玩
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x0b, 0x30, //》
0x4b, 0x4e, //之
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x20, 0x9f, //鼠
0x07, 0x68  //标
};
////////////////////////产品字符串结束////////////////////////////

//字符串“2008-07-07”的Unicode编码
//8位小端格式
code uint8 SerialNumberStringDescriptor[22]={
22,         //该描述符的长度为22字节
0x03,       //字符串描述符的类型编码为0x03
0x32, 0x00, //2
0x30, 0x00, //0
0x30, 0x00, //0
0x38, 0x00, //8
0x2d, 0x00, //-
0x30, 0x00, //0
0x37, 0x00, //7
0x2d, 0x00, //-
0x30, 0x00, //0
0x37, 0x00  //7
};

void delay_xms(uint16 x)
{
    uint16 i;
    uint16 j;
    for (i = 0; i < x; i++)
        for (j = 0; j < 227; j++)
            ;
}

void usb_disconnect(void)
{
    #ifdef DEBUG0
    prints("断开USB连接。\r\n");
    #endif
    d12_writecommand(D12_SET_MODE);
    d12_writebyte(0x06);
    d12_writebyte(0x47);
    delay_xms(1000);
}

void usb_connect(void)
{
    #ifdef DEBUG0
    prints();
    #endif
    d12_writecommand(D12_SET_MODE);
    d12_writebyte(0x16);
    d12_writebyte(0x47);
}

void usb_bussuspend(void)
{
    #ifdef DEBUG0
    prints("");
    #endif
    /* do nothing */
}

void usb_busreset(void)
{
    #ifdef DEBUG0
    prints("");
    #endif
    ep1in_isbusy = 0;
}

static void usb_ep0_senddata(void)
{
    if (SendLength > DeviceDescriptor[7]) {
        d12_writeendpbuffer(1, DeviceDescriptor[7], pSendData);
        SendLength -= DeviceDescriptor[7];
        pSendData += DeviceDescriptor[7];
    }
    else {
        if(SendLength != 0) {
            d12_writeendpbuffer(1, SendLength, pSendData);
            SendLength = 0;
        }
        else {
            if (NeedZeroPacket == 1) {
                d12_writeendpbuffer(1, 0, pSendData);
                NeedZeroPacket = 0;
            }
        }
    }
}

void usb_ep0out(void)
{
    #ifdef DEBUG0
    prints("");
    #endif
    if (d12_readendpoint_laststatus(0) & 0x20) {
        d12_readendpbuffer(0, 16, edp0_buffer);
        d12_acksetup();
        d12_clearbuffer();

        bmRequestType = edp0_buffer[0];
        bRequest = edp0_buffer[1];
        wValue = edp0_buffer[2]+(((uint16)edp0_buffer[3])<<8);
        wIndex = edp0_buffer[4]+(((uint16)edp0_buffer[5])<<8);
        wLength = edp0_buffer[6]+(((uint16)edp0_buffer[7])<<8);

        /* D7 is 1, means it is a in request */
        if ((bmResqustType & 0x80) == 0x80) {
            switch ((bmRequstType>>5) & 0x03) {
                case 0:
                    #ifdef DEBUG0
                    prints("");
                    #endif
                    switch (bRequest) {
                        case GET_CONFIGURATION:
                            #ifdef DEBUG0
                            prints("");
                            #endif
                            break;
                        case GET_DESCRIPTOR:
                            #ifdef DEBUG0
                            prints("");
                            #endif
                            switch ((wValue>>8) & 0xff) {
                                case DEVICE_DESCRIPTOR:
                                    #ifdef DEBUG0
                                    prints("");
                                    #endif
                                    pSendData = DeviceDescriptor;
                                    /* the send length */
                                    if (wLength > DeviceDescriptor[0]) {
                                        SendLength = DeviceDescriptor[0];
                                        if(SendLength % DeviceDescriptor[7] == 0)
                                            NeedZeroPacket = 1;
                                    }
                                    else SendLength = wLength;
                                    usb_ep0_senddata();
                                    break;
                            }
                    }

            }
        }
    }
}




