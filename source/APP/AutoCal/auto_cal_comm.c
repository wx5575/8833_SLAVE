/*
 * Copyright(c) 2016,南京长盛仪器
 * All rights reserved
 * 文件名称：auto_cal_comm.c
 * 摘  要  ：自动校准系统的通信
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#include "stm32f10x.h"
#include "auto_cal_comm.h"
#include "auto_calibration.h"
#include "serve_test.h"
#include "app.h"
#include "auto_cal_crc.h"
#include "mb_cmd_define.h"
#include "string.h"
#include "auto_cal_server.h"
#include "stdio.h"




void enter_auto_cal_ui(void)
{
	g_exit = 1;
	g_return = 1;
	test_flag.into_auto_cal_ui = 1;
	app_flag.auto_cal_st = 1;//进入校准状态
    auto_cal.state = AUTOCAL_CONNECTED;
}

void exit_auto_cal_ui(void)
{
	g_exit = 1;
	g_return = 1;
	test_flag.into_test = 1;
	app_flag.auto_cal_st = 0;//退出校准状态
    auto_cal.state = AUTOCAL_WAIT_CONNECT;
}

uint8_t *get_cur_point_mode(void)
{
	uint8_t n = cal_order[g_cur_cal_opt_num].type;
	
	return cal_mode_pool[n][0];
}
uint8_t *get_cur_point_range(uint8_t *buf)
{
	sprintf((char*)buf, "%d", cal_order[g_cur_cal_opt_num].range);
	
	return buf;
}

void get_auto_cal_status(uint16_t  *data)
{
	sprintf((char*)data, "%d", auto_cal.state);
}

void auto_cal_start_output(void)
{
    auto_cal.state = AUTOCAL_OUTPUTING;
}

void auto_cal_stop_output(void)
{
    auto_cal.state = AUTOCAL_WAIT_OUTPUT;
}

void set_auto_cal_measure(uint64_t measure)
{
    auto_cal.state = AUTOCAL_SAVEDATA;
	auto_measure = measure;
}
void get_auto_cal_points(uint16_t *data)
{
	sprintf((char*)data, "%d", auto_cal.cal_total_points);
}
int16_t load_auto_cal_point(uint16_t data)
{
	if(data <= auto_cal.cal_total_points && data > 0)
	{
		g_cur_cal_opt_num = data - 1;/* 更新下当前的校准编号 */
		return 0;
	}
	
	return 1;
}


/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/
