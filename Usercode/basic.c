/****************************************************************************
   ����:   WELLCOM _zys
   ʱ��:   2012_11_15
   ˵��:   ����TCS1��TCS2
****************************************************************************/
#include  "IncludeAll.h"

//����ָʾ��
#define CLR_LED_R2()	gpio_setportval(GPIO_PORTC, 0x1 << 10, 0x0 << 10)
#define SET_LED_R2()	gpio_setportval(GPIO_PORTC, 0x1 << 10, 0x1 << 10)
#define CLR_LED_B2()	gpio_setportval(GPIO_PORTC, 0x1 << 8, 0x0 << 8)
#define SET_LED_B2()	gpio_setportval(GPIO_PORTC, 0x1 << 8, 0x1 << 8)


//�ϵ�ָʾ��
#define CLR_LED_G2()	gpio_setportval(GPIO_PORTC, 0x1 << 9, 0x0 << 9)
#define SET_LED_G2()	gpio_setportval(GPIO_PORTC, 0x1 << 9, 0x1 << 9)

//ͨ�Ź�����
#define CLR_LED_Y2()	gpio_setportval(GPIO_PORTC, 0x1 << 11, 0x0 << 11)
#define SET_LED_Y2()	gpio_setportval(GPIO_PORTC, 0x1 << 11, 0x1 << 11)


/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void LED_RGY(UINT8 flag)
{
	if(flag == OFF)
	{
		SET_LED_R2();
		SET_LED_G2();
		SET_LED_Y2();
		SET_LED_B2();
	}
	else
	{
		CLR_LED_R2();
		CLR_LED_G2();
		CLR_LED_Y2();
		CLR_LED_B2();
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlinkRGY(UINT8 cnt, UINT16 ms)
{
	UINT8 i;
	for(i = 0; i < cnt; i++)
	{
		LED_RGY(ON);
		Delayms(ms);
		LED_RGY(OFF);
		Delayms(ms);
	}
}

//���ָʾ��
void LED_R2(UINT8 flag)
{
	if(flag == OFF)
	{
		SET_LED_R2();
	}
	else
	{
		CLR_LED_R2();
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlinkR2(UINT8 cnt, UINT16 ms)
{
	UINT8 i;
	for(i = 0; i < cnt; i++)
	{
		LED_R2(ON);
		Delayms(ms);
		LED_R2(OFF);
		Delayms(ms);
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlinkLedByPowerLevel()
{
	static int flag=0;
	int iret =power_read();
	if(iret==1)
	{
		//������ �̵Ƴ���
		if(gucisChargeDone==1)
		{
			LED_B2(ON);
		}
		else
		{
			LED_B2(OFF);
		}

		//�����
		if(gucisCharge ==1)
		{
			LED_R2(ON);
			gudSleeptime = gDefineSleeptime*1000;
		}
		else
		{
			//��������
			if(gucPowerLevel<=2)
			{
				if(flag==0)
				{
					LED_R2(ON);
					flag =1;
				}
				else
				{
					LED_R2(OFF);
					flag =0;
				}
			}
			else
			{
				LED_R2(OFF);
			}
		}
	}
	
}

//ͨ�ŵ�
void LED_Y2(UINT8 flag)
{
	if(flag == OFF)
	{
		SET_LED_Y2();
	}
	else
	{
		CLR_LED_Y2();
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlinkY2(UINT8 cnt, UINT16 ms)
{
	UINT8 i;
	for(i = 0; i < cnt; i++)
	{
		LED_Y2(ON);
		Delayms(ms);
		LED_Y2(OFF);
		Delayms(ms);
	}
}

//��Դ��
void LED_G2(UINT8 flag)
{
	if(flag == OFF)
	{
		SET_LED_G2();
	}
	else
	{
		CLR_LED_G2();
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlinkG2(UINT8 cnt, UINT16 ms)
{
	UINT8 i;
	for(i = 0; i < cnt; i++)
	{
		LED_G2(ON);
		Delayms(ms);
		LED_G2(OFF);
		Delayms(ms);
	}
}
//��Դ��
void LED_B2(UINT8 flag)
{
	if(flag == OFF)
	{
		SET_LED_B2();
	}
	else
	{
		CLR_LED_B2();
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlinkB2(UINT8 cnt, UINT16 ms)
{
	UINT8 i;
	for(i = 0; i < cnt; i++)
	{
		LED_B2(ON);
		Delayms(ms);
		LED_B2(OFF);
		Delayms(ms);
	}
}
//��ʱ����

void Delayms(UINT16 ms)
{
	UINT16 i;
	while(ms--)
	{
		for(i = 0; i < 25000; i++)
		{
			;
		}
	}
}
void Delayus(UINT16 us)
{
	UINT16 i;
	while(us--)
	{
		for(i = 0; i < 20; i++)
		{
			;
		}
	}
}
/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void BlueToothInit()
{
	//sw_btn
	gpio_setportpull(GPIO_PORTA, 0x1 << 3, 0x1 << 3);
	gpio_setportdir(GPIO_PORTA, 0x1 << 3, 0x1 << 3);
	gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x0 << 3);

	//wakeup
	gpio_setportpull(GPIO_PORTA, 0x1 << 2, 0x1 << 2);
	gpio_setportdir(GPIO_PORTA, 0x1 << 2, 0x1 << 2);
	gpio_setportval(GPIO_PORTA, 0x1 << 2, 0x1 << 2);
	//sleep
	gpio_setportpull(GPIO_PORTB, 0x1<<4, 0x1<<4);
	gpio_setportdir(GPIO_PORTB, 0x1<<4, 0x1<<4);
	gpio_setportval(GPIO_PORTB, 0x1<<4, 0x1<<4);
	
	gpio_setportpull(GPIO_PORTE, 0x1<<0, 0x1<<0);
	gpio_setportdir(GPIO_PORTE, 0x1<<0, 0x1<<0);
	gpio_setportval(GPIO_PORTE, 0x1<<0, 0x1<<0);

	gpio_setportpull(GPIO_PORTE, 0x1<<1, 0x1<<1);
	gpio_setportdir(GPIO_PORTE, 0x1<<1, 0x1<<1);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
	
}
void In2BT()
{
	//P20_L
	gpio_setportval(GPIO_PORTE, 0x1<<0, 0x0<<0);
	Delayms(50);
	//poweer_down
	gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x0 << 3);
	Delayms(50);
	//poweer_on
	gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x1 << 3);
#if 0
	//RST
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
	Delayms(50);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x1<<1);
#else
		//RST
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x1<<1);
	Delayms(50);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
#endif

	Delayms(1500);
}
void Out2BT()
{

	Delayms(50);
	//P20_H
	gpio_setportval(GPIO_PORTE, 0x1<<0, 0x1<<0);
	Delayms(50);
	//poweer_down
	gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x0 << 3);
	Delayms(50);
	//poweer_on
	gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x1 << 3);
#if 0
	//RST
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
	Delayms(50);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x1<<1);
#else
	//RST
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x1<<1);
	Delayms(50);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
#endif

	OpenBlueTooth();


}
/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void OpenBlueTooth()
{
	//sw_btn
	gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x1 << 3);
#if 0
	//RST
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
	Delayms(50);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x1<<1);
#else
	//RST
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x1<<1);
	Delayms(50);
	gpio_setportval(GPIO_PORTE, 0x1<<1, 0x0<<1);
#endif
	//wakeup
	WakeupBlueTooth();
}
void WakeupBlueTooth()
{
	gpio_setportval(GPIO_PORTA, 0x1 << 2, 0x0 << 2);
	Delayms(5);
	gpio_setportval(GPIO_PORTA, 0x1 << 2, 0x1 << 2);

	gpio_setportval(GPIO_PORTB, 0x1<<4, 0x0<<4);
	Delayms(5);
	gpio_setportval(GPIO_PORTB, 0x1<<4, 0x1<<4);
}
/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void CloseBlueTooth()
{
	//sw_btn
	//gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x0 << 3);
	//wakeup
	//gpio_setportval(GPIO_PORTA, 0x1<<2, 0x1<<2);
}

//===========================================================
// ������Crc8
// ���ܣ�����CRC8У���
// ������wLength�����ݳ���
//       cpCrcBuf������ָ�룬ָ�������У��͵�����
// ���أ������У���ֵ
// Ӱ��: ��
//===========================================================
BYTE Crc8(WORD wLength, BYTE *cpCrcBuf)
{
	BYTE cOutdata = 0, cFind;
	WORD j;
	for(j = 0; j < wLength; j++)
	{
		cFind = cpCrcBuf[j] ^ cOutdata;
		cOutdata = gcCRC8DATA[cFind];
	}
	return (cOutdata);
}

//===========================================================
// ����: GlobalVarInit
// ����: ȫ�ֱ����ĳ�ʼ��
// ���: ��
// ����: ��
// Ӱ��: ���е�ȫ�ֱ���
//===========================================================
BYTE GlobalVarInit(void)
{
	INT i;

	gdwProgramLen = 0;

	// Ϊģ������ռ�
	for(i = 0; i < LENOFMBLIB; i++)
	{
		spMBLib[i].cFlag = 0x00;
	}

	// ��ʼ��MB������
	for(i = 0; i < LENOFMBINDEX; i++)
	{
		gcpMBIndex[i] = 0x00;
	}

	// ��������
	gcpProgramBuf = NULL;
	return (ERR_OK);
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
BYTE PowerOnRecovery(void)
{
	BYTE cRetCode;

	// ��ȡ������
	cRetCode = FlashReadWritePara(&gsParaTable, FLASHREAD);
	if(cRetCode != ERR_OK)
	{
		return ERR_READ_FLASH;
	}
	gDefineSleeptime = gsParaTable.ucTime;
	cRetCode = GetUserInfo();
	if(cRetCode != ERR_OK)
	{
		giMBCounter = 0;
		return ERR_READ_FLASH;
	}
	return (ERR_OK);
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int power_read(void)
{
	unsigned char value[10];
	unsigned char mask[1];
	int  j;
	INT32 rtval;
	unsigned char bat_val = 0;

	i2cInit();
	rtval = i2cOpen();
	if(rtval < 0)
	{
		rtval = 0;
		i2cExit();
		return 0;
	}

	i2cIoctl(I2C_IOC_SET_DEV_ADDRESS, 0x3a, 0);
	i2cIoctl(I2C_IOC_SET_SPEED, 100, 0);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x06, 1);
	j = 10;
	i2cRead_OV(&bat_val, 1);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x05, 1);
	i2cRead_OV(&value[0], 1);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x03, 1);
	mask[0] = 0x1f;
	i2cWrite(mask, 1);


	//������
	if(value[0] & 0x04)
	{
		gucisChargeDone = 1;
	}
	//���δ���
	else
	{
		gucisChargeDone = 0;
	}

	//�����
	if(value[0] & 0x02)
	{
		gucisCharge = 1;
	}
	else
	{
		gucisCharge = 0;
	}

	if(bat_val == 1)
	{
		gucPowerLevel = 1;
	}
	if(bat_val > 1 && bat_val < 4)
	{
		gucPowerLevel = 2;
	}
	if(bat_val >= 4 && bat_val < 8)
	{
		gucPowerLevel = 3;
	}
	if(bat_val >= 8)
	{
		gucPowerLevel = 4;
	}

	i2cClose();
	rtval = 1;
	i2cExit();
	return rtval;
}

//********************************************************************************
//��������void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable)
//��  �ܣ���cpBuf�д���ļ�Para����Ϣ���뵽spParaTable�ṹ������
//��  ����cpBuf���������Para��Ϣ
//        spParaTable��������Para��Ϣ
//����ֵ���ޣ�
//*******************************************************************************
void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable)
{
	SHORT i;
	int ipos=0;
	spParaTable->ucBtFlag= cpBuf[ipos++];
	spParaTable->ucTime = cpBuf[ipos++];
	spParaTable->cDeviceIDFlag = cpBuf[ipos++];      // �豸���к���Ч���,0x01��ʾ��Ч
	for(i = 0; i < LENOFDEVICEID; i++)
	{
		spParaTable->cpDeviceID[i] = cpBuf[ipos++];  // �豸���к�
	}

	spParaTable->cCrc = cpBuf[ipos++];                 // У��
}

//********************************************************************************
//��������void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable)
//��  �ܣ���cpBuf�д���ļ�Para����Ϣ���뵽spParaTable�ṹ������
//��  ����cpBuf���������Para��Ϣ
//        spParaTable��������Para��Ϣ
//����ֵ���ޣ�
//*******************************************************************************
void ICCompressPara(struct StruPARA *spParaTable, BYTE *cpBuf)
{
	SHORT i;
	int ipos=0;
	cpBuf[ipos++]= spParaTable->ucBtFlag;
	cpBuf[ipos++]= spParaTable->ucTime;  
	cpBuf[ipos++]= spParaTable->cDeviceIDFlag;        // �豸���к���Ч���,0x01��ʾ��Ч
	for(i = 0; i < LENOFDEVICEID; i++)
	{
		cpBuf[ipos++] = spParaTable->cpDeviceID[i];  // �豸���к�
	}
	cpBuf[ipos++] = spParaTable->cCrc;                 // У��
}

//*****************************************************************
// ����: FlashReadWritePara
// ����: ��DFLASH�ж�ȡ/д��Para������
// ���:
//       cpSerial: �����ȡ/д��Ĳ�����
//       cMode:    ��/д��ʽ
// ����: ERR_OK      ����ɹ�
//       ERR_NOTOK   ���治�ɹ�
// Ӱ��:
//*****************************************************************
BYTE FlashReadWritePara(struct StruPARA *spParaTable, BYTE cMode)
{
	DWORD dwBegAddr, dwLen;
	BYTE cRetCode, cCrc;
	BYTE cpPara[SIZEOFPARA];
	BYTE *cpBuffer;

	dwBegAddr = PARA_AREA_ADDRESS;                          // ����������ʼ��ַ
	dwLen = SIZEOFPARA;

	if(cMode == FLASHREAD)
	{
		cRetCode = DFlashRWBlock(dwBegAddr, cpPara, dwLen, FLASHREAD);
		if(cRetCode != ERR_OK)
		{
			return (cRetCode);
		}
		cCrc = Crc8(dwLen - 1, cpPara);
		if(cpPara[dwLen - 1] != cCrc)
		{
			return (ERR_CRC);
		}
		ICUncompressPara(cpPara, spParaTable);
		return (ERR_OK);
	}
	else
	{
		cpBuffer = (BYTE *)malloc(dwLen);
		ICCompressPara(spParaTable, cpPara);
		cCrc = Crc8(dwLen - 1, cpPara);
		cpPara[dwLen - 1] = cCrc;
		cRetCode = DFlashRWBlock(dwBegAddr, cpPara, dwLen, FLASHWRITE);         // д������Ϣ
		free(cpBuffer);
		return (cRetCode);
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
BYTE CrcOfMBLibItem(struOneUserInfo *spMBLibItem)
{
	BYTE cpTmpBuf[512], cCRC;
	WORD wSize, i;

	wSize = 0;
	cpTmpBuf[wSize++] = spMBLibItem->cFlag;

	cpTmpBuf[wSize++] = spMBLibItem->ucUsrid;
	for(i = 0; i < LENOFMB; i++)
	{
		cpTmpBuf[wSize++] = spMBLibItem->cpMB[i];
	}

	cCRC = Crc8(wSize, cpTmpBuf);
	return (cCRC);
}

//*****************************************************************
// ����: GetUserInfo
// ����: ��DataFlash �ռ��ж�����Ա��Ϣ,���ѹ�Ա��Ϣ���浽gwMBLibCounter
// ���: ��
// ����: ��
// Ӱ��:
//*****************************************************************
BYTE GetUserInfo(void)
{
	DWORD dwLen;
	DWORD dwAddr;
	BYTE cRetCode;
	WORD wMBIndex, i;

	cRetCode = FlashReadWriteMBIndex(gcpMBIndex, FLASHREAD);
	if(cRetCode == ERR_OK)
	{
		wMBIndex = gcpMBIndex[0] * 256 + gcpMBIndex[1];                   // ȡģ����й�Ա����
	}
	else
	{
		giMBCounter = 0;
		return cRetCode;
	}

	dwAddr = MB_AREA_ADDRESS;
	dwLen = sizeof(struOneUserInfo) * wMBIndex;
	cRetCode = DFlashRWBlock(dwAddr, (BYTE *)spMBLib, dwLen, READMODE);
	if(cRetCode != ERR_OK)
	{
		return 0X2F;
	}
	for(i = 0; i < wMBIndex; i++)
	{
		if(spMBLib[i].cFlag == 0x01 || spMBLib[i].cFlag == 0x02)
		{
			giMBCounter++;
		}
	}
	return (ERR_OK);
}

//*****************************************************************
// ����: FlashReadWriteMBIndex
// ����: ��DFLASH�ж�ȡ/д��ģ���������Ϣ
// ���:
//       cpMBIndex: �����/д�����к�
//       dwLen:    ���кŵĳ���
//       cMode:    ģʽ
// ����: ERR_OK      ����ɹ�
//       ERR_NOTOK   ���治�ɹ�
// Ӱ��:
//*****************************************************************
BYTE FlashReadWriteMBIndex(BYTE *cpMBIndex, BYTE cMode)
{
	DWORD dwBegAddr;
	BYTE cRetCode, cCrc;
	DWORD dwLen;
	BYTE *cpBuffer;

	dwBegAddr = MBINDEX_AREA_ADDRESS;                           // ����������ʼ��ַ
	dwLen = LENOFMBINDEX;

	if(cMode == FLASHREAD)
	{
		cRetCode = DFlashRWBlock(dwBegAddr, cpMBIndex, dwLen, FLASHREAD);
		if(cRetCode != ERR_OK)
		{
			return (cRetCode);
		}
		cCrc = Crc8(dwLen - 1, cpMBIndex);
		if(cpMBIndex[dwLen - 1] != cCrc)
		{
			return (ERR_NOTOK);
		}
		return (ERR_OK);
	}
	else
	{
		cpBuffer = (BYTE *)malloc(dwLen);
		cCrc = Crc8(dwLen - 1, cpMBIndex);
		cpMBIndex[dwLen - 1] = cCrc;
		cRetCode = DFlashRWBlock(dwBegAddr, cpMBIndex, dwLen, FLASHWRITE);          // д������Ϣ
		free(cpBuffer);
		return (cRetCode);
	}
}

//*****************************************************************
// ����: WriteMBLibToFlash
// ����: ����ָ��ģ�嵽DataFlash�е�ָ�ƿ� --- �������й�Ա����Ϣ
// ���: ��
// ����: ERR_OK     ����ɹ�
//       ERR_NOTOK  ���治�ɹ�
// Ӱ��:
//*****************************************************************
BYTE WriteMBLibToFlash(void)
{
	BYTE *cpBuffer, *cpBuf;
	DWORD dwLength;
	BYTE cRetCode;
	DWORD dwAddr, i;

	dwAddr = MB_AREA_ADDRESS;                    // ָ��ģ��������ʼ��ַ;

	if(giMBCounter == 0)
	{
		return (ERR_NOTOK);
	}

	//���㳤��
	dwLength = sizeof(struOneUserInfo) * giMBCounter;

	//����ռ�
	cpBuffer = (BYTE *)malloc(dwLength);
	cpBuf = (BYTE*)spMBLib;

	for(i = 0; i < dwLength; i++)
	{
		cpBuffer[i] = cpBuf[i];
	}
	cRetCode = DFlashRWBlock(dwAddr, cpBuffer, dwLength, WRITEMODE);
	if(cRetCode != ERR_OK)
	{
		free(cpBuffer);

		return ERR_NOTOK;
	}
	free(cpBuffer);

	gcpMBIndex[0] = giMBCounter / 256;
	gcpMBIndex[1] = giMBCounter % 256;

	cRetCode = FlashReadWriteMBIndex(gcpMBIndex, FLASHWRITE);

	if(cRetCode != ERR_OK)
	{
		return ERR_NOTOK;
	}

	return (ERR_OK);
}

/************************************** The End Of File **************************************/

