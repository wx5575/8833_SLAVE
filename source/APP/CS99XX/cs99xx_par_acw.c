/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_acw.c
 * 摘  要  ：设置ACW的参数.
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"

void updata_ac_cur_gear_affect_par_info(uint8_t mode, uint8_t gear)
{
    gear %= AC_GEAR_END;
    
    switch(mode)
    {
        case ACW:
            menu_par[GUI_ACW_CUR_GEAR].unit[0]       = ac_gear[gear].name;
            menu_par[GUI_ACW_CUR_GEAR].unit[1]       = ac_gear[gear].name;
            
            menu_par[GUI_ACW_CUR_H].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_ACW_CUR_H].decs             = ac_gear[gear].decs;
            menu_par[GUI_ACW_CUR_H].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_ACW_CUR_H].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_ACW_CUR_H].upper            = ac_gear[gear].high_max;
            menu_par[GUI_ACW_CUR_H].lower            = ac_gear[gear].high_min;
            
            menu_par[GUI_ACW_CUR_L].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_ACW_CUR_L].decs             = ac_gear[gear].decs;
            menu_par[GUI_ACW_CUR_L].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_ACW_CUR_L].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_ACW_CUR_L].upper            = ac_gear[gear].low_max;
            menu_par[GUI_ACW_CUR_L].lower            = ac_gear[gear].low_min;
        
            menu_par[GUI_ACW_REAL_CUR].unit[0]       = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_ACW_REAL_CUR].upper_str     = ac_gear[gear].high_str;
            menu_par[GUI_ACW_REAL_CUR].lower_str     = ac_gear[gear].low_str;
            menu_par[GUI_ACW_REAL_CUR].decs          = ac_gear[gear].decs;
            menu_par[GUI_ACW_REAL_CUR].upper         = ac_gear[gear].low_max;
            menu_par[GUI_ACW_REAL_CUR].lower         = ac_gear[gear].low_min;
			
			/* 电流偏移 */
			if(g_custom_sys_par.offset_set_en)
			{
				menu_par[GUI_COM_CUR_OFFSET].unit[0]       = unit_pool[ac_gear[gear].unit];
				menu_par[GUI_COM_CUR_OFFSET].upper_str     = ac_gear[gear].high_str;
				menu_par[GUI_COM_CUR_OFFSET].lower_str     = ac_gear[gear].low_str;
				menu_par[GUI_COM_CUR_OFFSET].decs          = ac_gear[gear].decs;
				menu_par[GUI_COM_CUR_OFFSET].upper         = ac_gear[gear].low_max;
				menu_par[GUI_COM_CUR_OFFSET].lower         = ac_gear[gear].low_min;
			}
			
            break;
        case CC:
            menu_par[GUI_CC_CUR_GEAR].unit[0]       = ac_gear[gear].name;
            menu_par[GUI_CC_CUR_GEAR].unit[1]       = ac_gear[gear].name;
            
            menu_par[GUI_CC_CUR_H].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_CC_CUR_H].decs             = ac_gear[gear].decs;
            menu_par[GUI_CC_CUR_H].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_CC_CUR_H].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_CC_CUR_H].upper            = ac_gear[gear].high_max;
            menu_par[GUI_CC_CUR_H].lower            = ac_gear[gear].high_min;
            
            menu_par[GUI_CC_CUR_L].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_CC_CUR_L].decs             = ac_gear[gear].decs;
            menu_par[GUI_CC_CUR_L].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_CC_CUR_L].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_CC_CUR_L].upper            = 500;//ac_gear[gear].low_max;
            menu_par[GUI_CC_CUR_L].lower            = 0;//ac_gear[gear].low_min;
            
            menu_par[GUI_CC_REAL_CUR].unit[0]       = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_CC_REAL_CUR].upper_str     = ac_gear[gear].high_str;
            menu_par[GUI_CC_REAL_CUR].lower_str     = ac_gear[gear].low_str;
            menu_par[GUI_CC_REAL_CUR].decs          = ac_gear[gear].decs;
            menu_par[GUI_CC_REAL_CUR].upper         = ac_gear[gear].low_max;
            menu_par[GUI_CC_REAL_CUR].lower         = ac_gear[gear].low_min;
            
            break;
    }
}

void set_ac_cur_gear(void *p_node)
{
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
