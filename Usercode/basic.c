/****************************************************************************
   作者:   WELLCOM _zys
   时间:   2012_11_15
   说明:   兼容TCS1和TCS2
****************************************************************************/
#include  "IncludeAll.h"

//电量指示灯
#define CLR_LED_R2()	gpio_setportval(GPIO_PORTC, 0x1 << 10, 0x0 << 10)
#define SET_LED_R2()	gpio_setportval(GPIO_PORTC, 0x1 << 10, 0x1 << 10)
#define CLR_LED_B2()	gpio_setportval(GPIO_PORTC, 0x1 << 8, 0x0 << 8)
#define SET_LED_B2()	gpio_setportval(GPIO_PORTC, 0x1 << 8, 0x1 << 8)


//上电指示灯
#define CLR_LED_G2()	gpio_setportval(GPIO_PORTC, 0x1 << 9, 0x0 << 9)
#define SET_LED_G2()	gpio_setportval(GPIO_PORTC, 0x1 << 9, 0x1 << 9)

//通信工作灯
#define CLR_LED_Y2()	gpio_setportval(GPIO_PORTC, 0x1 << 11, 0x0 << 11)
#define SET_LED_Y2()	gpio_setportval(GPIO_PORTC, 0x1 << 11, 0x1 << 11)


/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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

//充电指示灯
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void BlinkLedByPowerLevel()
{
	static int flag=0;
	int iret =power_read();
	if(iret==1)
	{
		//充电完成 绿灯常亮
		if(gucisChargeDone==1)
		{
			LED_B2(ON);
		}
		else
		{
			LED_B2(OFF);
		}

		//充电中
		if(gucisCharge ==1)
		{
			LED_R2(ON);
			gudSleeptime = gDefineSleeptime*1000;
		}
		else
		{
			//电量不足
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

//通信灯
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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

//电源灯
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
//电源灯
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
//延时函数

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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void CloseBlueTooth()
{
	//sw_btn
	//gpio_setportval(GPIO_PORTA, 0x1 << 3, 0x0 << 3);
	//wakeup
	//gpio_setportval(GPIO_PORTA, 0x1<<2, 0x1<<2);
}

//===========================================================
// 函数：Crc8
// 功能：计算CRC8校验和
// 参数：wLength：数据长度
//       cpCrcBuf：数据指针，指向需计算校验和的数据
// 返回：计算的校验和值
// 影响: 无
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
// 函数: GlobalVarInit
// 功能: 全局变量的初始化
// 入口: 无
// 返回: 无
// 影响: 所有的全局变量
//===========================================================
BYTE GlobalVarInit(void)
{
	INT i;

	gdwProgramLen = 0;

	// 为模板库分配空间
	for(i = 0; i < LENOFMBLIB; i++)
	{
		spMBLib[i].cFlag = 0x00;
	}

	// 初始化MB库索引
	for(i = 0; i < LENOFMBINDEX; i++)
	{
		gcpMBIndex[i] = 0x00;
	}

	// 程序升级
	gcpProgramBuf = NULL;
	return (ERR_OK);
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
BYTE PowerOnRecovery(void)
{
	BYTE cRetCode;

	// 读取参数表
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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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


	//充电完成
	if(value[0] & 0x04)
	{
		gucisChargeDone = 1;
	}
	//充电未完成
	else
	{
		gucisChargeDone = 0;
	}

	//充电中
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
//函数名：void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable)
//功  能：将cpBuf中存放文件Para的信息解码到spParaTable结构类型中
//参  数：cpBuf：待解码的Para信息
//        spParaTable：解码后的Para信息
//返回值：无；
//*******************************************************************************
void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable)
{
	SHORT i;
	int ipos=0;
	spParaTable->ucBtFlag= cpBuf[ipos++];
	spParaTable->ucTime = cpBuf[ipos++];
	spParaTable->cDeviceIDFlag = cpBuf[ipos++];      // 设备序列号有效标记,0x01表示有效
	for(i = 0; i < LENOFDEVICEID; i++)
	{
		spParaTable->cpDeviceID[i] = cpBuf[ipos++];  // 设备序列号
	}

	spParaTable->cCrc = cpBuf[ipos++];                 // 校验
}

//********************************************************************************
//函数名：void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable)
//功  能：将cpBuf中存放文件Para的信息解码到spParaTable结构类型中
//参  数：cpBuf：待解码的Para信息
//        spParaTable：解码后的Para信息
//返回值：无；
//*******************************************************************************
void ICCompressPara(struct StruPARA *spParaTable, BYTE *cpBuf)
{
	SHORT i;
	int ipos=0;
	cpBuf[ipos++]= spParaTable->ucBtFlag;
	cpBuf[ipos++]= spParaTable->ucTime;  
	cpBuf[ipos++]= spParaTable->cDeviceIDFlag;        // 设备序列号有效标记,0x01表示有效
	for(i = 0; i < LENOFDEVICEID; i++)
	{
		cpBuf[ipos++] = spParaTable->cpDeviceID[i];  // 设备序列号
	}
	cpBuf[ipos++] = spParaTable->cCrc;                 // 校验
}

//*****************************************************************
// 函数: FlashReadWritePara
// 功能: 从DFLASH中读取/写入Para表数据
// 入口:
//       cpSerial: 保存读取/写入的参数表
//       cMode:    读/写方式
// 返回: ERR_OK      保存成功
//       ERR_NOTOK   保存不成功
// 影响:
//*****************************************************************
BYTE FlashReadWritePara(struct StruPARA *spParaTable, BYTE cMode)
{
	DWORD dwBegAddr, dwLen;
	BYTE cRetCode, cCrc;
	BYTE cpPara[SIZEOFPARA];
	BYTE *cpBuffer;

	dwBegAddr = PARA_AREA_ADDRESS;                          // 参数区的起始地址
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
		cRetCode = DFlashRWBlock(dwBegAddr, cpPara, dwLen, FLASHWRITE);         // 写参数信息
		free(cpBuffer);
		return (cRetCode);
	}
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
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
// 函数: GetUserInfo
// 功能: 从DataFlash 空间中读出柜员信息,并把柜员信息保存到gwMBLibCounter
// 入口: 无
// 返回: 无
// 影响:
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
		wMBIndex = gcpMBIndex[0] * 256 + gcpMBIndex[1];                   // 取模板库中柜员数量
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
// 函数: FlashReadWriteMBIndex
// 功能: 从DFLASH中读取/写入模板的索引信息
// 入口:
//       cpMBIndex: 保存读/写的序列号
//       dwLen:    序列号的长度
//       cMode:    模式
// 返回: ERR_OK      保存成功
//       ERR_NOTOK   保存不成功
// 影响:
//*****************************************************************
BYTE FlashReadWriteMBIndex(BYTE *cpMBIndex, BYTE cMode)
{
	DWORD dwBegAddr;
	BYTE cRetCode, cCrc;
	DWORD dwLen;
	BYTE *cpBuffer;

	dwBegAddr = MBINDEX_AREA_ADDRESS;                           // 参数区的起始地址
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
		cRetCode = DFlashRWBlock(dwBegAddr, cpMBIndex, dwLen, FLASHWRITE);          // 写参数信息
		free(cpBuffer);
		return (cRetCode);
	}
}

//*****************************************************************
// 函数: WriteMBLibToFlash
// 功能: 保存指纹模板到DataFlash中的指纹库 --- 保存所有柜员的信息
// 入口: 无
// 返回: ERR_OK     保存成功
//       ERR_NOTOK  保存不成功
// 影响:
//*****************************************************************
BYTE WriteMBLibToFlash(void)
{
	BYTE *cpBuffer, *cpBuf;
	DWORD dwLength;
	BYTE cRetCode;
	DWORD dwAddr, i;

	dwAddr = MB_AREA_ADDRESS;                    // 指纹模板区的起始地址;

	if(giMBCounter == 0)
	{
		return (ERR_NOTOK);
	}

	//计算长度
	dwLength = sizeof(struOneUserInfo) * giMBCounter;

	//分配空间
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

