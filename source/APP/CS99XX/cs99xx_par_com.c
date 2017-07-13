/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_com.c
 * 摘  要  ：设置各个模式共有的变量
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 

#include	<string.h>
#include    <stdio.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include 	"MC14094.h"
#include    "cs99xx_result.h"

void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit);
// void init_offset_par(UN_STRUCT *p);
// static DATA_INFO*  server_combination_key(const uint16_t keyvalue);


/*
 * 函数名：set_contrast
 * 描述  ：设置液晶屏的对比度
 * 输入  ：const int8_t gear 对比度的档位选择
 * 输出  ：无
 */
void set_contrast(const int8_t gear)
{
	uint8_t data = 0xbb;
	MC14094_CMD_RA(data+4*gear);
	MC14094_Updata_RA();
}
/*
 * 函数名：define_modes
 * 描述  ：确定模式 及模式的数目
 * 输入  ：无
 * 输出  ：mode_buf 放模式名 flag 放对应mode_buf的模式代号 kinds 带回模式的数目
 * 返回  ：无
 */
uint16_t define_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds)
{
	int32_t k = 0;
	
	if(MODEL_EN & __ACW)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][ACW];
		flag[k] = ACW;
	}
	if(MODEL_EN & __DCW)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][DCW];
		flag[k] = DCW;
	}
	if(MODEL_EN & __IR)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][IR];
		flag[k] = IR;
	}
	if(MODEL_EN & __GR)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][GR];
		flag[k] = GR;
	}
	if(MODEL_EN & __CC)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][CC];
		flag[k] = CC;
	}
	if(MODEL_EN & __BBD)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][BBD];
		flag[k] = BBD;
	}
    
	if(kinds != NULL)
	{
		*kinds = k;
	}
	
	mode_buf[0]=mode_buf[1];
	flag[0] = flag[1];
    
	return k;
}
uint16_t define_g_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds)
{
	int32_t k = 0;
	if(MODEL_EN & __ACW)
	{
		mode_buf[++k]="ACW";
		flag[k] = ACW;
	}
	if(MODEL_EN & __DCW)
	{
		mode_buf[++k]="DCW";
		flag[k] = DCW;
	}
	if(kinds != NULL)
	{
		*kinds = k;
	}
	mode_buf[0]=mode_buf[1];
	flag[0] = flag[1];
	return k;
}
void init_mode(NODE_STEP *p)
{
	switch(p->one_step.com.mode)
	{
		case ACW:
			init_acw_step(p);
			break;
		case DCW:
			init_dcw_step(p);
			break;
		case IR:
			init_ir_step(p);
			break;
		case GR:
			init_gr_step(p);
			break;
		case BBD:
			init_bbd_step(p);
			break;
        case CC:
            init_cc_step(p);
            break;
	}
	
	init_par_info_list(&cur_node);
	init_offset_par(p);/* 模式被修改了以后要清除偏移测试信息 */
}

void updata_title_step(UN_STRUCT *p)
{
}

void updata_step(UN_STRUCT *p)
{
}

void serve_numkey(const uint16_t keyvalue)
{
	uint8_t flag = 0;

	switch(keyvalue)
	{
		case KEY_0:
			change_data(0);
			break;
		case KEY_1:
			change_data(1);
			break;
		case KEY_2:
			change_data(2);
			break;
		case KEY_3:
			change_data(3);
			break;
		case KEY_4:
			change_data(4);
			break;
		case KEY_5:
			change_data(5);
			break;
		case KEY_6:
			change_data(6);
			break;
		case KEY_7:
			change_data(7);
			break;
		case KEY_8:
			change_data(8);
			break;
		case KEY_9:
			change_data(9);
			break;
		default:
			flag = 1;
			break;
	}

	if(flag == 0)
	{
		update_data(pmenu_data);
		shift_cursor(pmenu_data);
	}
}

DATA_INFO* find_menu(uint8_t n)
{
	uint32_t i = 0;
	DATA_INFO* p = par_info_list.head;
	
	if(n > par_info_list.tail->page)
	{
		return 0;
	}
	
	for(i=1; i < 20; i++)
	{
		if(p->page == n)
		{
			return p;
		}
		p = p->next;
	}
    
	return NULL;
}
DATA_INFO*  server_combination_key(const uint16_t keyvalue)
{
	DATA_INFO* temp = NULL;
	switch(keyvalue)
	{
		case KEY_0&KEY_1:
			app_flag.goto_menu_page = 1;
			break;
		case KEY_0&KEY_2:
			app_flag.goto_menu_page = 2;
			break;
		case KEY_0&KEY_3:
			app_flag.goto_menu_page = 3;
			break;
		case KEY_0&KEY_4:
			app_flag.goto_menu_page = 4;
			break;
		case KEY_0&KEY_5:
			app_flag.goto_menu_page = 5;
			break;
		case KEY_0&KEY_6:
			app_flag.goto_menu_page = 6;
			break;
		default:
			app_flag.goto_menu_page = 0;
			g_skip_page = 0;
			break;
	}
    
	if(app_flag.goto_menu_page)
	{
		g_skip_page = 1;
	}
    
	return temp;
}




/*
 * 函数名：set_step
 * 描述  ：显示一个菜单项的值
 * 输入  ：p_node 指向中间变量结构，当用户需要保存数据时才会更新到测试步的参数中
 * 输出  ：无
 * 返回  ：无
 */
void set_step(void *p_node)
{
}



void set_mode(void *p_node)
{
}


void init_offset_par(NODE_STEP *p)
{
	
    if(p == NULL)
    {
        return;
    }
    
	switch(p->one_step.com.mode)
	{
		case ACW:
			p->one_step.acw.offset_cur = 0;/* 偏移电流 */
			p->one_step.acw.offset_real = 0;/* 偏移真实电流 */
			p->one_step.acw.offset_result = 0;/* 偏移电流测量结果 0失败 1合格 */

			break;
		case DCW:
			p->one_step.dcw.offset_cur = 0;/* 偏移电流 */
			p->one_step.dcw.offset_result = 0;/* 偏移电流测量结果 0失败 1合格 */
			break;
		case GR:
			p->one_step.gr.offset_res = 0;/* 偏移电阻 */
			p->one_step.gr.offset_result = 0;/* 偏移电流测量结果 0失败 1合格 */
			break;
		default:
			break;
	}
}

void set_voltage(void *p_node)
{
}

void set_all_ports(void *p_node)
{
}

void set_test_time(void *p_node)
{
}

void set_either_or_par(void *p_node)
{
}

void set_float_par(void *p_node)
{
}

void set_float_par_for_syn(void *p_node)
{
}


void set_frequency(void *p_node)
{
}

void set_option_par(void *p_node)
{
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
