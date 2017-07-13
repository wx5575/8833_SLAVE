/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：STM32_ADC.c
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：孙世成
 * 历史版本：
 * 修改记录：2014.11.21 修改人：王鑫 
 *
 */

#include "STM32_ADC.H"
#include "serve_test.h"

u16 ADC_Voltage,ADC_Current;



void NVIC_Config_For_ADC_Watchdog(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_Init(&NVIC_InitStructure);
}
void ADC_WatchdogConfig(uint16_t HighThreshold)
{
    ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_13);
    ADC_AnalogWatchdogThresholdsConfig(ADC1, HighThreshold, 0);
    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
    
    NVIC_Config_For_ADC_Watchdog();
}

void ADC_WatchdogON(void)
{
//     ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
}
void ADC_WatchdogOFF(void)
{
    ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
}

/*
 * 函数名：ADC_Config
 * 描述  ：配置ADC
 * 输入  ：无
 * 输出  ：无
 */
void ADC_Config(void)
{
	ADC_InitTypeDef 	ADC_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	//PA1 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
    ADC_WatchdogConfig(4095);
}


void ADC1_2_IRQHandler(void)
{
	uint16_t ad = 0;
	
    ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
    
    if(SET == ADC_GetFlagStatus(ADC1, ADC_FLAG_AWD))
    {
        ADC_ClearFlag(ADC1, ADC_FLAG_AWD);
        ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		
		ad = Get_Adc(13);
		
		if(ad > g_ad_dog + 8)
		{
			ad = g_ad_dog + 3;
		}
		
		cur_ave = ad * cur_adc_cur_k;
		irq_stop_relay_motion();
		record_exception_scene();/* 记录异常现场 */
		ERR_NUM = ERR_HIGH;
		ADC_WatchdogOFF();
    }
    
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
}
//获得ADC值
u16 Get_Adc(u8 ch)
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}


#if DMA_FOR_ADC_EN==0
u16 Get_Adc_Average(u8 ch,u8 times)
{
#define DEEP	51
	u32 sum=0;
	u16 buf[DEEP] = {0};
	uint16_t temp = 0;
	u8 i,j,k;
	u8 N = DEEP;
	
	i = j = k = 0;
	for(k=0;k<times;k++)
	{
		/* 采集 */
		for(i=0;i<N;i++)
		{
			u8 t;
			buf[i]=Get_Adc(ch);
			for(t=0;t<2;t++);
		}
		/* 中值滤波 */
		for(j=N-1;j>0;j--)
		{
			for(i=0;i<j;i++)
			{
				if(buf[i]>buf[i+1])
				{
					temp	 = buf[i];
					buf[i]	 = buf[i+1];
					buf[i+1] = temp;
				}
			}
		}
		sum+=buf[N/2];
	}
	/* 均值滤波 */
	return sum/times;
}
#endif
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
