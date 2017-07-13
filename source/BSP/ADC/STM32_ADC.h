/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：STM32_ADC.h
 * 摘  要  ：头文件
 * 当前版本：V1.0，孙世成编写
 * 修改记录：
 *
 */
#ifndef __STM32_ADC_H
#define __STM32_ADC_H


#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
			
/******************* <Include> ********************/
#include "stm32f10x.h"
			
/******************* <define> *********************/
extern u16 ADC_Voltage,ADC_Current;

/******************* 函数声明 *********************/
extern void ADC_Config(void);
extern u16 Get_Adc_Average(u8 ch,u8 times);
extern u16 Get_Adc(u8 ch);
extern void ADC_WatchdogConfig(uint16_t HighThreshold);
extern void ADC_WatchdogON(void);
extern void ADC_WatchdogOFF(void);

/**************************************************/
#if defined(__cplusplus)
    }
#endif 
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
#endif

