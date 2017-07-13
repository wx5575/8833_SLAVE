/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_it.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CS99XX_EXTI__
#define __CS99XX_EXTI__

/******************** 加载文件 ************************/
#include <stm32f10x.h>

/********************  函数声明 ************************/
extern void init_exit(void);
extern void exit_sw(void);
extern void exit_off(void);
extern void exit_off_all(void);
extern void disable_stop_exit(void);
extern void enable_stop_exit(void);

#endif //__CS99XX_EXTI__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
