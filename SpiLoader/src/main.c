/****************************************************************************
*                                                                           *
* Copyright (c) 2009 Nuvoton Tech. Corp. All rights reserved.               *
*                                                                           *
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"
#include "SpiLoader.h"
#include "w55fa93_gpio.h"

#define E_CLKSKEW 0x00888800
#define __CLK_CTL__
#define __No_RTC__
//#define __UPLL_240__
#define __UPLL_192__
//#define __UPLL_288__

#define	SET_WDI()	gpio_setportval(GPIO_PORTB, 0x1<<2, 0x1<<2);//gpio_setportval(GPIO_PORTE, 0x1<<0, 0x1<<0);
#define	CLR_WDI()	gpio_setportval(GPIO_PORTB, 0x1<<2, 0x0<<2);//gpio_setportval(GPIO_PORTE, 0x1<<0, 0x0<<0);

volatile BOOL gWdiFlag = TRUE;
UINT8 image_buffer[4096];
unsigned char *imagebuf;
unsigned int *pImageList;

void Delayus(UINT32 ulDelay)
{
    UINT32 i=0;

    for(i=0;i<=ulDelay*25;i++);
}

void GpioInit(void)
{
	gpio_setportpull(GPIO_PORTE, 0x1<<0, 0x1<<0);
	gpio_setportdir(GPIO_PORTE, 0x1<<0, 0x1<<0);
	gpio_setportval(GPIO_PORTE, 0x1<<0, 0x1<<0);
	
	gpio_setportpull(GPIO_PORTB, 0x1<<2, 0x1<<2);
	gpio_setportdir(GPIO_PORTB, 0x1<<2, 0x1<<2);
	gpio_setportval(GPIO_PORTB, 0x1<<2, 0x1<<2);
}

void FeedWatchdog(void)
{
	if(gWdiFlag == TRUE)
	{
		gWdiFlag = FALSE;
		SET_WDI();	
	}
	else
	{
		gWdiFlag = TRUE;
		CLR_WDI();	
	}
}

void init(void)
{
	WB_UART_T 	uart;
	UINT32 		u32ExtFreq;	    	    	
	UINT32 u32Cke = inp32(REG_AHBCLK);
	
	/* Reset SIC engine to fix USB update kernel and mvoie file */
	outp32(REG_AHBCLK, u32Cke  | (SIC_CKE | NAND_CKE | SD_CKE)); 
	outp32(REG_AHBIPRST, inp32(REG_AHBIPRST )|SICRST ); 
	outp32(REG_AHBIPRST, 0); 
	outp32(REG_AHBCLK,u32Cke);	
	sysEnableCache(CACHE_WRITE_BACK);
	
	/* init clock */	
#ifdef __CLK_CTL__
	u32ExtFreq = sysGetExternalClock();    	/* KHz unit */	
	outp32(REG_DQSODS, 0x1010);
	outp32(REG_CKDQSDS, E_CLKSKEW);		
    if(u32ExtFreq==12000)
	{
		outp32(REG_SDREF, 0x805A);						
	}
	else
	{
		outp32(REG_SDREF, 0x80C0);						
	}	    				
#ifdef __UPLL_216__	
    #ifdef __DDR_75__	
	outp32(REG_SDTIME, 0x0A129649);	// DDR Speed grade-75
    #endif	
    #ifdef __DDR_6__
	outp32(REG_SDTIME, 0x0A149529);	// DDR Speed grade-6
    #endif	
    #ifdef __DDR_5__
	outp32(REG_SDTIME, 0x09907525);	// DDR Speed grade-5
    #endif
	outp32(REG_SDMR, 0x32);			//Cas Latency = 3
	sysSetSystemClock(eSYS_UPLL, 	//E_SYS_SRC_CLK eSrcClk,	
					216000,		//UINT32 u32PllKHz, 	
					216000,		//UINT32 u32SysKHz,
					216000,		//UINT32 u32CpuKHz,
					216000/2,		//UINT32 u32HclkKHz,
					  216000/4);		//UINT32 u32ApbKHz		  
#endif		
	
#ifdef __UPLL_192__	
	
    #ifdef __DDR_75__
	outp32(REG_SDTIME, 0x098E7549);	// DDR Speed grade-75
    #endif
    #ifdef __DDR_6__		
	outp32(REG_SDTIME, 0x094E7425);	// DDR Speed grade-6
    #endif	
    #ifdef __DDR_5__
	outp32(REG_SDTIME, 0x094E6425);	// DDR Speed grade-5
    #endif
    #ifdef __DDR2__
	outp32(REG_SDMR, 0x32);			//Cas Latency = 3
	outp32(REG_DQSODS, 0x00001212);
	outp32(REG_MISCPCR,0x00000001);	//Driving strength
	outp32(REG_SDTIME, 0x2BDE9649);
	#else
	outp32(REG_SDMR, 0x22);			//Cas Latency = 2
	#endif
	#if 1
	sysSetSystemClock(eSYS_UPLL, 		//E_SYS_SRC_CLK eSrcClk,	
					192000,			//UINT32 u32PllKHz, 	
					192000,			//UINT32 u32SysKHz,
					192000,			//UINT32 u32CpuKHz,
					192000/2,			//UINT32 u32HclkKHz,
					192000/4);		//UINT32 u32ApbKHz	
	#else
	sysSetSystemClock(eSYS_UPLL, 		//E_SYS_SRC_CLK eSrcClk,	
					192000,			//UINT32 u32PllKHz, 	
					96000,			//UINT32 u32SysKHz,
					96000,			//UINT32 u32CpuKHz,
					96000/2,			//UINT32 u32HclkKHz,
					96000/2);		//UINT32 u32ApbKHz	
	#endif											
    
#endif

#ifdef __UPLL_240__	
    #ifdef __DDR_75__	
	outp32(REG_SDTIME, 0x0A129649);	// DDR Speed grade-75
    #endif	
    #ifdef __DDR_6__
	outp32(REG_SDTIME, 0x0A149529);	// DDR Speed grade-6
    #endif	
    #ifdef __DDR_5__
	outp32(REG_SDTIME, 0x09928525);	// DDR Speed grade-5
    #endif	
	outp32(REG_SDMR, 0x32);			//Cas Latency = 3
	sysSetSystemClock(eSYS_UPLL, 		//E_SYS_SRC_CLK eSrcClk,	
					240000,			//UINT32 u32PllKHz, 	
					240000,			//UINT32 u32SysKHz,
					240000,			//UINT32 u32CpuKHz,
					240000/2,			//UINT32 u32HclkKHz,
					240000/4);		//UINT32 u32ApbKHz							
	
#endif

#ifdef __UPLL_288__	
    #ifdef __DDR_75__	
	outp32(REG_SDTIME, 0x0A129649);	// DDR Speed grade-75
    #endif	
    #ifdef __DDR_6__
	outp32(REG_SDTIME, 0x0A14A74A);	// DDR Speed grade-6
    #endif	
    #ifdef __DDR_5__
	outp32(REG_SDTIME, 0x0A14A64A);	// DDR Speed grade-5
    #endif	
	outp32(REG_SDMR, 0x32);			//Cas Latency = 3
	outp32(REG_MISCPCR, 0x1);		//Driving strength	
	sysSetSystemClock(eSYS_UPLL, 		//E_SYS_SRC_CLK eSrcClk,	
					288000,			//UINT32 u32PllKHz, 	
					288000,			//UINT32 u32SysKHz,
					288000,			//UINT32 u32CpuKHz,
					288000/2,			//UINT32 u32HclkKHz,
					288000/4);		//UINT32 u32ApbKHz							
#endif

#endif
    
    /* enable UART */
    sysUartPort(1);
	uart.uiFreq = u32ExtFreq*1000;					/* Hz unit */	
	uart.uiBaudrate = 115200;
	uart.uiDataBits = WB_DATA_BITS_8;
	uart.uiStopBits = WB_STOP_BITS_1;
	uart.uiParity = WB_PARITY_NONE;
	uart.uiRxTriggerLevel = LEVEL_1_BYTE;
	sysInitializeUART(&uart);
	    
	sysprintf("\r\nSPI NVT Loader Start\r\n");
	
	sysSetLocalInterrupt(ENABLE_IRQ);			
}

int main(void)
{
	unsigned int startBlock;
	unsigned int endBlock;
	unsigned int fileLen;
	unsigned int executeAddr;
	
	int count, i;	
	void	(*fw_func)(void);
#ifndef __No_RTC__
	outp32(AER,0x0000a965);	 	
	 	
	while(1)
	{
		if((inp32(AER) & 0x10000) ==0x10000)
			break;  		
	}

	outp32(PWRON, 0x60005);    	/* Press Power Key during 6 sec to Power off (0x'6'0005) */
  	outp32(RIIR,0x4);    
#endif	
	
	GpioInit();
	FeedWatchdog();
	
	init();			
	FeedWatchdog();	

	imagebuf = (UINT8 *)((UINT32)image_buffer | 0x80000000);
	pImageList=((unsigned int *)(((unsigned int)image_buffer)|0x80000000));

	/* Initial DMAC and NAND interface */
	SPI_OpenSPI();
	FeedWatchdog();
	
	memset(imagebuf, 0, 1024);
	sysprintf("Load Image From Spi Flash\r\n");
	
	/* read image information */
	SPIReadFast(0, 63*1024, 1024, (UINT32*)imagebuf);  /* offset, len, address */

	if (((*(pImageList+0)) == 0xAA554257) && ((*(pImageList+3)) == 0x63594257))
	{
		count = *(pImageList+1);

		pImageList=((unsigned int*)(((unsigned int)image_buffer)|0x80000000));
		startBlock = endBlock = fileLen = executeAddr = 0;

		/* load execution file */
		pImageList = pImageList+4;
		for (i=0; i<count; i++)
		{
			if (((*(pImageList) >> 16) & 0xffff) == 1)	// execute
			{
				startBlock = *(pImageList + 1) & 0xffff;
				endBlock = (*(pImageList + 1) & 0xffff0000) >> 16;
				executeAddr = *(pImageList + 2);
				fileLen = *(pImageList + 3);
				
				SPIReadFast(0, startBlock * 0x10000, fileLen, (UINT32*)executeAddr);
				FeedWatchdog();
				
				sysSetGlobalInterrupt(DISABLE_ALL_INTERRUPTS);
				sysSetLocalInterrupt(DISABLE_FIQ_IRQ);
						
				// Invalid and disable cache
				sysDisableCache();
				sysInvalidCache();
		
				//sysprintf("Ready to Jump\r\n");
				
				sysFlushCache(I_D_CACHE);
				FeedWatchdog();
				
				//sysprintf("Begin Delay\r\n");
				//for (j=0; j<100000; j++) ;
				Delayus(500);
				FeedWatchdog();
						
				// JUMP to kernel
				sysprintf("Jump to Kernel\r\n");
									
				fw_func = (void(*)(void))(executeAddr);
				fw_func();				
				break;
			}
			/* pointer to next image */
			pImageList = pImageList+12;
		}
	}	
	
	return(0); // avoid compilation warning
}