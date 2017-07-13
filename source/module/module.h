
#ifndef __MODULE_H__
#define __MODULE_H__

#include "stm32f10x.h"

#define MODULE_MAX_NUM  15

typedef struct{
    char name[20]; /* 模块名称 */
    char version[10];/* 模块版本 */
    uint32_t id;/* 模块ID */
}MODULE_INFO;

#ifdef   MODULE_GLOBALS
#define  MODULE_EXT
#else
#define  MODULE_EXT  extern
#endif

MODULE_EXT MODULE_INFO interface_info;

extern void scan_modules(void);
extern void auto_scan_modules(void);

#endif //__MODULE_H__

/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

