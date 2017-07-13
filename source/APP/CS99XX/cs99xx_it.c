/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_it.c
 * 摘  要  ：初始化外部中断并为这些中断提供管理接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include "cs99xx_it.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "BSP_config.h"
#include "stm32f10x.h"
#include "STM32_ADC.H"


/*
 * 函数名：init_exit
 * 描述  ：外部中断配置
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_exit(void)
{
	/*************************    定义初始化结构变量    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //定义数据结构的变量
	NVIC_InitTypeDef NVIC_InitStructure;//定义数据结构的变量 //选择优先组
	
	/*************************    开启外设时钟    *************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD
							| RCC_APB2Periph_GPIOE, ENABLE);  //允许GPIOB、GPIOE时钟
	
	/*************** 引脚配置 *********************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 //上拉输入
	
	/* STOP按键，AMP,SHORT,ARC,GFI输入引脚配置 */
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/**************** 中断向量设置 ****************/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=EXIT_IT_PRE_PRIO;    //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //启动此通道的中断
	
	/*************** 外部中断优先级配置 ************/
	/* STOP--KEY PA6  AMP PA9 */
	NVIC_InitStructure.NVIC_IRQChannel=(EXTI9_5_IRQn);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=STOP_IT_SUB_PRIO;
	NVIC_Init(&NVIC_InitStructure);
	
	/* SHORT,ARC,GFI * PA10,PA11,PA12 */
	NVIC_InitStructure.NVIC_IRQChannel=(EXTI15_10_IRQn);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=SAG_IT_SUB_PRIO;
	NVIC_Init(&NVIC_InitStructure);
	
	/**************** 外部中断配置 ****************/
	EXTI_DeInit();//初始化为缺省值
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//选择下降_Falling或上升_Rising沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);  //管脚选择
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//将以上入口放在EXTI_Line5这条线上
	EXTI_Init(&EXTI_InitStructure);
	
	/* Place protection exti lines */
	/* PA9功放报警改用查询的方法 */
	// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);  //管脚选择
	// 	EXTI_InitStructure.EXTI_Line = EXTI_Line9;//将以上入口放在EXTI_Linex这条线上
	// 	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);  //管脚选择
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;//将以上入口放在EXTI_Linex这条线上
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);  //管脚选择
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;//将以上入口放在EXTI_Linex这条线上
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);  //管脚选择
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;//将以上入口放在EXTI_Linex这条线上
	EXTI_Init(&EXTI_InitStructure);
	
	/* 关中断 */
	STOP_INT(DISABLE);	/* 复位中断 */
	SHORT_INT(DISABLE);	/* 短路中断 */
	GFI_INT(DISABLE);	/* GFI 中断 */
	ARC_INT(DISABLE);	/* ARC 中断 */
}
/*
 * 函数名：exit_sw
 * 描述  ：测试时外部中断的切换开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void exit_sw(void)
{
    exit_off();
	/********************************* 开中断 ****************************/
	RTC_INI(DISABLE);
	/* 判断是否打开GFI中断  接地时关闭浮地时打开 */
	if(sys_par.test_method == FLOAT_MODE && sys_par.is_gfi_protect == GFI_ON)
	{
		GFI_INT(ENABLE);
	}
	else
	{
		GFI_INT(DISABLE);
	}
	
    STOP_INT(ENABLE);	/* 复位中断 */
    
	ADC_WatchdogOFF();
	
	switch(cur_mode)
	{
		case ACW:
			SHORT_INT(ENABLE);	/* 短路中断 */
			
			/* 电弧侦测 */
			if(cur_arc_gear)
			{
				ARC_INT(ENABLE);
			}
			else
			{
				ARC_INT(DISABLE);
			}
			
			ADC_WatchdogON();
			break;
		case DCW:
			STOP_INT(ENABLE);	/* 复位中断 */
			SHORT_INT(ENABLE);	/* 短路中断 */
			
			/* 电弧侦测 */
			if(cur_arc_gear)
			{
				ARC_INT(ENABLE);
			}
			else
			{
				ARC_INT(DISABLE);
			}
			/* 直流耐压 电弧侦测关闭 触发电弧中断作短路处理 */
// 			ARC_INT(ENABLE);
			
			ADC_WatchdogON();
			break;
		case IR:
			STOP_INT(ENABLE);	/* 复位中断 */
            
			if(type_spe.ir_short_int_en == ENABLE)
			{
				SHORT_INT(ENABLE);	/* 短路中断 */
			}
			else
			{
				SHORT_INT(DISABLE);	/* 短路中断 */
			}
			
			ARC_INT(DISABLE);	/* 电弧侦测 */
			break;
		case GR:
			STOP_INT(ENABLE);	/* 复位中断 */
			SHORT_INT(DISABLE);	/* 短路中断 */
			ARC_INT(DISABLE);	/* 电弧侦测 */
			break;
		case BBD:
			STOP_INT(ENABLE);	/* 复位中断 */
			SHORT_INT(DISABLE);	/* 短路中断 */
			ARC_INT(DISABLE);	/* 电弧侦测 */
			break;
        case CC:
			STOP_INT(ENABLE);	/* 复位中断 */
			SHORT_INT(DISABLE);	/* 短路中断 */
			ARC_INT(DISABLE);	/* 电弧侦测 */
            break;
	}
}

/*
 * 函数名：exit_off
 * 描述  ：关闭外部中断
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void exit_off(void)
{
	ARC_INT(DISABLE);	/* 关电弧侦测 */
	STOP_INT(DISABLE);  /* 关闭复位中断 */
	GFI_INT(DISABLE);   /* 关闭GFI中断 */
}
/*
 * 函数名：exit_off_all
 * 描述  ：关闭全部外部中断
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void exit_off_all(void)
{
	ARC_INT(DISABLE);	/* 关电弧侦测 */
	SHORT_INT(DISABLE);  /* 关闭复位中断 */
	GFI_INT(DISABLE);   /* 关闭GFI中断 */
}
/*
 * 函数名：disable_stop_exit
 * 描述  ：关闭外部中断
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void disable_stop_exit(void)
{
	STOP_INT(DISABLE);  /* 关闭复位中断 */
}
/*
 * 函数名：enable_stop_exit
 * 描述  ：关闭外部中断
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void enable_stop_exit(void)
{
	STOP_INT(ENABLE);  /* 打开复位中断 */
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
