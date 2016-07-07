/****************************************************************************
����:   zhangys
��Ȩ��	�㽭ά���Ƽ��ɷ����޹�˾
˵��:   ����Ӳ��ATB/AFC/AFB ���ݴ�����FPC/TCS1/TCS2 
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
1��	������ȡ����
�������ƣ�GFP_ExtractAny
����ԭ�ͣ�INT GFP_ExtractAny(INT width, INT height, INT resolution, BYTE *cpImage, BYTE *cpFeature, INT *ipSize);
����˵������ȡָ������
��ڲ�����
width��ָ��ͼ����
height��ָ��ͼ��߶�
resolution��ָ��ͼ��ֱ���TCS1��TCS2��resolution��508,FPC��363
cpImage��ָ��ͼ������
cpFeature����������
ipSize�������������ݴ�С��256�ֽڣ�
����ֵ��0��ʾ�ɹ���������ʾʧ�ܡ�
======================================================================================================================*/
 extern int GFP_ExtractAny(int width, int height, int resolution, unsigned char *cpImage, unsigned char *cpFeature, int *ipSize);

/*====================================================================================================================
2��	�ϳ�ģ�庯��
�������ƣ�GFP_EnrollX
����ԭ�ͣ�INT GFP_EnrollX(BYTE *cpTZ1, BYTE *cpTZ2, BYTE *cpTZ3, BYTE *cpFeatures, INT *ipSize)
����˵����������ָ�������ϳ�Ϊָ��ģ��
��ڲ�����
cpTZ1��ָ������1
cpTZ2��ָ������2
cpTZ3��ָ������3
cpFeatures��ָ��ģ��
ipSize������ģ�����ݴ�С��256�ֽڣ�
����ֵ��0��ʾ�ɹ���-2��ʾ������������ݸ�ʽ���ԣ�������ʾʧ�ܡ�
====================================================================================================================*/
extern int GFP_EnrollX( unsigned char *cpTZ1,  unsigned char *cpTZ2,  unsigned char *cpTZ3,  unsigned char *cpFeatures,  int *ipSize);
///3.0
///2.0
/*====================================================================================================================
3��	�ȶԺ���
�������ƣ�GFP_ Verify
����ԭ�ͣ�INT GFP_Verify(BYTE *cpFeatures1, BYTE *cpFeatures2, INT iSecurityLevel)
����˵������ȡָ������
��ڲ�����
cpFeatures1��ָ������1
cpFeatures2��ָ������2
iSecurityLevel����ȫ�ȼ���ͨ����Ϊ3��
����ֵ������0��ʾ�ȶԳɹ��������ʾ�ȶ�ʧ�ܡ�
======================================================================================================================*/
extern int GFP_Verify( unsigned char *cpFeatures1,  unsigned char *cpFeatures2,int iSecurityLevel);
#else
/*==================================================================================================================
1��	������ȡ����
�������ƣ�GFP_Extract
����ԭ�ͣ�INT GFP_Extract(INT width, INT height, INT resolution, BYTE *cpImage, BYTE *cpFeature, INT *ipSize);
����˵������ȡָ������
��ڲ�����
width��ָ��ͼ����
height��ָ��ͼ��߶�
resolution��ָ��ͼ��ֱ���TCS1��TCS2��resolution��508,FPC��363
cpImage��ָ��ͼ������
cpFeature����������
ipSize�������������ݴ�С��256�ֽڣ�
����ֵ��0��ʾ�ɹ���������ʾʧ�ܡ�
======================================================================================================================*/
 extern int GFP_Extract(int width, int height, int resolution, unsigned char *cpImage, unsigned char *cpFeature, int *ipSize);


/*====================================================================================================================
2��	�ϳ�ģ�庯��
�������ƣ�GFP_EnrollX
����ԭ�ͣ�INT GFP_EnrollX(BYTE *cpTZ1, BYTE *cpTZ2, BYTE *cpTZ3, BYTE *cpFeatures, INT *ipSize)
����˵����������ָ�������ϳ�Ϊָ��ģ��
��ڲ�����
cpTZ1��ָ������1
cpTZ2��ָ������2
cpTZ3��ָ������3
cpFeatures��ָ��ģ��
ipSize������ģ�����ݴ�С��256�ֽڣ�
����ֵ��0��ʾ�ɹ���-2��ʾ������������ݸ�ʽ���ԣ�������ʾʧ�ܡ�
====================================================================================================================*/
extern int GFP_EnrollX( unsigned char *cpTZ1,  unsigned char *cpTZ2,  unsigned char *cpTZ3,  unsigned char *cpFeatures,  int *ipSize);
/*====================================================================================================================
3��	�ȶԺ���
�������ƣ�GFP_ Verify
����ԭ�ͣ�INT GFP_Verify(BYTE *cpFeatures1, BYTE *cpFeatures2, INT iSecurityLevel)
����˵������ȡָ������
��ڲ�����
cpFeatures1��ָ������1
cpFeatures2��ָ������2
iSecurityLevel����ȫ�ȼ���ͨ����Ϊ3��
int*���صķ���
����ֵ������0��ʾ�ȶԳɹ��������ʾ�ȶ�ʧ�ܡ�
======================================================================================================================*/
extern int GFP_Verify( unsigned char *cpFeatures1,  unsigned char *cpFeatures2,int iSecurityLevel,int *);


#endif

//��������
INT GFP_Search(BYTE *cpFeatures, BYTE **cpFeaturesGroup, INT iNumber, INT iSecurityLevel, INT *pFeatureID);

#endif


