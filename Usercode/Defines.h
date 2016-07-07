#ifndef __JZT_H__
#define __JZT_H__

//*********************************************************
//*        			ϵͳ��ʹ�õ��ĳ�����
//*********************************************************
#ifndef BYTE
	typedef void 				VOID;
	typedef unsigned char 		BYTE;
	typedef signed char 		SBYTE;
	typedef unsigned short 		WORD;
	typedef unsigned short 		USHORT;
	typedef short 				SHORT;
	typedef int 				INT;
	typedef long 				LONG;
	typedef unsigned int 		UINT;
	typedef unsigned long 		DWORD;
	typedef char 				CHAR;
	typedef float 				FLOAT;
	typedef double 				DOUBLE;

	#define uint8			unsigned char
	#define uint16 			unsigned short
	#define uint32			unsigned long
	#define sint8			signed char
	#define sint16			signed short
	#define sint32			signed long

	#define INT8U 			unsigned char
	#define INT8			signed   char
	#define INT16U			unsigned short
	#define INT16 			signed   short
	#define INT32U			unsigned int
	#define INT32			signed   int
	#define FP32			float
	#define FP64			double
#endif

///////////////////////////////////////////////////////////////////
//���汾������Ϣ
#define FINGER_LIB  3

//////////////////////////////////////////////////////////////////



// ******************** �����ೣ�� ********************
#define ON						0x01
#define OFF						0x00
#define READMODE				0x00		// ��Flash���ж�����
#define WRITEMODE				0x01		// ��Flash����д����
#define JIEMI              		0x00		// ����
#define JIAMI              		0x01		// ����
#define READMODE				0x00
#define WRITEMODE				0x01
#define FLASHREAD				0x00
#define FLASHWRITE				0x01
#define NORMAL                  0x01
#define BUFSIZE 				4096
#define LOW_BAUDRATE 			9600		//������9600
#define MID_BAUDRATE 			57600		//������57600
#define HIGH_BAUDRATE 			115200		//������115200
#define HIGH4_BAUDRATE 			(115200*8)	//������115200*4
/* ---------------- ��ָ�ƴ�������� ---------------- */
#define SensorType  			2		// ָ�ƴ���������, 1��ʾ1A-2E, 2��ʾ2F-3B
#define SENSOR_FPC1011			0			// FPC1011ָ�ƴ�����
#define SENSOR_TCS1				1			// UPAK������	
#define SENSOR_TCS2				2			// UPAK������	
#define SENSOR_FPC1020			3			// FPC1020������	

#define FPC1011_WIDTH			152			// FPC1011ָ�ƴ�������ԭʼ���
#define FPC1011_HEIGHT			200			// FPC1011ָ�ƴ�������ԭʼ�߶�
#define FPC1020_WIDTH			192			// FPC1020ָ�ƴ�������ԭʼ���
#define FPC1020_HEIGHT			192			// FPC1020ָ�ƴ�������ԭʼ�߶�
#define TCS1_WIDTH				256			// UPEK_TCS1ָ�ƴ�����ԭʼ���
#define TCS1_HEIGHT				360			// UPEK_TCS1ָ�ƴ�����ԭʼ�߶�
#define TCS2_WIDTH				208			// UPEK_TCS2ָ�ƴ�����ԭʼ���
#define TCS2_HEIGHT				288			// UPEK_TCS2ָ�ƴ�����ԭʼ�߶�


/* ---------------- ��ָ��ģ���й� ---------------- */
#define LENOFUSERINFO			150
#define LENOFMBINFOINDEX		100
#define LENOFMBLIB	  			255		// ģ��������Ϣ
#define MINFEATURES		  		10			// ����ָ��ģ����
#define MAIN_FINGER				1			// ָ��ģ������� - ��ģ��
#define AUX_FINGER				2			// ָ��ģ������� - ��ģ��
#define SEARCH_ALL				0			// ��ָ��ģ����������ʽ - ȫ����
#define SEARCH_MAIN				1			// ��ָ��ģ����������ʽ - ������ģ��
#define SEARCH_AUX				2			// ��ָ��ģ����������ʽ - ������ģ��
#define LENOFVERSION			64

#if FINGER_LIB ==2
#define LENOFMBINFO				256+2
#define LENOFMB  				256       		 // ָ��ģ��ĳ���,��CRC
#define LENOFTZ  				256      		 // ָ�������ĳ���,��CRC
#else
#define LENOFMBINFO				384+2
#define LENOFMB  				384       		 // ָ��ģ��ĳ���,��CRC
#define LENOFTZ  				384      		 // ָ�������ĳ���,��CRC
#endif

#define FPC_MEAN_VALUE			55			// 50
#define FPC_UP_MEAN_VALUE		255-35		// 50
#define MEANVALUE 				0x0C
#define FPC_WAIT_FINGER			150			// Org = 16, About 4.5s Delay
#define COMMANDBLINKDELAY		40

//*********************************************************
//*     ����������DataFlash�еĴ���λ���й�
//*********************************************************
#define LENOFUPDATEFRAME			1024		// USBͨ�������������ݱ��Ĵ�С

#define APP_OFFSET_ADDRESS			0x010000	/*APP������*/
#define APP_LENGTH					0x080000	//��С512K

#define SYS_OFFSET_ADDRESS			0x00FC00	/*�ļ�ϵͳ��־��*/
#define SYS_LENGTH					0x000400	//��С1K

#define TCS1_DATA_OFFSETADDR		0x090000    /*TCS��ʼ������*/
#define TCS1_DATA_LENGTH			0x1000		//��С4K

#define PARA_AREA_ADDRESS			0x091000	// ����������ʼ��ַ
#define PARA_AREA_SIZE				0x00100		// �������Ĵ�С

#define MBINDEX_AREA_ADDRESS		0x092000	// ģ������������ʼ��ַ
#define MBINDEX_AREA_SIZE			0x00200		// ģ���������Ĵ�С
#define MB_AREA_ADDRESS				0x092200	// ģ��������ʼ��ַ
#define MB_AREA_SIZE				0x40000		// ģ�����Ĵ�С

#define RESERVE_ARER_ADDRESS		0x0D2200	// �����������ʼ��ַ
#define RESERVE_ARER_SIZE			0x2DE00		// ��������Ĵ�С

#define LENOFMBINDEX				128					// Flash��ָ��ģ���Ĵ�С
//*********************************************************
//*     �����ķ���ֵ������ķ���ֵ
//*********************************************************
//����������
#define ERR_OK						0x00			//�����ɹ�
#define ERR_READ_FINGERPRINT		0x01			//��ȡָ��ʧ��
#define ERR_READ_FLASH				0x02			//��ȡflashʧ��
#define ERR_WRITE_FLASH				0x03			//дflashʧ��
#define ERR_TIMEOUT          		0x04            //��ʱ����
#define ERR_NOIMAGE					0x05            //ͼ��ʧ��
#define ERR_UPDATEDATEERROR			0x06			//����ʧ��
#define ERR_CRC						0x07			//У��ʧ��
#define ERR_INDEXFULL				0x08			//ָ������
#define ERR_MALLOC					0x09			//����ռ�
#define ERR_PARAM					0x10			//��������
#define ERR_MATCH					0x11			//�ȶ�ʧ��
#define ERR_SAMPLE					0x12			//��ȡ����ʧ��
#define ERR_SPACE					0x13			//�ռ䲻��
#define ERR_NOTOK					0xFF			//ͨ�ô���
//*********************************************************
//*        			USB����������롡
//*********************************************************
#define COM_GETVERISON			0x01	//��ȡ�豸�汾
#define COM_GETDEVID			0x0C	//��ȡ�豸ID
#define COM_SETDEVID			0x0B	//�����豸ID
#define COM_GETFPIMG			0x1E	//��ȡָ��ͼ��
#define COM_GETFPTZ				0x12	//��ȡָ������
#define COM_GETFPMB				0x10	//��ȡָ��ģ��
#define COM_GETFPMATCH			0xA0	//��ȡָ�Ʊȶ�
#define COM_GETFPSEARCH			0x31	//��ȡָ������
#define COM_GETFPCHECK			0x43	//�����ָ 
#define COM_GETMBCNT			0x23	//��ȡģ������
#define COM_GETCLEARMB			0x20	//���ģ��
#define COM_INSERTMB			0x21	//����ģ��
#define COM_GETLIBVERISON		0xFE	//��ȡ�㷨�汾
#define COM_GETSENSORTYPE		0xFF	//��ȡ����������
#define COM_INITSENSOR			0x73	//��ʼ��������
#define COM_UPDATE				0x50	//����Ӧ�ó���

#define USB_HID_TIMEOUT				5					// ����3MS��ʱ
#define APPINTOSLEEP				(60*1000)			// ����15S��ʱ
//*********************************************************
//*        			UART����������롡
//*********************************************************
#define UART0_GETVERISON		0xB0	//��ȡ�豸�汾
#define UART1_GETVERISON		0xD0

#define UART0_GETDEVID			0xB1	//��ȡ�豸ID
#define UART1_GETDEVID			0xD1

#define UART0_SETDEVID			0xB2	//�����豸ID
#define UART1_SETDEVID			0xD2

#define UART0_GETFPIMG			0xB3	//��ȡָ��ͼ��
#define UART1_GETFPIMG			0xD3

#define UART0_GETFPTZ			0xB4	//��ȡָ������
#define UART1_GETFPTZ			0xD4

#define UART0_GETFPMB			0xB5	//��ȡָ��ģ��
#define UART1_GETFPMB			0xD5

#define UART0_GETFPMATCH		0xB6	//��ȡָ�Ʊȶ�
#define UART1_GETFPMATCH		0xD6

#define UART0_GETFPSEARCH		0xB7	//��ȡָ������
#define UART1_GETFPSEARCH		0xD7

#define UART0_GETFPCHECK		0xB8	//�����ָ 
#define UART1_GETFPCHECK		0xD8

#define UART0_GETENDATA			0xB9	//���ݼ��ܲ��� 
#define UART1_GETENDATA			0xD9

#define UART0_GETDEDATA			0xBA	//���ݽ��ܲ��� 
#define UART1_GETDEDATA			0xDA

#define UART0_GETMBCNT			0xBB	//��ȡģ������
#define UART1_GETMBCNT			0xDB

#define UART0_GETCLEARMB		0xBC	//���ģ�� 
#define UART1_GETCLEARMB		0xDC

#define UART0_SETBTNAME			0xBD	//���ò���
#define UART1_SETBTNAME			0xDD

#define UART0_SETBTSLEEPTIME	0xBE	//����ʱ�� 
#define UART1_SETBTSLEEPTIME	0xDE


//* ---------------- ϵͳ���������ݽṹ�ĳ��� ---------------- */
#define SIZEOFPARA				0x40			// ϵͳ������ṹ�ĳ���
#define LENOFDEVICEID			32				// �豸���кŵĳ���
#define USERBUF_LENTH			(200*1024)
/* ---------------  ϵͳ������  ------------------*/
struct StruPARA
{
	BYTE ucBtFlag;								//��������
	BYTE ucTime;
	BYTE cDeviceIDFlag;							// �豸���к���Ч���,0x01��ʾ��Ч
	BYTE cpDeviceID[LENOFDEVICEID];				// �豸���к�
	BYTE cCrc;									// У��
};

/* ---------------  ģ���Ľṹ  ------------------*/
typedef struct StruMBLIB
{
	BYTE cFlag;									// ָ��ģ����,0x00��ʾ����
	BYTE ucUsrid;             					// �û�	id
	BYTE cpMB[LENOFMB];							// ָ��ģ�� LENOFMB = 256
	BYTE cCrc;									// У��
} struOneUserInfo;

typedef struct{
	WORD	iPacket;//����ַ
	BYTE  	ucRcvFlg;//�����жϱ�ʶ	
	BYTE	ucRcvTime;//������ɱ�ʶ
}StuHid;

#endif

