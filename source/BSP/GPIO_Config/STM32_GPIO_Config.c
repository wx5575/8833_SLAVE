/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：STM32_GPIO_Config.c
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#include "STM32_GPIO_Config.h"

/*
*************************************************
*函数: void GPIO_Configuration(void）
*功能: GPIO配置
*参数: 无
*返回: 无
*************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOG, ENABLE);
	
	/************************** 输入 ***************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* 上拉输入 */
	
	/* 中断引脚配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 型号选择引脚配置 */
	/* TYPE1-6 */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	/* TYPE7-10 */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 /* | GPIO_Pin_9 | GPIO_Pin_10 */;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
// 	/* TYPE12 */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 校准开关PG15 */
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode	=GPIO_Mode_IPU;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/************************** 输出 *****************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
	
	//配置FALL,PASS,TEST 灯PC6、PG7、PG8为输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//配置FMQ 蜂鸣器PB1为输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    LED_ALL  = LED_OFF;/* 关闭 LED */
	
	BUZZER = BUZZER_OFF;/* 蜂鸣器 */
	
	return;
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
