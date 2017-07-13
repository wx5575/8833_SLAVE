
#include "stm32f10x.h"
#include "Timer5_Config.h"
#include "BSP_config.h"



static void (*timer5_call_back)(void);

/*
 *函数名：set_timer5_callback
 *描述：初始化定时器5的回调函数
 *输入：无
 *输出 :无
 *返回：无
*/
void init_timer5_callback(void(*fun)(void))
{
    timer5_call_back = fun;
}
/*
 *函数名：TIM5_it_init
 *描述：定时器5配置
 *输入：arr 重载值
 *      psc 分频比
 *输出 :无
 *返回：无
*/
void TIM5_it_init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;	//自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //用来作为TIMx时钟频率除数的预分频值。它的取值必须在0x0000和0xFFFF之间。
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;//设置了时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择了计数器模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);/* 关闭中断 */
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIM2更新中断标志
	
	/* 中断优先级NVIC设置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM5_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM5_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);/* 打开中断 */
	TIM_Cmd(TIM5, ENABLE);/* 在外面用开关来控制 */
	return;
}
/*
 *函数名：TIM5_IRQHandler
 *描述：定时器5中断服务程序
 *变量：无
 *返回：无
*/
void TIM5_IRQHandler(void)
{
	OSIntEnter();
	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);	     //清中断标记
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 
        
        if(NULL != timer5_call_back)
        {
            timer5_call_back();
        }
	}
	
	OSIntExit();
}
