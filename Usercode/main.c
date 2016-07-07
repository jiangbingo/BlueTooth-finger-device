//////////////////////////////////////////////////////////////////////////////////
//开发人员：zhangys
//当前版本：v1.0.0.1
//创建时间：2013/3/24
//修改时间：2013/3/24
//////////////////////////////////////////////////////////////////////////////////

#include  "GlobalVar.h"
#include  "IncludeAll.h"
#include "wblib.h"
#include "usbd.h"

extern void WelSendHID_Data(void);


extern void USB_Timer_Handler (void);
void WDT_Init(void);
void PLOW(void);
void Timer0_Callback(void)
{
	static int udSofCnt_bak = 0;
	static int itimechip =0;
	//串口0
	if(gStruUart0.rx_tim>0)
	{
		gStruUart0.rx_tim--;
		if(gStruUart0.rx_tim==0)
		{
			gStruUart0.rx_flg=2;	//接收完成标志
		}
	}
	//串口1
	if(gStruUart1.rx_tim>0)
	{
		gStruUart1.rx_tim--;
		if(gStruUart1.rx_tim==0)
		{
			gStruUart1.rx_flg=2;	//接收完成标志
		}
	}

	guisReadpower++;
	//usb hid timer
	USB_Timer_Handler();

	if(gudTimeOut>0)
		gudTimeOut --;
	if(gudSleeptime>0)
		gudSleeptime --;

	//静电干扰使用
	itimechip++;
	if(itimechip>1500)// 1.5s时间片
	{
		itimechip = 0;
		//已经枚举过
		if(gisEnum ==1)
		{
			if(udSofCnt_bak!=gudSofCnt)
				udSofCnt_bak = gudSofCnt ; 
			else
			{
				WDT_Init();
				gisEnum = 0;
			}
		}
	}
	
}


/////////////////////////////////////////////////////////////////////////////////
//串口初始化
//zhangys
//20130219
////////////////////////////////////////////////////////////////////////////////
void UartPort_Init(UINT8 Port,UINT32 Baudrate)
{
	WB_UART_T uart;
	u32ExtFreq = sysGetExternalClock();
	sysUartPort(Port);
	uart.uart_no = Port;//port 0 high 1 normal
	uart.uiFreq = u32ExtFreq*1000;
	uart.uiBaudrate = Baudrate;
	uart.uiDataBits = WB_DATA_BITS_8;
	uart.uiStopBits = WB_STOP_BITS_1;
	uart.uiParity = WB_PARITY_NONE;
	uart.uiRxTriggerLevel = LEVEL_1_BYTE;
	sysInitializeUART(&uart);
	if(Port==0)
	{
		Uart0StructVarInit();
		gudBaudrate0=Baudrate;
	}
	else
	{
		Uart1StructVarInit();
		gudBaudrate1=Baudrate;
	}
	
}
////////////////////////////////////////////////////////////////////////////////
//定时器初始化
//zhangys
//20130219
///////////////////////////////////////////////////////////////////////////////
void Timer_Init(void)
{
	u32ExtFreq = sysGetExternalClock();
	sysSetTimerReferenceClock(TIMER1, u32ExtFreq*1000);		 //External Crystal
	sysStartTimer(TIMER1, 1000, PERIODIC_MODE);				/* 1000 ticks/per sec ==> 1tick/1ms */
	sysSetTimerEvent(TIMER1,1 , (PVOID)Timer0_Callback);	/* 1 ticks = 100us call back */
	sysSetLocalInterrupt(ENABLE_IRQ);
}
void WatchDog_ISR()
{
	if(gisEnablewdt==1)
	{
    	sysClearWatchDogTimerInterruptStatus();
		sysClearWatchDogTimerCount();
	}
	else
	{
	
	}
	
	//
}


void WDT_Init(void)
{
	UINT32 u32ExtFreq;
	
	u32ExtFreq = sysGetExternalClock()*1000;
	sysSetTimerReferenceClock(TIMER0, u32ExtFreq);
	sysStartTimer(TIMER0, 1, PERIODIC_MODE); /* SW Modify sysStartTimer(TIMER0, 0, PERIODIC_MODE);*/

	/* set up watch dog timer */
	sysInstallWatchDogTimerISR(IRQ_LEVEL_1, (PVOID)WatchDog_ISR);
	sysEnableWatchDogTimer();
	sysEnableWatchDogTimerReset();
}

////////////////////////////////////////////////////////////////////////////////
//IO初始化
//huayj
//20130219
///////////////////////////////////////////////////////////////////////////////
void GpioInit(void)
{
	//led端口初始化
	gpio_setportpull(GPIO_PORTC, 0x1<<9, 0x1<<9);
	gpio_setportdir(GPIO_PORTC, 0x1<<9, 0x1<<9);
	gpio_setportval(GPIO_PORTC, 0x1<<9, 0x1<<9);

	gpio_setportpull(GPIO_PORTC, 0x1<<8, 0x1<<8);
	gpio_setportdir(GPIO_PORTC, 0x1<<8, 0x1<<8);
	gpio_setportval(GPIO_PORTC, 0x1<<8, 0x1<<8);

	gpio_setportpull(GPIO_PORTC, 0x1<<10, 0x1<<10);
	gpio_setportdir(GPIO_PORTC, 0x1<<10, 0x1<<10);
	gpio_setportval(GPIO_PORTC, 0x1<<10, 0x1<<10);

	gpio_setportpull(GPIO_PORTC, 0x1<<11, 0x1<<11);
	gpio_setportdir(GPIO_PORTC, 0x1<<11, 0x1<<11);
	gpio_setportval(GPIO_PORTC, 0x1<<11, 0x1<<11);

	gpio_setportpull(GPIO_PORTB, 0x1<<1, 0x1<<1);
	gpio_setportdir(GPIO_PORTB, 0x1<<1, 0x1<<1);
	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x0<<1);

	gpio_setportpull(GPIO_PORTA, 0x1<<6, 0x1<<6);
	gpio_setportdir(GPIO_PORTA, 0x1<<6, 0x1<<6);
	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x0<<6);

	
}

void DemoInit(int flag)
{
	BYTE Ret=ERR_OK;
	
	
	sysSetSystemClock(eSYS_UPLL, 	//E_SYS_SRC_CLK eSrcClk,	
					192000,		//UINT32 u32PllKHz, 	
					192000,		//UINT32 u32SysKHz,
					192000,		//UINT32 u32CpuKHz,
					192000,		//UINT32 u32HclkKHz,
					48000);		//UINT32 u32ApbKHz
	
	sysEnableCache(CACHE_WRITE_BACK);
	//PLOW();
	Timer_Init();
	//WDT_Init();
	u32ExtFreq = sysGetExternalClock();

	//GPIO初始化
	GpioInit();
	BlinkRGY(3,50);
	if(flag ==0)
	{
		BlueToothInit();
		OpenBlueTooth();
	}
	else
	{
		WakeupBlueTooth();
	}
	
	UartPort_Init(0,HIGH4_BAUDRATE);
	UartPort_Init(1,HIGH_BAUDRATE);
	spiPortInit();
	FpiGetSensorType();
	//USB
	udcDeinit();
	udcOpen();
	mscdInit();
	udcInit();
	Ret = PowerOnRecovery();
	
	
	LED_RGY(OFF);
	LED_G2(ON);
	gudSleeptime = gDefineSleeptime*1000;
	
}
extern INT32 sysPowerDown(UINT32 u32WakeUpSrc);
void PLOW(void)
{
	//USB Host disable
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | USBH_CKE);				
	outp32(0xb1009200, 0x08000000);
	//ADC disable 
	outp32(REG_APBCLK, inp32(REG_APBCLK) | ADC_CKE);					
	outp32 (REG_ADC_CON, inp32(REG_ADC_CON) & ~ADC_CON_ADC_EN);
	outp32(REG_MISCR, inp32(REG_MISCR) & ~LVR_EN);		
	outp32(REG_APBCLK, inp32(REG_APBCLK) & ~ADC_CKE);
	//DAC VDD33 power down 																													
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | (SPU_CKE | ADO_CKE));	
	outp32(REG_SPU_DAC_VOL, inp32(REG_SPU_DAC_VOL) | ANA_PD);	
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) & ~(SPU_CKE | ADO_CKE));															
	//TV DAC
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | VPOST_CKE);				
	outp32(REG_LCM_TVCtl, inp32(REG_LCM_TVCtl) | TVCtl_Tvdac);
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) & ~VPOST_CKE);
}
void PowerDownWakeUp(void)
{
	UINT32 reg_AHBCLK;
	CloseBlueTooth();
	//设置IO状态

	gpio_setportpull(GPIO_PORTB, 0x01<<2, 0x01<<2);		
	gpio_setportdir(GPIO_PORTB, 0x01<<2, 0x00<<2);				
	gpio_setsrcgrp(GPIO_PORTB, 0x01<<2, 0x00);		
	gpio_setintmode(GPIO_PORTB, 0x01<<2, 0x01<<2, 0x00<<2);	
	outp32(REG_IRQTGSRC0, 0X01<<18);
	outp32(REG_IRQLHSEL, 0x11);

	//USB Host disable
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | USBH_CKE);				
	outp32(0xb1009200, 0x08000000);

	//ADC disable 
	outp32(REG_APBCLK, inp32(REG_APBCLK) | ADC_CKE);					
	outp32 (REG_ADC_CON, inp32(REG_ADC_CON) & ~ADC_CON_ADC_EN);
	outp32(REG_MISCR, inp32(REG_MISCR) & ~LVR_EN);		
	outp32(REG_APBCLK, inp32(REG_APBCLK) & ~ADC_CKE);

	//DAC VDD33 power down 																													
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | (SPU_CKE | ADO_CKE));	
	outp32(REG_SPU_DAC_VOL, inp32(REG_SPU_DAC_VOL) | ANA_PD);	
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) & ~(SPU_CKE | ADO_CKE));															
																
	//USB phy disable														
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | USBD_CKE);				
	outp32(PHY_CTL, 0x0);
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) & ~USBD_CKE);
	//TV DAC
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) | VPOST_CKE);				
	outp32(REG_LCM_TVCtl, inp32(REG_LCM_TVCtl) | TVCtl_Tvdac);
	outp32(REG_AHBCLK, inp32(REG_AHBCLK) & ~VPOST_CKE);

	/*Save AHB clock */
	reg_AHBCLK = inp32(REG_AHBCLK);
	outp32(REG_AHBCLK, 0x011F);
	
	
	//outp32(REG_GPAFUN, 0x0);
	//outp32(REG_GPBFUN, 0x0);
	outp32(REG_GPCFUN, 0x0);
	//outp32(REG_GPDFUN, 0x0);
	//outp32(REG_GPEFUN, 0x0);

	//outp32(REG_GPIOA_OMD, 0x0);
	//outp32(REG_GPIOB_OMD, 0x0);
	outp32(REG_GPIOC_OMD, 0x0);
	//outp32(REG_GPIOD_OMD, 0x0);
	//outp32(REG_GPIOE_OMD, 0x0);
	
	//outp32(REG_GPIOA_PUEN, 0xFFFF);
	//outp32(REG_GPIOB_PUEN, 0xFFFF);
	outp32(REG_GPIOC_PUEN, 0xFFFF);
	//outp32(REG_GPIOD_PUEN, 0xFFFF);
	//outp32(REG_GPIOE_PUEN, 0xFFFF);	
	//进入深度睡眠状态
	sysPowerDown(WE_GPIO);
	//唤醒整个机制
	/*Restore AHB clock */
	outp32(REG_AHBCLK, reg_AHBCLK);
	DemoInit(1);

}
UINT8 *Sprintfs;

void updateAPP()
{
	if(gisupdateApp==1)
		WDT_Init();
}
void sysChangeClock(int flag)
{
	
	sysDisableInterrupt(IRQ_TMR0);
	if(flag ==0)
	{
		sysClockDivSwitchStart(7);
	}
	else
	{
		sysClockDivSwitchStart(0);
	}	
	sysEnableInterrupt(IRQ_TMR0);
	UartPort_Init(0,HIGH4_BAUDRATE);
	UartPort_Init(1,HIGH_BAUDRATE);
	Timer_Init();
	
	
	
}
INT main(VOID)
{
	char buf[3]= {0x00,0x1f,0x00};
	//初始化用户自定义空间
	gcUserBuf = (UINT8*)malloc(sizeof(UINT8)*USERBUF_LENTH);
	memset(gcUserBuf,0,USERBUF_LENTH);
	Sprintfs= (UINT8*)malloc(sizeof(UINT8)*1*1024);
	memset(Sprintfs,0,1*1024);
	gisEnablewdt =0;
	DemoInit(0);
	if(gsParaTable.ucBtFlag == 0)
	{
		SetBTparam(0x07,buf,3);
		SetBTparam(0x07,buf,3);
		gsParaTable.ucBtFlag =1;
		FlashReadWritePara(&gsParaTable, FLASHWRITE);
	}
	//sysChangeClock(OFF);
	while(1)
	{
		if(gStuHidTrans.ucRcvFlg == 2)
		{
			gudSleeptime = gDefineSleeptime*1000;

			//sysChangeClock(ON);
			WelSendHID_Data();
			//sysChangeClock(OFF);
			
			gStuHidTrans.iPacket =0;
			gStuHidTrans.ucRcvFlg =0;
			gStuHidTrans.ucRcvTime = 0;
		}
		else
		{
			if(guisReadpower>=1000)
			{
				guisReadpower =0;
				BlinkLedByPowerLevel();
			}
		}
		//睡眠时间>0,未枚举,未充电
		if((gDefineSleeptime>0)&&(gisEnum==0||gucisCharge ==0))
		{
			if(gudSleeptime <=0)
			{
				PowerDownWakeUp();
				//sysChangeClock(0);
			}
		}
		Uart_handler();
		updateAPP();
	}
}
