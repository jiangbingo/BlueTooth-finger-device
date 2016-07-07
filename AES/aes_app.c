#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include"aes_app.h"

#define AES_LEN   16

enum  _STATUS_CODE{
    STATUS_SUCCESS ,                
    STATUS_ERROR  ,                        
};

static unsigned char g_AlgoKey[AES_LEN]={
    0x1B,'@',0x2C,'*',0x32,'&',
    0x0E,'!',0x5A,'%',0x1D,'(',
    0x61,')',0x38,0x0F
  };
 /**************************************************************************************
** Function name:	       ArrayCompare
** Descriptions:         compare two array
** input :	 arry1,arry2,len
** output    none
** Returned value:			0 or 1, sucess: 0, else 1
** Used global variables:	None
** Calling modules:	       None
***************************************************************************************/ 
static unsigned char ArrayCompare(unsigned char *arry1, 
                                  unsigned char *arry2, 
                                  int len)
{
    int i;
    for(i=0; i<len; i++)
    {
        if(arry1[i]!=arry2[i])
        {
            return STATUS_ERROR;
        }
    }
    return STATUS_SUCCESS;
}
/**************************************************************************************
** Function name:	       generateRandom
** Descriptions:         generate 16 bytes random data 
** input :	 randLen--length is 16
** output    randArry--random data address
** Returned value:			None
** Used global variables:	None
** Calling modules:	       None
***************************************************************************************/
static void generateRandom(unsigned char * randArry , int randLen)
{
     int i;
     static int num=0;
     if(randLen%16!=0)
     {
     		return;
     }
     //srand( (unsigned)time(0) );  执行该函数后程序死机
     num++;
     for( i = 0; i < randLen;i++ )            
     {
          randArry[i] = ((rand()%255)+(num&0xff))&0xff;
     }        

}

/**************************************************************************************
** Function name:	       GetAlgoKey
** Descriptions:         Original key Transaction,  the 16B key transfered from AP, 
                         used for AES enc/dec operation,stores in pDest.
** input :	 pSour--source data address
** output    pDest--dectination data address
** Returned value:			None
** Used global variables:	None
** Calling modules:	       None
***************************************************************************************/
static void GetAlgoKey(unsigned char  *pSour,unsigned char  *pDest)
{
    unsigned char i=0;
	
    unsigned char Arr[AES_LEN];
    Arr[ 0] = ~((*(pSour ) & 0x0f) << 4) | ((*(pSour + 15) & 0xf0) >> 4 );
    for (i = 0; i < AES_LEN - 1;i++) // 
      Arr[ 1 + i] = ~((*(pSour + i) & 0x5a) >> 4) & ((*(pSour + 1 + i) & 0x96) << 4 );

    for (i = 0; i < AES_LEN; i++)  // 
      Arr[i] ^= *(pSour + i);	   //XOR with original data

    memcpy( pDest, Arr, AES_LEN);  //stores AES key locate at pDest
}

/**************************************************************************************
** Function name:	       EncryptVerify
** Descriptions:         encrypt data , then send to AP to decrypt. 
** input :	 outCipher--length is 32
** output:   outCipher --cipher data address
             len--cipher data len 
** Returned value:			1: ERROR, 0:SUCCESS
** Used global variables:	None
** Calling modules:	       GetAlgoKey,generateRandom,Aes_Encrypt
***************************************************************************************/
void  EncryptVerify(unsigned char * outCipher, int *len)
{
    unsigned char tempAesKey[AES_LEN]={0};
    unsigned char randData[AES_LEN]={0};
    //if((sizeof(g_AlgoKey)%16!=0)||(sizeof(tempAesKey)%16!=0)||(sizeof(randData)%16!=0))
    //{
    //		return;
    //}
    GetAlgoKey(g_AlgoKey, tempAesKey);

    generateRandom(randData,AES_LEN);
    memcpy(outCipher,randData,AES_LEN);
    Aes_Encrypt(randData,tempAesKey,&outCipher[AES_LEN]);   
    *len = AES_LEN*2;       
}