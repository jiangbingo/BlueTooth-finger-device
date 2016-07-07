//*******************************************************************************
//说明：维尔 USART指令数据处理流程
//时间：20130131
//作者：wellcom
//*******************************************************************************

#include "IncludeAll.h"
#include"aes_app.h"
#pragma Otime
extern UINT8 *Sprintfs;
void Uart_GetVersion(int port, UINT8 *cpRecv);
void Uart_GetDevID(int port, UINT8 *cpRecv);
void Uart_SetDevID(int port, UINT8 *cpRecv);
void Uart_GetFpImage(int port, UINT8 *cpRecv);
void Uart_GetFpFeature(int port, UINT8 *cpRecv);
void Uart_GetFpTemplet(int port, UINT8 *cpRecv);
void Uart_GetFpMatch(int port, UINT8 *cpRecv);
void Uart_GetFpSearch(int port, UINT8 *cpRecv);
void Uart_GetFpCheck(int port, UINT8 *cpRecv);
void Uart_DataEncrypt(int port, UINT8 *cpRecv);
void Uart_DataDecrypt(int port, UINT8 *cpRecv);
void Uart_GetMBcnt(int port, UINT8 *cpRecv);
void Uart_ClearMBindex(int port, UINT8 *cpRecv);
void UartOrgTransferDatas(int port, UINT8 *cpBuf, UINT8 Command);
void UartProcessFailed(int port);
void UartProcessHandle(void);
void Uart_SetParam(int port, UINT8 *cpRecv);
UINT8 Uart_SetSleeptime4Bt(int port, UINT8 *cpRecv);
/*********************************************************************************************************
** 函数名称：Uart0StructVarInit
** 函数描述：串口变量初始化
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void Uart0StructVarInit(void)
{
	//初始化串口0变量
	gStruUart0.tx_flg = 1;                              //发送标志
	gStruUart0.tx_cnt = 0;                              //发送长度
	gStruUart0.st_cnt = 0;
	gStruUart0.rx_flg = 0;                              //接收标志
	gStruUart0.rx_cnt = 0;                              //接收长度
	gStruUart0.rx_tim = 0;                              //超时时间
	gStruUart0.time_out = 5;                            //接收超时中断2ms
	gStruUart0.rx_buf = &gcUserBuf[0];                  //数据缓存
	gStruUart0.tx_buf = &gcUserBuf[USERBUF_LENTH / 2];  //数据缓存
}

/*********************************************************************************************************
** 函数名称：Uart1StructVarInit
** 函数描述：串口变量初始化
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void Uart1StructVarInit(void)
{
	//初始化串口1变量
	gStruUart1.tx_flg = 1;                              //发送标志
	gStruUart1.tx_cnt = 0;                              //发送长度
	gStruUart1.st_cnt = 0;
	gStruUart1.rx_flg = 0;                              //接收标志
	gStruUart1.rx_cnt = 0;                              //接收长度
	gStruUart1.rx_tim = 0;                              //超时时间
	gStruUart1.time_out = 25;                           //接收超时中断2ms
	gStruUart1.rx_buf = &gcUserBuf[0];                  //数据缓存
	gStruUart1.tx_buf = &gcUserBuf[USERBUF_LENTH / 2];
}

/*********************************************************************************************************
** 函数名称：UartSendString
** 函数描述：向串口发送字节数据，并等待数据发送完成，使用查询方式
** 输入参数：buflen :数据长度 txbuf:   要发送的数据
** 返回值  ：无
*********************************************************************************************************/
void UartSendString(int port, UINT8 *txbuf, UINT32 buflen)
{
	while(buflen-- > 0)                                 /* 发送条件                     */
	{
		UartSendByte(port, *txbuf++);                   /* 发送字节数据                 */
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
void Uart_handler(void)
{
	UINT8 ucRet = 0;
	int port = (gStruUart1.rx_flg == 2) ? 1 : 0;
	if( (gStruUart0.rx_flg == 2) || (gStruUart1.rx_flg == 2) )
	{
		gudSleeptime = gDefineSleeptime*1000;
		//sysChangeClock(1);
#if 1
		if(gcUserBuf[0]!=0x02)
		{
			if(gStruUart1.rx_flg==2)
				UartSendString(port, gcUserBuf, gStruUart1.rx_cnt);
			else
				UartSendString(port, gcUserBuf, gStruUart0.rx_cnt);
			gStruUart1.rx_flg = 0;
			gStruUart1.rx_cnt = 0;
			gStruUart0.rx_flg = 0;
			gStruUart0.rx_cnt = 0;
			return ;
		}
		else
		{
			ucRet = wellcom_check_data();
			if(ucRet != ERR_OK)
			{
				if(gStruUart0.rx_flg)
				{
					UartProcessFailed(0);
				}
				else
				{
					UartProcessFailed(1);
				}
			}
			else
			{
				//sysChangeClock(ON);
				UartProcessHandle();
				//sysChangeClock(OFF);
			}
		}
		
#endif
		gStruUart1.rx_flg = 0;
		gStruUart1.rx_cnt = 0;
		gStruUart0.rx_flg = 0;
		gStruUart0.rx_cnt = 0;
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
void Uart0_handler(void)
{
	if( (gStruUart0.rx_flg != 2) || (gStruUart0.rx_cnt == 0) )
	{
		return;
	}
	gStruUart0.rx_flg = 0;
	gStruUart0.rx_cnt = 0;
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
void Uart1_handler(void)
{
	if( (gStruUart1.rx_flg != 2) || (gStruUart1.rx_cnt == 0) )
	{
		return;
	}
	gStruUart1.rx_flg = 0;
	gStruUart1.rx_cnt = 0;
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
void UartProcessHandle()
{
	int port = (gStruUart1.rx_flg == 2) ? 1 : 0;
	UINT8 CommandP1;

	CommandP1 = gcUserBuf[3];

	switch(CommandP1)
	{
		case UART0_GETVERISON:  //读取设备版本
		case UART1_GETVERISON:
			Uart_GetVersion(port, gcUserBuf);
			break;
		case UART0_GETDEVID:    //读取设备ID
		case UART1_GETDEVID:
			Uart_GetDevID(port, gcUserBuf);
			break;
		case UART0_SETDEVID:    //设置设备ID
		case UART1_SETDEVID:
			Uart_SetDevID(port, gcUserBuf);
			break;
		case UART0_GETFPIMG:    //读取指纹图像
		case UART1_GETFPIMG:
			Uart_GetFpImage(port, gcUserBuf);
			break;
		case UART0_GETFPTZ:     //读取指纹特征
		case UART1_GETFPTZ:
			Uart_GetFpFeature(port, gcUserBuf);
			break;
		case UART0_GETFPMB:     //读取指纹模板
		case UART1_GETFPMB:
			Uart_GetFpTemplet(port, gcUserBuf);
			break;
		case UART0_GETFPCHECK:  //检测手指
		case UART1_GETFPCHECK:
			Uart_GetFpCheck(port, gcUserBuf);
			break;
		case UART0_GETFPMATCH:  //读取指纹比对
		case UART1_GETFPMATCH:
			Uart_GetFpMatch(port, gcUserBuf);
			break;
		case UART0_GETENDATA:   //数据加密测试
		case UART1_GETENDATA:
			Uart_DataEncrypt(port, gcUserBuf);
			break;
		case UART0_GETFPSEARCH: //读取指纹搜索
		case UART1_GETFPSEARCH:
			Uart_GetFpSearch(port, gcUserBuf);
			break;
		case UART0_GETDEDATA:   //数据解密测试
		case UART1_GETDEDATA:
			Uart_DataDecrypt(port, gcUserBuf);
			break;
		case UART0_GETMBCNT:    //获取模板数量
		case UART1_GETMBCNT:
			Uart_GetMBcnt(port, gcUserBuf);
			break;
		case UART0_GETCLEARMB:  //清空模板
		case UART1_GETCLEARMB:
			Uart_ClearMBindex(port, gcUserBuf);
			break;
		case UART0_SETBTNAME:	//设置名称
		case UART1_SETBTNAME:
			Uart_SetParam(port, gcUserBuf);
		case UART0_SETBTSLEEPTIME:
			Uart_SetSleeptime4Bt(port, gcUserBuf);
		case UART1_SETBTSLEEPTIME:
			break;
		default:
			UartProcessFailed(port);
		break;
	}

	//UartOrgTransferDatas(port, gcUserBuf);
}
UINT8 Uart_SetSleeptime4Bt(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	struct StruPARA sParaTable;
	UINT8 cRetCode = ERR_OK;
	int i=0;
	gDefineSleeptime = CommandP2 ;
	
	cpSend[0] = 0x02;
	cpSend[1] = (2)/256;
	cpSend[2] = (2)%256;
	cpSend[3] = cRetCode;
	cpSend[4] = gDefineSleeptime;
	
	sParaTable = gsParaTable;
	sParaTable.ucTime =gDefineSleeptime; 
	cRetCode = FlashReadWritePara(&sParaTable, FLASHWRITE);
	UartOrgTransferDatas(port, cpSend, CommandP4);
	return (ERR_OK);
}
void Uart_SetParam(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	int iaddr =cpRecv[7]*256+cpRecv[8];
	int inlen = cpRecv[9];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	int i=0;
	char buf[240]={0};
	memcpy(buf,cpRecv+10,inlen);
	cRetCode =SetBTparam(iaddr,buf,inlen);
	if(cRetCode!=ERR_OK)
		cRetCode =SetBTparam(iaddr,buf,inlen);
	cpSend[0] = 0x02;
	cpSend[1] = (2)/256;
	cpSend[2] = (2)%256;
	cpSend[3] = cRetCode;
	cpSend[4] = ERR_OK;
	UartOrgTransferDatas(port, cpSend, CommandP4);
	
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
void Uart_GetVersion(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	WORD wLen = 0;
	WORD i = 0;
	if(gcSensorType == SENSOR_FPC1011)
		wLen = strlen((char*)NEWVERSION_1011);
	else
		wLen = strlen((char*)NEWVERSION_1020);
	cpSend[0] = 0x02;
	cpSend[1] = (wLen + 2) / 256;
	cpSend[2] = (wLen + 2) % 256;
	cpSend[3] = ERR_OK;
	cpSend[4] = 0x00;

	for(i = 0; i < wLen; i++)
	{
		if(gcSensorType == SENSOR_FPC1011)
			cpSend[5 + i] = NEWVERSION_1011[i];
		else
			cpSend[5 + i] = NEWVERSION_1020[i];
	}

	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_GetDevID(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT32 i;

	cpSend[0] = 0x02;
	if(gsParaTable.cDeviceIDFlag == 0x01)
	{
		cpSend[1] = (LENOFDEVICEID + 2) / 256;
		cpSend[2] = (LENOFDEVICEID + 2) % 256;
		cpSend[3] = ERR_OK;
		cpSend[4] = 0x00;
		for(i = 0; i < LENOFDEVICEID; i++)
		{
			cpSend[5 + i] = gsParaTable.cpDeviceID[i];
		}
		
	}
	else
	{
		cpSend[1] = (2) / 256;
		cpSend[2] = (2) % 256;
		cpSend[3] = ERR_NOTOK;
		cpSend[4] = 0x00;
	}
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_SetDevID(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	INT cRetCode;
	INT i;
	struct StruPARA sParaTable;
	UINT8 cpDeviceID[LENOFDEVICEID * 2];

	memcpy(cpDeviceID, cpRecv + 7, LENOFDEVICEID);
	sParaTable = gsParaTable;
	for(i = 0; i < LENOFDEVICEID; i++)
	{
		sParaTable.cpDeviceID[i] = cpDeviceID[i];
	}
	sParaTable.cDeviceIDFlag = 0x01;

	cRetCode = FlashReadWritePara(&sParaTable, FLASHWRITE);

	cpSend[0] = 0x02;
	cpSend[1] = 0x00;
	cpSend[2] = 0x02;
	if(cRetCode == ERR_OK)
	{
		gsParaTable = sParaTable;
		cpSend[3] = ERR_OK;
	}
	else
	{
		cpSend[3] = ERR_WRITE_FLASH;
	}

	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_GetFpImage(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 *cpImage = cpSend + 9;
	UINT8 cRetCode = ERR_OK;


	LED_Y2(ON);
	cRetCode = FpiGetImage(cpImage,CommandP2);
	LED_Y2(OFF);
	
	cpSend[0] = 0x02;
	if(cRetCode == ERR_OK)
	{
		if(gcSensorType==SENSOR_FPC1011)
		{
		cpSend[1] = (FPC1011_WIDTH * FPC1011_HEIGHT + 0x06) / 256;
		cpSend[2] = (FPC1011_WIDTH * FPC1011_HEIGHT + 0x06) % 256;
		cpSend[3] = cRetCode;
		cpSend[5] = FPC1011_WIDTH / 256;
		cpSend[6] = FPC1011_WIDTH % 256;
		cpSend[7] = FPC1011_HEIGHT / 256;
		cpSend[8] = FPC1011_HEIGHT % 256;
		}
		else if(gcSensorType==SENSOR_FPC1020)
		{
		cpSend[1] = (FPC1020_WIDTH * FPC1020_HEIGHT + 0x06) / 256;
		cpSend[2] = (FPC1020_WIDTH * FPC1020_HEIGHT + 0x06) % 256;
		cpSend[3] = cRetCode;
		cpSend[5] = FPC1020_WIDTH / 256;
		cpSend[6] = FPC1020_WIDTH % 256;
		cpSend[7] = FPC1020_HEIGHT / 256;
		cpSend[8] = FPC1020_HEIGHT % 256;
		}
	}
	else
	{
		cpSend[1] = (2) / 256;
		cpSend[2] = (2) % 256;
		cpSend[3] = ERR_READ_FINGERPRINT;
	}
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_GetFpFeature(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	INT wLenOfTZ,i;
	
	LED_Y2(ON);
	if(CommandP2==0)
		CommandP2 =15;
	cRetCode = FpiGetImage(gcUserBuf,CommandP2);
	LED_Y2(OFF);


	if(cRetCode==ERR_OK)
	{
		if(gcSensorType == SENSOR_FPC1011)
		{
			#if FINGER_LIB ==2
			cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,gcUserBuf, gcTzBuff, &wLenOfTZ);
			#else
			cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,gcUserBuf, gcTzBuff, &wLenOfTZ);
			#endif
		}
		else if(gcSensorType == SENSOR_FPC1020)
		{
			#if FINGER_LIB ==2
			cRetCode = GFP_ExtractAny(FPC1020_WIDTH, FPC1020_HEIGHT, 508 ,gcUserBuf, gcTzBuff, &wLenOfTZ);
			#else
			cRetCode = GFP_Extract(FPC1020_WIDTH, FPC1020_HEIGHT, 508 ,gcUserBuf, gcTzBuff, &wLenOfTZ);
			#endif
		}
	}
	
	// 发送包头
	cpSend = &gcUserBuf[0];
	cpSend[0] = 0x02;
	if(cRetCode==ERR_OK)
	{
		cpSend[1] = (LENOFMBINFO + 2)/256;
		cpSend[2] = (LENOFMBINFO + 2)%256;
		cpSend[3] = ERR_OK;

		cpSend[5] = wLenOfTZ/256;
		cpSend[6] = wLenOfTZ%256;
		for (i=0; i<wLenOfTZ; i++)           			// 有效数据
			cpSend[7+i] = gcTzBuff[i];
		for (i=wLenOfTZ; i<LENOFMBINFO - 2; i++)        // 无效数据
			cpSend[7+i] = 0x00;

	}
	else
	{
		cpSend[1] = (2)/256;
		cpSend[2] = (2)%256;
		cpSend[3] = ERR_SAMPLE;
	}
	UartOrgTransferDatas(port, cpSend, CommandP4);
}
int SaveMBLib(uint8 id,uint8 *cpmb)
{
	int i=0;
	for(i=0;i<giMBCounter;i++)
	{
		if(id == spMBLib[i].ucUsrid)
		{
			memcpy(spMBLib[i].cpMB,cpmb,LENOFMB);
			spMBLib[i].cCrc = CrcOfMBLibItem(&spMBLib[i]);
			return WriteMBLibToFlash();
			
		}
	}
	spMBLib[i].cFlag=1;
	spMBLib[i].ucUsrid=id;
	memcpy(spMBLib[i].cpMB,cpmb,LENOFMB);
	spMBLib[i].cCrc = CrcOfMBLibItem(&spMBLib[i]);
	giMBCounter++;
	return WriteMBLibToFlash();
	
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
void Uart_GetFpTemplet(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	INT cRetCode=0,step=0;
	INT wLenOfMB;
	UINT16 i;
	UINT8 *cpSendBuf = &gcUserBuf[0];
	UINT8 *cpImageBuf =&gcUserBuf[0];//图像数据

	if(CommandP2==0)
		CommandP2 =15;
		
	memset(gcTz1Buff,0,512*3);
	memset(gcMBBuff,0,sizeof(gcMBBuff));
	while(CommandP2--)
	{
		LED_Y2(ON);
		cRetCode = FpiGetImage(cpImageBuf,1);
		LED_Y2(OFF);

		if(cRetCode == ERR_OK)
		{
			//手指按下
			if(gcSensorType == SENSOR_FPC1011)
			{
				#if FINGER_LIB ==2
				cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,cpImageBuf, &gcTz1Buff[step][0], &wLenOfMB);
				#else
				cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,cpImageBuf, &gcTz1Buff[step][0], &wLenOfMB);
				#endif
			}
			else if(gcSensorType == SENSOR_FPC1020)
			{
				#if FINGER_LIB ==2
				cRetCode = GFP_ExtractAny(FPC1020_WIDTH, FPC1020_HEIGHT, 508 ,cpImageBuf,  &gcTz1Buff[step][0], &wLenOfMB);
				#else
				cRetCode = GFP_Extract(FPC1020_WIDTH, FPC1020_HEIGHT, 508 ,cpImageBuf,  &gcTz1Buff[step][0], &wLenOfMB);
				#endif
			}
	
			if(cRetCode==ERR_OK)
			{
				step++;
			}
			if(step>=3)
				break;
		}
	}
	

	if(cRetCode==ERR_OK&&step>=3)
	{
		#if FINGER_LIB ==2
		cRetCode = GFP_EnrollX(&gcTz1Buff[0][0],&gcTz1Buff[1][0],&gcTz1Buff[2][0],gcMBBuff,&wLenOfMB);
		#else
		cRetCode = GFP_EnrollX(&gcTz1Buff[0][0],&gcTz1Buff[1][0],&gcTz1Buff[2][0],gcMBBuff,&wLenOfMB);
		#endif
		
	}
	else
	{
		cRetCode=ERR_SAMPLE;
	}
	if(cRetCode==ERR_OK)
	{
		if(CommandP3>0)
		{
			cRetCode = SaveMBLib(CommandP3,gcMBBuff);
		}
	}
	
	cpSendBuf[0] = 0x02;
	if(cRetCode==ERR_OK&&step>=3)
	{
		cpSendBuf[1] = (LENOFMBINFO + 2)/256;
		cpSendBuf[2] = (LENOFMBINFO + 2)%256;
		cpSendBuf[3] = ERR_OK;

		cpSendBuf[5] = wLenOfMB/256;
		cpSendBuf[6] = wLenOfMB%256;
		for (i=0; i<wLenOfMB; i++)           			// 有效数据
			cpSendBuf[7+i] = gcMBBuff[i];
		for (i=wLenOfMB; i<LENOFMBINFO - 2; i++)        // 无效数据
			cpSendBuf[7+i] = 0x00;
	}
	else
	{
		cpSendBuf[1] = (2)/256;
		cpSendBuf[2] = (2)%256;
		cpSendBuf[3] = ERR_SAMPLE;
	}
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_GetFpCheck(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	UINT8 *cpImage = gcUserBuf;
	
	cRetCode = FpiGetImage(cpImage,0);
	cpSend[0] = 0x02;
	cpSend[1] = (1 + 2)/256;
	cpSend[2] = (1 + 2)%256;
	cpSend[3] = ERR_OK;
	if(cRetCode == ERR_OK)
		cpSend[5] = 1;
	else	
		cpSend[5] = 0;
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_GetFpMatch(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	INT32 cRetCode;
	int score;
	#if FINGER_LIB ==2
	cRetCode=GFP_Verify(gcMBBuff, gcTzBuff, 3);
	if(cRetCode>=ERR_OK)
	{
		//回传分数
		cpSend[5] = cRetCode/256;
		cpSend[6] = cRetCode%256;
		cRetCode=ERR_OK;
	
	}
	else
	{
		cpSend[5] = 0x00;
		cpSend[6] = 0x00;
		cRetCode=ERR_MATCH;
	}
	#else
	cRetCode=GFP_Verify(gcMBBuff, gcTzBuff, 3,&score);
	if(cRetCode==ERR_OK)
	{
		//回传分数
		if(score>0)
		{
			cpSend[5] = score/256;
			cpSend[6] = score%256;
			cRetCode=ERR_OK;
		}
		else
		{
			cpSend[5] = 0x00;
			cpSend[6] = 0x00;
			cRetCode=ERR_MATCH;
		}
	}
	else
	{
		cpSend[5] = 0x00;
		cpSend[6] = 0x00;
		cRetCode=ERR_MATCH;
	}
	#endif
	cpSend[0] = 0x02;
	cpSend[1] = 0x00;
	cpSend[2] = 0x02+2;
	cpSend[3] =cRetCode;
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_DataEncrypt(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	UINT8 cpDst[32]={0};
	INT iLen=32;
	EncryptVerify(cpDst,&iLen);	
	cpSend[0] = 0x02;
	cpSend[1] = (iLen + 2)/256;
	cpSend[2] = (iLen + 2)%256;
	cpSend[3] = ERR_OK;
	memcpy(cpSend+5,cpDst,iLen);
	UartOrgTransferDatas(port, cpSend, CommandP4);
}

int  SearchByVerify(uint8 *cptz)
{
	INT32 cRetCode,i,score=0;
	for(i=0;i<giMBCounter;i++)
	{
		#if FINGER_LIB ==2
			cRetCode=GFP_Verify(spMBLib[i].cpMB, cptz, 3);
			if(cRetCode>=ERR_OK)
			{
				return spMBLib[i].ucUsrid;
			}

		#else
			cRetCode=GFP_Verify(spMBLib[i].cpMB, cptz, 3,&score);
			if(cRetCode==ERR_OK&&score>0)
			{
				return spMBLib[i].ucUsrid;
			}
		#endif
	}
	return -1;
	
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
void Uart_GetFpSearch(int port, UINT8 *cpRecv)
{
#if 0
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];

	BYTE cpFeature[LENOFTZ];         // 指纹数组
	BYTE cRetCode = ERR_NOTOK;
	WORD wStartIndex, wEndIndex;
	WORD i, iLength;
	INT wLenOfTZ;                   // 指纹长度
	BYTE cSearchFlag;
	WORD wIndex;
	UINT8 *cpImageBuf =  (BYTE*)malloc(TCS1_WIDTH*TCS1_HEIGHT);//图像

	wEndIndex=giMBCounter;
	cSearchFlag=0;
	cRetCode = FpiGetImage(cpImageBuf,15);
	memset(cpFeature,0,sizeof(cpFeature));
	if(cRetCode==ERR_OK)
	{
		#if FINGER_LIB ==2
		cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,cpImageBuf, cpFeature, &wLenOfTZ);
		#else
		cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,cpImageBuf, cpFeature, &wLenOfTZ);
		#endif
	}
	free(cpImageBuf);

	LED_Y2(ON);
	
	if (cRetCode==ERR_OK)
	{
		cRetCode = FpiSearch(cpFeature, cSearchFlag, wEndIndex, &wIndex);
	}
	// 组包

	iLength = 2 + LENOFUSERINFO + 2;
	cpSend[0] = 0x02;
	cpSend[1] = iLength/256;
	cpSend[2] = iLength%256;
	cpSend[3] = cRetCode;
	cpSend[4] = wIndex;

	if (cRetCode==ERR_OK)
	{
		cpSend[5] = spMBLib[wIndex].wLenOfUserInfo/256;
		cpSend[6] = spMBLib[wIndex].wLenOfUserInfo%256;

		for (i=0; i<spMBLib[wIndex].wLenOfUserInfo; i++)
			cpSend[7 + i] = spMBLib[wIndex].cpUserInfo[i];
		for (i=spMBLib[wIndex].wLenOfUserInfo; i<LENOFUSERINFO; i++)
			cpSend[7 + i] = 0x00;

	}
	else
	{
		cpSend[5] = 0x00;
		cpSend[6] = 0x00;
		for (i=0; i<LENOFUSERINFO; i++)
			cpSend[7 + i] = 0x00;
	}

	cpSend[7 + LENOFUSERINFO] = 0x00;					// CRC
	cpSend[8 + LENOFUSERINFO] = 0x03;

	UartOrgTransferDatas(port, cpSend, CommandP4);
#else
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];

	BYTE cpFeature[LENOFTZ];         // 指纹数组
	BYTE cRetCode = ERR_NOTOK;
	WORD wStartIndex, wEndIndex;
	WORD i, iLength;
	INT wLenOfTZ,id;                   // 指纹长度
	BYTE cSearchFlag;
	WORD wIndex;
	UINT8 *cpImageBuf =  (BYTE*)malloc(TCS1_WIDTH*TCS1_HEIGHT);//图像

	wEndIndex=giMBCounter;
	cSearchFlag=0;
	LED_Y2(ON);
	cRetCode = FpiGetImage(cpImageBuf,15);
	LED_Y2(OFF);
	memset(cpFeature,0,sizeof(cpFeature));
	if(cRetCode==ERR_OK)
	{
		#if FINGER_LIB ==2
		cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,cpImageBuf, cpFeature, &wLenOfTZ);
		#else
		cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363 ,cpImageBuf, cpFeature, &wLenOfTZ);
		#endif
	}
	free(cpImageBuf);

	
	
	if (cRetCode==ERR_OK)
	{
		id = SearchByVerify(cpFeature);
		if(id<0)
		{
			cRetCode =ERR_MATCH;
			wIndex =0;
		}
		else
		{
			cRetCode=ERR_OK;
			wIndex =id;
		}
	}
	// 组包
	
	iLength = 3 ;
	cpSend[0] = 0x02;
	cpSend[1] = iLength/256;
	cpSend[2] = iLength%256;
	cpSend[3] = cRetCode;
	cpSend[5] = wIndex;
	UartOrgTransferDatas(port, cpSend, CommandP4);
#endif
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
void Uart_DataDecrypt(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];

	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_GetMBcnt(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	cpSend[0] = 0x02;
	cpSend[1] = (1 + 2)/256;
	cpSend[2] = (1 + 2)%256;
	cpSend[3] = ERR_OK;
	cpSend[5] = giMBCounter;
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void Uart_ClearMBindex(int port, UINT8 *cpRecv)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	int i=0;
	if(CommandP2 ==0)
	{
		memset((BYTE*)spMBLib,0,sizeof(spMBLib));
		cRetCode =WriteMBLibToFlash();
		giMBCounter = 0;
	}
	else
	{
		for(i=0;i<giMBCounter;i++)
		{
			if(CommandP3 == spMBLib[i].ucUsrid)
			{
				memset(spMBLib[i].cpMB,0,LENOFMB);
				spMBLib[i].cFlag =0;
				spMBLib[i].cCrc = CrcOfMBLibItem(&spMBLib[i]);
				
				cRetCode =WriteMBLibToFlash();
				giMBCounter--;
				break;
			}
		}
	}
	cpSend[0] = 0x02;
	cpSend[1] = (2)/256;
	cpSend[2] = (2)%256;
	cpSend[3] = cRetCode;
	UartOrgTransferDatas(port, cpSend, CommandP4);
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
void UartProcessFailed(int port)
{
	gcUserBuf[0] = 0x02;
	gcUserBuf[1] = 0x00;
	gcUserBuf[2] = 0x02;
	gcUserBuf[3] = ERR_NOTOK;
	gcUserBuf[4] = ERR_NOTOK;
	gcUserBuf[5] = 0x00;                    // CRC
	gcUserBuf[6] = 0x03;
	UartOrgTransferDatas(port, gcUserBuf, 0);
}

// 组织数据发送
int UartCmdGenAck(UINT8 *cpBuf, UINT8 Command)
{
	UINT32 wLen;
	UINT32 i;
	UINT8 cCrc;

	// 取数据部分的长度
	wLen = cpBuf[1] * 256 + cpBuf[2];
	cpBuf[4] = Command;
	// 计算CRC校验
	cCrc = 0;
	for(i = 0; i < wLen + 2; i++)
	{
		cCrc ^= cpBuf[i + 1];
	}
	cpBuf[wLen + 3] = cCrc;
	cpBuf[wLen + 4] = 0x03;
	return wLen + 5;
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
void UartOrgTransferDatas(int port, UINT8 *cpBuf, UINT8 Command)
{
	int ilen;
	BlinkY2(2,15);
	ilen= UartCmdGenAck(cpBuf, Command);
	UartSendString(port, cpBuf, ilen);
	//UartSendString(1, cpBuf, ilen);
}
extern void In2BT();
//获取蓝牙参数要求的最大长度
int GetBTparamLenth(int addr ,int *len)
{
	unsigned char paddr[10]=
	{	0x00,		//mac
		0x07,		//cod
		0x0b,		//name
		0xbc,		//Enter_Sniff_Waiting_Time
		0xc7,		//Supervision Timeout
		
		0xc3,		//Passkey_Entry_Timeout
		0xBE,		//Linkback_Times	
		0xc8,   //Lose_Link_Linkback_Times	
		0xc0,   //Remote_Disconnent_Standby_Time
		0xc1,		//Wakeup_External_MCU_Wait_Time

	};
	unsigned char  plenth[10]=
	{	
		6,		//mac 
		3,		//cod
		16,		//name
		1,		//Enter_Sniff_Waiting_Time
		1,		//Supervision Timeout
		
		1,		//Passkey_Entry_Timeout
		1,		//Linkback_Times	
		1,    //Lose_Link_Linkback_Times	
		1,    //Remote_Disconnent_Standby_Time
		1,		//Wakeup_External_MCU_Wait_Time
	};
	unsigned char i=0;
	for(i=0;i<10;i++)
	{
		if(paddr[i]==addr)
		{
			*len = plenth[i];
			return 1;
		}
	}
			
	*len = 0;	
	return 0;

}
//设置蓝牙参数信息
int  SetBTparam(int addr,char *cpbuf,int ilen)
{

	int i=0,j=0;
	int rlen=0;
	int iaddr = addr;
	int iret=0;
	uint8 buf[256]={0};
	uint8 by;
	In2BT();
	iret = GetBTparamLenth(addr,&rlen);
	if(iret==0)
		return ERR_PARAM;
	if(ilen>rlen)
		return ERR_PARAM;
	
	i=0;
	//type
	buf[i++]=0x01;
	//command
	buf[i++]=0x27;
	//OGF
	buf[i++]=0xFC;
	//lenth
	buf[i++]=rlen+2+1;//data(len)+addr(2)+len4data(1)
	buf[i++]=iaddr>>8;
	buf[i++]=iaddr&0xff;
	buf[i++]=rlen;
	
	memcpy(buf+i,cpbuf,ilen);
	sysDisableInterrupt(IRQ_UART);
	UartSendString(1, buf, buf[3]+4);
	i=0;
	iret = sysGetChar(1,1500,&by);
	if(iret ==0)
	{
		sysEnableInterrupt(IRQ_UART);
		Out2BT();
		return ERR_TIMEOUT;
	}
	buf[i++] = by;
	if(buf[0]!=0x04)
	{
		sysEnableInterrupt(IRQ_UART);
		Out2BT();
		return ERR_NOTOK;
	}
	while(1)
	{
		
		iret = sysGetChar(1,100,&by);
		if(iret ==0)
		{
			sysEnableInterrupt(IRQ_UART);
			Out2BT();
			return ERR_TIMEOUT;
		}
		buf[i++] = by;
		if(i==7)
		{
			if(buf[6]==0x00)
				iret =ERR_OK;
			else
				iret = ERR_NOTOK;
			break;
		}
	}

	Delayms(1000);
	sysEnableInterrupt(IRQ_UART);
	Out2BT();
	return iret;
	
}
/************************************** The End Of File **************************************/

