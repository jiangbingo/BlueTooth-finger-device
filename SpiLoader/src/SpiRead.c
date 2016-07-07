#include "w55fa93_reg.h"
#include "wblib.h"


#define	SPICMD_DUMMY				0x00
#define	SPICMD_READ_DATA			0x03

extern void FeedWatchdog(void);

int usiActive()
{
	outp32(REG_SPI0_CNTRL, inp32(REG_SPI0_CNTRL)|0x01);
	while(inp32(REG_SPI0_CNTRL) & 0x01);

	return 0;
}

int usiTxLen(int count, int bitLen)//ok
{
	UINT32 reg;

	reg = inp32(REG_SPI0_CNTRL);

	if ((count < 0) || (count > 3))
		return 1;

	if ((bitLen <= 0) || (bitLen > 32))
		return 1;

	if (bitLen == 32)
		reg = reg & 0xffffff07;
	else
		reg = (reg & 0xffffff07) | (bitLen << 3);
		
	reg = (reg & 0xfffffcff) | (count << 8);

	outp32(REG_SPI0_CNTRL, reg);

	return 0;
}

int usiCheckBusy()
{
	// check status
	outp32(REG_SPI0_SSR, inp32(REG_SPI0_SSR) | 1);	// CS0

	// status command
	outp32(REG_SPI0_TX0, 0x05);
	usiTxLen(0, 8);
	usiActive();

	// get status
	while(1)
	{
		outp32(REG_SPI0_TX0, 0xff);
		usiTxLen(0, 8);
		usiActive();
		if (((inp32(REG_SPI0_RX0) & 0xff) & 0x01) != 0x01)
			break;
	}


	outp32(REG_SPI0_SSR, inp32(REG_SPI0_SSR) & 0xfe);	// CS0


	return 0;
}

int SPIReadFast(BOOL bEDMAread, UINT32 addr, UINT32 len, UINT32 *buf)
{
	int volatile i;
	UINT32 u32Tmp,u32Count;
	
	sysprintf("Load file length 0x%x, execute address 0x%x\r\n", len, (UINT32)buf);

	outp32(REG_SPI0_CNTRL, inp32(REG_SPI0_CNTRL) & ~BYTE_ENDIN);		// disabe BYTE endian
	
	outp32(REG_SPI0_SSR, inp32(REG_SPI0_SSR) | 0x01);	// CS0

	buf = (UINT32 *) ((UINT32)buf | (UINT32)0x80000000);

	// read command
	outp32(REG_SPI0_TX0, 0x0B);
	usiTxLen(0, 8);
	usiActive();

	// address
	outp32(REG_SPI0_TX0, addr);
	usiTxLen(0, 24);
	usiActive();

	// dummy byte
	outp32(REG_SPI0_TX0, 0xff);
	usiTxLen(0, 8);
	usiActive();

	{
		u32Count = len/4;
		if(len % 4)
			u32Count++;
			
		// data
		for (i=0; i<u32Count; i++)
		{
			outp32(REG_SPI0_TX0, 0xffffffff);
			usiTxLen(0, 32);
			usiActive();
			u32Tmp = inp32(REG_SPI0_RX0);
			*buf++ = ((u32Tmp & 0xFF) << 24) | ((u32Tmp & 0xFF00) << 8) | ((u32Tmp & 0xFF0000) >> 8)| ((u32Tmp & 0xFF000000) >> 24);
			FeedWatchdog();
		}
	}
	outp32(REG_SPI0_SSR, inp32(REG_SPI0_SSR) & 0xfe);	// CS0
	
	return 0;
}

VOID SPI_OpenSPI(VOID)
{
	// turn on SPI clk
	outp32(REG_APBCLK, inp32(REG_APBCLK) | SPIMS0_CKE);
	//Reset SPI controller first
	outp32(REG_APBIPRST, inp32(REG_APBIPRST) | SPI0RST);
	outp32(REG_APBIPRST, inp32(REG_APBIPRST) & ~SPI0RST);
	
	outp32(REG_GPDFUN, inp32(REG_GPDFUN) | (MF_GPD15|MF_GPD14|MF_GPD13|MF_GPD12));
	outp32(REG_SPI0_SSR, 0x00);			// CS active low
	outp32(REG_SPI0_CNTRL, 0x04);		// Tx: falling edge, Rx: rising edge
	
	outp32(REG_SPI0_DIVIDER, 0x00);
}

VOID SPI_CloseSPI(VOID)
{
	outp32(REG_APBCLK, inp32(REG_APBCLK) & ~SPIMS0_CKE);
	/* Disable Pin function */  	
	outp32(REG_GPDFUN, inp32(REG_GPDFUN) & ~(MF_GPD15|MF_GPD14|MF_GPD13|MF_GPD12));	
}






























