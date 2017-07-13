/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：ADC_DMA.h
 * 摘  要  ：用DMA实现AD采样
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __ADC__DMA__INCLUDED__
#define __ADC__DMA__INCLUDED__

#include "stm32f10x.h"

#define ADC_BUF_DEEP		10
#define ADC_CHANNLE			2

extern __IO uint32_t ADCConvertedValue[2];
extern __IO uint16_t ADCTem;
extern __IO uint16_t ADC_VOL;
extern __IO uint16_t ADC_CUR;

extern __IO uint16_t dma_buf[ADC_CHANNLE*ADC_BUF_DEEP];

extern void ADC_DMA_Config(void);
extern void read_adc_from_dma(void);
extern void adc_sample(void);

extern void on_dma(void);
extern void off_dma(void);

#endif 

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
