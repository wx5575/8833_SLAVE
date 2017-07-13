
#include "stm32f10x.h"
#include "cs99xx_buzzer.h"
#include "app.h"
#include "STM32_GPIO_Config.h"
#include "serve_test.h"



/*
 *函数名：buzzer_flicker
 *描述：蜂鸣器闪响
 *输入：无
 *输出 :无
 *返回：无
*/
void buzzer_flicker(void)
{
    static int n = 0;
    
    if(app_flag.buzzer_flicker_en)
    {
        if(n == 80)
        {
            BUZZER = BUZZER_ON;
        }
        
        if(n == 200)
        {
            BUZZER = BUZZER_OFF;
        }
        
        if(++n > 200)
        {
            n = 0;
        }
    }
}

/*
 *函数名：buzzer_delay_off
 *描述：延时关闭蜂鸣器
 *输入：无
 *输出 :无
 *返回：无
*/
void buzzer_delay_off(void)
{
    if(g_buzzer_time)
    {
        if(0 == --g_buzzer_time)/* 蜂鸣器定时器 */
        {
            BUZZER = BUZZER_OFF;
        }
    }
}

