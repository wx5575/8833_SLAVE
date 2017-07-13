/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_gr.c
 * 摘  要  ：设置GR的参数函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
#include	<string.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"

void dir_gr_range_str(uint32_t v_h, uint32_t v_l)
{
}
void set_gr_output_cur(void *p_node)
{
}
void set_gr_res_upper(void *p_node)
{
}

void set_gr_res_lower(void *p_node)
{
}



void set_gr_vol_upper(void *p_node)
{
}
void set_gr_vol_lower(void *p_node)
{
}

void updata_gr_cur_affect_par_info(UN_STRUCT *p)
{
    menu_par[GUI_GR_OUTPUT_CUR].upper = GR_CUR_H;
    menu_par[GUI_GR_OUTPUT_CUR].lower = GR_CUR_L;
}

void updata_gr_vol_gear_affect_par_info(UN_STRUCT *p)
{
	uint8_t temp = *(uint8_t*)menu_par[GUI_GR_TEST_METHOD].pData;
	switch(temp)
	{
		case 1:
			menu_par[GUI_GR_TEST_METHOD].unit[0] = "连续";
			menu_par[GUI_GR_TEST_METHOD].unit[1] = "Cont.";
			menu_par[GUI_GR_TEST_METHOD].unit[2] = "脉冲";
			menu_par[GUI_GR_TEST_METHOD].unit[3] = "Pulse";
			break;
		case 0:
			menu_par[GUI_GR_TEST_METHOD].unit[0] = "脉冲";
			menu_par[GUI_GR_TEST_METHOD].unit[1] = "Pulse";
			menu_par[GUI_GR_TEST_METHOD].unit[2] = "连续";
			menu_par[GUI_GR_TEST_METHOD].unit[3] = "Cont.";
			break;
	}
	
	menu_par[GUI_COM_MODE].unit[0] = "GR";
	menu_par[GUI_COM_MODE].unit[1] = "GR";
    
	switch(p->gr.voltage_gear)
	{
		case 0:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "20mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "20mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper_str = "20.00mV";
			menu_par[GUI_GR_VOL_H].lower_str = "00.01mV";
			menu_par[GUI_GR_VOL_L].upper_str = "00.00mV";
			menu_par[GUI_GR_VOL_L].lower_str = "00.00mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 2;
			menu_par[GUI_GR_VOL_L].decs = 2;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			break;
        }
		case 1:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "20mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "20mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper		= 2000;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "20.00mV";
			menu_par[GUI_GR_VOL_H].lower_str = "00.01mV";
			menu_par[GUI_GR_VOL_L].upper_str = "00.00mV";
			menu_par[GUI_GR_VOL_L].lower_str = "00.00mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 2;
			menu_par[GUI_GR_VOL_L].decs = 2;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			break;
        }
		case 2:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "200mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "200mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper		= 2000;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "200.0mV";
			menu_par[GUI_GR_VOL_H].lower_str = "000.1mV";
			menu_par[GUI_GR_VOL_L].upper_str = "000.0mV";
			menu_par[GUI_GR_VOL_L].lower_str = "000.0mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 1;
			menu_par[GUI_GR_VOL_L].decs = 1;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			break;
        }
		case 3:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "2000mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "2000mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper		= 2000;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "2000mV";
			menu_par[GUI_GR_VOL_H].lower_str = "0001mV";
			menu_par[GUI_GR_VOL_L].upper_str = "0000mV";
			menu_par[GUI_GR_VOL_L].lower_str = "0000mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 0;
			menu_par[GUI_GR_VOL_L].decs = 0;
			menu_par[GUI_GR_VOL_H].lon = 4;
			menu_par[GUI_GR_VOL_L].lon = 4;
			menu_par[GUI_GR_VOL_H].type = 0;
			menu_par[GUI_GR_VOL_L].type = 0;
			break;
        }
		case 4:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "12V";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "12V";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "mA";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 1;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "500.0mA";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "100.0mA";
			
			menu_par[GUI_GR_VOL_H].upper		= 1200;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "12.00V";
			menu_par[GUI_GR_VOL_H].lower_str = "00.01V";
			menu_par[GUI_GR_VOL_L].upper_str = "12.00V";
			menu_par[GUI_GR_VOL_L].lower_str = "00.10V";
			menu_par[GUI_GR_VOL_H].unit[0] = "V";
			menu_par[GUI_GR_VOL_L].unit[0] = "V";
			menu_par[GUI_GR_VOL_H].decs = 2;
			menu_par[GUI_GR_VOL_L].decs = 2;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			
			menu_par[GUI_COM_TEST_T].upper_str = "999.9s";
			menu_par[GUI_COM_TEST_T].lower_str = "0,0.4s";
			menu_par[GUI_COM_TEST_T].lower = 4;
			break;
        }
	}
}

void set_gr_vol_gear(void *p_node)
{
}

void set_gr_test_method(void *p_node)
{
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
