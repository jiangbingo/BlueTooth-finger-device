#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"


extern void wellcom_Rcv_Data(void);		//ά�����ݽ���
extern void wellcom_Send_Data(void);	//ά�����ݷ���
extern UINT8 UsbCmdProcess(UINT8 *cpUSBCmdBuf,INT32 *ilen);
extern UINT8 wellcom_check_data(void);
extern void sysClockDivSwitchStart(UINT32 u32SysDiv);
