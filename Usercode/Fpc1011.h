#ifndef  fingerH
#define  fingerH

// FPC¼Ä´æÆ÷³£Á¿
#define FPC_RD_SENSOR		0x11
#define FPC_RD_SPIDATA		0x20
#define FPC_RD_SPISTAT		0x21
#define FPC_RD_REGS			0x50
#define FPC_WR_DRIVC		0x75
#define FPC_WR_ADCREF		0x76
#define FPC_WR_SENSEM		0x77
#define FPC_WR_FIFO_TH		0x7C
#define FPC_WR_XSENSE		0x7F
#define FPC_WR_YSENSE		0x81
#define FPC_WR_XSHIFT		0x82
#define FPC_WR_YSHIFT		0x83
#define FPC_WR_XREADS		0x84


#if (SensorType==1)
    #define FPC_DRIVC		0xff		// For 1A_2E Version Sensor
    #define FPC_ADCREF		0x01		// For 1A_2E Version Sensor
#endif
#if (SensorType==2)
    #define FPC_DRIVC		0x80		// For 2F_3A Version Sensor
    #define FPC_ADCREF		0x02		// For 2F_3A Version Sensor
#endif    
#define FPC_SENSEM		0x00		//
#define FPC_FIFO_TH		8		//
#define FPC_XSENSE		0xff		//
#define FPC_YSENSE		0x01		//
#define FPC_XSHIFT		18		//
#define FPC_YSHIFT		199		//
#define FPC_XREADS		1		//

BYTE FPC1011GetImage(BYTE *cpImageBuff);
void Fpc1011Power(BYTE OnOff);
BYTE Fpc1011Init(void);
#endif
