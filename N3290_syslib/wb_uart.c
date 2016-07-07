/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technolog. All rights reserved.              *
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
* FILENAME
*   wb_uart.c
*
* VERSION
*   1.0
*
* DESCRIPTION
*   The UART related function of Nuvoton ARM9 MCU
*
* HISTORY
*   2008-06-25  Ver 1.0 draft by Min-Nan Cheng
*
* REMARK
*   None
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"

#include "w55fa93_reg.h"
#include "Userusart.h"
extern StruUART gStruUart0;
extern StruUART gStruUart1;

#pragma Otime

#ifdef	__HW_SIM__
#undef 	REG_UART_THR                       
#define	REG_UART_THR  	(0xFFF04400)		// TUBE ON
#endif

#define LEVEL_30_BYTES     0x4
#define LEVEL_46_BYTES     0x5
#define LEVEL_62_BYTES     0x6	

#define vaStart(list, param) list = (INT8*)((INT)&param + sizeof(param))
#define vaArg(list, type) ((type *)(list += sizeof(type)))[-1]

/* Global variables */
BOOL volatile _sys_bIsUARTInitial = FALSE;
BOOL volatile _sys_bIsUseUARTInt = TRUE;
UINT32 _sys_uUARTClockRate = EXTERNAL_CRYSTAL_CLOCK;


#define sysTxBufReadNextOne()	(((_sys_uUartTxHead+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead+1)
#define sysTxBufWriteNextOne()	(((_sys_uUartTxTail+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail+1)
#define UART_BUFFSIZE	1024+256
UINT8 _sys_ucUartTxBuf[UART_BUFFSIZE];
UINT32 volatile _sys_uUartTxHead, _sys_uUartTxTail;
PVOID  _sys_pvOldUartVect;

UINT32 u32UartPort =0x100;


void sysUartPort(UINT32 u32Port)
{
	
	u32UartPort = (u32Port & 0x1)*0x100;
	if(u32Port==0)
	{//High Speed UART
		outp32(REG_GPDFUN, (inp32(REG_GPDFUN) & ~(MF_GPD2 | MF_GPD1)) | (0x5<<2) );
		outp32(REG_APBCLK, inp32(REG_APBCLK) | UART0_CKE);
	}
	else if(u32Port==1)
	{//Nornal Speed UART
		outp32(REG_GPAFUN, inp32(REG_GPAFUN) | (MF_GPA11 | MF_GPA10));
		outp32(REG_APBCLK, inp32(REG_APBCLK) | UART1_CKE);
	}
}

VOID sysUart1ISR()
{
	UINT32 volatile regIIR,regIER,i;
	UINT32 volatile u32Count;
	
	regIER = inpw(REG_UART_IER + 0x100);
	regIIR = inpw(REG_UART_ISR + 0x100);

	if ((regIIR & THRE_IF) & ((regIER & THRE_IEN)))
	{	// buffer empty
		if (_sys_uUartTxHead == _sys_uUartTxTail)	
		{	//Disable interrupt if no any request!  
			outpw((REG_UART_IER + 0x100), inpw(REG_UART_IER + 0x100) & (~THRE_IEN));
		}	
		else
		{	//Transmit data 
			for (i=0; i<8; i++)
			{
#ifdef __HW_SIM__

#else			
				outpb(REG_UART_THR + 0x100, _sys_ucUartTxBuf[_sys_uUartTxHead]);
#endif								
				_sys_uUartTxHead = sysTxBufReadNextOne();
				if (_sys_uUartTxHead == _sys_uUartTxTail)	// buffer empty
					break;
			}
		}
	}
	
	if((regIIR & RDA_IF) & (regIER & RDA_IEN))
	{
		u32Count = (inpw(REG_UART_FSR + 0x100) & Rx_Pointer) >> 8;				
		for(i=0;i<u32Count;i++)
		{ 		
			gStruUart1.rx_buf[gStruUart1.rx_cnt] = (inpb(REG_UART_RBR + 0x100));
			gStruUart1.rx_cnt++;
		}
		gStruUart1.rx_tim = gStruUart1.time_out;									//接收超时中断5ms;
		
	}

}
VOID sysUart0ISR()
{
	UINT32 volatile regIIR,regIER,i;
	UINT32 volatile u32Count;
	regIER = inpw(REG_UART_IER + 0*0x100);
	regIIR = inpw(REG_UART_ISR + 0*0x100);

	if ((regIIR & THRE_IF) & ((regIER & THRE_IEN)))
	{	// buffer empty
		if (_sys_uUartTxHead == _sys_uUartTxTail)	
		{	//Disable interrupt if no any request!  
			outpw((REG_UART_IER + 0*0x100), inpw(REG_UART_IER + 0*0x100) & (~THRE_IEN));
		}	
		else
		{	//Transmit data 
			for (i=0; i<8; i++)
			{
#ifdef __HW_SIM__

#else			
				outpb(REG_UART_THR + 0*0x100, _sys_ucUartTxBuf[_sys_uUartTxHead]);
#endif								
				_sys_uUartTxHead = sysTxBufReadNextOne();
				if (_sys_uUartTxHead == _sys_uUartTxTail)	// buffer empty
					break;
			}
		}
	}
	if((regIIR & RDA_IF) & (regIER & RDA_IEN))
	{
		u32Count = (inpw(REG_UART_FSR + 0*0x100) & Rx_Pointer) >> 8;				
		for(i=0;i<u32Count;i++)
		{ 		
			gStruUart0.rx_buf[gStruUart0.rx_cnt] = (inpb(REG_UART_RBR + 0*0x100));
			gStruUart0.rx_cnt++;
		}
		gStruUart0.rx_tim = gStruUart0.time_out;									//接收超时中断5ms;

	}

}
static VOID sysSetBaudRate(UINT32 uBaudRate)
{
	UINT32 _mBaudValue;

	/* First, compute the baudrate divisor. */
#if 0		
	// mode 0
	_mBaudValue = (_sys_uUARTClockRate / (uBaudRate * 16));
	if ((_sys_uUARTClockRate % (uBaudRate * 16)) > ((uBaudRate * 16) / 2))
	  	_mBaudValue++;
	_mBaudValue -= 2;
	outpw(REG_UART_BAUD+u32UartPort, _mBaudValue);		
#else
	// mode 3
	_mBaudValue = (_sys_uUARTClockRate / uBaudRate)-2;
	outpw(REG_UART_BAUD+u32UartPort,  ((0x30<<24)| _mBaudValue));	
#endif	
}


INT32 sysInitializeUART(WB_UART_T *uart)
{
	/* Enable UART multi-function pins*/
	//outpw(REG_PINFUN, inpw(REG_PINFUN) | 0x80);
	//outpw(REG_GPAFUN, inpw(REG_GPAFUN) | 0x00F00000);	//Normal UART pin function 

	/* Check the supplied parity */
	if ((uart->uiParity != WB_PARITY_NONE) &&
	    (uart->uiParity != WB_PARITY_EVEN) &&
	    (uart->uiParity != WB_PARITY_ODD))

	    	/* The supplied parity is not valid */
	    	return WB_INVALID_PARITY;

	/* Check the supplied number of data bits */
	else if ((uart->uiDataBits != WB_DATA_BITS_5) &&
	         (uart->uiDataBits != WB_DATA_BITS_6) &&
	         (uart->uiDataBits != WB_DATA_BITS_7) &&
	         (uart->uiDataBits != WB_DATA_BITS_8))

	    	/* The supplied data bits value is not valid */
	    	return WB_INVALID_DATA_BITS;

	/* Check the supplied number of stop bits */
	else if ((uart->uiStopBits != WB_STOP_BITS_1) &&
	         (uart->uiStopBits != WB_STOP_BITS_2))

	    	/* The supplied stop bits value is not valid */
	    	return WB_INVALID_STOP_BITS;

	/* Verify the baud rate is within acceptable range */
	else if (uart->uiBaudrate < 1200)
	    	/* The baud rate is out of range */
	    	return WB_INVALID_BAUD;

	/* Reset the TX/RX FIFOs */
	outpw(REG_UART_FCR+u32UartPort, 0x07);

	/* Setup reference clock */
	_sys_uUARTClockRate = uart->uiFreq;

	/* Setup baud rate */
	sysSetBaudRate(uart->uiBaudrate);

	/* Set the modem control register. Set DTR, RTS to output to LOW,
	and set INT output pin to normal operating mode */ 
	//outpb(UART_MCR, (WB_DTR_Low | WB_RTS_Low | WB_MODEM_En)); 

	/* Setup parity, data bits, and stop bits */
	outpw(REG_UART_LCR+u32UartPort,(uart->uiParity | uart->uiDataBits | uart->uiStopBits));

	/* Timeout if more than ??? bits xfer time */
	outpw(REG_UART_TOR+u32UartPort, 0x80+0x20);

	/* Setup Fifo trigger level and enable FIFO */
	outpw(REG_UART_FCR+u32UartPort, (uart->uiRxTriggerLevel << 4)|0x01);

	// hook UART interrupt service routine
	if (u32UartPort)
	{
		/* Enable HUART interrupt Only (Receive Data Available Interrupt & RX Time out Interrupt) */
		outpw(REG_UART_IER+u32UartPort,inpw(REG_UART_IER+u32UartPort) |RDA_IEN |RTO_IEN |Time_out_EN);
			
		_sys_uUartTxHead = _sys_uUartTxTail = NULL;
		_sys_pvOldUartVect = sysInstallISR(IRQ_LEVEL_1, IRQ_UART, (PVOID)sysUart1ISR);
		sysEnableInterrupt(IRQ_UART);
		sysSetLocalInterrupt(ENABLE_IRQ);
	}
	else
	{

		/* Enable HUART interrupt Only (Receive Data Available Interrupt & RX Time out Interrupt) */
		outpw(REG_UART_IER+u32UartPort,inpw(REG_UART_IER+u32UartPort) |RDA_IEN |RTO_IEN |Time_out_EN);
			
		_sys_uUartTxHead = _sys_uUartTxTail = NULL;
		_sys_pvOldUartVect = sysInstallISR(IRQ_LEVEL_2, IRQ_HUART, (PVOID)sysUart0ISR);
		sysEnableInterrupt(IRQ_HUART);
		sysSetLocalInterrupt(ENABLE_IRQ);
	}
	
	_sys_bIsUARTInitial = TRUE;

	return Successful;
}


VOID _PutChar_f(UINT8 ucCh,UINT8 uartno)
{

	if (_sys_bIsUseUARTInt == TRUE)
	{
		while(sysTxBufWriteNextOne() == _sys_uUartTxHead) ;	// buffer full

		_sys_ucUartTxBuf[_sys_uUartTxTail] = ucCh;
		_sys_uUartTxTail = sysTxBufWriteNextOne();


		if (!(inpw(REG_UART_IER+uartno*0x100) & THRE_IEN))
		outpw(REG_UART_IER+uartno*0x100, inpw(REG_UART_IER+0x100) | THRE_IEN);
	}
	else
	{
		/* Wait until the transmitter buffer is empty */		
		while (!(inpw(REG_UART_FSR+uartno*0x100) & 0x400000));
		/* Transmit the character */
		outpb(REG_UART_THR+uartno*0x100, ucCh);

	}
}

VOID sendbyte(UINT8 ucCh)
{
	if (_sys_bIsUseUARTInt == TRUE)
	{
		while(sysTxBufWriteNextOne() == _sys_uUartTxHead) ;	// buffer full

		_sys_ucUartTxBuf[_sys_uUartTxTail] = ucCh;
		_sys_uUartTxTail = sysTxBufWriteNextOne();

		if (!(inpw(REG_UART_IER+0x100) & THRE_IEN))
			outpw(REG_UART_IER+0x100, inpw(REG_UART_IER+0x100) | THRE_IEN);
	}
	else
	{
		/* Wait until the transmitter buffer is empty */		
		while (!(inpw(REG_UART_FSR+u32UartPort) & 0x400000));
		/* Transmit the character */
		outpb(REG_UART_THR+u32UartPort, ucCh);
	}
}

VOID sysPutString(INT8 *string)
{
	while (*string != '\0')
	{
		_PutChar_f(*string,1);
		string++;
	}
}


static VOID sysPutRepChar(INT8 c, INT count)
{
	while (count--)
	sendbyte(c);
}


static VOID sysPutStringReverse(INT8 *s, INT index)
{
	while ((index--) > 0)
	_PutChar_f(s[index],1);
}

static VOID sysPutNumber(INT value, INT radix, INT width, INT8 fill)
{
	INT8    buffer[40];
	INT     bi = 0;
	UINT32  uvalue;
	UINT16  digit;
	UINT16  left = FALSE;
	UINT16  negative = FALSE;

	if (fill == 0)
	    	fill = ' ';

	if (width < 0)
	{
		width = -width;
		left = TRUE;
	}

	if (width < 0 || width > 80)
	    	width = 0;

	if (radix < 0)
	{
		radix = -radix;
		if (value < 0)
		{
			negative = TRUE;
			value = -value;
	    	}
	}

	uvalue = value;

	do
	{
		if (radix != 16)
		{
			digit = uvalue % radix;
			uvalue = uvalue / radix;
		}
		else
		{
			digit = uvalue & 0xf;
			uvalue = uvalue >> 4;
		}
		buffer[bi] = digit + ((digit <= 9) ? '0' : ('A' - 10));
		bi++;

		if (uvalue != 0)
		{
			if ((radix == 10)
			    && ((bi == 3) || (bi == 7) || (bi == 11) | (bi == 15)))
			{
				buffer[bi++] = ',';
			}
		}
	}
	while (uvalue != 0);

	if (negative)
	{
		buffer[bi] = '-';
		bi += 1;
	}

	if (width <= bi)
		sysPutStringReverse(buffer, bi);
	else
	{
		width -= bi;
		if (!left)
			sysPutRepChar(fill, width);
		sysPutStringReverse(buffer, bi);
		if (left)
		    	sysPutRepChar(fill, width);
	}
}

static INT8 *FormatItem(INT8 *f, INT a)
{
	INT8   c;
	INT    fieldwidth = 0;
	INT    leftjust = FALSE;
	INT    radix = 0;
	INT8   fill = ' ';

	if (*f == '0')
		fill = '0';

	while ((c = *f++) != 0)
	{
		if (c >= '0' && c <= '9')
		{
			fieldwidth = (fieldwidth * 10) + (c - '0');
		}
		else
			switch (c)
			{
				case '\000':
					return (--f);
				case '%':
				    	_PutChar_f('%',1);
				    	return (f);
				case '-':
				    	leftjust = TRUE;
				    	break;
				case 'c':
				{
				        if (leftjust)
				        	_PutChar_f(a & 0x7f,1);

				        if (fieldwidth > 0)
				            	sysPutRepChar(fill, fieldwidth - 1);

				        if (!leftjust)
				            	_PutChar_f(a & 0x7f,1);
				        return (f);
				}
				case 's':
				{
				        if (leftjust)
				        	sysPutString((PINT8)a);

				        if (fieldwidth > strlen((PINT8)a))
				            	sysPutRepChar(fill, fieldwidth - strlen((PINT8)a));

				        if (!leftjust)
				           	sysPutString((PINT8)a);
				        return (f);
				}
				case 'd':
				case 'i':
				   	 radix = -10;
				break;
				case 'u':
				    	radix = 10;
				break;
				case 'x':
				    	radix = 16;
				break;
				case 'X':
				    	radix = 16;
				break;
				case 'o':
				    	radix = 8;
				break;
				default:
				    	radix = 3;
				break;      /* unknown switch! */
			}
		if (radix)
		    break;
	}

	if (leftjust)
	    	fieldwidth = -fieldwidth;

	sysPutNumber(a, radix, fieldwidth, fill);

	return (f);
}

VOID sysPrintf(PINT8 pcStr,...)
{
	WB_UART_T uart;
	INT8  *argP;

    	_sys_bIsUseUARTInt = TRUE;
	if (!_sys_bIsUARTInitial)
	{
		uart.uart_no = WB_UART_0;
		uart.uiFreq = sysGetExternalClock()*1000;
		uart.uiBaudrate = 115200;
		uart.uiDataBits = WB_DATA_BITS_8;
		uart.uiStopBits = WB_STOP_BITS_1;
		uart.uiParity = WB_PARITY_NONE;
		uart.uiRxTriggerLevel = LEVEL_1_BYTE;
		sysInitializeUART(&uart);		
    	}
    
	vaStart(argP, pcStr);       /* point at the end of the format string */
	while (*pcStr)
	{                       /* this works because args are all ints */
	    	if (*pcStr == '%')
	        	pcStr = FormatItem(pcStr + 1, vaArg(argP, INT));
	    	else
	        	_PutChar_f(*pcStr++,1);
	}
}

VOID sysprintf(PINT8 pcStr,...)
{
	WB_UART_T uart;
	INT8  *argP;

	_sys_bIsUseUARTInt = FALSE;
	if (!_sys_bIsUARTInitial)
	{//Default use external clock 12MHz as source clock. 
		uart.uart_no = WB_UART_0;
		uart.uiFreq = sysGetExternalClock()*1000;
		uart.uiBaudrate = 115200;
		uart.uiDataBits = WB_DATA_BITS_8;
		uart.uiStopBits = WB_STOP_BITS_1;
		uart.uiParity = WB_PARITY_NONE;
		uart.uiRxTriggerLevel = LEVEL_1_BYTE;
		sysInitializeUART(&uart);
	}

	vaStart(argP, pcStr);       /* point at the end of the format string */
	while (*pcStr)
	{                       /* this works because args are all ints */
		if (*pcStr == '%')
		    	pcStr = FormatItem(pcStr + 1, vaArg(argP, INT));
		else
		    	_PutChar_f(*pcStr++,1);
	}
}

extern void Delayms(UINT16 ms);
int sysGetChar(int port,int timeout,UINT8 *bt)
{
	u32UartPort = (port & 0x1)*0x100;

	while (timeout--)
	{
		if (inpw(REG_UART_ISR+u32UartPort) & 0x01)	
		{
			*bt =  (inpb(REG_UART_RBR+u32UartPort));
			return 1;
		}
		Delayms(1);
	}
	*bt = 0;
	return 0;
}

VOID sysPutChar(UINT8 ucCh)
{
	/* Wait until the transmitter buffer is empty */		
		while (!(inpw(REG_UART_FSR+u32UartPort) & 0x400000));
	/* Transmit the character */
		outpb(REG_UART_THR+u32UartPort, ucCh); 	
}

//zhangys 20130220 发送数据 发送模式
//0 查询 1 中断

//zhangys 20130220 发送数据 发送模式
VOID UartSendByte(int port,UINT8 ucCh)
{
	_sys_bIsUseUARTInt = 0;
	_PutChar_f(ucCh,port);	
}





