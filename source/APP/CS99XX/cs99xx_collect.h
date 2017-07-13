/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_vref.c
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CS99XX_COLLECT_H__
#define __CS99XX_COLLECT_H__

#include "stm32f10x.h"

extern void clear_slither_data(void);
extern int8_t judge_err(void);
extern void adc_sample(void);
extern void set_dc_gr_open_err(void);
extern uint8_t get_dc_gr_open_err(void);
extern void set_dc_gr_amp_err(void);
extern uint8_t get_dc_gr_amp_err(void);
extern void clear_slither_data(void);

#endif //__CS99XX_COLLECT_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
