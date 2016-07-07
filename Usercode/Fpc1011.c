/****************************************************************************
����:   WELLCOM _zhangys
ʱ��:   20130217
˵��:   FPC�ײ�
****************************************************************************/

#include  "IncludeAll.h"


#define	CLR_FPC_CS()	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x02 );	// CS1			
#define	SET_FPC_CS()	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfd);		// CS1	

#define	CLR_FPC_RST()	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x0<<6);		
#define	SET_FPC_RST()	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x1<<6);

#define	CLR_FPC_PWR()	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x0<<1)		
#define	SET_FPC_PWR()	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x1<<1)

//****************************************************************************
//* ���ƣ�_BYTE FPCReadWriteByte(BYTE cByte)
//* ���ܣ�ͨ��SPI����ָ��ģ���ж���дһ���ֽ�
//* ��ڲ�����cByte 		Ҫд������
//* ���ڲ���������������
//* ˵������д����ʱ����λ���ȡ�
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
//* ���� Fpc1011Power(BYTE OnOff)
//* ����:����PFC��Դ
//* ��ڲ�����OnOff
//* ���ڲ�������
//* ˵����
//****************************************************************************
void Fpc1011Power(BYTE OnOff)
{
	if(OnOff==ON)
		SET_FPC_PWR();
	else
		CLR_FPC_PWR();
}	


//****************************************************************************
//* ���� FPCWriteCP(BYTE cCommand,BYTE cParameter)
//* ���ܣ���ָ��ģ����д����Ͳ���
//* ��ڲ�����cCommand,cParameter
//* ���ڲ�������
//* ˵����
//****************************************************************************
void FPCWriteCP(BYTE cCommand,BYTE cParameter)
{
	
	FPCReadWriteByte(cCommand);
	FPCReadWriteByte(cParameter);

}

//****************************************************************************
//* ���� FPCReadData(_BYTE cCommand,_BYTE *cpDataBuff,_BYTE cLength)
//* ���ܣ���ָ��ģ���ж�����
//* ��ڲ�����cCommand,*cpDataBuff,cLength
//* ���ڲ�������
//* ˵����
//****************************************************************************
void FPCReadData(BYTE cCommand,BYTE *cpDataBuff,WORD cLength)
{
	WORD i;
	
	FPCWriteCP(cCommand,0x00);
	for(i=0;i<cLength;i++) 
		*(cpDataBuff+i) = FPCReadWriteByte(0x00);
}

//****************************************************************************
//* ���� FPCFPCSerialReadDataReadData(BYTE cCommand,BYTE *cpDataBuff,BYTE cLength)
//* ���ܣ���ָ��ģ��������������
//* ��ڲ�����cCommand,*cpDataBuff,cLength
//* ���ڲ�������
//* ˵����
//****************************************************************************
void FPCSerialReadData(BYTE cCommand,BYTE *cpDataBuff,BYTE cLength,BYTE cEnd)
{
	BYTE i;
	
	*cpDataBuff++ = FPCReadWriteByte(cCommand);
	*cpDataBuff++ = FPCReadWriteByte(0x00);
	for(i=0;i<cLength-2;i++) *cpDataBuff++ = FPCReadWriteByte(0x00);
	
}

//****************************************************************************
//* ���� FPCInit()
//* ���ܣ���ʼ��ָ��ģ��
//* ��ڲ�������
//* ���ڲ�������
//* ˵����
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
//* ���� FPCStart()
//* ���ܣ�����ָ��ģ��
//* ��ڲ�������
//* ���ڲ�������
//* ˵����
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
//* ���� FPC1011GetImage(BYTE *cpImageBuff)
//* ���ܣ���ָ��ͼ��
//* ��ڲ�������
//* ���ڲ�����*cpImageBuff
//* ˵����
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

//����ԭ�ͣ�BYTE Fpc1011Init(void)
//�������ܣ���ʼ��ָ�ƴ�����
//��ڲ�������
//���ڲ�������
//�������أ�ERR_OK����ʾ��ʼ���ɹ�
//ERR_NOTOK����ʾ��ʼ��ʧ��
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


