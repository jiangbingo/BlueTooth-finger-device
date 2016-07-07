/****************************************************************************
作者:   zhangys
版权：	浙江维尔科技股份有限公司
说明:   兼容硬件ATB/AFC/AFB 兼容传感器FPC/TCS1/TCS2 
****************************************************************************/
#ifndef __FingerPrintMathLib_H 
#define __FingerPrintMathLib_H
#include "Defines.h"

#define		FPTemplate_Size		256
typedef		unsigned char				FPTemplate[FPTemplate_Size];

#define		FPIndex_Size			100
typedef		unsigned char				FPIndex[FPIndex_Size];
int ICreateIndex(unsigned char *membuf,FPTemplate* pTemp, FPIndex* pIndex);

#if FINGER_LIB ==2
/*==================================================================================================================
1．	特征提取函数
函数名称：GFP_ExtractAny
函数原型：INT GFP_ExtractAny(INT width, INT height, INT resolution, BYTE *cpImage, BYTE *cpFeature, INT *ipSize);
功能说明：提取指纹特征
入口参数：
width：指纹图像宽度
height：指纹图像高度
resolution：指纹图像分辨率TCS1和TCS2的resolution填508,FPC的363
cpImage：指纹图像数据
cpFeature：特征数据
ipSize：返回特征数据大小（256字节）
返回值：0表示成功，其他表示失败。
======================================================================================================================*/
 extern int GFP_ExtractAny(int width, int height, int resolution, unsigned char *cpImage, unsigned char *cpFeature, int *ipSize);

/*====================================================================================================================
2．	合成模板函数
函数名称：GFP_EnrollX
函数原型：INT GFP_EnrollX(BYTE *cpTZ1, BYTE *cpTZ2, BYTE *cpTZ3, BYTE *cpFeatures, INT *ipSize)
功能说明：将三幅指纹特征合成为指纹模板
入口参数：
cpTZ1：指纹特征1
cpTZ2：指纹特征2
cpTZ3：指纹特征3
cpFeatures：指纹模板
ipSize：返回模板数据大小（256字节）
返回值：0表示成功，-2表示传入的特征数据格式不对，其他表示失败。
====================================================================================================================*/
extern int GFP_EnrollX( unsigned char *cpTZ1,  unsigned char *cpTZ2,  unsigned char *cpTZ3,  unsigned char *cpFeatures,  int *ipSize);
///3.0
///2.0
/*====================================================================================================================
3．	比对函数
函数名称：GFP_ Verify
函数原型：INT GFP_Verify(BYTE *cpFeatures1, BYTE *cpFeatures2, INT iSecurityLevel)
功能说明：提取指纹特征
入口参数：
cpFeatures1：指纹特征1
cpFeatures2：指纹特征2
iSecurityLevel：安全等级（通常设为3）
返回值：大于0表示比对成功，否则表示比对失败。
======================================================================================================================*/
extern int GFP_Verify( unsigned char *cpFeatures1,  unsigned char *cpFeatures2,int iSecurityLevel);
#else
/*==================================================================================================================
1．	特征提取函数
函数名称：GFP_Extract
函数原型：INT GFP_Extract(INT width, INT height, INT resolution, BYTE *cpImage, BYTE *cpFeature, INT *ipSize);
功能说明：提取指纹特征
入口参数：
width：指纹图像宽度
height：指纹图像高度
resolution：指纹图像分辨率TCS1和TCS2的resolution填508,FPC的363
cpImage：指纹图像数据
cpFeature：特征数据
ipSize：返回特征数据大小（256字节）
返回值：0表示成功，其他表示失败。
======================================================================================================================*/
 extern int GFP_Extract(int width, int height, int resolution, unsigned char *cpImage, unsigned char *cpFeature, int *ipSize);


/*====================================================================================================================
2．	合成模板函数
函数名称：GFP_EnrollX
函数原型：INT GFP_EnrollX(BYTE *cpTZ1, BYTE *cpTZ2, BYTE *cpTZ3, BYTE *cpFeatures, INT *ipSize)
功能说明：将三幅指纹特征合成为指纹模板
入口参数：
cpTZ1：指纹特征1
cpTZ2：指纹特征2
cpTZ3：指纹特征3
cpFeatures：指纹模板
ipSize：返回模板数据大小（256字节）
返回值：0表示成功，-2表示传入的特征数据格式不对，其他表示失败。
====================================================================================================================*/
extern int GFP_EnrollX( unsigned char *cpTZ1,  unsigned char *cpTZ2,  unsigned char *cpTZ3,  unsigned char *cpFeatures,  int *ipSize);
/*====================================================================================================================
3．	比对函数
函数名称：GFP_ Verify
函数原型：INT GFP_Verify(BYTE *cpFeatures1, BYTE *cpFeatures2, INT iSecurityLevel)
功能说明：提取指纹特征
入口参数：
cpFeatures1：指纹特征1
cpFeatures2：指纹特征2
iSecurityLevel：安全等级（通常设为3）
int*返回的分数
返回值：大于0表示比对成功，否则表示比对失败。
======================================================================================================================*/
extern int GFP_Verify( unsigned char *cpFeatures1,  unsigned char *cpFeatures2,int iSecurityLevel,int *);


#endif

//搜索函数
INT GFP_Search(BYTE *cpFeatures, BYTE **cpFeaturesGroup, INT iNumber, INT iSecurityLevel, INT *pFeatureID);

#endif


