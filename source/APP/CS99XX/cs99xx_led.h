/*
 * Copyright(c) 2016,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_led.h
 * 摘  要  ：主程序
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __CS99XX_LED_H__
#define __CS99XX_LED_H__

enum{
    LED_ST_PASS,
    LED_ST_PASS_OFF,
    LED_ST_FAIL,
    LED_ST_RESET,
};

extern void led_cmd(uint8_t st);

#endif //__CS99XX_LED_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
