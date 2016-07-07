#include "w55fa93_gnand.h"
#define LANGID_English_UnitedStates			0x04090304
typedef struct{
	UINT32	Base_Addr;//?迄米??﹞
	UINT32  Rxed_Flag;//?車那??D??㊣那那?	
	UINT32	Txed_Flag;//﹞⊿?赤赤那3谷㊣那那?
	UINT32	_usbd_DMA_Dir;
	UINT32	_usbd_Less_MPS;
	UINT32 	USBD_DMA_LEN;
	UINT32	Mass_LUN;
}HID_INFO_T;

/* MSC Class Command */
#define	BULK_ONLY_MASS_STORAGE_RESET	0xFF
#define	GET_MAX_LUN						0xFE

/* length of descriptors */
#define USB_DEVICE_DSCPT_LEN		0x12
#define USB_CONFIG_DSCPT_LEN		41
#define USB_STR0_DSCPT_LEN			0x04
#define USB_QUALIFIER_DSCPT_LEN		0x0a
#define HID_DESC_SIZE    			0x22
 
/* extern functions */
void mscdSDRAM2USB_Bulk(UINT32 DRAM_Addr ,UINT32 Tran_Size);
void mscdUSB2SDRAM_Bulk(UINT32 DRAM_Addr ,UINT32 Tran_Size);
VOID mscdInit(VOID);
void hidHighSpeedInit(void);
void hidFullSpeedInit(void);
VOID mscdDeinit(VOID);