/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：Timer-Config.c
 * 摘  要  ：配置程序中用到的定时器
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

/**********************包含头文件 ************************/
#define _TIM_CONF_GLOBALS

#include    "ucos_ii.h"
#include    "stm32f10x.h"
#include	"BSP_config.h"
#include	"Timer_Config.h"
#include 	"app.h"
#include 	"serve_test.h"

#include 	"SysTemConfig.h"
#include 	"cs99xx_type.h"
#include    "irq.h"

/************************函数定义 ***********************/
/*
 *
 *函数名：TIM2_Int_Init
 *描述：	这里时钟选择为APB1的2倍，而APB1为36M
 *变量：	arr：自动重装值。
 *		psc：时钟预分频数
 *说明：这里使用的是定时器2为测试功能提供时钟
 *
*/
void TIM2_it_init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);/* 关闭中断 */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIM2更新中断标志
	
	/* 中断优先级NVIC设置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM2_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);/* 打开中断 */
	TIM_Cmd(TIM2, DISABLE);/* 在外面用开关来控制 */
	return;
}

/*
 *函数名：bsp_tim2_close
 *描述：关闭定时器
 *变量：无
 *返回：无
*/
void bsp_tim2_close(void)
{
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
}

/*
 *函数名：bsp_tim2_open
 *描述：打开定时器
 *变量：无
 *返回：无
*/
void bsp_tim2_open(void)
{
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
/*
 *函数名：TIM2_IRQHandler
 *描述：定时器2中断服务程序
 *变量：无
 *返回：无
*/
void TIM2_IRQHandler(void)
{
    static int32_t c = 0;
    
	OSIntEnter();
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
		
        if(++c >= 20)
        {
            c = 0;
            g_test_time++;
            
            OSSemPost(ScheduleSem);
        }
        
        if(test_flag.gradation == STAGE_RISE //上升
            || test_flag.gradation == STAGE_FALL //下降
            || test_flag.gradation == STAGE_CHARGE //充电
            || test_flag.gradation == STAGE_CHANGE) //缓变
        {
            test_flag.vol_rise_step_t++;
            vol_change_200hz(test_flag.vol_rise_step_t);
//             vol_change_1000hz(test_flag.vol_rise_step_t);
        }
	}
	OSIntExit();
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
