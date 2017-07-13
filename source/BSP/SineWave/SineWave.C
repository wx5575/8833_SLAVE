/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：SineWave.c
 * 摘  要  ：主程序
 * 当前版本：V1.0，孙世成编写
 * 修改记录：
 *
 */

#include "SineWave.h"
#include "cs99xx.h"

u8 SIN_FLAG = 0;


/*
 * 函数名：PWM_Config
 * 描述  ：引脚配置
 * 输入  ：arr：自动重装值 psc：时钟预分频数
 * 输出  ：无
 */
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);  //使能GPIOB外设时钟使能
	
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure); //初始化GPIO
	/* -----------------------------------------------------------------------
	TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
	例：TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
	TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	TIM3 Frequency = 72 KHz.
	TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
	----------------------------------------------------------------------- */
//Time base configuration		 
	TIM_TimeBaseStructure.TIM_Period = arr-1;       //例：定时器从0计数到999，即为1000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = psc;	    //设置预分频：不预分频，即为72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//PWM4 Mode configuration: Channel3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = arr/2;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //使能通道3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);		// 使能TIM4重载寄存器ARR
	//TIM3 enable counter
	TIM_Cmd(TIM4, ENABLE);                   //使能定时器4  

}

/*
 * 函数名：set_sine_hz
 * 描述  ：初始化正弦波的频率 和cpld的基数值
 * 输入  ：n	频率档位
 * 输出  ：无
 * 返回  ：无
 */
// void set_sine_hz(const int8_t n)
// {
// 	switch(n)
// 	{
// 		case 1:
// 			SET_SIN(SIN_50HZ_PERIOD);
// 			cpld_base = 10000;
// 			break;
// 		case 2:
// 			SET_SIN(SIN_60HZ_PERIOD);
// 			cpld_base = 8333;
// 			break;
// 		case 3:
// 			SET_SIN(SIN_100HZ_PERIOD);
// 			cpld_base = 5000;
// 			break;
// 		case 4:
// 			SET_SIN(SIN_150HZ_PERIOD);
// 			cpld_base = 3333;
// 			break;
// 		case 5:
// 			SET_SIN(SIN_200HZ_PERIOD);
// 			cpld_base = 2500;
// 			break;
// 		case 6:
// 			SET_SIN(SIN_250HZ_PERIOD);
// 			cpld_base = 2000;
// 			break;
// 		case 7:
// 			SET_SIN(SIN_300HZ_PERIOD);
// 			cpld_base = 1667;
// 			break;
// 		case 8:
// 			SET_SIN(SIN_350HZ_PERIOD);
// 			cpld_base = 1429;
// 			break;
// 		case 9:
// 			SET_SIN(SIN_400HZ_PERIOD);
// 			cpld_base = 1250;
// 			break;
// 		default:
// 			SET_SIN(SIN_50HZ_PERIOD);
// 			cpld_base = 10000;
// 			break;
// 	}
// }

