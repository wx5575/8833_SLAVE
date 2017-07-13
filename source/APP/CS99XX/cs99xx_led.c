/*
 * Copyright(c) 2016,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_led.c
 * 摘  要  ：主程序
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include "stm32f10x.h"
#include "cs99xx_led.h"
#include "STM32_GPIO_Config.h"

void led_cmd(uint8_t st)
{
    switch(st)
    {
        case LED_ST_PASS:
            LED_TEST = LED_OFF;/* 关闭测试灯 */
            LED_PASS = LED_ON;/* 打开PASS灯 */
            break;
        case LED_ST_PASS_OFF:
            
            break;
        case LED_ST_FAIL:
            
            break;
        case LED_ST_RESET:
            
            break;
    }
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/

