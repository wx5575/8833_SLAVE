/*
 * Copyright(c) 2013,
 * All rights reserved
 * �ļ����ƣ�SineWave.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0�������ɱ�д
 * �޸ļ�¼��
 *
 */

#include "SineWave.h"
#include "cs99xx.h"

u8 SIN_FLAG = 0;


/*
 * ��������PWM_Config
 * ����  ����������
 * ����  ��arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
 * ���  ����
 */
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);  //ʹ��GPIOB����ʱ��ʹ��
	
   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���ù������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure); //��ʼ��GPIO
	/* -----------------------------------------------------------------------
	TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
	����TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
	TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	TIM3 Frequency = 72 KHz.
	TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
	----------------------------------------------------------------------- */
//Time base configuration		 
	TIM_TimeBaseStructure.TIM_Period = arr-1;       //������ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = psc;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//PWM4 Mode configuration: Channel3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = arr/2;	   //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);		// ʹ��TIM4���ؼĴ���ARR
	//TIM3 enable counter
	TIM_Cmd(TIM4, ENABLE);                   //ʹ�ܶ�ʱ��4  

}

/*
 * ��������set_sine_hz
 * ����  ����ʼ�����Ҳ���Ƶ�� ��cpld�Ļ���ֵ
 * ����  ��n	Ƶ�ʵ�λ
 * ���  ����
 * ����  ����
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
