/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_dcw.c
 * 摘  要  ：设置DCW的参数函数
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

void updata_dc_cur_gear_affect_par_info(uint8_t mode, uint8_t gear)
{
    gear %= DC_GEAR_END;
    
    switch(mode)
    {
        case DCW:
            menu_par[GUI_DCW_CUR_GEAR].unit[0]      = dc_gear[gear].name;
            menu_par[GUI_DCW_CUR_GEAR].unit[1]      = dc_gear[gear].name;
            
            menu_par[GUI_DCW_CUR_H].unit[0]         = unit_pool[dc_gear[gear].unit];
            menu_par[GUI_DCW_CUR_H].decs            = dc_gear[gear].decs;
            menu_par[GUI_DCW_CUR_H].upper_str       = dc_gear[gear].high_str;
            menu_par[GUI_DCW_CUR_H].lower_str       = dc_gear[gear].low_str;
            menu_par[GUI_DCW_CUR_H].upper           = dc_gear[gear].high_max;
            menu_par[GUI_DCW_CUR_H].lower           = dc_gear[gear].high_min;
            
            menu_par[GUI_DCW_CUR_L].unit[0]         = unit_pool[dc_gear[gear].unit];
            menu_par[GUI_DCW_CUR_L].decs            = dc_gear[gear].decs;
            menu_par[GUI_DCW_CUR_L].upper_str       = dc_gear[gear].high_str;
            menu_par[GUI_DCW_CUR_H].lower_str       = dc_gear[gear].low_str;
            menu_par[GUI_DCW_CUR_H].upper           = dc_gear[gear].low_max;
            menu_par[GUI_DCW_CUR_L].lower           = dc_gear[gear].low_min;
            
            menu_par[GUI_DCW_CHARGE_CUR].unit[0]    = unit_pool[dc_gear[gear].unit];
            menu_par[GUI_DCW_CHARGE_CUR].decs       = dc_gear[gear].decs;
            menu_par[GUI_DCW_CHARGE_CUR].upper_str  = dc_gear[gear].high_str;
            menu_par[GUI_DCW_CHARGE_CUR].lower_str  = dc_gear[gear].low_str;
            menu_par[GUI_DCW_CHARGE_CUR].upper      = dc_gear[gear].low_max;
            menu_par[GUI_DCW_CHARGE_CUR].lower      = dc_gear[gear].low_min;
			
			/* 电流偏移 */
			if(g_custom_sys_par.offset_set_en)
			{
				menu_par[GUI_COM_CUR_OFFSET].unit[0]         = unit_pool[dc_gear[gear].unit];
				menu_par[GUI_COM_CUR_OFFSET].decs            = dc_gear[gear].decs;
				menu_par[GUI_COM_CUR_OFFSET].upper_str       = dc_gear[gear].high_str;
				menu_par[GUI_COM_CUR_OFFSET].lower_str       = dc_gear[gear].low_str;
				menu_par[GUI_COM_CUR_OFFSET].upper           = dc_gear[gear].low_max;
				menu_par[GUI_COM_CUR_OFFSET].lower           = dc_gear[gear].low_min;
			}
			
            break;
    }
}

void set_dcw_cur_gear(void *p_node)
{
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
