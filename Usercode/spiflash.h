
#ifndef __SPIFLASH_H__
#define __SPIFLASH_H__

extern INT spiFlashBufferWrite(UINT8* pBuffer, UINT32 WriteAddr, UINT32 NumByteToWrite);
extern INT spiFlashBufferRead(UINT8* pBuffer, UINT32 ReadAddr, UINT32 NumByteToRead);
extern INT DFlashRWBlock(DWORD dwAddr,BYTE*cpBuffer,DWORD dwCount,BYTE cType );
extern INT spiFlashBlockErase(UINT32 addr, UINT32 secCount);
extern INT spiFlashSectorErase(UINT32 addr, UINT32 secCount);
extern INT spiPortInit(void);
#endif