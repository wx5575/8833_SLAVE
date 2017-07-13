/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：ADC_DMA.c
 * 摘  要  ：用DMA实现AD采样
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#include	<stdio.h>
#include    "ADC_DMA.h"
#include 	"STM32_ADC.H"
#include 	"serve_test.h"

#define ADC1_DR_Address ((uint32_t)0x4001244C)//定义硬件ADC1的物理地址

__IO uint32_t ADCConvertedValue[2];//转换的5通道AD值
__IO uint16_t ADCTem;
__IO uint16_t ADC_VOL;
__IO uint16_t ADC_CUR;
__IO uint16_t dma_buf[ADC_CHANNLE*ADC_BUF_DEEP];
/**********************************************************
** 函数名: void All_GPIO_Config(void)
** 功能描述: 所有的GPIO口配置放在这里
** 输入参数: 无
** 输出参数: 无
***********************************************************/
// void All_GPIO_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO结构体
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
// 	/*将PB0配置为推挽输出*/
// 	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;//PB0
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHz时钟速度
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// }
/**********************************************************
** 函数名: ADC1_DMA_Config
** 功能描述: ADC1的DMA方式配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void ADC1_DMA_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;//定义ADC结构体
	DMA_InitTypeDef DMA_InitStructure;//定义DMA结构体
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1| RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE ); //使能ADC1及GPIOA时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);/* 很重要 */
	
	/*作为ADC1的6通道模拟输入的GPIO初始化配置*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//Pc2,3配置为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*DMA1的通道1配置*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (ADC1_BASE + 0x4c);//传输的源头地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&dma_buf;//目标地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设作源头
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNLE*ADC_BUF_DEEP;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设传输以半字为单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存以 半字为单位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4优先级之一的(高优先)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //非内存到内存
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根据以上参数初始化DMA_InitStructure
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能DMA1的通道1
	
	/*下面为ADC1的配置*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//模数转换工作在扫描模式（多通道）
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//模数转换工作在连续模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;//转换的ADC通道的数目为2
	
	ADC_Init(ADC1, &ADC_InitStructure);//要把以下参数初始化ADC_InitStructure
	
	/* 设置ADC1的3个规则组通道，设置它们的转化顺序和采样时间*/
	//转换时间Tconv=采样时间+12.5个周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_239Cycles5); //ADC1通道0转换顺序为1，采样时间为7.5个周期 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);/*使能ADC1的DMA传输方式*/
	
	ADC_Cmd(ADC1, ENABLE);/*使能ADC1 */
	
	ADC_ResetCalibration(ADC1);/*重置ADC1的校准寄存器 */
	
	while(ADC_GetResetCalibrationStatus(ADC1));/*获取ADC重置校准寄存器的状态*/
	ADC_StartCalibration(ADC1); /*开始校准ADC1*/
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能ADC1软件转换
}

void ADC_DMA_Config(void)
{
	ADC1_DMA_Config();
}
/* 冒泡排序 */
void bubble_sort (uint16_t data[], size_t size) 
{
	uint32_t i = 0;
	
	for (i = 0; i < size - 1; ++i) 
	{
		int ordered = 1;
		uint32_t j;
		
		for (j = 0; j < size - 1 - i; ++j)
		{
			if (data[j+1] < data[j]) 
			{
				int swap = data[j];
				data[j] = data[j+1];
				data[j+1] = swap;
				ordered = 0;
			}
		}
		
		if(ordered)
		{
			break;
		}
	}
}

void read_adc_from_dma(void)
{
#define DEEP 31
    
    int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t sum[2] = {0};
	static uint16_t temp_buf[ADC_CHANNLE][DEEP];
	static uint16_t adc_buf[ADC_CHANNLE][ADC_BUF_DEEP];
	
	for(k = 0; k < DEEP; k++)
	{
		for(j = 0; j < ADC_CHANNLE; j++)
		{
			for(i = 0; i<ADC_BUF_DEEP; i++)
			{
				adc_buf[j][i] = dma_buf[i*ADC_CHANNLE+j];
			}
		}
		
		/* 中值滤波 */
		for(i = 0; i < ADC_CHANNLE; i++)
		{
			bubble_sort(adc_buf[i], ADC_BUF_DEEP);
			temp_buf[i][k] = adc_buf[i][ADC_BUF_DEEP/2];
		}
	}
	
	/* 均值滤波 */
	for(i=0; i<ADC_CHANNLE; i++)
	{
		for(j = 0; j < DEEP; j++)
		{
			sum[i]+=temp_buf[i][j];
		}
		adc_value[i] = sum[i] / DEEP;
	}
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
