/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：DAC_VREF.c
 * 摘  要  ：主程序
 * 当前版本：V1.0，孙世成编写
 * 修改记录：2014.7 修改人：王鑫
 *
 */
/********************* 包含头文件 ************************/
#include "DAC_VREF.h"
#include "ucos_ii.h"
#include "BSP_config.h"

/************************* 片选引脚进行位绑定 **********************/
#define		CD4051_Pin_A		(*(u8 *)((u32)0x42218194)) /* PB5 */
#define		CD4051_Pin_B		(*(u8 *)((u32)0x422401B4)) /* PG13 */

/************************* 全局变量声明 ****************************/
u16	DAC_Vol[3] = {HV_VREF_RESET,SHORT_VREF_RESET,ARC_VREF_RESET};/* 通过改变该数组元素的值就能在da引脚输出不同的电压*/
																	/* HV_VREF_RESET 高压基准*/
																	/* SHORT_VREF_RESET 短路基准 */
																	/* ARC_VREF_RESET 电弧侦测基准 */

/************************* 内部函数声明 **************************/
static void Dac1_Init(void);
static void CD4051_Init(void);
static void TIM3_Int_Init(u16 arr,u16 psc);
// static void DAC_Delay_ms(unsigned long dly_ms);

/************************* 函数定义 ******************************/

void dac_vref_init(void)
{
	Dac1_Init();
	CD4051_Init();
	TIM3_Int_Init(10-1,7200-1);	/* 10Khz的计数频率，计数到10为1ms */
}

static void Dac1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	/* 使能GPIOA通道时钟 */
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	/* 使能DAC通道时钟 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	/* 端口配置 */
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	/* 模拟输入 */
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;	/* PA.4 输出高 */
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	/* 不使用触发功能 TEN1=0 */
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;	/* 不使用波形发生 */
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	/* 屏蔽、幅值设置 */
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	/* DAC1输出缓存关闭 BOFF1=1 */
    DAC_Init(DAC_Channel_1,&DAC_InitType);	/* 初始化DAC通道1 */
	
	DAC_Cmd(DAC_Channel_1, ENABLE);	/* 使能DAC1 */
	
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);	/* 12位右对齐数据格式设置DAC值 */
}

static void CD4051_Init(void)
{
	/*声明结构体变量*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开对应IO时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);
	/*配置IO*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	/* 复用推挽输出 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	/* 初始化端口B */
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	/* 初始化端口G */
}

static void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /* 时钟使能 */
	
	/* 定时器TIM3初始化 */
	TIM_TimeBaseStructure.TIM_Period = arr;	/* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; /* 设置用来作为TIMx时钟频率除数的预分频值 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	/* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	/* TIM向上计数模式 */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* 根据指定的参数初始化TIMx的时间基数单位 */
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); /* 使能指定的TIM3中断,允许更新中断 */
	
	/* 中断优先级NVIC设置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  /* TIM3中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM3_IT_PRE_PRIO;  /* 先占优先级0级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM3_IT_SUB_PRIO;  /* 从优先级4级 排在TIM2后面 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* IRQ通道被使能 */
	NVIC_Init(&NVIC_InitStructure);  /* 初始化NVIC寄存器 */
	
	TIM_Cmd(TIM3, ENABLE);  /* 使能TIMx */
}

// static void DAC_Delay_ms(unsigned long dly_ms)
// {
//   unsigned int dly_i;
//   while(dly_ms--)
//     for(dly_i=0;dly_i<8020;dly_i++);
// }

/* 定时器3中断服务程序 */
// void VREF_Scan(void)   /* TIM3中断 */
// {
// 	static int8_t   DAC_VREF_Change=0;
// 	if(DAC_VREF_Change > 2)DAC_VREF_Change=0;
// 	CD4051_Pin_B = 1;
// 	CD4051_Pin_A = 1;
// 	DAC_Delay_ms(1);
// 	DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[DAC_VREF_Change]);
// 	DAC_Delay_ms(1);
// 	CD4051_Pin_B = (DAC_VREF_Change & 0x02)>>1;
// 	CD4051_Pin_A = DAC_VREF_Change & 0x01;
// 	DAC_VREF_Change++;
// }


void vref_scan_fun(void)   /* TIM3中断 */
{
	static int32_t flag;
	static int32_t count;
	
	CD4051_Pin_B = 1; CD4051_Pin_A = 1;/* 关闭三个基准通道 */
	if(count == 0)
	{
		count = 1;
		return;
	}
	
	switch(flag)
	{
		case 0:
// 		case 1:
// 		case 3:
// 		case 4:
			if(count == 2)
			{
				CD4051_Pin_B = 0; CD4051_Pin_A = 0;/* 打开高压基准通道 */
			}
			else
			{
                DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[0]);
				count = 2;
				return;
			}
			break;
		case 1:
			if(count == 2)
			{
				CD4051_Pin_B = 0; CD4051_Pin_A = 1;/* 打开短路基准通道 */
			}
			else
			{
                DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[1]);
				count = 2;
				return;
			}
			
			break;
		case 2:
			if(count == 2)
			{
				CD4051_Pin_B = 1; CD4051_Pin_A = 0;/* 打开电弧基准通道 */
			}
			else
			{
                DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[2]);
				count = 2;
				return;
			}
			
			break;
		default:
			flag = 3;
			CD4051_Pin_B = 1; CD4051_Pin_A = 1;/* 关闭三个基准通道 */
			break;
	}
	
	count = 0;
	if(++flag > 2)
	{
		flag = 0;
	}
}
/*
 *函数介绍：DAC的定时扫描切换
 *输入参数：无
 *输出参数：无
 *备    注：无
 */
void TIM3_IRQHandler(void)   //TIM3中断
{
	OSIntEnter();
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		vref_scan_fun();
	}
	OSIntExit();
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
