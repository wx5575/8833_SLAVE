/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：Timer_Config.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
/*************** 声明全局变量 *****************/
#ifndef __TIMER_CONFIG_H__
#define __TIMER_CONFIG_H__

#ifdef   _TIM_CONF_GLOBALS
#define  _TIM_EXT
#else
#define  _TIM_EXT  extern
#endif


/********************** 函数声明 ***************/
extern void TIM2_it_init(u16 arr,u16 psc);
extern void TIM2_IRQHandler(void);
extern void bsp_tim2_close(void);
extern void bsp_tim2_open(void);

extern void TIM5_it_init(u16 arr,u16 psc);
extern void TIM5_IRQHandler(void);

#endif	//__TIMER_CONFIG_H__
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
