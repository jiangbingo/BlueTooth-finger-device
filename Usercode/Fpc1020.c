/*功能：FPC1020 zhangys 2014年6月20日22:19:09
*/
#include  "IncludeAll.h"  

#define	CLR_FPC_CS()	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) | 0x02 );	// CS1			
#define	SET_FPC_CS()	outpw(REG_SPI0_SSR, inpw(REG_SPI0_SSR) & 0xfd);		// CS1	

#define	CLR_FPC_RST()	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x0<<6);		
#define	SET_FPC_RST()	gpio_setportval(GPIO_PORTA, 0x1<<6, 0x1<<6);

#define	CLR_FPC_PWR()	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x0<<1)		
#define	SET_FPC_PWR()	gpio_setportval(GPIO_PORTB, 0x1<<1, 0x1<<1)   
                                      
extern BYTE FPCReadWriteByte(BYTE cByte);
//片选选中
void Fpc1020Select(void)
{
    CLR_FPC_CS(); 
    Delayus(20);
}
//片选禁用
void Fpc1020UnSelect(void)
{
   Delayus(20);
   SET_FPC_CS();
  
} 
//复位 拉低
void Fpc1020Rst(void)
{
	CLR_FPC_RST();
	Delayus(20);
}
//复位 拉高
void Fpc1020UnRst(void)
{
   SET_FPC_RST();
   Delayus(20);
} 

void FPC1020_spi_init(void)
{
    Fpc1020UnRst();
    Fpc1020UnSelect();
}

//打开电源
void Fpc1020PowerOn(void)
{
   SET_FPC_PWR();
   Delayms(2);
} 
//关闭电源
void Fpc1020PowerOff(void)
{
	Delayms(2);
   CLR_FPC_PWR();
  
} 


unsigned char Fpc1020Transfer(unsigned char command)
{
	return FPCReadWriteByte(command);
}
void Fpc1020SendByte(unsigned char command)
{
	//FPCWriteByte(command); 
	FPCReadWriteByte(command);
}
void Fpc1020WriteSensor(char *szFName,const unsigned char *pucRegBuf,int stLength )
{
    int i=0;

    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    for(i=0;i<stLength;i++)
        Fpc1020SendByte(pucRegBuf[i]);
    Fpc1020UnSelect();		//Chip select back up. 
    return ;   
}

int Fpc1020_gpio_reset(void)
{

	int i=0;
	for(i=0;i<2;i++)
	{
		Fpc1020UnRst();
		Delayms(10);

		Fpc1020Rst();
		Delayms(10);

		Fpc1020UnRst();
		Delayms(10);
	}
	return ERR_OK;
}
int Fpc1020_reset(void)
{


    Fpc1020_gpio_reset();     //硬件复位 

	Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(FPC1020_REG_READ_INTERRUPT_WITH_CLEAR);	//Send command for reading the interrupt ID
    Fpc1020Transfer(0); 		//Dummy byte. The correct result should be irq=0xFF.
    Fpc1020UnSelect();		//Chip select back up.
    
    return ERR_OK;

}

int Fpc1020_write_sensor_setup(void)
{
  
    //unsigned char temp_u64[9] = {FPC1020_REG_SAMPLE_PX_DLY,0x36,0x36,0x36,0x36,0x3f,0x3f,0x3f,0x3f};
    unsigned char temp_u64[9] = {FPC1020_REG_SAMPLE_PX_DLY,0x08, 0x08,0x08,0x08,0x14,0x14,0x14,0x14};
    unsigned char temp_u8_1[2] = {FPC1020_REG_PXL_RST_DLY,0x33};
    //unsigned char temp_u8_2[2] = {FPC1020_REG_FINGER_DRIVE_DLY,0x37};
    unsigned char temp_u8_2[2] = {FPC1020_REG_FINGER_DRIVE_DLY,0x09};
    unsigned char temp_u8_3[6] = {FPC1020_REG_ADC_SETUP,0x55,0x40,0x00,0x3f,0x24};
    unsigned char temp_u8_4[5] = {FPC1020_REG_ANA_TEST_MUX,0x00,0x08,0x00,0x00};
    unsigned char temp_u8_5[6] = {FPC1020_REG_ADC_SETUP,0x55,0x40,0x00,0x3f,0x34};
    unsigned char temp_u8_6[2] = {FPC1020_REG_FINGER_DRIVE_CONF,0x32};
   
    //unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x05,0x06};
    //unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x0f,0x03};
    unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x0e,0x00};
    //unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x0f,0x02};
    //unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x00,0x02};
    //unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x00,0x01};
    //unsigned char temp_u8_7[3] = {FPC1020_REG_ADC_SHIFT_GAIN,0x01,0x01};
    unsigned char temp_u8_8[3] = {FPC1020_REG_PXL_CTRL,0x0f,0x1b};
    unsigned char temp_u8_9[2] = {FPC1020_REG_IMAGE_SETUP,0x0b};
   
    unsigned char temp_u8_10[2] = {FPC1020_REG_FNGR_DET_THRES,0x50};
    unsigned char temp_u8_11[3] = {FPC1020_REG_FNGR_DET_CNTR,0x00,0xff};
    unsigned char temp_u8_12[5] = {FPC1020_REG_IMG_CAPT_SIZE,0x00,192,0,192};
   
    //WriteSensor( "temp_u8_1", temp_u8_1, sizeof(temp_u8_1) );
    Fpc1020WriteSensor( "temp_u8_2", temp_u8_2, sizeof(temp_u8_2) );  
    Fpc1020WriteSensor( "temp_u64", temp_u64, sizeof(temp_u64) );
    //WriteSensor( "temp_u8_3", temp_u8_3, sizeof(temp_u8_3) );
    //WriteSensor( "temp_u8_4", temp_u8_4, sizeof(temp_u8_4) );
    //WriteSensor( "temp_u8_5", temp_u8_5, sizeof(temp_u8_5) );
    Fpc1020WriteSensor( "temp_u8_6", temp_u8_6, sizeof(temp_u8_6) );
    Fpc1020WriteSensor( "temp_u8_7", temp_u8_7, sizeof(temp_u8_7) );
    Fpc1020WriteSensor( "temp_u8_8", temp_u8_8, sizeof(temp_u8_8) );
    Fpc1020WriteSensor( "temp_u8_9", temp_u8_9, sizeof(temp_u8_9) );   
    Fpc1020WriteSensor( "temp_u8_10", temp_u8_10, sizeof(temp_u8_10) );   
    Fpc1020WriteSensor( "temp_u8_11", temp_u8_11, sizeof(temp_u8_11) );   
    Fpc1020WriteSensor( "temp_u8_12", temp_u8_12, sizeof(temp_u8_12) );
    
    return ERR_OK;

}
extern UINT8 *Sprintfs;
int Fpc1020_init(void)
{
    unsigned char Ret=0xff;
    unsigned char Hw_id[2] = {0};
	
	Fpc1020_reset();               //复位FPC1020   
    Ret = 0x55;
   
    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(20);	
    Ret = Fpc1020Transfer(0); 	
    Fpc1020UnSelect();		//Chip select back up. 
    Delayus(100);

    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(252);	//Send command for reading the interrupt ID
	
    Hw_id[0]= Fpc1020Transfer(0); 		
    Hw_id[1]= Fpc1020Transfer(0); 	
    Fpc1020UnSelect();		//Chip select back up. 
	if(Hw_id[0]==0x02&&Hw_id[1]==0x0a)
		Ret = ERR_OK;
    Fpc1020_write_sensor_setup();
    return Ret;
}
 int Fpc1020_read_Image(unsigned char *cpImage)
{
    unsigned char Ret;
    int i;
    unsigned char Regit_value[6];

    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(28);
    Regit_value[1] = Fpc1020Transfer(0); 		//
    Fpc1020UnSelect();		//Chip select back up. 
    Delayus(100);
  

    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(FPC1020_CMD_CAPTURE_IMAGE);	//
    Ret = Fpc1020Transfer(0); 		//
    Fpc1020UnSelect();		//Chip select back up. 
    Delayus(100);
   
    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(FPC1020_CMD_READ_IMAGE);	//
	Delayus(100);
    Fpc1020Transfer(0);
	
    
    for(i=0;i<192*192;i++)
    {
        cpImage[i]= Fpc1020Transfer(0); 		//
    }    
    Fpc1020UnSelect();		//Chip select back up. 	
    
    memcpy(cpImage,Regit_value,2);

    Fpc1020Select();		//Chip select signal to sensor Active (low)	
    Fpc1020Transfer(52);        //
    cpImage[1] = Fpc1020Transfer(0); 		//
    Fpc1020UnSelect();		//Chip select back up. 
    return ERR_OK;
}

int FPC1020_ReadImage(unsigned char *cpImage)
{
	Fpc1020PowerOn();
    Fpc1020_read_Image(cpImage);
    Fpc1020PowerOff();
	return ERR_OK;
}