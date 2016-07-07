//////////////////////////////////////////////////////////////////////////////////
//开发人员：zhangys
//当前版本：v1.00
//创建时间：2013/3/24
//修改时间：2013/3/24
//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"
#include "w55fa93_reg.h"
#include "usbd.h"
#include "usbclass.h"
#include "Defines.h"
#include "Userusb.h"
#include "UsbInfo.h"

extern UINT32 gUsrHidPacket;
extern UINT8 *gcUserBuf;
extern UINT8 gcSensorType;



#define	CACHE_ON						0x00000000
#define	CACHE_OFF						0x80000000
#define	CACHE_MODE						CACHE_OFF


/* Mass_Storage command base address */
extern volatile USBD_INFO_T usbdInfo;

/* USB Device Property */
extern USB_CMD_T	_usb_cmd_pkt;
UINT32 volatile  g_u32MscMassAddr;
extern StuHid gStuHidTrans;

/*********************************************************************************************************
** 函数名称：USB_Timer_Handler
** 函数描述：中断处理函数
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void USB_Timer_Handler (void)
{
	if(gStuHidTrans.ucRcvFlg==1)
	{
		if(gStuHidTrans.ucRcvTime>0)
		{
			gStuHidTrans.ucRcvTime --;
			if(gStuHidTrans.ucRcvTime == 0)
				gStuHidTrans.ucRcvFlg = 2;
		}
	}
	
}

extern UINT8 *Sprintfs;

//////////////////////////////////////////////////////////////////////////////////
//维尔数据发送
//zhangys
//20130131
//////////////////////////////////////////////////////////////////////////////////
extern void UsbProcessFailed(INT32 *ilen);
void WelSendHID_Data(void)
{
	UINT32 i;
	UINT8 iret;
	INT32 len;
	
	iret=wellcom_check_data();
	
	if(iret==0)
	{
		UsbCmdProcess(gcUserBuf,&len);
	}
	else
	{
		BlinkY2(1,20);
		UsbProcessFailed(&len);
		BlinkY2(1,20);
	}

	for (i= 0; i<len; i++)
	{
		outp8(HIDInfo.Base_Addr+i,gcUserBuf[i]);
	}
	
	if (len%64!=0)
	{
		len= (len/64+1)*64;
	}
	
	for ( ;i<len; i++)
	{
		outp8(HIDInfo.Base_Addr+i,0x00);
	}
	mscdSDRAM2USB_Bulk(HIDInfo.Base_Addr ,len);
}
VOID HidResetCallBack(VOID)
{
}

/* USB Endpoint B Interrupt Callback function */
VOID HidRxdCallBack(UINT32 u32IntEn,UINT32 u32IntStatus)
{
	UINT32 len;
	INT Lenth=0;
	
	/* Receive data from HOST (Data) */
	if(u32IntStatus & DATA_RxED_IS)
	{
		HIDInfo.Rxed_Flag = 1;
		len = inp32(EPB_DATA_CNT);
		mscdUSB2SDRAM_Bulk(HIDInfo.Base_Addr, len);
	
		memcpy(gcUserBuf+(gStuHidTrans.iPacket*len),(UINT8*)HIDInfo.Base_Addr,len);	
		gStuHidTrans.iPacket++;
		//第一包数据
		if(gStuHidTrans.iPacket==1)
		{
			if(gcUserBuf[0] ==0x02)
			{
				Lenth = gcUserBuf[1]*256 + gcUserBuf[2];	//除规定部分长度
				Lenth	+= 5;//数据总长度
				//数据长度小于等于64认为接收完成
				if(Lenth<=64)
					gStuHidTrans.ucRcvFlg =2;
				//数据长度大于64继续接收
				else
				{
					gStuHidTrans.ucRcvFlg =1;
					gStuHidTrans.ucRcvTime = USB_HID_TIMEOUT;
				}
			}
			//不清楚头的情况下只能等超时处理
			else
			{
				gStuHidTrans.ucRcvFlg =1;
				gStuHidTrans.ucRcvTime = USB_HID_TIMEOUT;
			}
		}
		else
		{
			gStuHidTrans.ucRcvFlg =1;
			gStuHidTrans.ucRcvTime = USB_HID_TIMEOUT;
		}
    }
		
}

/* USB Endpoint A Interrupt Callback function */
VOID HidTxdCallBack(UINT32 u32IntEn,UINT32 u32IntStatus)
{
	/* Send data to HOST (CSW/Data) */
	if(u32IntStatus & DATA_TxED_IS)
	{
		HIDInfo.Txed_Flag=1;
	}
}
VOID USB_DmaCallBack(VOID)
{
}


/* USB Transfer (DMA configuration) */
void mscdSDRAM_USB_Transfer(UINT32 DRAM_Addr ,UINT32 Tran_Size)
{
	
	outp32(USB_IRQ_ENB, (USB_DMA_REQ | USB_RST_STS | USB_SUS_REQ|VBUS_IE|SOF_IE));
	outp32(AHB_DMA_ADDR, DRAM_Addr);
	outp32(DMA_CNT, Tran_Size);

	
	if((inp32(DMA_CTRL_STS) & 0x03) == 0x01)
		HIDInfo.Txed_Flag = 0;



	outp32(DMA_CTRL_STS, inp32(DMA_CTRL_STS)|0x00000020);


	if((inp32(DMA_CTRL_STS) & 0x03) == 0x01)
	{
		while((inp32(EPA_DATA_CNT) != 0)&&(HIDInfo.Txed_Flag   == 0));
		
	}
	else
	{
		while((inp32(EPB_DATA_CNT) != 0)&&(HIDInfo.Rxed_Flag == 0));
		
	}
	outp32(USB_IRQ_STAT,DMACOM_IS);

}

/* USB means usb host, sdram->host=> bulk in */
void mscdSDRAM2USB_Bulk(UINT32 DRAM_Addr ,UINT32 Tran_Size)
{
	UINT32 volatile count=0;

	outp32(DMA_CTRL_STS, 0x11);	// bulk in, dma read, ep1
	if (Tran_Size < usbdInfo.usbdMaxPacketSize)
	{
		while(usbdInfo.USBModeFlag)
		{
			if (inp32(EPA_IRQ_STAT) & 0x02)
			{
				mscdSDRAM_USB_Transfer(DRAM_Addr, Tran_Size);
				break;
			}
		}
	}
	else if (Tran_Size <= HIDInfo.USBD_DMA_LEN)
	{
		count = Tran_Size / usbdInfo.usbdMaxPacketSize;
		if (count != 0)
		{
			while(usbdInfo.USBModeFlag)
			{
				if (inp32(EPA_IRQ_STAT) & 0x02)
				{
					mscdSDRAM_USB_Transfer(DRAM_Addr, usbdInfo.usbdMaxPacketSize*count);
					break;
				}
			}
		}

		if ((Tran_Size % usbdInfo.usbdMaxPacketSize) != 0)
		{
			while(usbdInfo.USBModeFlag)
			{
				if (inp32(EPA_IRQ_STAT) & 0x02)
				{
					mscdSDRAM_USB_Transfer((DRAM_Addr+count*usbdInfo.usbdMaxPacketSize),(Tran_Size%usbdInfo.usbdMaxPacketSize));
					break;
				}
			}
		}
	}

}
/* USB means usb host, host->sdram => bulk out */
void mscdUSB2SDRAM_Bulk(UINT32 DRAM_Addr ,UINT32 Tran_Size)
{
	unsigned int volatile count=0;
	int volatile i;

	outp32(DMA_CTRL_STS, 0x02);	// bulk out, dma write, ep2

	if (Tran_Size >= HIDInfo.USBD_DMA_LEN)
	{
		count = Tran_Size / HIDInfo.USBD_DMA_LEN;
		for (i=0; i<count; i++)
			mscdSDRAM_USB_Transfer((DRAM_Addr+i*HIDInfo.USBD_DMA_LEN),HIDInfo.USBD_DMA_LEN);

		if ((Tran_Size % HIDInfo.USBD_DMA_LEN) != 0)
			mscdSDRAM_USB_Transfer((DRAM_Addr+i*HIDInfo.USBD_DMA_LEN),(Tran_Size%HIDInfo.USBD_DMA_LEN));
	}
	else
		mscdSDRAM_USB_Transfer(DRAM_Addr,Tran_Size);
}


/* USB Class Data IN Callback function for Get MaxLun Command */
VOID USB_ClassDataInCallBack(VOID)
{
	if (_usb_cmd_pkt.bRequest == GET_MAX_LUN)
	{
		if(_usb_cmd_pkt.wValue != 0 || _usb_cmd_pkt.wIndex != 0  || _usb_cmd_pkt.wLength != 1)
		{
			/* Invalid Get MaxLun Command */
			outp32(CEP_IRQ_ENB, (inp32(CEP_IRQ_ENB) | (CEP_SETUP_TK_IE | CEP_SETUP_PK_IE)));
			outp32(CEP_CTRL_STAT, CEP_SEND_STALL);
		}
		else
		{
			/* Valid Get MaxLun Command */
			outp8(CEP_DATA_BUF, (HIDInfo.Mass_LUN - 1));
			outp32(IN_TRNSFR_CNT, 1);
		}
	}
	else
	{
		/* Valid GET Command */
		outp32(CEP_IRQ_ENB, (inp32(CEP_IRQ_ENB) | (CEP_SETUP_TK_IE | CEP_SETUP_PK_IE)));
		outp32(CEP_CTRL_STAT, CEP_SEND_STALL);
	}
}

/* USB Class Data OUT Callback function for BOT MSC Reset Request */
VOID USB_ClassDataOutCallBack(VOID)
{
	if(_usb_cmd_pkt.bRequest == BULK_ONLY_MASS_STORAGE_RESET)
	{
		if(_usb_cmd_pkt.wValue != 0 || _usb_cmd_pkt.wIndex != 0 || _usb_cmd_pkt.wLength != 0)
		{
			/* Invalid BOT MSC Reset Command */
			outp32(CEP_IRQ_ENB, (inp32(CEP_IRQ_ENB) | (CEP_SETUP_TK_IE | CEP_SETUP_PK_IE)));
			outp32(CEP_CTRL_STAT, CEP_SEND_STALL);
		}
		else
		{
			/* Valid BOT MSC Reset Command */
			HIDInfo.Rxed_Flag = 0;
			outp32(USB_IRQ_ENB, (RST_IE|SUS_IE|RUM_IE|VBUS_IE|SOF_IE));
			outp32(CEP_IRQ_STAT, ~(CEP_SETUP_TK_IS | CEP_SETUP_PK_IS));
			outp32(CEP_CTRL_STAT,FLUSH);
			outp32(EPA_RSP_SC, 0);
			outp32(EPA_RSP_SC, BUF_FLUSH);	/* flush fifo */
			outp32(EPA_RSP_SC, TOGGLE);
			outp32(EPB_RSP_SC, 0);
			outp32(EPB_RSP_SC, BUF_FLUSH);	/* flush fifo */
			outp32(EPB_RSP_SC, TOGGLE);
			outp32(CEP_CTRL_STAT, CEP_ZEROLEN);
		}
	}
	else
	{
		/* Invalid SET Command */
		outp32(CEP_IRQ_ENB, (inp32(CEP_IRQ_ENB) | (CEP_SETUP_TK_IE | CEP_SETUP_PK_IE)));
		outp32(CEP_CTRL_STAT, CEP_SEND_STALL);
	}
}

/* High Speed Init */
void hidHighSpeedInit()
{
	usbdInfo.usbdMaxPacketSize = 0x00000040;
	outp32(EPA_MPS, 0x00000040);		// mps 512
	while(inp32(EPA_MPS) != 0x00000040);		// mps 512

	/* bulk in */
	outp32(EPA_IRQ_ENB, 0x00000008);	// tx transmitted
	outp32(EPA_RSP_SC, 0x00000000);		// auto validation
	outp32(EPA_MPS, 0x00000040);		// mps 512
	outp32(EPA_CFG, 0x0000001d);//outp32(EPA_CFG, 0x0000001E);		// bulk in ep no 1
	outp32(EPA_START_ADDR, 0x00000080);
	outp32(EPA_END_ADDR, 0x000003ff);
	
	/* bulk out */
	outp32(EPB_IRQ_ENB, 0x00000010);	// data pkt received  and outtokenb
	outp32(EPB_RSP_SC, 0x00000000);		// auto validation
	outp32(EPB_MPS, 0x00000040);		// mps 512
	outp32(EPB_CFG, 0x00000025);		// bulk out ep no 2
	outp32(EPB_START_ADDR, 0x00000400);
	outp32(EPB_END_ADDR, 0x000007ff);
}

/* Full Speed Init */
VOID hidFullSpeedInit(VOID)
{
	usbdInfo.usbdMaxPacketSize = 0x40;
	/* bulk in */
	outp32(EPA_IRQ_ENB, 0x00000008);	// tx transmitted
	outp32(EPA_RSP_SC, 0x00000000);		// auto validation
	outp32(EPA_MPS, 0x00000040);		// mps 64
	outp32(EPA_CFG, 0x0000001d);		//outp32(EPA_CFG, 0x0000001E);		// interrupt in ep no 1
	outp32(EPA_START_ADDR, 0x00000100);
	outp32(EPA_END_ADDR, 0x0000017f);

	/* bulk out */
	outp32(EPB_IRQ_ENB, 0x00000010);	// data pkt received  and outtokenb
	outp32(EPB_RSP_SC, 0x00000000);		// auto validation
	outp32(EPB_MPS, 0x00000040);		// mps 64
	outp32(EPB_CFG, 0x00000025);		// bulk out ep no 2
	outp32(EPB_START_ADDR, 0x00000200);
	outp32(EPB_END_ADDR, 0x0000027f);
}

VOID mscdInit(VOID)
{
	/* Set Endpoint map */
	usbdInfo.i32EPA_Num = 1;	/* Endpoint 1 */
	usbdInfo.i32EPB_Num = 2;	/* Endpoint 2 */

	/* Set Callback Function */
	usbdInfo.pfnDMACompletion 			= USB_DmaCallBack;
	usbdInfo.pfnClassDataINCallBack		= USB_ClassDataInCallBack;
	usbdInfo.pfnClassDataOUTCallBack 	= USB_ClassDataOutCallBack;
	usbdInfo.pfnReset 	 				= HidResetCallBack;
	usbdInfo.pfnEPBCallBack 			= HidRxdCallBack;
	usbdInfo.pfnEPACallBack 			= HidTxdCallBack;

	/* Set MSC initialize function */
	usbdInfo.pfnFullSpeedInit 			= hidFullSpeedInit;
	usbdInfo.pfnHighSpeedInit 			= hidHighSpeedInit;

	/* Set Descriptor pointer */
	usbdInfo.pu32DevDescriptor 			= (PUINT32) &USB_DeviceDescriptor;
	usbdInfo.pu32QulDescriptor 			= (PUINT32) &USB_QualifierDescriptor;
	usbdInfo.pu32HSConfDescriptor 		= (PUINT32) &USB_ConfigurationBlock;
	usbdInfo.pu32FSConfDescriptor 		= (PUINT32) &USB_ConfigurationBlock;
	usbdInfo.pu32HOSConfDescriptor 		= (PUINT32) &USB_ConfigurationBlock;
	usbdInfo.pu32FOSConfDescriptor 		= (PUINT32) &USB_ConfigurationBlock;
	usbdInfo.pu32HIDDescriptor			= (PUINT32) &HID_Discr;
	usbdInfo.pu32StringDescriptor[0] 	= (PUINT32) &USB_StringDescriptor;
	//zhangys 2013-7-26 12:14:03 解决部分电脑无法正常启动
	usbdInfo.pu32StringDescriptor[1] 	= (PUINT32) &USB_stringMAXLUN;
	usbdInfo.pu32HIDRPTDescriptor		= (PUINT32) &HID_Discr;

	/* Set Descriptor length */
	usbdInfo.u32DevDescriptorLen 		=  USB_DEVICE_DSCPT_LEN;
	usbdInfo.u32HSConfDescriptorLen 	=  USB_CONFIG_DSCPT_LEN;
	usbdInfo.u32FSConfDescriptorLen 	=  USB_CONFIG_DSCPT_LEN;
	usbdInfo.u32HOSConfDescriptorLen 	=  USB_CONFIG_DSCPT_LEN;
	usbdInfo.u32FOSConfDescriptorLen 	=  USB_CONFIG_DSCPT_LEN;
	usbdInfo.u32HIDDescriptorLen		=  HID_DESC_SIZE;
	usbdInfo.u32StringDescriptorLen[0]	=  USB_STR0_DSCPT_LEN;
	usbdInfo.u32QulDescriptorLen 		=  USB_QUALIFIER_DSCPT_LEN;

	/* Set MSC property */
	HIDInfo.USBD_DMA_LEN = 0x20000;
	HIDInfo.Mass_LUN=0;

	g_u32MscMassAddr = (UINT32) malloc(sizeof(UINT8) * 1055);
	HIDInfo.Base_Addr = (g_u32MscMassAddr + 0x1F) & ~0x1F;
	HIDInfo.Base_Addr |= CACHE_MODE;

}

VOID mscdDeinit(VOID)
{
	free((UINT8*)g_u32MscMassAddr);
}
