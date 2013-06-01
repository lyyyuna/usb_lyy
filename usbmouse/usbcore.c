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


/* HID report descriptor */
code uint8 ReportDescriptor[] = {
/* Usage Page (Generic Desktop) */
0x05, 0x01,
/* Usage (Mouse) */
0x09, 0x02,
/*      Collection (Application) */
0xa1, 0x01,
/*      Usage (Pointer) */
0x09, 0x01,
/*      Collection (Physical) */
0xa1, 0x00,
/*          Usage Page (Buttons) */
0x05, 0x09,
/*          Usage Minimum (1) */
0x19, 0x01,
/*          Usage Maximum (2) */
0x29, 0x03,
/*          Logical Minimum (0) */
0x15, 0x00,
/*          Logical Maximum (1) */
0x25, 0x01,
/*          Report Count (3) */
0x95, 0x03,
/*          Report Size (1) */
0x75, 0x01,
/*          Input (Data, Variable, Absolute) */
0x81, 0x02,
/*          Report Count (1) */
0x95, 0x01,
/*          Report Size (5) */
0x75, 0x05,
/*          Input (Constant) */
0x81, 0x03,
/*          Usage Page (Generic Page) */
0x05, 0x01,
/*          Usage (x) */
0x09, 0x30,
/*          Usage (y) */
0x09, 0x31,
/*          Logical Minimum (-127) */
0x15, 0x81,
/*          Logical Maximum (127) */
0x25, 0x7f,
/*          Report Size (8) */
0x75, 0x08,
/*          Report Count (3) */
0x95, 0x03,
/*          Input (Data, Variable, Relative) */
0x81, 0x06,
/*      End Collection */
0xc0,
/* End Collection */
0xc0
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

/************************����ID�Ķ���********************/
code uint8 LanguageId[4]=
{
 0x04, //���������ĳ���
 0x03, //�ַ���������
 //0x0409Ϊ��ʽӢ���ID
 0x09,
 0x04
};
////////////////////////����ID���//////////////////////////////////

/**************************************************/
/*********        ��ת���������         **********/
/********* Http://computer00.21ic.org    **********/
/*********        ����: ����ȦȦ         **********/
/*********         ��ӭ���ʹ��          **********/
/*********    ��Ȩ���У�������д������   **********/
/**************************************************/

//http://computer00.21ic.org/user1/2198/archives/2007/42769.html
//�ַ���������ȦȦ��USBר�� Http://group.ednchina.com/93/����Unicode����
//8λС�˸�ʽ
code uint8 ManufacturerStringDescriptor[82]={
82,         //���������ĳ���Ϊ82�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x35, 0x75, //��
0x11, 0x81, //��
0x08, 0x57, //Ȧ
0x08, 0x57, //Ȧ
0x84, 0x76, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x13, 0x4e, //ר
0x3a, 0x53, //��
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
/////////////////////////�����ַ�������/////////////////////////////

//�ַ�������ȦȦ������USB��֮USB��ꡱ��Unicode����
//8λС�˸�ʽ
code uint8 ProductStringDescriptor[34]={
34,         //���������ĳ���Ϊ34�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x0a, 0x30, //��
0x08, 0x57, //Ȧ
0x08, 0x57, //Ȧ
0x59, 0x65, //��
0x60, 0x4f, //��
0xa9, 0x73, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x0b, 0x30, //��
0x4b, 0x4e, //֮
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x20, 0x9f, //��
0x07, 0x68  //��
};
////////////////////////��Ʒ�ַ�������////////////////////////////

//�ַ�����2008-07-07����Unicode����
//8λС�˸�ʽ
code uint8 SerialNumberStringDescriptor[22]={
22,         //���������ĳ���Ϊ22�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
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
    prints("�Ͽ�USB���ӡ�\r\n");
    #endif
    d12_writecommand(D12_SET_MODE);
    d12_writebyte(0x06);
    d12_writebyte(0x47);
    delay_xms(1000);
}

void usb_connect(void)
{
    #ifdef DEBUG0
    prints("����USB��\r\n");
    #endif
    d12_writecommand(D12_SET_MODE);
    d12_writebyte(0x16);
    d12_writebyte(0x47);
}

void usb_bussuspend(void)
{
    #ifdef DEBUG0
    prints("USB���߹���\r\n");
    #endif
    /* do nothing */
}

void usb_busreset(void)
{
    #ifdef DEBUG0
    prints("USB���߸�λ��\r\n");
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
    prints("USB�˵�0����жϡ�\r\n");
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
        if ((bmRequestType & 0x80) == 0x80) {
            switch ((bmRequestType>>5) & 0x03) {
                case 0:
                    #ifdef DEBUG0
                    prints("USB��׼��������");
                    #endif
                    switch (bRequest) {
                        case GET_CONFIGURATION:
                            #ifdef DEBUG0
                            prints("��ȡ���á�\r\n");
                            #endif
                            break;
                        case GET_DESCRIPTOR:
                            #ifdef DEBUG0
                            prints("��ȡ����������");
                            #endif
                            switch ((wValue>>8) & 0xff) {
                                case DEVICE_DESCRIPTOR:
                                    #ifdef DEBUG0
                                    prints("�豸��������\r\n");
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

                                case CONFIGURATION_DESCRIPTOR:
                                    #ifdef DEBUG0
                                    prints("������������\r\n");
                                    #endif
                                    pSendData = ConfigurationDescriptor;
                                    SendLength = ConfigurationDescriptor[3];
                                    SendLength = SendLength*256 + ConfigurationDescriptor[2];
                                    if (wLength > SendLength) {
                                        if (SendLength % DeviceDescriptor[7] == 0)
                                            NeedZeroPacket = 1;
                                    }
                                    else SendLength = wLength;
                                    usb_ep0_senddata();
                                    break;

                                case STRING_DESCRIPTOR:
                                    #ifdef DEBUG0
                                    prints("�ַ���������");
                                    #endif
                                    switch (wValue & 0xff) {
                                        case 0: /* language id */
                                            #ifdef DEBUG0
                                            prints("(����ID)��\r\n");
                                            #endif
                                            pSendData = LanguageId;
                                            SendLength = LanguageId[0];
                                            break;
                                        case 1: /* manufacture id */
                                            #ifdef DEBUG0
                                            prints("(��������)��\r\n");
                                            #endif
                                            pSendData = ManufacturerStringDescriptor;
                                            SendLength = ManufacturerStringDescriptor[0];
                                            break;
                                        case 2:
                                            #ifdef DEBUG0
                                            prints("(��Ʒ����)��\r\n");
                                            #endif
                                            pSendData = ProductStringDescriptor;
                                            SendLength = ProductStringDescriptor[0];
                                            break;
                                        case 3:
                                            #ifdef DEBUG0
                                            prints("(��Ʒ���к�)��\r\n");
                                            #endif
                                            pSendData = SerialNumberStringDescriptor;
                                            SendLength = SerialNumberStringDescriptor[0];
                                            break;
                                        default:
                                            #ifdef DEBUG0
                                            prints("(δ֪������ֵ)��\r\n");
                                            #endif
                                            /* if it is a unknown descriptor, return a 0 length packet */
                                            SendLength = 0;
                                            NeedZeroPacket = 1;
                                            break;
                                    }
                                    /* if the requested length is longer */
                                    if (wLength > SendLength) {
                                        if (SendLength % DeviceDescriptor[7] == 0)
                                            NeedZeroPacket = 1;
                                    }
                                    else SendLength = wLength;
                                    usb_ep0_senddata();
                                    break;

                                case REPORT_DESCRIPTOR:
                                    #ifdef DEBUG0
                                    prints("������������\r\n");
                                    #endif
                                    pSendData = ReportDescriptor;
                                    SendLength = sizeof(ReportDescriptor);
                                    if (wLength > SendLength) {
                                        if (SendLength % DeviceDescriptor[7] == 0)
                                            NeedZeroPacket = 1;
                                    }
                                    else SendLength = wLength;
                                    usb_ep0_senddata();
                                    break;

                                default:
                                    #ifdef DEBUG0
                                    prints("���������������������룺");
                                    printHex((wValue>>8)&0xFF);
                                    prints("\r\n");
                                    #endif
                                    break;
                            }
                            break;

                        case GET_INTERFACE:
                            #ifdef DEBUG0
                            prints("��ȡ�ӿڡ�\r\n");
                            #endif
                            break;

                        case GET_STATUS:
                            #ifdef DEBUG0
                            prints("��ȡ״̬��\r\n");
                            #endif
                            break;

                        case SYNCH_FRAME:
                            #ifdef DEBUG0
                            prints("ͬ��֡��\r\n");
                            #endif
                            break;

                        default:
                            #ifdef DEBUG0
                            prints("����δ����ı�׼��������\r\n");
                            #endif
                            break;
                    }
                    break;

                case 1: /* class request */
                    #ifdef DEBUG0
                    prints("USB����������\r\n");
                    #endif
                    break;

                case 2: /* manufacture request */
                    #ifdef DEBUG0
                    prints("USB������������\r\n");
                    #endif
                    break;

                default:
                    #ifdef DEBUG0
                    prints("����δ�������������\r\n");
                    #endif
                    break;

            }
        }
        else {
            switch ((bmRequestType>>5) & 0x03) {
                case 0: /* standard request */
                    #ifdef DEBUG0
                    prints("USB��׼�������");
                    #endif

                    /* the following is some standard request */
                    switch (bRequest) {
                        case CLEAR_FEATURE:
                            #ifdef DEBUG0
                            prints("������ԡ�\r\n");
                            #endif
                            break;

                        case SET_ADDRESS:
                            #ifdef DEBUG0
                            prints("���õ�ַ����ַΪ��");
                            printHex(wValue&0xFF);
                            prints("\r\n");
                            #endif
                            d12_setaddr(wValue & 0xff);
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            usb_ep0_senddata();
                            break;

                        case SET_CONFIGURATION:
                            #ifdef DEBUG0
                            prints("�������á�\r\n");
                            #endif
                            configvalue = wValue & 0xff;
                            d12_setendpenable(configvalue);
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            usb_ep0_senddata();
                            break;

                        case SET_DESCRIPTOR:
                            #ifdef DEBUG0
                            prints("������������\r\n");
                            #endif
                            break;

                        case SET_FEATURE:
                            #ifdef DEBUG0
                            prints("�������ԡ�\r\n");
                            #endif
                            break;

                        case SET_INTERFACE:
                            #ifdef DEBUG0
                            prints("���ýӿڡ�\r\n");
                            #endif
                            break;

                        default:
                            #ifdef DEBUG0
                            prints("����δ����ı�׼�������\r\n");
                            #endif
                            break;
                    }
                    break;

                case 1:
                    #ifdef DEBUG0
                    prints("USB���������");
                    #endif
                    switch (bRequest) {
                        case SET_IDLE:
                            #ifdef DEBUG0
                            prints("���ÿ��С�\r\n");
                            #endif
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            usb_ep0_senddata();
                            break;

                        default:
                            #ifdef DEBUG0
                            prints("δ֪����\r\n");
                            #endif
                            break;
                    }
                    break;

                case 2:
                    #ifdef DEBUG0
                    prints("USB�����������\r\n");
                    #endif
                    break;

                default:
                    #ifdef DEBUG0
                    prints("����δ������������\r\n");
                    #endif
                    break;
            }
        }
    }
    else {
        d12_readendpbuffer(0, 16, edp0_buffer);
        d12_clearbuffer();
    }
}

void usb_ep0in(void)
{
    #ifdef DEBUG0
    prints("USB�˵�0�����жϡ�\r\n");
    #endif
    d12_readendpoint_laststatus(1);
    usb_ep0_senddata();
}

void usb_ep1out(void)
{
    #ifdef DEBUG0
    prints("USB�˵�1����жϡ�\r\n");
    #endif
}

void usb_ep1in(void)
{
    #ifdef DEBUG0
    prints("USB�˵�1�����жϡ�\r\n");
    #endif
    d12_readendpoint_laststatus(3);
    ep1in_isbusy = 0;
}

void usb_ep2out(void)
{
    #ifdef DEBUG0
    prints("USB�˵�2����жϡ�\r\n");
    #endif
}

void usb_ep2in(void)
{
    #ifdef DEBUG0
    prints("USB�˵�2�����жϡ�\r\n");
    #endif
}




