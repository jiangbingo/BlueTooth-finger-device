#ifndef basic_H
#define basic_H

void Delayms(UINT16 ms);
void LED_R2(UINT8 flag);
void BlinkR2(UINT8 cnt,UINT16 ms);
void BlinkLedByPowerLevel(void);
void LED_Y2(UINT8 flag);
void BlinkY2(UINT8 cnt,UINT16 ms);
void LED_G2(UINT8 flag);
void BlinkG2(UINT8 cnt,UINT16 ms);
void LED_B2(UINT8 flag);
void BlinkB2(UINT8 cnt, UINT16 ms);
void LED_RGY(UINT8 flag);
void BlinkRGY(UINT8 cnt,UINT16 ms);
void BlueToothInit(void);
void OpenBlueTooth(void);
void CloseBlueTooth(void);
void WakeupBlueTooth(void);
BYTE Crc8(WORD wLength,BYTE *cpCrcBuf);
BYTE PowerOnRecovery(void);
int power_read (void);
void ICUncompressPara(BYTE *cpBuf, struct StruPARA *spParaTable);
void ICCompressPara(struct StruPARA *spParaTable, BYTE *cpBuf);
BYTE FlashReadWritePara(struct StruPARA *spParaTable, BYTE cMode);
BYTE CrcOfMBLibItem(struOneUserInfo *spMBLibItem);
BYTE GetUserInfo(void);
BYTE FlashReadWriteMBIndex(BYTE *cpMBIndex, BYTE cMode);
BYTE WriteMBLibToFlash(void);
void Delayus(UINT16 us);

#endif
