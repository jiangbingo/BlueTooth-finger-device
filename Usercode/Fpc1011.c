/****************************************************************************
作者:   WELLCOM _zhangys
时间:   20130217
说明:   FPC底层
****************************************************************************/

#include  "IncludeAll.h"


#define	CLR_FPC_CS()	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x02 );	// CS1			
#define	SET_FPC_CS()	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfd);		// CS1	

#define	CLR_FPC_RST()	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x0<<6);		
#define	SET_FPC_RST()	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x1<<6);

#define	CLR_FPC_PWR()	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x0<<1)		
#define	SET_FPC_PWR()	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x1<<1)

//****************************************************************************
//* 名称：_BYTE FPCReadWriteByte(BYTE cByte)
//* 功能：通过SPI口在指纹模块中读或写一个字节
//* 入口参数：cByte 		要写的数据
//* 出口参数：读到的数据
//* 说明：读写数据时，高位优先。
//****************************************************************************

BYTE FPCReadWriteByte(BYTE cByte)
{	
	BYTE cByteIn = 0;

	// data 16 bit
	outpw(REG_SPI0_TX0, cByte);
	spiTxLen(0, 0, 8);
	spiActive(0);
	cByteIn = inpw(REG_SPI0_RX0) & 0xff;
	
	return(cByteIn);		
}

void FPCWriteByte(BYTE cByte)
{	
	CLR_FPC_CS();	// CS1

	outpw(REG_SPI0_TX0, cByte);
	spiTxLen(0, 0, 8);
	spiActive(0);
	
	SET_FPC_CS();	// CS1		
}

BYTE FPCReadByte(void)
{
	BYTE cByteIn = 0;
	
	CLR_FPC_CS();	// CS1

	outpw(REG_SPI0_TX0, 0xAA);
	spiTxLen(0, 0, 8);
	spiActive(0);
	cByteIn = inpw(REG_SPI0_RX0) & 0xff;
	
	SET_FPC_CS();	// CS1
	
	return(cByteIn);		
}


//****************************************************************************
//* 名称 Fpc1011Power(BYTE OnOff)
//* 功能:开关PFC电源
//* 入口参数：OnOff
//* 出口参数：无
//* 说明：
//****************************************************************************
void Fpc1011Power(BYTE OnOff)
{
	if(OnOff==ON)
		SET_FPC_PWR();
	else
		CLR_FPC_PWR();
}	


//****************************************************************************
//* 名称 FPCWriteCP(BYTE cCommand,BYTE cParameter)
//* 功能：在指纹模块中写命令和参数
//* 入口参数：cCommand,cParameter
//* 出口参数：无
//* 说明：
//****************************************************************************
void FPCWriteCP(BYTE cCommand,BYTE cParameter)
{
	
	FPCReadWriteByte(cCommand);
	FPCReadWriteByte(cParameter);

}

//****************************************************************************
//* 名称 FPCReadData(_BYTE cCommand,_BYTE *cpDataBuff,_BYTE cLength)
//* 功能：在指纹模块中读数据
//* 入口参数：cCommand,*cpDataBuff,cLength
//* 出口参数：无
//* 说明：
//****************************************************************************
void FPCReadData(BYTE cCommand,BYTE *cpDataBuff,WORD cLength)
{
	WORD i;
	
	FPCWriteCP(cCommand,0x00);
	for(i=0;i<cLength;i++) 
		*(cpDataBuff+i) = FPCReadWriteByte(0x00);
}

//****************************************************************************
//* 名称 FPCFPCSerialReadDataReadData(BYTE cCommand,BYTE *cpDataBuff,BYTE cLength)
//* 功能：在指纹模块中连续读数据
//* 入口参数：cCommand,*cpDataBuff,cLength
//* 出口参数：无
//* 说明：
//****************************************************************************
void FPCSerialReadData(BYTE cCommand,BYTE *cpDataBuff,BYTE cLength,BYTE cEnd)
{
	BYTE i;
	
	*cpDataBuff++ = FPCReadWriteByte(cCommand);
	*cpDataBuff++ = FPCReadWriteByte(0x00);
	for(i=0;i<cLength-2;i++) *cpDataBuff++ = FPCReadWriteByte(0x00);
	
}

//****************************************************************************
//* 名称 FPCInit()
//* 功能：初始化指纹模块
//* 入口参数：无
//* 出口参数：无
//* 说明：
//****************************************************************************
void FPCInit(void)
{
	CLR_FPC_RST();
	Delayms(15);
	SET_FPC_RST();
	
	//CLR_FPC_CS();	// CS1
	
	FPCWriteCP(FPC_WR_DRIVC,FPC_DRIVC);		// Set DRIVC
	FPCWriteCP(FPC_WR_ADCREF,FPC_ADCREF);	// Set ADCREF
	
	//SET_FPC_CS();	// CS1
}

//****************************************************************************
//* 名称 FPCStart()
//* 功能：启动指纹模块
//* 入口参数：无
//* 出口参数：无
//* 说明：
//****************************************************************************
BYTE FPCStart(void)
{
	WORD i;
	BYTE SPISTAT[1];
	
	//CLR_FPC_CS();	// CS1
	FPCWriteCP(FPC_RD_SENSOR,0x00);
	for(i=0;i<300;i++)
	{
		FPCReadData(FPC_RD_SPISTAT,SPISTAT,1);
		if(SPISTAT[0] == 0x03)
		{
			return(ERR_OK);
		}
	}
	//SET_FPC_CS();	// CS1
	return(ERR_NOTOK);
}

//****************************************************************************
//* 名称 FPC1011GetImage(BYTE *cpImageBuff)
//* 功能：读指纹图像
//* 入口参数：无
//* 出口参数：*cpImageBuff
//* 说明：
//****************************************************************************
BYTE FPC1011GetImage(BYTE *cpImageBuff)
{
     
	BYTE i,j;
	BYTE cRetCode;
	i=0;
	CLR_FPC_CS();	// CS1
	FPCInit();
	cRetCode = FPCStart();
	if(cRetCode !=ERR_OK)	
	{	
		SET_FPC_CS();	// CS1
		return(ERR_NOTOK);
	}
	FPCReadData(FPC_RD_SPIDATA,cpImageBuff,8);
	cpImageBuff += 8;
	for(j=0;j<18;j++)
	{					
		FPCSerialReadData(FPC_RD_SPIDATA,cpImageBuff,8,i);
		cpImageBuff += 8;				 		
	}	
	for(i=0;i<199;i++){						
		for(j=0;j<19;j++){					
			FPCSerialReadData(FPC_RD_SPIDATA,cpImageBuff,8,i);
			cpImageBuff += 8;
		}	 	
	}
	SET_FPC_CS();	// CS1
	return(ERR_OK);

}

//函数原型：BYTE Fpc1011Init(void)
//函数功能：初始化指纹传感器
//入口参数：无
//出口参数：无
//函数返回：ERR_OK：表示初始化成功
//ERR_NOTOK：表示初始化失败
BYTE Fpc1011Init(void)
{
	BYTE cRet=ERR_NOTOK;
	Fpc1011Power(ON);
	CLR_FPC_CS();
	FPCInit();
	cRet = FPCStart();
	SET_FPC_CS();
	Fpc1011Power(OFF);
	return(cRet);
}


