/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：BSP_config.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef  __BSP_CONFIG_H 
#define  __BSP_CONFIG_H

#include "stm32f10x.h" 
#include "ucos_ii.h"

/*********************** 宏定义 *****************************/
#define SOFT_DELAY		200

/* 中断优先级管理 */
enum{
CS99XX_IT_GROUP	  = NVIC_PriorityGroup_3,/* 抢断优先级3位 0-7 响应优先级是0-1 */
UART2_IT_PRE_PRIO = 0,
UART2_IT_SUB_PRIO = 0,

UART5_IT_PRE_PRIO = 0,
UART5_IT_SUB_PRIO = 1,

TIM2_IT_PRE_PRIO  = 1,
TIM2_IT_SUB_PRIO  = 0,

EXIT_IT_PRE_PRIO  = 3,
STOP_IT_SUB_PRIO  = 0,
SAG_IT_SUB_PRIO   = 1,

TIM3_IT_PRE_PRIO  = 2,
TIM3_IT_SUB_PRIO  = 0,

TIM4_IT_PRE_PRIO  = 3,
TIM4_IT_SUB_PRIO  = 1,

TIM5_IT_PRE_PRIO  = 4,
TIM5_IT_SUB_PRIO  = 0,

TIM6_IT_PRE_PRIO  = 4,
TIM6_IT_SUB_PRIO  = 1,

RTC_IT_PRE_PRIO	  = 5,
RTC_IT_SUB_PRIO	  = 0,
};

/*********************** 函数声明 *************************/
extern void init_4094(void);
extern void bsp_init(void);
extern void CRC_Config(void);
extern void systick_init(void);
extern void soft_delay_us(u32 dly_us);
extern void soft_delay_10us(u32 dly_us);

extern void on_switch_hv(void);
extern void off_switch_hv(void);

extern void cs_delay_ms(u16 time);
extern void cs_delay_us(int time);

#endif //__BSP_CONFIG_H
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
