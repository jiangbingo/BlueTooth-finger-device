//*******************************************************************************
//说明：维尔 USB指令数据处理流程
//时间：20130131
//作者：wellcom
//*******************************************************************************
#include  "IncludeAll.h"
extern UINT8 *Sprintfs;
#pragma Otime

UINT8 g_Record[1600];
extern StuHid gStuHidTrans;

//维尔用户函数
UINT8 UsbCmdGetVersion(UINT8 *cpRecBuf, INT *ilen);         //获取版本号

UINT8 UsbCmdGetLibVersion(UINT8 *cpRecBuf, INT *ilen);     //获取算法版本号
UINT8 UsbCmdSetDeviceID(UINT8 *cpRecBuf, INT *ilen);        //设置序列号


UINT8 UsbCmdGetDeviceID(UINT8 *cpRecBuf, INT *ilen);        //获取序列号


UINT8 UsbCmdSample(UINT8 *cpRecBuf, INT *ilen);             //采集模板


UINT8 UsbCmdEnroll(UINT8 *cpRecBuf, INT *ilen);             //采集特征


UINT8 UsbCmdUpdate(UINT8 *cpRecBuf, INT *ilen);             //程序升级


UINT8 UsbCmdGetImage(UINT8 *cpRecBuf, INT *ilen);           //立即采图像


UINT8 UsbCmdCheckFinger(UINT8 *cpRecBuf, INT *ilen);        //检测手指


UINT8 UsbCmdClearMBLib(UINT8 *cpRecBuf, INT *ilen);         //清楚指纹模板库


UINT8 UsbCmdInsertToMBLib(UINT8 *cpRecBuf, INT *ilen);      //增加指纹模板


UINT8 UsbCmdWriteMBLib(UINT8 *cpRecBuf, INT *ilen);         //写指纹模板


UINT8 UsbCmdGetMBLibCounter(UINT8 *cpRecBuf, INT *ilen);    //获取指纹模板数量


UINT8 UsbCmdSearch(UINT8 *cpRecBuf, INT *ilen);             //搜索指纹


UINT8 UsbCmdVerifyFinger(UINT8 *cpRecBuf, INT *ilen);       //验证对比


UINT8 UsbCmdGetSensorType(UINT8 *cpRecBuf, INT *ilen);


void UsbProcessFailed(INT32 *ilen);

//begin 此功能是同步蓝牙接口而设置的功能 zhangys 2014年7月23日 

UINT8 USBGetFpSearch4Bt(UINT8 *cpRecv,INT *ilen);
UINT8 UsbGetMBcnt4Bt(UINT8 *cpRecv,INT *ilen);
UINT8 UsbClearMBindex4Bt(UINT8 *cpRecv,INT *ilen);
UINT8 UsbSetName4Bt(UINT8 *cpRecv,INT *ilen);	
UINT8 UsbSetSleeptime4Bt(UINT8 *cpRecv,INT *ilen);
//end 此功能是同步蓝牙接口而设置的功能 zhangys 2014年7月23日 
//*******************************************************************************
//维尔数据校验
//zhangys
//20130131
//*******************************************************************************
UINT8 wellcom_check_data(void)
{
	UINT32 i, len;
	UINT8 cCrc;

	if(gcUserBuf[0] == 0x02)
	{
		len = gcUserBuf[1] * 256 + gcUserBuf[2];    //除规定部分长度
		len += 5;                                   //数据总长度
		if(gcUserBuf[len - 1] == 0x03)              //尾判断
		{
			cCrc = 0;
			for(i = 1; i < len - 2; i++)
			{
				cCrc ^= gcUserBuf[i];
			}

			if(cCrc == gcUserBuf[len - 2])         //解析数据通过
			{
				return 0;
			}
		}
	}
	return 0xff;
}

//*******************************************************************************
//维尔数据组织
//zhangys
//20130131
//*******************************************************************************
int CmdGenAck(UINT8 *cpBuf)
{
	UINT32 wLen;
	UINT32 i;
	UINT8 cCrc;

	// 取数据部分的长度
	wLen = cpBuf[1] * 256 + cpBuf[2];

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

//*******************************************************************************
//维尔数据处理流程
//zhangys
//20130131
//*******************************************************************************
UINT8 UsbCmdProcess(UINT8 *cpUSBCmdBuf, INT32 *ilen)
{
	UINT8 CommandP1;
	UINT8 cRet = 0;
	INT Len;

	CommandP1 = cpUSBCmdBuf[3];
	if(CommandP1 != COM_UPDATE)
	{
		switch(CommandP1)
		{
			case COM_GETVERISON:                                // 读取版本
				cRet = UsbCmdGetVersion(cpUSBCmdBuf, &Len);
				BlinkY2(1, 20);
				break;
			case COM_GETLIBVERISON:
				cRet = UsbCmdGetLibVersion(cpUSBCmdBuf, &Len);
				BlinkY2(1, 20);
				break;
			case COM_SETDEVID:
				cRet = UsbCmdSetDeviceID(cpUSBCmdBuf, &Len);    // 设置序列号
				BlinkY2(1, 20);
				break;
			case COM_GETDEVID:
				cRet = UsbCmdGetDeviceID(cpUSBCmdBuf, &Len);    // 读取序列号
				BlinkY2(1, 20);
				break;
			case COM_GETFPMB:                                   // 读模版
				cRet = UsbCmdEnroll(cpUSBCmdBuf, &Len);
				break;
			case COM_GETFPTZ:                                   // 读特征
				cRet = UsbCmdSample(cpUSBCmdBuf, &Len);
				break;
			case COM_GETFPCHECK:
				cRet = UsbCmdCheckFinger(cpUSBCmdBuf, &Len);
				break;
			case COM_GETFPIMG:
				cRet = UsbCmdGetImage(cpUSBCmdBuf, &Len);       //立即采图像并且上传
				break;
			case COM_GETFPMATCH:
				cRet = UsbCmdVerifyFinger(cpUSBCmdBuf, &Len);
				BlinkY2(1, 20);
				break;
			case COM_GETSENSORTYPE:
				cRet = UsbCmdGetSensorType(cpUSBCmdBuf, &Len);
				BlinkY2(1, 20);
				break;
			case COM_GETCLEARMB:
				cRet = UsbCmdClearMBLib(cpUSBCmdBuf, &Len);         //清空模板库
				BlinkY2(1, 20);
				break;
			case COM_INSERTMB:
				cRet = UsbCmdInsertToMBLib(cpUSBCmdBuf, &Len);      //插入模板库
				BlinkY2(1, 20);
				break;
			case COM_GETMBCNT:
				cRet = UsbCmdGetMBLibCounter(cpUSBCmdBuf, &Len);    //获取模板数量
				BlinkY2(1, 20);
				break;
			case COM_GETFPSEARCH:
				cRet = UsbCmdSearch(cpUSBCmdBuf, &Len);             //指纹搜索对比
				break;
			//begin 此功能是同步蓝牙接口而设置的功能 zhangys 2014年7月23日 
			//指纹搜索，此过程采用直接比对方式
			case UART0_GETFPSEARCH:
				USBGetFpSearch4Bt(cpUSBCmdBuf, &Len);
			break;
			//获取指纹模板数量
			case UART0_GETMBCNT:
				 UsbGetMBcnt4Bt(cpUSBCmdBuf, &Len);
			break;
			//删除指定的模板或者全部删除
			case UART0_GETCLEARMB:
				UsbClearMBindex4Bt(cpUSBCmdBuf, &Len);
			break;
			//设置名称
			case UART0_SETBTNAME:
				UsbSetName4Bt(cpUSBCmdBuf, &Len);
				BlinkY2(1, 20);
			break;
			//设置时间 
			case UART0_SETBTSLEEPTIME:	
				UsbSetSleeptime4Bt(cpUSBCmdBuf, &Len);
				BlinkY2(1, 20);
			break;
			//end 此功能是同步蓝牙接口而设置的功能 zhangys 2014年7月23日 
			default:
				UsbProcessFailed(&Len);
				BlinkY2(1, 20);
				break;
		}
		
	}
	else
	{   //程序升级
		cRet = UsbCmdUpdate(cpUSBCmdBuf, &Len);
	}
	*ilen = Len;
	return (cRet);
}
//begin 此功能是同步蓝牙接口而设置的功能 zhangys 2014年7月23日
int  SearchByVerify(uint8 *cptz);
UINT8 USBGetFpSearch4Bt(UINT8 *cpRecv,INT *ilen)
{
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
			id =0;
		}
		else
		{
			cRetCode=ERR_OK;
		}
	}
	// 组包
	
	iLength = 3 ;
	cpSend[0] = 0x02;
	cpSend[1] = iLength/256;
	cpSend[2] = iLength%256;
	cpSend[3] = cRetCode;
	cpSend[4] = id;
	cpSend[5] = id;

	*ilen = CmdGenAck(cpSend);
	return (ERR_OK);

}
UINT8 UsbGetMBcnt4Bt(UINT8 *cpRecv,INT *ilen)
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
	cpSend[4] = ERR_OK;
	cpSend[5] = giMBCounter;
	*ilen = CmdGenAck(cpSend);
	return (ERR_OK);
}

UINT8 UsbClearMBindex4Bt(UINT8 *cpRecv,INT *ilen)
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
	cpSend[4] = ERR_OK;
	*ilen = CmdGenAck(cpSend);
	return (ERR_OK);
}
UINT8 UsbSetName4Bt(UINT8 *cpRecv,INT *ilen)
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
	cpSend[0] = 0x02;
	cpSend[1] = (2)/256;
	cpSend[2] = (2)%256;
	cpSend[3] = cRetCode;
	cpSend[4] = ERR_OK;
	*ilen = CmdGenAck(cpSend);
	return (ERR_OK);
}

UINT8 UsbSetSleeptime4Bt(UINT8 *cpRecv,INT *ilen)
{
	UINT8 CommandP2 = cpRecv[4];
	UINT8 CommandP3 = cpRecv[5];
	UINT8 CommandP4 = cpRecv[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	struct StruPARA sParaTable;
	int i=0;
	gDefineSleeptime = CommandP2 ;
	
	cpSend[0] = 0x02;
	cpSend[1] = (2)/256;
	cpSend[2] = (2)%256;
	cpSend[3] = cRetCode;
	cpSend[4] = gDefineSleeptime;
	
	sParaTable = gsParaTable;
	sParaTable.ucTime =gDefineSleeptime; 
	gsParaTable.ucTime = gDefineSleeptime;
	cRetCode = FlashReadWritePara(&sParaTable, FLASHWRITE);
	*ilen = CmdGenAck(cpSend);
	return (ERR_OK);
}
//end 此功能是同步蓝牙接口而设置的功能 zhangys 2014年7月23日 
UINT8 UsbCmdGetLibVersion(UINT8 *cpRecBuf, INT *ilen)     //获取算法版本号
{
	UINT32 i;
	UINT32 wLen;
	UINT8 *cpSendBuf = &gcUserBuf[0];

	wLen = strlen( (char*)LIBVERSION);
	cpSendBuf[0] = 0x02;
	cpSendBuf[1] = (wLen + 3) / 256;
	cpSendBuf[2] = (wLen + 3) % 256;
	cpSendBuf[3] = ERR_OK;
	cpSendBuf[4] = 0x00;

	cpSendBuf[5] = wLen;
	for(i = 0; i < wLen; i++)
	{
		cpSendBuf[6 + i] = LIBVERSION[i];
	}
	*ilen = CmdGenAck(cpSendBuf);
	return (ERR_OK);

}
// ***************************************************
// 函数: CmdGetVersion
// 功能: 读取版本信息
// 入口: 无
// 返回: ERR_OK  返回成功
// 影响:
// ***************************************************
UINT8 UsbCmdGetVersion(UINT8 *cpRecBuf, INT *ilen)
{
	UINT32 i;
	UINT32 wLen;
	UINT8 *cpSendBuf = &gcUserBuf[0];

	cpSendBuf[0] = 0x02;
	cpSendBuf[1] = (LENOFVERSION + 2) / 256;
	cpSendBuf[2] = (LENOFVERSION + 2) % 256;
	cpSendBuf[3] = ERR_OK;
	cpSendBuf[4] = 0x00;

	if(gcSensorType == SENSOR_FPC1011)
		wLen = strlen((char*)NEWVERSION_1011);
	else
		wLen = strlen((char*)NEWVERSION_1020);
	for(i = 0; i < wLen; i++)
	{
		if(gcSensorType == SENSOR_FPC1011)
			cpSendBuf[6 + i] = NEWVERSION_1011[i];
		else
			cpSendBuf[6 + i] = NEWVERSION_1020[i];
	}
	cpSendBuf[5] = wLen;


	for(i = wLen; i < LENOFVERSION - 1; i++)
	{
		cpSendBuf[6 + i] = 0x00;
	}

	cpSendBuf[6 + LENOFVERSION - 1] = 0x00;           // CRC
	cpSendBuf[7 + LENOFVERSION - 1] = 0x03;
	CmdGenAck(cpSendBuf);
	*ilen = 7 + LENOFVERSION;
	return (ERR_OK);
}

//=============================================================
// 0x0B - 设置指纹模块序列号
//=============================================================
UINT8 UsbCmdSetDeviceID(UINT8 *cpRecBuf, INT *ilen)
{
	INT cRetCode;
	INT i;
	UINT8 *gcpSendBuf = &gcUserBuf[0];
	UINT8 cpDeviceID[LENOFDEVICEID * 2];
	struct StruPARA sParaTable;

	memcpy(cpDeviceID, cpRecBuf + 7, LENOFDEVICEID);
	sParaTable = gsParaTable;
	for(i = 0; i < LENOFDEVICEID; i++)
	{
		sParaTable.cpDeviceID[i] = cpDeviceID[i];
	}
	sParaTable.cDeviceIDFlag = 0x01;

	cRetCode = FlashReadWritePara(&sParaTable, FLASHWRITE);
	if(cRetCode == ERR_OK)
	{
		gsParaTable = sParaTable;
	}
	else
	{
		cRetCode = ERR_READ_FLASH;
	}

	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = 0x00;
	gcpSendBuf[2] = 0x02;
	gcpSendBuf[3] = cRetCode;
	gcpSendBuf[4] = 0x00;
	gcpSendBuf[5] = 0x00;                   // CRC
	gcpSendBuf[6] = 0x03;
	CmdGenAck(gcpSendBuf);

	*ilen = 7;
	return (ERR_OK);
}

//=============================================================
// 0x0C - 读取指纹模块的设备序号
//=============================================================
UINT8 UsbCmdGetDeviceID(UINT8 *cpRecBuf, INT *ilen)
{
	UINT32 i;
	UINT8 *gcpSendBuf = &gcUserBuf[0];

	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = (LENOFDEVICEID + 2) / 256;
	gcpSendBuf[2] = (LENOFDEVICEID + 2) % 256;

	if(gsParaTable.cDeviceIDFlag == 0x01)
	{
		gcpSendBuf[3] = ERR_OK;
		gcpSendBuf[4] = 0x00;
		for(i = 0; i < LENOFDEVICEID; i++)
		{
			gcpSendBuf[5 + i] = gsParaTable.cpDeviceID[i];
		}
	}
	else
	{
		gcpSendBuf[3] = ERR_READ_FLASH;
		gcpSendBuf[4] = 0x00;
		for(i = 0; i < LENOFDEVICEID; i++)
		{
			gcpSendBuf[5 + i] = 0x00;
		}
	}
	gcpSendBuf[5 + LENOFDEVICEID] = 0x00;           // CRC
	gcpSendBuf[6 + LENOFDEVICEID] = 0x03;
	CmdGenAck(gcpSendBuf);
	*ilen = 7 + LENOFDEVICEID;
	return ERR_OK;
}

//=============================================================
// 函数: UsbCmdGetImage
// 功能: 立即获取指纹图像并且上传指纹图像
// 入口: 无
// 返回: ERR_OK     返回成功
//       非ERR_OK   返回失败
// 影响:
//=============================================================
UINT8 UsbCmdGetImage(UINT8 *cpRecBuf, INT *ilen)
{
	UINT8 CommandP2 = cpRecBuf[4];
	UINT8 CommandP3 = cpRecBuf[5];
	UINT8 CommandP4 = cpRecBuf[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 *cpImage = cpSend + 9;
	UINT8 cRetCode = ERR_OK;
	
	LED_Y2(ON);
	cRetCode = FpiGetImage(cpImage, CommandP2);
	LED_Y2(OFF);

	cpSend[0] = 0x02;
	if(CommandP2 == 0)
	{
		cRetCode = ERR_OK;
	}

	if(cRetCode == ERR_OK)
	{
		if(gcSensorType == SENSOR_FPC1011)
		{
			cpSend[1] = (FPC1011_WIDTH * FPC1011_HEIGHT + 0x06) / 256;
			cpSend[2] = (FPC1011_WIDTH * FPC1011_HEIGHT + 0x06) % 256;
			cpSend[3] = cRetCode;
			cpSend[5] = FPC1011_WIDTH / 256;
			cpSend[6] = FPC1011_WIDTH % 256;
			cpSend[7] = FPC1011_HEIGHT / 256;
			cpSend[8] = FPC1011_HEIGHT % 256;
		}
		else if(gcSensorType == SENSOR_FPC1020)
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
	*ilen = CmdGenAck(cpSend);
	return (ERR_OK);
}

//=============================================================
// 函数: USBCmdCheckFinger
// 功能:检测手指
// 入口: 无
// 返回: ERR_OK     返回成功
//       非ERR_OK   返回失败
// 影响:
//=============================================================
BYTE UsbCmdCheckFinger(BYTE *cpRecBuf, INT *ilen)
{
	//BYTE cRetCode;
	UINT8 *cpSendBuf = &gcUserBuf[0], cRetCode;
	UINT8 *cpSourceImage = &gcUserBuf[100];

	LED_Y2(ON);
	cRetCode = FpiGetImage(cpSourceImage, 0);
	if(cRetCode == ERR_OK)
	{
		cpSendBuf[5] =1;
	}
	else
	{
		cpSendBuf[5] =0;
	}
	cpSendBuf[0] = 0x02;
	cpSendBuf[1] = 0x00;
	cpSendBuf[2] = 0x03;
	cpSendBuf[3] = 0x00;
	cpSendBuf[4] = 0x00;
	cpSendBuf[7] = 0x00;                    // CRC
	cpSendBuf[8] = 0x03;
	CmdGenAck(cpSendBuf);
	*ilen = 9;
	LED_Y2(OFF);
	return (ERR_OK);
}

//=============================================================
// 0x12 - 采集指纹特征
//2013-7-2 zhangys 特征长度由原来256-->384
//=============================================================
UINT8 UsbCmdSample(UINT8 *cpRecBuf, INT *ilen)
{
	UINT8 CommandP2 = cpRecBuf[4];
	UINT8 CommandP3 = cpRecBuf[5];
	UINT8 CommandP4 = cpRecBuf[6];
	UINT8 *cpSend = &gcUserBuf[0];
	UINT8 cRetCode = ERR_OK;
	UINT32 i;
	INT wLenOfTZ;
	if(CommandP2 == 0)
	{
		CommandP2 = 15;
	}

	LED_Y2(ON);
	cRetCode = FpiGetImage(gcUserBuf, CommandP2);
	LED_Y2(OFF);

	if(cRetCode == ERR_OK)
	{
		if(gcSensorType == SENSOR_FPC1011)

		{
			# if FINGER_LIB == 2
			cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363, gcUserBuf, gcTzBuff, &wLenOfTZ);
#else
			cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363, gcUserBuf, gcTzBuff, &wLenOfTZ);
#endif
		}
		else if(gcSensorType == SENSOR_FPC1020)
		{
#if FINGER_LIB == 2
			cRetCode = GFP_ExtractAny(FPC1020_WIDTH, FPC1020_HEIGHT, 508, gcUserBuf, gcTzBuff, &wLenOfTZ);
#else
			cRetCode = GFP_Extract(FPC1020_WIDTH, FPC1020_HEIGHT, 508, gcUserBuf, gcTzBuff, &wLenOfTZ);
#endif
		}
	}

	// 发送包头
	cpSend = &gcUserBuf[0];
	cpSend[0] = 0x02;
	if(cRetCode == ERR_OK)
	{
		cpSend[1] = (LENOFMBINFO + 2) / 256;
		cpSend[2] = (LENOFMBINFO + 2) % 256;
		cpSend[3] = ERR_OK;

		cpSend[5] = wLenOfTZ / 256;
		cpSend[6] = wLenOfTZ % 256;
		for(i = 0; i < wLenOfTZ; i++)                       // 有效数据
		{
			cpSend[7 + i] = gcTzBuff[i];
		}
		for(i = wLenOfTZ; i < LENOFMBINFO - 2; i++)         // 无效数据
		{
			cpSend[7 + i] = 0x00;
		}
	}
	else
	{
		cpSend[1] = (2) / 256;
		cpSend[2] = (2) % 256;
		cpSend[3] = ERR_SAMPLE;
	}
	*ilen = CmdGenAck(cpSend);
	return ERR_OK;
}
int SaveMBLib(uint8 id,uint8 *cpmb);
//=============================================================
// 0x10 :采模板
//2013-7-2 zhangys 模板长度由原来256-->384
//=============================================================
UINT8 UsbCmdEnroll(UINT8 *cpRecBuf, INT *ilen)
{
	UINT8 CommandP2 = cpRecBuf[4];
	UINT8 CommandP3 = cpRecBuf[5];
	UINT8 CommandP4 = cpRecBuf[6];
	INT cRetCode = 0, step = 0;
	INT wLenOfMB;
	UINT16 i;
	UINT8 *cpSendBuf = &gcUserBuf[0];
	UINT8 *cpImageBuf = &gcUserBuf[0]; //图像数据

	memset(gcTz1Buff, 0, 512 * 3);
	memset(gcMBBuff, 0, sizeof(gcMBBuff) );
	if(CommandP2 == 0)
	{
		CommandP2 = 15;
	}
	while(CommandP2--)
	{
		LED_Y2(ON);
		cRetCode = FpiGetImage(cpImageBuf, 1);
		LED_Y2(OFF);

		if(cRetCode == ERR_OK)
		{
			//手指按下
			if(gcSensorType == SENSOR_FPC1011)
			{
#if FINGER_LIB == 2
				cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363, cpImageBuf, &gcTz1Buff[step][0], &wLenOfMB);
#else
				cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363, cpImageBuf, &gcTz1Buff[step][0], &wLenOfMB);
#endif
			}

			else if(gcSensorType == SENSOR_FPC1020)
			{
#if FINGER_LIB == 2
				cRetCode = GFP_ExtractAny(FPC1020_WIDTH, FPC1020_HEIGHT, 508, cpImageBuf, &gcTz1Buff[step][0], &wLenOfMB);
#else
				cRetCode = GFP_Extract(FPC1020_WIDTH, FPC1020_HEIGHT, 508, cpImageBuf, &gcTz1Buff[step][0], &wLenOfMB);
#endif
			}
			
			if(cRetCode == ERR_OK)
			{
				step++;
			}
			if(step >= 3)
			{
				break;
			}
		}
	}

	if(cRetCode == ERR_OK && step >= 3)
	{
#if FINGER_LIB == 2
		cRetCode = GFP_EnrollX(&gcTz1Buff[0][0], &gcTz1Buff[1][0], &gcTz1Buff[2][0], gcMBBuff, &wLenOfMB);
#else
		cRetCode = GFP_EnrollX(&gcTz1Buff[0][0], &gcTz1Buff[1][0], &gcTz1Buff[2][0], gcMBBuff, &wLenOfMB);
#endif
	}
	else
	{
		cRetCode = ERR_SAMPLE;
	}
	if(cRetCode==ERR_OK)
	{
		if(CommandP3>0)
		{
			cRetCode = SaveMBLib(CommandP3,gcMBBuff);
		}
	}
	cpSendBuf[0] = 0x02;
	if(cRetCode == ERR_OK && step >= 3)
	{
		cpSendBuf[1] = (LENOFMBINFO + 2) / 256;
		cpSendBuf[2] = (LENOFMBINFO + 2) % 256;
		cpSendBuf[3] = ERR_OK;

		cpSendBuf[5] = wLenOfMB / 256;
		cpSendBuf[6] = wLenOfMB % 256;
		for(i = 0; i < wLenOfMB; i++)                       // 有效数据
		{
			cpSendBuf[7 + i] = gcMBBuff[i];
		}
		for(i = wLenOfMB; i < LENOFMBINFO - 2; i++)         // 无效数据
		{
			cpSendBuf[7 + i] = 0x00;
		}
	}
	else
	{
		cpSendBuf[1] = (2) / 256;
		cpSendBuf[2] = (2) % 256;
		cpSendBuf[3] = ERR_SAMPLE;
	}
	*ilen = CmdGenAck(cpSendBuf);
	return (ERR_OK);
}

//=============================================================
// 0x50 - 程序升级命令（Update）
//=============================================================
UINT8 UsbCmdUpdate(UINT8 *cpRecBuf, INT *ilen)
{
	INT cRetCode = ERR_OK;
	UINT8 cCmdP2, cCmdP3;
	UINT32 wIndex;
	UINT32 wPacketLen;
	UINT32 count, i;
	UINT8 cCRC1, cCRC2;
	UINT8 *cpSendBuf = &gcUserBuf[0];
	UINT32 *pImageList;
	static int flag = 0;
	INT32 startBlock, endBlock, fileLen, executeAddr;

	cCmdP2 = cpRecBuf[4];   //升级步骤
	cCmdP3 = cpRecBuf[5];   //升级文件的类型

	switch(cCmdP2)
	{
		case 0x01:
		{
			gcpProgramBuf = &gcUserBuf[0] + 256 * 10 + 100;
			gdwProgramLen = cpRecBuf[7] * 0x1000000 + cpRecBuf[8] * 0x10000 + cpRecBuf[9] * 0x100 + cpRecBuf[10] * 0x1;
			cpSendBuf[0] = 0x02;
			cpSendBuf[1] = 0x00;
			cpSendBuf[2] = 0x02;
			cpSendBuf[4] = 0x00;
			cpSendBuf[5] = 0x00;                // CRC
			cpSendBuf[6] = 0x03;
			gdwProgramRecLen = 0;
			if(gdwProgramLen < (8 * 64 * 1024 + 1) )
			{
				cpSendBuf[3] = ERR_OK;
			}
			else
			{
				cpSendBuf[3] = ERR_NOTOK;
			}
			CmdGenAck(cpSendBuf);
			*ilen = 7;
		}
		break;

		case 0x02:
		{
			wIndex = cpRecBuf[7] * 256 + cpRecBuf[8];
			//单包数据长度
			wPacketLen = cpRecBuf[1] * 256 + cpRecBuf[2];
			wPacketLen -= 6;
			//拷贝数据
			memcpy(&gcpProgramBuf[gdwProgramRecLen], &cpRecBuf[9], wPacketLen);
			//文件长度
			gdwProgramRecLen += wPacketLen;
			cpSendBuf[0] = 0x02;
			cpSendBuf[1] = 0x00;
			cpSendBuf[2] = 0x02;
			cpSendBuf[3] = ERR_OK;
			cpSendBuf[4] = 0x00;
			cpSendBuf[5] = 0x00;                // CRC
			cpSendBuf[6] = 0x03;
			CmdGenAck(cpSendBuf);
			*ilen = 7;

			if(wIndex % 64 == 0)
			{
				if(wIndex % 128 == 0)
				{
					flag = 0;
				}
				else
				{
					flag = 1;
				}
			}
			else
			{
				flag = 0xff;
			}

			if(flag == 0)
			{
				LED_Y2(ON);
			}
			else if(flag == 1)
			{
				LED_Y2(OFF);
			}
		}
		break;

		case 0x03:
		{
			cCRC1 = cpRecBuf[7];

			if(gdwProgramRecLen != gdwProgramLen)
			{
				cRetCode = ERR_UPDATEDATEERROR;
			}

			if(cRetCode == ERR_OK)
			{
				cCRC2 = 0;
				for(i = 0; i < gdwProgramLen; i++)
				{
					cCRC2 += gcpProgramBuf[i];
				}

				if(cCRC1 != cCRC2)
				{
					cRetCode = ERR_CRC;
				}

				if(cRetCode == ERR_OK)
				{
					cRetCode = spiFlashBlockErase(APP_OFFSET_ADDRESS, 8);
					if(cRetCode == ERR_OK)
					{
						cRetCode = DFlashRWBlock(APP_OFFSET_ADDRESS, gcpProgramBuf, gdwProgramLen, WRITEMODE);
						if(cRetCode == ERR_OK)
						{
							cRetCode = DFlashRWBlock(SYS_OFFSET_ADDRESS, gcpProgramBuf, SYS_LENGTH, READMODE);
							pImageList = (unsigned int *)gcpProgramBuf;
							if( ( (*(pImageList + 0) ) == 0xAA554257) && ( (*(pImageList + 3) ) == 0x63594257) )
							{
								count = *(pImageList + 1);
								//load execution file
								pImageList = pImageList + 4;
								for(i = 0; i < count; i++)
								{
									if( ( (*(pImageList) >> 16) & 0xffff) == 1) // execute
									{
										startBlock = *(pImageList + 1) & 0xffff;
										endBlock = (*(pImageList + 1) & 0xffff0000) >> 16;
										executeAddr = *(pImageList + 2);
										fileLen = *(pImageList + 3);

										if( (startBlock > 0) && (endBlock < 9) && (executeAddr == 0) )
										{
											fileLen = gdwProgramLen;
											endBlock = startBlock + gdwProgramLen / (64 * 1204);
											executeAddr = 0x0;
											*(pImageList + 1) = (INT32)startBlock + (INT32)(endBlock << 16);
											*(pImageList + 2) = executeAddr;
											*(pImageList + 3) = fileLen;
											cRetCode = DFlashRWBlock(SYS_OFFSET_ADDRESS, gcpProgramBuf, SYS_LENGTH, WRITEMODE);
										}
										else
										{
											cRetCode = ERR_NOTOK;
										}
										break;
									}
									//pointer to next image
									pImageList = pImageList + 12;
								}
							}
						}
					}
				}
			}
			gdwProgramRecLen = gdwProgramLen = 0;
			gcpProgramBuf = NULL;
			cpSendBuf[0] = 0x02;
			cpSendBuf[1] = 0x00;
			cpSendBuf[2] = 0x02;
			cpSendBuf[3] = cRetCode;
			cpSendBuf[4] = 0x00;
			cpSendBuf[5] = 0x00;                // CRC
			cpSendBuf[6] = 0x03;
			CmdGenAck(cpSendBuf);
			*ilen = 7;
			gisupdateApp = 1;
		}
		break;
	}
	return (ERR_OK);
}

//=============================================================
// 0x20 - 清空指纹库（ClearMB）
// 清空指纹模板库
//=============================================================
UINT8 UsbCmdClearMBLib(UINT8 *cpRecBuf, INT *ilen)
{
	BYTE cpMBIndex[LENOFMBINDEX];
	BYTE cRetCode;
	WORD i;
	UINT8 *gcpSendBuf = &gcUserBuf[0];
	for(i = 0; i < LENOFMBINDEX; i++)
	{
		cpMBIndex[i] = 0x00;
	}

	LED_Y2(ON);

	// 判断是非验证口令成功
	cRetCode = FlashReadWriteMBIndex(cpMBIndex, FLASHWRITE);
	if(cRetCode == ERR_OK)
	{
		giMBCounter = 0;
		for(i = 0; i < LENOFMBINDEX; i++)
		{
			gcpMBIndex[i] = 0x00;
		}
		for(i = 0; i < LENOFMBLIB; i++)
		{
			spMBLib[i].cFlag = 0x00;
		}
	}
	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = 0x00;
	gcpSendBuf[2] = 0x02;
	gcpSendBuf[3] = cRetCode;
	gcpSendBuf[4] = 0x00;
	gcpSendBuf[5] = 0x00;                   // CRC
	gcpSendBuf[6] = 0x03;
	CmdGenAck(gcpSendBuf);

	LED_Y2(OFF);
	*ilen = 7;
	return (ERR_OK);
}

//=============================================================
// 0x21 - 向指纹库中插入一枚指纹模板（InsertMB）
//=============================================================
UINT8 UsbCmdInsertToMBLib(UINT8 *cpRecBuf, INT *ilen)
{
	WORD wLenOfMB;
	WORD wLenOfUserInfo;
	WORD wMBFlag;
	BYTE cRetCode = ERR_OK;
	INT i;
	UINT8 *gcpSendBuf = &gcUserBuf[0];
	UINT8 *gcpCreatIndexBuf = &gcUserBuf[150 * 1024];
	UINT8 *gcpIndexBuf = &gcUserBuf[90 * 1024];
	int aa;
	memset(gcpIndexBuf, 0, 10 * 1024);

	wMBFlag = cpRecBuf[7] * 256 + cpRecBuf[8];
	wMBFlag = 0;
	if(giMBCounter >= LENOFMBLIB)
	{
		cRetCode = ERR_INDEXFULL;
	}
	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = 0x00;
	gcpSendBuf[2] = 0x02;
	gcpSendBuf[3] = cRetCode;
	gcpSendBuf[4] = 0x00;
	gcpSendBuf[5] = 0x00;                   // CRC
	gcpSendBuf[6] = 0x03;
	CmdGenAck(gcpSendBuf);
	*ilen = 7;
	return (ERR_OK);
}

//=============================================================
// 0x22 - 将下载的指纹模板写入到指纹模板库中
//=============================================================
UINT8 UsbCmdWriteMBLib(UINT8 *cpRecBuf, INT *ilen)
{
	BYTE cRetCode;
	UINT8 *gcpSendBuf = &gcUserBuf[0];

	LED_Y2(ON);
	cRetCode = WriteMBLibToFlash();

	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = 0x00;
	gcpSendBuf[2] = 0x02;
	gcpSendBuf[3] = cRetCode;
	gcpSendBuf[4] = 0x00;
	gcpSendBuf[5] = 0x00;                   // CRC
	gcpSendBuf[6] = 0x03;
	CmdGenAck(gcpSendBuf);

	*ilen = 7;
	LED_Y2(OFF);
	return (ERR_OK);
}

//=============================================================
// 0x22 - 将下载的指纹模板写入到指纹模板库中
//=============================================================
UINT8 UsbCmdGetMBLibCounter(UINT8 *cpRecBuf, INT *ilen)
{
	BYTE cRetCode = ERR_OK;
	UINT8 *gcpSendBuf = &gcUserBuf[0];

	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = 0x00;
	gcpSendBuf[2] = 0x04;
	gcpSendBuf[3] = cRetCode;
	gcpSendBuf[4] = 0x00;
	gcpSendBuf[5] = giMBCounter / 256;
	gcpSendBuf[6] = giMBCounter % 256;
	gcpSendBuf[7] = 0x00;                   // CRC
	gcpSendBuf[8] = 0x03;
	CmdGenAck(gcpSendBuf);

	*ilen = 9;
	return (ERR_OK);
}

//=============================================================
// 0x31 - 指纹搜索命令（Search）
//=============================================================

UINT8 UsbCmdSearch(UINT8 *cpRecBuf, INT *ilen)
{
	BYTE cpFeature[LENOFTZ];                                        // 指纹数组
	BYTE cRetCode = ERR_NOTOK;
	WORD wStartIndex, wEndIndex;
	WORD i, iLength;
	INT wLenOfTZ;                                                   // 指纹长度
	BYTE cSearchFlag;
	WORD wIndex;
	UINT8 *cpImageBuf = (BYTE*)malloc(TCS1_WIDTH * TCS1_HEIGHT);    //图像
	UINT8 *gcpSendBuf = &gcUserBuf[0];

	wEndIndex = giMBCounter;
	cSearchFlag = 0;
	cRetCode = FpiGetImage(cpImageBuf, 15);                         // 取指纹图像
	memset(cpFeature, 0, sizeof(cpFeature) );
	if(cRetCode == ERR_OK)
	{
#if FINGER_LIB == 2
		cRetCode = GFP_ExtractAny(FPC1011_WIDTH, FPC1011_HEIGHT, 363, cpImageBuf, cpFeature, &wLenOfTZ);
#else
		cRetCode = GFP_Extract(FPC1011_WIDTH, FPC1011_HEIGHT, 363, cpImageBuf, cpFeature, &wLenOfTZ);
#endif
	}
	free(cpImageBuf);

	LED_Y2(ON);

	if(cRetCode == ERR_OK)
	{
		////////////////////////////////////////////////////////////////////////////////////
		//注意该函数的实现功能需要调整，由于没有提供搜索算法库
		cRetCode = FpiSearch(cpFeature, cSearchFlag, wEndIndex, &wIndex);
		////////////////////////////////////////////////////////////////////////////////////
	}
	// 组包

	iLength = 2 + LENOFUSERINFO + 2;
	gcpSendBuf[0] = 0x02;
	gcpSendBuf[1] = iLength / 256;
	gcpSendBuf[2] = iLength % 256;
	gcpSendBuf[3] = cRetCode;
	gcpSendBuf[4] = wIndex;

	if(cRetCode == ERR_OK)
	{
	}
	else
	{
		gcpSendBuf[5] = 0x00;
		gcpSendBuf[6] = 0x00;
		for(i = 0; i < LENOFUSERINFO; i++)
		{
			gcpSendBuf[7 + i] = 0x00;
		}
	}

	gcpSendBuf[7 + LENOFUSERINFO] = 0x00;                   // CRC
	gcpSendBuf[8 + LENOFUSERINFO] = 0x03;
	CmdGenAck(gcpSendBuf);

	LED_Y2(OFF);
	*ilen = 9 + LENOFUSERINFO;
	return (ERR_OK);
}
//////////////////////////////////////////////////////////////////////////
//验证对比
//zhangys
/////////////////////////////////////////////////////////////////////////
UINT8 UsbCmdVerifyFinger(UINT8 *cpRecBuf, INT *ilen)             //验证对比
{
	INT iPos;
	INT cRetCode = ERR_OK;
	int iScore = 0;
	INT clenMB = 0, clenTZ = 0;

	// 取模板长度
	iPos = 7;
	clenMB = cpRecBuf[iPos] * 256 + cpRecBuf[iPos + 1];

	//
	iPos += 2;

	// 取模板数据
	memcpy(&gcTz1Buff[0][0], &cpRecBuf[iPos], clenMB);

	iPos += clenMB;

	// 取特征长度
	clenTZ = cpRecBuf[iPos] * 256 + cpRecBuf[iPos + 1];

	//
	iPos += 2;

	// 取特征数据
	memcpy(&gcTz1Buff[1][0], &cpRecBuf[iPos], clenTZ);

	if(cRetCode == 0)
	{
#if FINGER_LIB == 2
		cRetCode = GFP_Verify(&gcTz1Buff[0][0], &gcTz1Buff[1][0], 3);
		if(cRetCode >= 0)
		{
			//回传分数
			gcUserBuf[5] = cRetCode / 256;
			gcUserBuf[6] = cRetCode % 256;
			cRetCode = ERR_OK;
		}
		else
		{
			gcUserBuf[5] = 0;
			gcUserBuf[6] = 0;
			cRetCode = ERR_MATCH;
		}
#else
		cRetCode = GFP_Verify(&gcTz1Buff[0][0], &gcTz1Buff[1][0], 3, &iScore);
		if(cRetCode == 0)
		{
			//回传分数
			if(iScore > 0)
			{
				gcUserBuf[5] = iScore / 256;
				gcUserBuf[6] = iScore % 256;
				cRetCode = ERR_OK;
			}
			else
			{
				gcUserBuf[5] = 0;
				gcUserBuf[6] = 0;
				cRetCode = ERR_MATCH;
			}
		}
		else
		{
			gcUserBuf[5] = 0;
			gcUserBuf[6] = 0;
			cRetCode = ERR_MATCH;
		}
#endif
	}
	gcUserBuf[0] = 0x02;
	gcUserBuf[1] = 0x00;
	gcUserBuf[2] = 0x02 + 2;
	gcUserBuf[3] = cRetCode;
	gcUserBuf[4] = 0x00;
	gcUserBuf[7] = 0x00;                    // CRC
	gcUserBuf[8] = 0x03;
	CmdGenAck(gcUserBuf);
	*ilen = 9;
	return (cRetCode);
}

/***********************************************************
* Function:
* Description:
* Input:
* Output:
* Return:
* Others:         zhanngys
***********************************************************/
UINT8 UsbCmdGetSensorType(UINT8 *cpRecBuf, INT *ilen)
{
	INT cRetCode = ERR_OK;
	gcUserBuf[0] = 0x02;
	gcUserBuf[1] = 0x00;
	gcUserBuf[2] = 0x02 + 1;
	gcUserBuf[3] = cRetCode;
	gcUserBuf[4] = 0x00;
	gcUserBuf[5] = gcSensorType;
	CmdGenAck(gcUserBuf);
	*ilen = 9;
	return (cRetCode);
}

/***********************************************************
* Function:
* Description:
* Input:
* Output:
* Return:
* Others:         zhanngys
***********************************************************/
void UsbProcessFailed(INT32 *ilen)
{
	gcUserBuf[0] = 0x02;
	gcUserBuf[1] = 0x00;
	gcUserBuf[2] = 0x02;
	gcUserBuf[3] = ERR_NOTOK;
	gcUserBuf[4] = gStuHidTrans.iPacket;
	gcUserBuf[5] = 0x00;                    // CRC
	gcUserBuf[6] = 0x03;
	*ilen = 7;
	CmdGenAck(gcUserBuf);
}

/************************************** The End Of File **************************************/

