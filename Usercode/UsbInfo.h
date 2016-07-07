//*******************************************************************************
//?¶Ã?°¬°ÍoUSB ¶Ã°¡2?D-®∞®¶?®§1?
//®∫°¿??°Ío20130217
//°¡°¬??°Íowellcom
//*******************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"
#define HID_MOUSE_REPORT_DESC_SIZE    	0x22
#define USB_STRING_DESCRIPTOR_TYPE 		0x03
#define USB_VID							0x2796				/* Vendor ID */ 
#define USB_PID							0x0201				/* Product ID */

/* MSC Device Property */
__align(4) volatile HID_INFO_T HIDInfo = {0};

//…Ë±∏√Ë ˆ∑˚
__align(4) UINT8 USB_DeviceDescriptor[USB_DEVICE_DSCPT_LEN] =
{
	USB_DEVICE_DSCPT_LEN,
	0x01,
	0x10, 0x01,			/* bcdUSB - USB 2.0 */
	0x00,
	0x00,
	0x00,
	0x40,				/* bMaxPacketSize0 - 64 Bytes  */
	(USB_VID &0xFF) , ((USB_VID >> 8) & 0xFF) ,			/* Vendor ID */ 
	(USB_PID &0xFF) , ((USB_PID >> 8) & 0xFF),			/* Product ID */
	0x00, 0x01,		
	0,//0x01,				/* iManufacture */
	0,//0x02,				/* iProduct */
	0,//0x03,				/* iSerialNumber */
	0x01				/* bNumConfigurations */
};

__align(4) static UINT32 USB_QualifierDescriptor[3] = 
{
	0x0200060a, 0x40000000, 0x00000001
};

//≈‰÷√√Ë ˆ∑˚
__align(4) UINT8  USB_ConfigurationBlock[]=  			//?????®®®∫?°§?
{
	//≈‰÷√√Ë ˆ∑˚
	0x09,				// bLength: Size of descriptor
	0x02,				// bDescriptorType: Configuration
	0x29,0x00,			// wTotalLength: Cfg+Ifc+Class+Ep = 34 bytes
	0x01,				// bNumInterfaces: 1 interface
	0x01,				// bConfigurationValue: 1
	0x00,				// iConfiguration: none
	0xA0,				// bmAttributes: bus-powered, remote-wakeup
	0x40,				// MaxPower: 500mA

	//Ω”ø⁄√Ë ˆ∑˚

	0x09,				// bLength: Size of descriptor
	0x04,				// bDescriptorType: Interface
	0x00,				// bInterfaceNumber: #0
	0x00,				// bAlternateSetting: #0
	0x02,				// bNumEndpoints: 1
	0x03,				// bInterfaceClass: HID-class
	0x00,				// bInterfaceSubClass: none
	0x00,				// bInterfaceProtocol: BULK-ONLY TRANSPORT.
	0x00,				// iInterface: none

	// HID√Ë ˆ∑˚

	0x09,
	0x21,
	0x10, 0x01,
	0x00,
	0x01,
	0x22,
	HID_MOUSE_REPORT_DESC_SIZE,
	0x00,

	//∂Àµ„√Ë ˆ∑˚

	0x07,				// bLength: Size of descriptor
	0x05,				// bDescriptorType: HID Class
	0x81,
	0x03,				// bcdHID: HID Class version 1.1
	0x40,				// bCountryCode: none
	0x00,				// bNumDescriptors: 1 report descp
	0x01,				// bDescriptorType: report descriptor type


	//∂Àµ„√Ë ˆ∑˚

	0x07,				// bLength: Size of descriptor
	0x05,				// bDescriptorType: Endpoint
	0x02,				// bEndpointAddress: IN, EP1
	0x03,				// bmAttributes: Interrupt
	0x40,
	0x00,				// wMaxPacketSize:
	0x01				// bInterval: 10ms

};	
//±®∏Ê√Ë ˆ∑˚
__align(4) UINT8  HID_Discr [HID_MOUSE_REPORT_DESC_SIZE] =
{

	0x06, 0xff,0xff,
	0x09, 0x01,
	0xa1, 0x01, 	  //?°•o??a®∫?

	0x09, 0x03, 		//Usage Page  ®Æ?°§°ß
	0x15, 0x00, 	    //Logical  Minimun
	0x26, 0x00,0xff,		//Logical  Maximun
	0x75, 0x08,  		//Report Size
	0x95, 0x40,  		//Report Counet
	0x81, 0x02,  		//Input

	0x09, 0x04, 		//Usage Page  ®Æ?°§°ß
	0x15, 0x00, 		//Logical  Minimun
	0x26, 0x00,0xff,	//Logical  Maximun
	0x75, 0x08,  		//Report Size
	0x95, 0x40,  		//Report Counet
	0x91, 0x02,  		//Output

	0xC0

};

//”Ô—‘√Ë ˆ∑˚
__align(4) static UINT32 USB_StringDescriptor[1] = 
{
	LANGID_English_UnitedStates
};

//zhangys 2013-7-26 12:14:03
__align(4) UINT8 USB_stringMAXLUN[] = 
{
	0x00,
};




