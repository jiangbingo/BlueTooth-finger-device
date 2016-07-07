/* ==========================================================
*     开发人员：zhangys
*     当前版本：v1.00
*     创建时间：2013/3/24
*     修改时间：2013/3/24
*     功能说明：该头文件包含该工程项目用到的所有头文件
============================================================*/

#ifndef __INCLUDEALL_H__
#define __INCLUDEALL_H__

/////////////////////////////////////////////////////////////////////////////
//系统调用
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
//芯片固件
#include "wblib.h"
#include "w55fa93_spi.h"
#include "w55fa93_reg.h"
#include "w55fa93_gpio.h"
#include "usbd.h"
#include "usbclass.h"
#include "w55fa93_i2c.h"


/////////////////////////////////////////////////////////////////////////////
//用户函数头文件
#include "Defines.h"
#include "basic.h"
#include "Userusart.h"
#include "Userusb.h"
#include "Externs.h"
#include "FpiCom.h"
#include "Fpc1011.h"
#include "Fpc1020.h"
#include "spiflash.h"


/////////////////////////////////////////////////////////////////////////////
//指纹算法库
#include "FpiMathLib.h"


#endif