#ifndef __JZT_H__
#define __JZT_H__

//*********************************************************
//*        			系统中使用到的常量　
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
//各版本配置信息
#define FINGER_LIB  3

//////////////////////////////////////////////////////////////////



// ******************** 公共类常量 ********************
#define ON						0x01
#define OFF						0x00
#define READMODE				0x00		// 对Flash进行读操作
#define WRITEMODE				0x01		// 对Flash进行写操作
#define JIEMI              		0x00		// 加密
#define JIAMI              		0x01		// 解密
#define READMODE				0x00
#define WRITEMODE				0x01
#define FLASHREAD				0x00
#define FLASHWRITE				0x01
#define NORMAL                  0x01
#define BUFSIZE 				4096
#define LOW_BAUDRATE 			9600		//波特率9600
#define MID_BAUDRATE 			57600		//波特率57600
#define HIGH_BAUDRATE 			115200		//波特率115200
#define HIGH4_BAUDRATE 			(115200*8)	//波特率115200*4
/* ---------------- 与指纹传感器相关 ---------------- */
#define SensorType  			2		// 指纹传感器类型, 1表示1A-2E, 2表示2F-3B
#define SENSOR_FPC1011			0			// FPC1011指纹传感器
#define SENSOR_TCS1				1			// UPAK传感器	
#define SENSOR_TCS2				2			// UPAK传感器	
#define SENSOR_FPC1020			3			// FPC1020传感器	

#define FPC1011_WIDTH			152			// FPC1011指纹传感器的原始宽度
#define FPC1011_HEIGHT			200			// FPC1011指纹传感器的原始高度
#define FPC1020_WIDTH			192			// FPC1020指纹传感器的原始宽度
#define FPC1020_HEIGHT			192			// FPC1020指纹传感器的原始高度
#define TCS1_WIDTH				256			// UPEK_TCS1指纹传感器原始宽度
#define TCS1_HEIGHT				360			// UPEK_TCS1指纹传感器原始高度
#define TCS2_WIDTH				208			// UPEK_TCS2指纹传感器原始宽度
#define TCS2_HEIGHT				288			// UPEK_TCS2指纹传感器原始高度


/* ---------------- 与指纹模板有关 ---------------- */
#define LENOFUSERINFO			150
#define LENOFMBINFOINDEX		100
#define LENOFMBLIB	  			255		// 模板索引信息
#define MINFEATURES		  		10			// 最少指纹模板数
#define MAIN_FINGER				1			// 指纹模板的类型 - 主模板
#define AUX_FINGER				2			// 指纹模板的类型 - 辅模板
#define SEARCH_ALL				0			// 对指纹模板库的搜索方式 - 全搜索
#define SEARCH_MAIN				1			// 对指纹模板库的搜索方式 - 搜索主模板
#define SEARCH_AUX				2			// 对指纹模板库的搜索方式 - 搜索辅模板
#define LENOFVERSION			64

#if FINGER_LIB ==2
#define LENOFMBINFO				256+2
#define LENOFMB  				256       		 // 指纹模板的长度,含CRC
#define LENOFTZ  				256      		 // 指纹特征的长度,含CRC
#else
#define LENOFMBINFO				384+2
#define LENOFMB  				384       		 // 指纹模板的长度,含CRC
#define LENOFTZ  				384      		 // 指纹特征的长度,含CRC
#endif

#define FPC_MEAN_VALUE			55			// 50
#define FPC_UP_MEAN_VALUE		255-35		// 50
#define MEANVALUE 				0x0C
#define FPC_WAIT_FINGER			150			// Org = 16, About 4.5s Delay
#define COMMANDBLINKDELAY		40

//*********************************************************
//*     各类数据在DataFlash中的存贮位置有关
//*********************************************************
#define LENOFUPDATEFRAME			1024		// USB通信在线升级数据报的大小

#define APP_OFFSET_ADDRESS			0x010000	/*APP程序区*/
#define APP_LENGTH					0x080000	//大小512K

#define SYS_OFFSET_ADDRESS			0x00FC00	/*文件系统标志区*/
#define SYS_LENGTH					0x000400	//大小1K

#define TCS1_DATA_OFFSETADDR		0x090000    /*TCS初始化数据*/
#define TCS1_DATA_LENGTH			0x1000		//大小4K

#define PARA_AREA_ADDRESS			0x091000	// 参数区的起始地址
#define PARA_AREA_SIZE				0x00100		// 参数区的大小

#define MBINDEX_AREA_ADDRESS		0x092000	// 模板索引区的起始地址
#define MBINDEX_AREA_SIZE			0x00200		// 模板索引区的大小
#define MB_AREA_ADDRESS				0x092200	// 模板区的起始地址
#define MB_AREA_SIZE				0x40000		// 模板区的大小

#define RESERVE_ARER_ADDRESS		0x0D2200	// 保留区域的起始地址
#define RESERVE_ARER_SIZE			0x2DE00		// 保留区域的大小

#define LENOFMBINDEX				128					// Flash内指纹模板库的大小
//*********************************************************
//*     函数的返回值或命令的返回值
//*********************************************************
//操作返回码
#define ERR_OK						0x00			//操作成功
#define ERR_READ_FINGERPRINT		0x01			//读取指纹失败
#define ERR_READ_FLASH				0x02			//读取flash失败
#define ERR_WRITE_FLASH				0x03			//写flash失败
#define ERR_TIMEOUT          		0x04            //超时返回
#define ERR_NOIMAGE					0x05            //图像失败
#define ERR_UPDATEDATEERROR			0x06			//升级失败
#define ERR_CRC						0x07			//校验失败
#define ERR_INDEXFULL				0x08			//指纹已满
#define ERR_MALLOC					0x09			//分配空间
#define ERR_PARAM					0x10			//参数错误
#define ERR_MATCH					0x11			//比对失败
#define ERR_SAMPLE					0x12			//提取特征失败
#define ERR_SPACE					0x13			//空间不足
#define ERR_NOTOK					0xFF			//通用错误
//*********************************************************
//*        			USB命令的命令码　
//*********************************************************
#define COM_GETVERISON			0x01	//读取设备版本
#define COM_GETDEVID			0x0C	//读取设备ID
#define COM_SETDEVID			0x0B	//设置设备ID
#define COM_GETFPIMG			0x1E	//读取指纹图像
#define COM_GETFPTZ				0x12	//读取指纹特征
#define COM_GETFPMB				0x10	//读取指纹模板
#define COM_GETFPMATCH			0xA0	//读取指纹比对
#define COM_GETFPSEARCH			0x31	//读取指纹搜索
#define COM_GETFPCHECK			0x43	//检测手指 
#define COM_GETMBCNT			0x23	//获取模板数量
#define COM_GETCLEARMB			0x20	//清空模板
#define COM_INSERTMB			0x21	//插入模板
#define COM_GETLIBVERISON		0xFE	//获取算法版本
#define COM_GETSENSORTYPE		0xFF	//获取传感器类型
#define COM_INITSENSOR			0x73	//初始化传感器
#define COM_UPDATE				0x50	//升级应用程序

#define USB_HID_TIMEOUT				5					// 接收3MS超时
#define APPINTOSLEEP				(60*1000)			// 接收15S超时
//*********************************************************
//*        			UART命令的命令码　
//*********************************************************
#define UART0_GETVERISON		0xB0	//读取设备版本
#define UART1_GETVERISON		0xD0

#define UART0_GETDEVID			0xB1	//读取设备ID
#define UART1_GETDEVID			0xD1

#define UART0_SETDEVID			0xB2	//设置设备ID
#define UART1_SETDEVID			0xD2

#define UART0_GETFPIMG			0xB3	//读取指纹图像
#define UART1_GETFPIMG			0xD3

#define UART0_GETFPTZ			0xB4	//读取指纹特征
#define UART1_GETFPTZ			0xD4

#define UART0_GETFPMB			0xB5	//读取指纹模板
#define UART1_GETFPMB			0xD5

#define UART0_GETFPMATCH		0xB6	//读取指纹比对
#define UART1_GETFPMATCH		0xD6

#define UART0_GETFPSEARCH		0xB7	//读取指纹搜索
#define UART1_GETFPSEARCH		0xD7

#define UART0_GETFPCHECK		0xB8	//检测手指 
#define UART1_GETFPCHECK		0xD8

#define UART0_GETENDATA			0xB9	//数据加密测试 
#define UART1_GETENDATA			0xD9

#define UART0_GETDEDATA			0xBA	//数据解密测试 
#define UART1_GETDEDATA			0xDA

#define UART0_GETMBCNT			0xBB	//获取模板数量
#define UART1_GETMBCNT			0xDB

#define UART0_GETCLEARMB		0xBC	//清空模板 
#define UART1_GETCLEARMB		0xDC

#define UART0_SETBTNAME			0xBD	//设置参数
#define UART1_SETBTNAME			0xDD

#define UART0_SETBTSLEEPTIME	0xBE	//设置时间 
#define UART1_SETBTSLEEPTIME	0xDE


//* ---------------- 系统参数表数据结构的长度 ---------------- */
#define SIZEOFPARA				0x40			// 系统参数表结构的长度
#define LENOFDEVICEID			32				// 设备序列号的长度
#define USERBUF_LENTH			(200*1024)
/* ---------------  系统参数表  ------------------*/
struct StruPARA
{
	BYTE ucBtFlag;								//蓝牙设置
	BYTE ucTime;
	BYTE cDeviceIDFlag;							// 设备序列号有效标记,0x01表示有效
	BYTE cpDeviceID[LENOFDEVICEID];				// 设备序列号
	BYTE cCrc;									// 校验
};

/* ---------------  模板库的结构  ------------------*/
typedef struct StruMBLIB
{
	BYTE cFlag;									// 指纹模板标记,0x00表示空闲
	BYTE ucUsrid;             					// 用户	id
	BYTE cpMB[LENOFMB];							// 指纹模板 LENOFMB = 256
	BYTE cCrc;									// 校验
} struOneUserInfo;

typedef struct{
	WORD	iPacket;//基地址
	BYTE  	ucRcvFlg;//接收中断标识	
	BYTE	ucRcvTime;//发送完成标识
}StuHid;

#endif

