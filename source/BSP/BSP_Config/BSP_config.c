/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：BSP_config.c
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
/***********包含头文件 ************/
#include 	"STM32_GPIO_Config.h"
#include 	"STM32_FSMC.H"

#include	"BSP_config.h"
#include 	"GUI_Draw.h"
#include	"keyboard.h"
#include 	"MC14094.h"
#include 	"Timer_Config.h"
#include 	"STM32_ADC.H"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include	"i2c_ee.h"
#include	"cs99xx_it.h"
#include    "Timer5_Config.h"
#include    "serve_test.h"
#include    "cs99xx_update.h"
#include    "spi_flash.h"
#include    "USART3.H"
#include    "SPI_FPGA.H"



/*********** 函数定义 *************/
/*
 * 函数名：init_gpio_switch_hv
 * 描述  ：初始化高压开关IO
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_gpio_switch_hv(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/************************** 输入 ***************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 上拉输入 */
	
	/* 中断引脚配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*
 * 函数名：on_switch_hv
 * 描述  ：打开高压开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void on_switch_hv(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_0);
}
/*
 * 函数名：off_switch_hv
 * 描述  ：关闭高压开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void off_switch_hv(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}
/*
 * 函数名：CS99XX_NVIC_Group_Init
 * 描述  ：初始化系统中断分组，在程序中只需调用1次
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_cs99xx_nvic_group(void)
{
	NVIC_PriorityGroupConfig(CS99XX_IT_GROUP);
}
void set_vector_table(void)
{
#ifndef BOOT_LOADER
	NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0);//设置存入寄存器及地址偏移量
#endif
}
/*
 * 函数名：init_4094
 * 描述  ：4094配置
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_4094(void)
{
	MC14094_Init(); /*  */
}

/*
 * 函数名：bsp_init
 * 描述  ：硬件初始化
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void  bsp_init(void)
{
    set_vector_table();/* 中断向量表初始化 */
	init_cs99xx_nvic_group();/* 初始化NVIC分组 */
//	FSMC_Configuration();/* FSMC初始化 */
//	GUI_Init();			/* GUI 初始化 */
    
	GPIO_Configuration();	/* LED 中断引脚 宏定义 */
    
//    CRC_Config();/* crc 配置 */
    
	init_exit();/* 初始化外部中断 */
    
	TIM2_it_init((50-1), 7200-1);/* 0.1*50=5ms 1000Hz 0.1 * 1000 = 100ms */
    
    init_timer5_callback(serve_test_poll_task);/* 定时器回调函数 */
    TIM5_it_init((10-1), 7200-1);/* 0.1ms*10=1ms 1000Hz */
    
	init_keyboard();/* 按键初始化 */
	
	dac_vref_init();/* DAC初始化 */
	
	init_spi_cpld();/* 初始化与cpld通信引脚 */
	
#if DMA_FOR_ADC_EN>0
	ADC_DMA_Config();/* DMA通信 */
#else
	ADC_Config();/* ADC普通配置 */
#endif
    
    init_flash();/* 初始化flash */
    I2C_EE_Init();/* eep初始化 */
    
    init_gpio_switch_hv();/* 初始化高压开关GPIO */
    
	systick_init();		/* 嘀嗒时钟配置 */
    
    usart3_config(115200 * 1);
    init_spi_fpga_gpio();
}

/*
 * 函数名：systick_init
 * 描述  ：系统滴答定时器初始化
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void systick_init(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);//1ms 1000Hz
}
/*
 * 函数名：CRC_Config
 * 描述  ：使用硬件CRC需要配置启动时钟
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void CRC_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}

/*
 * 函数名：soft_delay_us
 * 描述  ：软件阻塞延时10us 个数
 * 输入  ：dly_us 个数
 * 输出  ：无
 * 返回  ：无
 */
void cs_delay_us(int time)
{
	u16 i = 0;
	
	while(time--)
	{
		i = 10; //自己定义
		
		while(i--);
	}
}
void cs_delay_ms(u16 time)
{
	u16 i = 0;
	
	while(time--)
	{
		i = 12000;  //自己定义
		while(i--);
	}
}
void soft_delay_us(u32 dly_us)
{
	unsigned int dly_i;
	while(dly_us--)
	{
		for(dly_i=0;dly_i<802;dly_i++);
	}
}
void soft_delay_10us(u32 dly_us)
{
    soft_delay_us(dly_us);
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
