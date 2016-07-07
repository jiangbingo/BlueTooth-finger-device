//*******************************************************************************
//˵����ά�� ָ��Ӧ����غ���
//ʱ�䣺2014��6��20��21:48:17
//���ߣ�zhangys
//*******************************************************************************
#include  "IncludeAll.h"

//ָ�������Ա�
BYTE FpiSearch(BYTE *cpFeature, WORD wSearchFlag, WORD wEndIndex, WORD *wIndex)
{
	WORD i;
	INT iRetCode;
	BYTE **cpMBLib;
	INT cpIndex[LENOFMBLIB];
	INT iNumber = 0;
	INT Index = -1;
	INT SC[LENOFMBLIB];
	INT MAX = 0;
	// ����MBLib�ռ�
	cpMBLib = (BYTE**)malloc(LENOFMBLIB * sizeof(BYTE*) );
	if(cpMBLib == NULL)
	{
		return (ERR_MALLOC);
	}

	if(giMBCounter > LENOFMBLIB)
	{
		return 0xff;
	}
	for(i = 0; i < giMBCounter; i++)
	{
		cpMBLib[i] = spMBLib[i].cpMB;
	}

	// ��ʼ����
	iRetCode = GFP_Search(cpFeature, cpMBLib, giMBCounter, 3, &Index);
	if(iRetCode < 0)
	{
		free(cpMBLib);
		return iRetCode;
	}
	else
	{
		if(Index < 0)
		{
			free(cpMBLib);
			return 0xaa;
		}
		if(Index < giMBCounter)
		{
			*wIndex = Index;
			free(cpMBLib);
			return (ERR_OK);
		}
		else
		{
			free(cpMBLib);
			return 0xbb;
		}
	}
}


//��ȡָ��ͼ��
BYTE FpiGetImage(BYTE *cpImage, WORD wTimeout)
{
	
	BYTE cRetCode=ERR_OK,pixel;
	UINT32 sum, mean, i, j, wFlag;
	
	gudTimeOut = wTimeout*1000;
	
	if(gcSensorType == SENSOR_FPC1011)
	{
		Fpc1011Power(ON);
		
		do
		{
			cRetCode = FPC1011GetImage(cpImage);
			if(cRetCode == ERR_OK)
			{
				sum = 0;

				for(i = 0; i < FPC1011_HEIGHT * FPC1011_WIDTH; i++)
				{
					sum += cpImage[i];
				}
				mean = (short)(sum >> 15);   // mean value is mandatory an unsigned byte in fact

				if(mean > FPC_MEAN_VALUE)
				{
					//��ָ����
					cRetCode = ERR_OK;
					break;
				}
				else
				{
					cRetCode = ERR_READ_FINGERPRINT;
				}
			}
		}
		while(gudTimeOut>0);
		
		Fpc1011Power(OFF);
		
		//ͼ��ת
		for(i = 0; i < FPC1011_HEIGHT / 2; i++)
		{
			for(j = 0; j < FPC1011_WIDTH; j++)
			{
				pixel = cpImage[i * FPC1011_WIDTH + j];
				cpImage[i * FPC1011_WIDTH + j] = 0xff - cpImage[(FPC1011_HEIGHT - 1 - i) * FPC1011_WIDTH + j];
				cpImage[(FPC1011_HEIGHT - 1 - i) * FPC1011_WIDTH + j] = 0xff - pixel;
			}
		}
	}
	else if(gcSensorType == SENSOR_FPC1020)
	{
		Fpc1011Power(ON);
		
		do
		{
		FPC1020_ReadImage(cpImage);
		sum = 0;

		for(i = (FPC1020_HEIGHT/2-120/2); i < (FPC1020_HEIGHT/2+120/2); i++)
		{
			for(j=(FPC1020_WIDTH/2-120/2);j<(FPC1020_WIDTH/2+120/2); j++)
				sum += cpImage[i*FPC1020_WIDTH+j];
		}
		mean = sum/(120*120);   // mean value is mandatory an unsigned byte in fact

		if(mean < 200)
		{
			//��ָ����
			cRetCode = ERR_OK;
			break;
		}
		else
		{
			cRetCode = ERR_READ_FINGERPRINT;
		}
		}
		while(gudTimeOut>0);
		
		Fpc1011Power(OFF);
		return cRetCode;
	}
		return cRetCode;

}

//��ȡָ���豸����
BYTE FpiGetSensorType(void)
{
	BYTE cRet = ERR_NOTOK;

	//�ȼ��FPC1011
	cRet = Fpc1011Init();
	if(cRet == ERR_OK)
	{
		gcSensorType = SENSOR_FPC1011;
		return ERR_OK;
	}
	//���FPC1020
	Fpc1020PowerOn();
	FPC1020_spi_init();
    cRet = Fpc1020_init();
    Fpc1020PowerOff();
    if(cRet == ERR_OK)
    {
		gcSensorType = SENSOR_FPC1020;
		return ERR_OK;
	}
	
	return ERR_NOTOK;
}

/************************************** The End Of File **************************************/

