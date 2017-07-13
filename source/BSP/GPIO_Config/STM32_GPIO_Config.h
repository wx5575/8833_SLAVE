/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：STM32_GPIO_Config.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __STM32_GPIO_CONFIG_H__
#define __STM32_GPIO_CONFIG_H__

#include "stm32f10x.h"


//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08

#define EXTI_LINE5_ENABLE       BIT_ADDR(EXTI_BASE, 5)	/* 复位中断 */
// #define EXTI_LINE9_ENABLE    BIT_ADDR(EXTI_BASE, 9)	/* 功放中断 */
#define EXTI_LINE10_ENABLE      BIT_ADDR(EXTI_BASE, 10)	/* 短路中断 */
#define EXTI_LINE11_ENABLE      BIT_ADDR(EXTI_BASE, 11)	/* ARC中断 */
#define EXTI_LINE12_ENABLE      BIT_ADDR(EXTI_BASE, 12)	/* GFI中断 */

#define RTC_INI_ENABLE          BIT_ADDR(RTC_BASE+0, 0)	/* RTC中断 */


//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)            BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PAin(n)             BIT_ADDR(GPIOA_IDR_Addr,n)  //输入

#define PBout(n)            BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
#define PBin(n)             BIT_ADDR(GPIOB_IDR_Addr,n)  //输入

#define PCout(n)            BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
#define PCin(n)             BIT_ADDR(GPIOC_IDR_Addr,n)  //输入

#define PDout(n)            BIT_ADDR(GPIOD_ODR_Addr,n)  //输出
#define PDin(n)             BIT_ADDR(GPIOD_IDR_Addr,n)  //输入

#define PEout(n)            BIT_ADDR(GPIOE_ODR_Addr,n)  //输出
#define PEin(n)             BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)            BIT_ADDR(GPIOF_ODR_Addr,n)  //输出
#define PFin(n)             BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)            BIT_ADDR(GPIOG_ODR_Addr,n)  //输出
#define PGin(n)             BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/////////////////////////////////////////////////////////////////

#define LED_TEST    (PCout(6))     //test
#define LED_PASS    (PGout(8))     //pass
#define LED_FAIL    (PGout(7))     //fail
#define LED_ALL     LED_TEST=LED_PASS=LED_FAIL

#define LED_OFF     (0)
#define LED_ON      (1)

#define ROW0	    (PAout(7))
#define ROW1	    (PBout(2))

#define BUZZER	    (PBout(1))

#define	BUZZER_OFF	(0)
#define BUZZER_ON	(1)

#define START_PIN   (PAin(6))   /* 读start引脚电平 */
#define STOP_PIN	(PAin(5))   /* 读stop引脚电平 */
#define AMP_PIN		(PAin(9))   /* 读功放引脚电平 */
#define SHORT_PIN 	(PAin(10))	/* 读短路引脚电平 */
#define ARC_PIN		(PAin(11))	/* 读ARC 引脚电平 */
#define GFI_PIN		(PAin(12))	/* 读GFI 引脚电平 */

#define CAL_PIN		(PGin(15))	/* 读校准开关引脚 */

extern void GPIO_Configuration(void);

#endif //__STM32_GPIO_CONFIG_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/

