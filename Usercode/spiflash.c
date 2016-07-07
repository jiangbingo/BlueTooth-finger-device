/*-----------------------------------------------------------------------------------*/
/* Nuvoton Technology Corporation confidential                                       */
/*                                                                                   */
/* Copyright (c) 2008 by Nuvoton Technology Corporation                              */
/* All rights reserved                                                               */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/
/****************************************************************************
 *
 * FILENAME
 *     spiflash.c
 *
 * VERSION
 *     1.0
 *
 * DESCRIPTION
 *     This file contains SPI flash library APIs.
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     10/12/07      Create Ver 1.0
 *     06/12/09      Add SPI flash
 *
 * REMARK
 *     None
 **************************************************************************/
/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"

#include "w55fa93_reg.h"
#include "w55fa93_spi.h"
#include "w55fa93_gpio.h"
#include "Defines.h"

#define FLASH_PP		0x02  /* Write to Memory instruction */
#define FLASH_WRSR		0x01  /* Write Status Register instruction */
#define FLASH_WREN		0x06  /* Write enable instruction */
#define FLASH_WRDI		0x04  /* Write disable instruction */
#define FLASH_FREAD		0x0B  /* Read at Higher Speed */
#define FLASH_READ		0x03  /* Read from Memory instruction */
#define FLASH_RDSR		0x05  /* Read Status Register instruction  */
#define FLASH_REMS		0x90  /* Read Electronic Manufacturer ID & Device ID  */
#define FLASH_RDID		0x9F  /* Read identification */
#define FLASH_SE		0x20  /* Sector Erase instruction */
#define FLASH_BE		0xD8  /* Block Erase instruction */
#define FLASH_CE		0xC7  /* Chip Erase instruction */

#define FLASH_PageSize		0x100
#define FLASH_SectorSize	0x1000
#define PAGESIZE			0x100
#define SECTORSIZE			0x1000


/************************************************************************************/
/*	SPI Flash 使用旺宏的MX25L8006E，容量1MB，16个Block(64K)，每个Block包含16个Sector*/
/*	每个Sector是4K																	*/
/*	0x000000-0x00FFFF	Block0			SpiLoader区域，APP程序不能操作该区域，64K	*/
/*	0x010000-0x08FFFF	Block1-Block8	App应用程序存储区域，512K					*/
/*	0x090000-0x0FFFFF	Block9-Block15	设备ID，模板等存储区域，后面为保留区域		*/
/*																					*/
/*																					*/
/*																					*/
/*																					*/
/************************************************************************************/

int usiCheckBusy()
{
	// check status
	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	// status command
	outpw(REG_SPI0_TX0, 0x05);
	spiTxLen(0, 0, 8);
	spiActive(0);

	// get status
	while(1)
	{
		outpw(REG_SPI0_TX0, 0xff);
		spiTxLen(0, 0, 8);
		spiActive(0);
		if (((inpw(REG_SPI0_RX0) & 0xff) & 0x01) != 0x01)
			break;
	}

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return Successful;
}

INT16 usiReadID()
{
	UINT16 volatile id;

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	// command 8 bit
	outpw(REG_SPI0_TX0, FLASH_REMS);
	spiTxLen(0, 0, 8);
	spiActive(0);

	// address 24 bit
	outpw(REG_SPI0_TX0, 0x000000);
	spiTxLen(0, 0, 24);
	spiActive(0);

	// data 16 bit
	outpw(REG_SPI0_TX0, 0xffff);
	spiTxLen(0, 0, 16);
	spiActive(0);
	id = inpw(REG_SPI0_RX0) & 0xffff;

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return id;
}


int usiWriteEnable()
{
	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	outpw(REG_SPI0_TX0, FLASH_WREN);
	spiTxLen(0, 0, 8);
	spiActive(0);

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return Successful;
}

int usiWriteDisable()
{
	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	outpw(REG_SPI0_TX0, FLASH_WRDI);
	spiTxLen(0, 0, 8);
	spiActive(0);

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return Successful;
}

int usiStatusWrite(UINT8 data)
{
	usiWriteEnable();

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	// status command
	outpw(REG_SPI0_TX0, FLASH_WRSR);
	spiTxLen(0, 0, 8);
	spiActive(0);

	// write status
	outpw(REG_SPI0_TX0, data);
	spiTxLen(0, 0, 8);
	spiActive(0);

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return Successful;
}


/**************************************************/
INT spiFlashInit(void)
{
	static BOOL bIsSpiFlashOK = 0;
	int volatile loop;

	if (!bIsSpiFlashOK)
	{
		outpw(REG_APBCLK, inpw(REG_APBCLK) | SPIMS0_CKE);	// turn on SPI clk
		//Reset SPI controller first
		outpw(REG_APBIPRST, inpw(REG_APBIPRST) | SPI0RST);
		outpw(REG_APBIPRST, inpw(REG_APBIPRST) & ~SPI0RST);
		// delay for time
		// Delay
		for (loop=0; loop<500; loop++);
		//configure SPI0 interface, Base Address 0xB800C000

		/* apb clock is 48MHz, output clock is 10MHz */
		spiIoctl(0, SPI_SET_CLOCK, 48, 10000);

		//Startup SPI0 multi-function features, chip select using SS0
		outpw(REG_GPDFUN, inpw(REG_GPDFUN) | 0xFF000000);

		outpw(REG_SPI0_SSR, 0x00);		// CS active low
		outpw(REG_SPI0_CNTRL, 0x04);		// Tx: falling edge, Rx: rising edge

		if ((loop=usiReadID()) == -1)
		{
			//sysprintf("read id error !! [0x%x]\n", loop&0xffff);
			return -1;
		}

		//sysprintf("SPI flash id [0x%x]\n", loop&0xffff);
		usiStatusWrite(0x00);	// clear block protect
		// delay for time
		// Delay
		for (loop=0; loop<0x20000; loop++);

		bIsSpiFlashOK = 1;
	}
	return 0;
}

INT spiPortInit(void)
{
	BOOL bIsSpiFlashOK = 0;
	int volatile loop;

	if (!bIsSpiFlashOK)
	{
		outpw(REG_APBCLK, inpw(REG_APBCLK) | SPIMS0_CKE);	// turn on SPI clk
		//Reset SPI controller first
		outpw(REG_APBIPRST, inpw(REG_APBIPRST) | SPI0RST);
		outpw(REG_APBIPRST, inpw(REG_APBIPRST) & ~SPI0RST);
		// delay for time
		// Delay
		for (loop=0; loop<500; loop++);
		//configure SPI0 interface, Base Address 0xB800C000

		/* apb clock is 48MHz, output clock is 10MHz */
		spiIoctl(0, SPI_SET_CLOCK, 48, 4000);

		//Startup SPI0 multi-function features, chip select using SS0
		//SPI_DO:GPDFUN[31:30]2b11
		//SPI_DI:GPDFUN[29:28]2b11
		//SPI_CS0:GPDFUN[27:26]2b11 For HardWare CS0;GPD13:GPDFUN[27:26]2b00 For SoftWare CS0
		//SPI_CLK:GPDFUN[25:24]2b11
		outpw(REG_GPDFUN, inpw(REG_GPDFUN) | 0xFF000000);

		//SPI_CS1:GPDFUN[11:10]2b11 For HardWare CS1;GPD13:GPDFUN[11:10]2b00 For SoftWare CS1
		outpw(REG_GPAFUN, inpw(REG_GPAFUN) | 0x00000C00);

		outpw(REG_SPI0_SSR, 0x00);		// CS active low
		outpw(REG_SPI0_CNTRL, 0x04);		// Tx: falling edge, Rx: rising edge

		//if ((loop=usiReadID()) == -1)
		//{
		//	//sysprintf("read id error !! [0x%x]\r\n", loop&0xffff);
		//	return -1;
		//}

		//sysprintf("SPI flash id [0x%x]\r\n", loop&0xffff);
		usiStatusWrite(0x00);	// clear block protect
		// delay for time
		// Delay
		for (loop=0; loop<0x20000; loop++);

		bIsSpiFlashOK = 1;
	}
	return 0;
}

INT spiFlashSectorErase(UINT32 addr, UINT32 secCount)
{
	int volatile i;

	if (addr < (60*1024))//Loader区域保护，最后面一个Block可以擦写
		return -1;

	if ((addr % (4*1024)) != 0)
		return -1;

	for (i=0; i<secCount; i++)
	{
		usiWriteEnable();

		outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

		// erase command
		outpw(REG_SPI0_TX0, FLASH_SE);
		spiTxLen(0, 0, 8);
		spiActive(0);

		// address
		outpw(REG_SPI0_TX0, addr+i*4*1024);
		spiTxLen(0, 0, 24);
		spiActive(0);

		outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

		// check status
		usiCheckBusy();
	}

	return Successful;
}


INT spiFlashBlockErase(UINT32 addr, UINT32 secCount)
{
	int volatile i;

	if (addr < (64*1024))//Loader区域保护
		return -1;

	if ((addr % (64*1024)) != 0)
		return -1;

	for (i=0; i<secCount; i++)
	{
		usiWriteEnable();

		outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

		// erase command
		outpw(REG_SPI0_TX0, FLASH_BE);
		spiTxLen(0, 0, 8);
		spiActive(0);

		// address
		outpw(REG_SPI0_TX0, addr+i*64*1024);
		spiTxLen(0, 0, 24);
		spiActive(0);

		outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

		// check status
		usiCheckBusy();
	}

	return Successful;
}


INT spiFlashEraseAll(void)
{
	usiWriteEnable();

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	outpw(REG_SPI0_TX0, FLASH_CE);
	spiTxLen(0, 0, 8);
	spiActive(0);

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	// check status
	usiCheckBusy();

	return Successful;
}
INT spiFlashPageProgram(UINT8* pBuffer, UINT32 WriteAddr, UINT16 NumByteToWrite)
{
	usiWriteEnable();

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	// write command
	outpw(REG_SPI0_TX0, FLASH_PP);
	spiTxLen(0, 0, 8);
	spiActive(0);

	// address
	outpw(REG_SPI0_TX0, WriteAddr);
	spiTxLen(0, 0, 24);
	spiActive(0);

	// write data
	while (NumByteToWrite--)
	{
		outpw(REG_SPI0_TX0, *pBuffer++);
		spiTxLen(0, 0, 8);
		spiActive(0);
	}

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	// check status
	usiCheckBusy();

	return Successful;
}

INT spiFlashWrite(UINT32 addr, UINT32 len, UINT8 *buf)
{
	int volatile count=0, page, i;

	count = len / 256;
	if ((len % 256) != 0)
		count++;

	for (i=0; i<count; i++)
	{
		// check data len
		if (len >= 256)
		{
			page = 256;
			len = len - 256;
		}
		else
			page = len;

		usiWriteEnable();

		outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

		// write command
		outpw(REG_SPI0_TX0, FLASH_PP);
		spiTxLen(0, 0, 8);
		spiActive(0);

		// address
		outpw(REG_SPI0_TX0, addr+i*256);
		spiTxLen(0, 0, 24);
		spiActive(0);

		// write data
		while (page-- > 0)
		{
			outpw(REG_SPI0_TX0, *buf++);
			spiTxLen(0, 0, 8);
			spiActive(0);
		}

		outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

		// check status
		usiCheckBusy();
	}

	return Successful;
}


INT spiFlashRead(UINT32 addr, UINT32 len, UINT8 *buf)
{
	int volatile i;
	UINT8 *ptr;

	ptr = buf;
	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	// read command
	outpw(REG_SPI0_TX0, FLASH_READ);
	spiTxLen(0, 0, 8);
	spiActive(0);

	// address
	outpw(REG_SPI0_TX0, addr);
	spiTxLen(0, 0, 24);
	spiActive(0);

	// data
	for (i=0; i<len; i++)
	{
		outpw(REG_SPI0_TX0, 0xff);
		spiTxLen(0, 0, 8);
		spiActive(0);
		*buf++ = inpw(REG_SPI0_RX0) & 0xff;
	}

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return Successful;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
INT spiFlashBufferWrite(UINT8* pBuffer, UINT32 WriteAddr, UINT32 NumByteToWrite)
{
	UINT8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % FLASH_PageSize;
	count = FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / FLASH_PageSize;
	NumOfSingle = NumByteToWrite % FLASH_PageSize;

	if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			spiFlashPageProgram(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			while (NumOfPage--)
			{
				spiFlashPageProgram(pBuffer, WriteAddr, FLASH_PageSize);
				WriteAddr +=  FLASH_PageSize;
				pBuffer += FLASH_PageSize;
			}
			spiFlashPageProgram(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
			{
				temp = NumOfSingle - count;

				spiFlashPageProgram(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count;

				spiFlashPageProgram(pBuffer, WriteAddr, temp);
			}
			else
			{
				spiFlashPageProgram(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / FLASH_PageSize;
			NumOfSingle = NumByteToWrite % FLASH_PageSize;

			spiFlashPageProgram(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;

			while (NumOfPage--)
			{
				spiFlashPageProgram(pBuffer, WriteAddr, FLASH_PageSize);
				WriteAddr +=  FLASH_PageSize;
				pBuffer += FLASH_PageSize;
			}

			if (NumOfSingle != 0)
			{
				spiFlashPageProgram(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}

	return Successful;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
INT spiFlashBufferRead(UINT8* pBuffer, UINT32 ReadAddr, UINT32 NumByteToRead)
{
	int volatile i;
	UINT8 *ptr;

	ptr = pBuffer;
	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x01);	// CS0

	// read command
	outpw(REG_SPI0_TX0, FLASH_READ);
	spiTxLen(0, 0, 8);
	spiActive(0);

	// address
	outpw(REG_SPI0_TX0, ReadAddr);
	spiTxLen(0, 0, 24);
	spiActive(0);

	// data
	for (i=0; i<NumByteToRead; i++)
	{
		outpw(REG_SPI0_TX0, 0xff);
		spiTxLen(0, 0, 8);
		spiActive(0);
		*ptr++ = inpw(REG_SPI0_RX0) & 0xff;
	}

	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfe);	// CS0

	return Successful;
}

INT DFlashRWBlock(DWORD dwAddr,BYTE*cpBuffer,DWORD dwCount,BYTE cType )
{
	DWORD dwSectorBegin;
	DWORD dwPageBegin;
	DWORD dwPageOffset;
	DWORD dwSectorOffset;
	DWORD dwSectorCount = 0;
	DWORD dwPageCount = 0;

	DWORD i;
	BYTE *cpReadBuf;//*cpBufPos;
	DWORD j;
	INT cRetCode;


	if(cType==WRITEMODE)		//写命令
	{
		dwPageBegin = (WORD) (dwAddr/PAGESIZE);			// 页起始地址
		dwPageOffset = (DWORD)(dwAddr%PAGESIZE);		// 页偏移量
		dwSectorOffset = (DWORD)(dwAddr%SECTORSIZE);	// 扇区偏移量
		dwSectorBegin = (WORD) (dwAddr/SECTORSIZE);		// 扇区起始地址

		
		
		// 计算页数
		if (dwCount<=(PAGESIZE - dwPageOffset))
			dwPageCount = 1;
		else
		{
			if(((dwCount-(PAGESIZE- dwPageOffset))%PAGESIZE)==0)
				dwPageCount = (dwCount-(PAGESIZE - dwPageOffset))/PAGESIZE+1;
			else
				dwPageCount = (dwCount-(PAGESIZE - dwPageOffset))/PAGESIZE+2;
		}

		// 计算扇区数
		if (dwCount<=(SECTORSIZE - dwSectorOffset))
			dwSectorCount = 1;
		else
		{
			if(((dwCount-(SECTORSIZE- dwSectorOffset))%SECTORSIZE)==0)
				dwSectorCount = (dwCount-(SECTORSIZE - dwSectorOffset))/SECTORSIZE+1;
			else
				dwSectorCount = (dwCount-(SECTORSIZE - dwSectorOffset))/SECTORSIZE+2;
		}

		dwPageCount = (SECTORSIZE/PAGESIZE);	//一个扇区中的总页数
		// 分配一个扇区大小，用于保存擦除之前的数据
		cpReadBuf = (BYTE*)malloc(SECTORSIZE+2);
		if(cpReadBuf ==NULL)
		{
			return ERR_NOTOK;
		}
		if(dwSectorCount==1)			// only 1 sector
		{
			spiFlashBufferRead(cpReadBuf,dwSectorBegin*SECTORSIZE,SECTORSIZE);	//读出数据
			memcpy(cpReadBuf+dwSectorOffset,cpBuffer,dwCount);

			cRetCode = spiFlashSectorErase(dwSectorBegin*SECTORSIZE,1);					// 擦除扇区
			if(cRetCode != ERR_OK)
			{
				free(cpReadBuf);
				return cRetCode;
			}
			for(i=0; i<dwPageCount; i++)
			{
				cRetCode = spiFlashPageProgram(cpReadBuf+i*PAGESIZE,dwSectorBegin*SECTORSIZE + i*PAGESIZE,PAGESIZE);	//页写数据
				if(cRetCode != ERR_OK)
				{
					free(cpReadBuf);
					return cRetCode;
				}
			}
			free(cpReadBuf);
			return(cRetCode);
		}
		else 		// more than 1 sector
		{
			i=0;	// process the first sector
			spiFlashBufferRead(cpReadBuf,dwSectorBegin*SECTORSIZE,SECTORSIZE);	//读出数据
			memcpy(cpReadBuf+dwSectorOffset,cpBuffer,(SECTORSIZE-dwSectorOffset));
			cRetCode = spiFlashSectorErase(dwSectorBegin*SECTORSIZE,1);					// 擦除扇区
			if(cRetCode != ERR_OK)
			{
				free(cpReadBuf);
				return cRetCode;
			}
			for(i=0; i<dwPageCount; i++)										// 写第一个扇区
			{
				cRetCode = spiFlashPageProgram(cpReadBuf+i*PAGESIZE,dwSectorBegin*SECTORSIZE + i*PAGESIZE,PAGESIZE);	//页写数据
				if(cRetCode != ERR_OK)
				{
					free(cpReadBuf);
					return cRetCode;
				}
			}
			for(j=0; j<(dwSectorCount-2); j++)		// process midle sectors
			{
				cRetCode = spiFlashSectorErase((dwSectorBegin+j+1)*SECTORSIZE,1);	// 擦除扇区
				if(cRetCode != ERR_OK)
				{
					free(cpReadBuf);
					return cRetCode;
				}
				for(i=0; i<dwPageCount; i++)										// 写第一个扇区
				{
					cRetCode = spiFlashPageProgram(cpBuffer+(SECTORSIZE-dwSectorOffset)+j*SECTORSIZE+i*PAGESIZE,(dwSectorBegin+j+1)*SECTORSIZE+i*PAGESIZE,PAGESIZE);	//页写数据
					if(cRetCode != ERR_OK)
					{
						free(cpReadBuf);
						return cRetCode;
					}
				}
			}
			// process the last sector
			spiFlashBufferRead(cpReadBuf,(dwSectorBegin+j+1)*SECTORSIZE,SECTORSIZE);
			memcpy(cpReadBuf,cpBuffer+(SECTORSIZE-dwSectorOffset+SECTORSIZE*j),(dwCount - (SECTORSIZE-dwSectorOffset)-SECTORSIZE*j));
			cRetCode = spiFlashSectorErase((dwSectorBegin+j+1)*SECTORSIZE,1);	// 擦除扇区
			if(cRetCode != ERR_OK)
			{
				free(cpReadBuf);
				return cRetCode;
			}
			for(i=0; i<dwPageCount; i++)										// 写第一个扇区
			{
				cRetCode = spiFlashPageProgram(cpBuffer+(SECTORSIZE-dwSectorOffset)+j*SECTORSIZE+i*PAGESIZE,(dwSectorBegin+j+1)*SECTORSIZE+i*PAGESIZE,PAGESIZE);	//页写数据
				if(cRetCode != ERR_OK)
				{
					free(cpReadBuf);
					return cRetCode;
				}
			}
			free(cpReadBuf);
			return(cRetCode);
		}
	}
	else		//读命令
		cRetCode = spiFlashBufferRead(cpBuffer,dwAddr,dwCount);
	return(cRetCode);

}
