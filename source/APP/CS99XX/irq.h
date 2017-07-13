/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：irq.h
 * 摘  要  ：中断服务程序接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef	__IRQ_H__
#define	__IRQ_H__

void gfi_cycle_clear_count(void);


void stop_irq(void);
void arc_irq(void);
void short_irq(void);
void gfi_irq(void);

void uart5_irq_for_syn_roads(void);
void uart5_irq_for_dc_gr(void);

#endif //__IRQ_H__
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
