#ifndef _Externs_H
#define _Externs_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"
#include "Userusart.h"
#include "Defines.h"

extern int gMBId;
extern int gudBaudrate0;
extern int gudBaudrate1;
extern UINT32 u32UartPort;
extern UINT32 u32ExtFreq;
extern StruUART gStruUart0;
extern StruUART gStruUart1;
extern INT giMBCounter;	
extern UINT32 guisReadpower;
extern UINT32 gudSleeptime;
extern UINT8 *gcUserBuf;
extern UINT8 *Sprintfs;
extern UINT32 gudTimeOut;
extern UINT8 gDefineSleeptime;
extern UINT8 NEWVERSION_1011[];
extern UINT8 NEWVERSION_1020[];
extern UINT8 LIBVERSION[];
extern UINT8 gucPowerLevel;
extern UINT8 gucisCharge; 
extern UINT8 gucisChargeDone; 
extern UINT8 gcSensorType;	
extern UINT8 gcImageFlag;						//����������
extern UINT8 gcTzBuff[512];
extern UINT8 gcTz1Buff[3][512];
extern UINT8 gcMBBuff[512];
extern UINT8 cpDeviceID[LENOFDEVICEID*2];
extern UINT8 gcpMBIndex[LENOFMBINDEX];					// ģ������������DAT���
extern BYTE const gcCRC8DATA[];
//����ʹ�õı���
extern UINT8 *gcpProgramBuf ;
extern UINT32 gdwProgramLen;
extern UINT32 gdwProgramRecLen;
//������
extern struct StruPARA gsParaTable;				
extern struct StruMBLIB spMBLib[LENOFMBLIB];	

//����
extern UINT8 CrcOfMBLibItem(struOneUserInfo *spMBLibItem);
//��ʱ����
extern void Delayms(UINT16 ms);

extern int gisEnablewdt ;
extern int gisupdateApp;
#endif
