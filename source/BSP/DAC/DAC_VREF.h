/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：DAC_VREF.h
 * 摘  要  ：头文件
 * 当前版本：V1.0，孙世成编写
 * 修改记录：2014.7 修改人:王鑫
 *
 */
 
#ifndef __DAC_VREF_H
#define __DAC_VREF_H


#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/******************* <Include> ********************/
#include "stm32f10x.h"
			
/******************* <define> *********************/
#define		HV_VREF_RESET		(0)		/* 0V */
#define		SHORT_VREF_RESET	(4095)//(3120)	/* 5V */
#define		ARC_VREF_RESET		(4095)//(3120)	/* 5V */

typedef enum{
    DAC_OUTPUT_VREF,/* 高压输出基准 */
    DAC_SHORT_VREF,/* 短路数据基准 */
    DAC_ARC_VREF,/* ARC输出基准 */
}DAC_VREF_ENUM;

extern 	u16		DAC_Vol[3];

/******************* 函数声明 *********************/
extern void dac_vref_init(void);

/**************************************************/
#if defined(__cplusplus)
    }
#endif

#endif

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
