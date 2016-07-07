
#ifndef  finger1020H
#define  finger1020H

#define FPC1020_HARDWARE_ID						0x020a
#define FPC1020_CLASS_NAME              		"fpsensor"
#define FPC1020_WORKER_THREAD_NAME				"fpc1020_worker"
#define FPC1020_MAJOR							235
#define FPC1020_PIXEL_ROWS						192U
#define FPC1020_PIXEL_COLUMNS					192U
#define FPC1020_FRAME_SIZE						(FPC1020_PIXEL_COLUMNS * FPC1020_PIXEL_ROWS)						 
#define FPC1020_IMAGE_BUFFER_SIZE				FPC1020_FRAME_SIZE
#define FPC1020_RESET_RETRIES					2
#define FPC1020_RESET_LOW_US					1000
#define FPC1020_RESET_HIGH1_US					100
#define FPC1020_RESET_HIGH2_US					1250
#define FPC1020_SLEEP_RETRIES					3
#define FPC1020_SLEEP_RETRY_TIME_US				100
#define FPC1020_FINGER_UP_THRESHOLD				0
#define FPC1020_REG_READ_INTERRUPT_WITH_CLEAR   28
#define FPC1020_CMD_CAPTURE_IMAGE				192
#define FPC1020_CMD_READ_IMAGE					196
#define FPC1020_CMD_WAIT_FOR_FINGER_PRESENT		36
#define FPC1020_CMD_SOFT_RESET					248
#define FPC1020_REG_SAMPLE_PX_DLY		        104
#define FPC1020_REG_PXL_RST_DLY			        108
#define FPC1020_REG_FINGER_DRIVE_DLY	        144
#define FPC1020_REG_ADC_SETUP			        156
#define FPC1020_REG_ANA_TEST_MUX		        136
#define FPC1020_REG_ADC_SETUP			        156
#define FPC1020_REG_FINGER_DRIVE_CONF	        140
#define FPC1020_REG_ADC_SHIFT_GAIN		        160
#define FPC1020_REG_PXL_CTRL			        168
#define FPC1020_REG_IMAGE_SETUP			        92                                                
#define FPC1020_REG_FNGR_DET_THRES		        216
#define FPC1020_REG_FNGR_DET_CNTR		        220
#define FPC1020_REG_IMG_CAPT_SIZE		        84	                                          	                                            
//传感器初始化                                  
#define FPC1020_REG_TST_COL_PATTERN_EN          120
#define FPC1020_REG_FINGER_DRIVE_CONF           140

void FPC1020_spi_init(void);
int Fpc1020_init(void);
int Fpc1020_read_Image(unsigned char *cpImage);
int FPC1020_ReadImage(unsigned char *cpImage);
// 调用方法:

// 1: FPC1020_spi_init();
// 2: fpc1020_init();
// 3: fpc1020_read_Image();
#endif

