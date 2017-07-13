/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_bbd.c
 * 摘  要  ：bbd更能
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
#include	"calibration.h"
/*
 * 函数名：bbd_dis_step
 * 描述  ：画出一步的数据信息
 * 输入  ：p 传入当前步的指针
 * 输出  ：无
 * 返回  ：无
 */
//static void bbd_dis_step(const uint8_t n)
//{
//}


/*
 * 函数名：offset_manual
 * 描述  ：对偏移测试进行细节说明
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
//static void bbd_manual(void)
//{
//}
/*
 * 函数名：serve_offset_shortcut
 * 描述  ：服务与偏移测试的更名键功能
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
//static void serve_bbd_shortcut(void)
//{
//}
/*
 * 函数名：dis_one_step
 * 描述  ：画出一步的偏移测试信息
 * 输入  ：p 传入当前步的指针 ， n 位置信息
 * 输出  ：无
 * 返回  ：无
 */
//static void dis_one_bbd_step(const UN_STRUCT *p, const int8_t n)
//{
//}

/*
 * 函数名：draw_one_page
 * 描述  ：画出一页的偏移测试信息
 * 输入  ：p 传入当前页的第一个步的指针
 * 输出  ：无
 * 返回  ：无
 */
//static void draw_bbd_one_page(NODE_STEP *p)
//{
//}
void clear_bbd_ok_flag(void)
{
	NODE_STEP *p_bak = g_cur_step;
	
	for(;g_cur_step;g_cur_step = g_cur_step->next)
	{
		if(g_cur_step->one_step.com.mode == BBD)
		{
			g_cur_step->one_step.bbd.get_cs_ok = 0;
		}
	}
    
	g_cur_step = p_bak;
}

void update_cap_limit(void)
{
	if(cap_ave * 5 < 10)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 1000 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 1000 * 0.8;
	}
	else if(cap_ave * 5 < 100)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 100 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 100 * 0.8;
	}
	else if(cap_ave * 5 < 1000)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 10 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 10 * 0.8;
	}
	else
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 1 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 1 * 0.8;
	}
}
/*
 * 函数名：sys_bbd
 * 描述  ：获取开路短路检测的容值
 * 输入  ：const int8_t w 标记调用函数
 * 输出  ：无
 * 返回  ：无
 */
void sys_bbd(uint32_t w)
{
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
