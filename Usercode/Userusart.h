#ifndef __wellcom_uart_H 
#define __wellcom_uart_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"



typedef struct 
{
	//tx
	UINT8  tx_flg; 		//发送标志              
	UINT32 tx_cnt; 		//发送长度 
	UINT8	*tx_buf;
	UINT32 st_cnt;            
	//rx
	UINT8  rx_flg;			//接收标志                      
	UINT32 rx_cnt;			//接收长度    
	UINT8  rx_tim;			//接收超时中断
	UINT32  time_out;		//接收超时中断
	UINT8	*rx_buf;
	
} StruUART;


extern BOOL volatile _sys_bIsUseUARTInt ;
extern void UartPort_Init(UINT8 Port,UINT32 Baudrate);
extern void Uart0StructVarInit(void);
extern void Uart1StructVarInit(void);
extern void UartSendString(int port,UINT8 *txbuf,UINT32 buflen);
extern void Uart_handler(void);
extern void Uart0_handler(void);
extern void Uart1_handler(void);
extern VOID UartSendByte(int port,UINT8 ucCh);
 
#endif