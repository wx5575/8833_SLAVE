/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：SineWave.h
 * 摘  要  ：头文件
 * 当前版本：V1.0，孙世成编写
 * 修改记录：
 *
 */
#ifndef __SineWave_H
#define __SineWave_H


#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
			
/******************* <Include> ********************/
#include "stm32f10x.h"
			
/******************* <define> *********************/
#define		SIN_50HZ_PERIOD		(5625)// Output frequency = 12800Hz,Error = 0.5Hz,  0.0039%
#define		SIN_60HZ_PERIOD		(4688)// Output frequency = 15360Hz,Error = -1Hz , -0.0065%
#define		SIN_100HZ_PERIOD	(2813)
/* output frequency = 25600Hz 72M/256 * 100=2813 */
#define		SIN_150HZ_PERIOD	(1875)
#define		SIN_200HZ_PERIOD	(1406)
#define		SIN_250HZ_PERIOD	(1125)
#define		SIN_300HZ_PERIOD	(938)
#define		SIN_350HZ_PERIOD	(804)
#define		SIN_400HZ_PERIOD	(703)// Output frequency = 102400Hz,Error = 30Hz , 0.0293%

extern		u8 SIN_FLAG;

// #define		SIN_OFF()			{TIM4_PWM_Init(0,0);GPIO_SetBits(GPIOG,GPIO_Pin_14);}
// #define		SIN_ON()			(GPIO_ResetBits(GPIOG,GPIO_Pin_14))

#define		SIN_50HZ()			{SIN_ON();TIM4_PWM_Init(SIN_50HZ_PERIOD,0);}
#define		SIN_60HZ()			{SIN_ON();TIM4_PWM_Init(SIN_60HZ_PERIOD,0);}
#define		SIN_100HZ()			{SIN_ON();TIM4_PWM_Init(SIN_100HZ_PERIOD,0);}
#define		SIN_150HZ()			{SIN_ON();TIM4_PWM_Init(SIN_150HZ_PERIOD,0);}
#define		SIN_200HZ()			{SIN_ON();TIM4_PWM_Init(SIN_200HZ_PERIOD,0);}
#define		SIN_250HZ()			{SIN_ON();TIM4_PWM_Init(SIN_250HZ_PERIOD,0);}
#define		SIN_300HZ()			{SIN_ON();TIM4_PWM_Init(SIN_300HZ_PERIOD,0);}
#define		SIN_350HZ()			{SIN_ON();TIM4_PWM_Init(SIN_350HZ_PERIOD,0);}
#define		SIN_400HZ()			{SIN_ON();TIM4_PWM_Init(SIN_400HZ_PERIOD,0);}

// #define		SET_SIN(FREQ)		{SIN_ON();TIM4_PWM_Init(FREQ,0);}
// #define		SIN_INFO()			SIN_FLAG

/******************* 函数声明 *********************/
void TIM4_PWM_Init(u16 arr,u16 psc);



/**************************************************/
#if defined(__cplusplus)
    }
#endif 
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
#endif

