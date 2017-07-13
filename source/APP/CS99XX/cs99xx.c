/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx.c
 * 摘  要  ：服务于整个测试程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

/****************** 包含头文件 **************************/
#include "cs99xx_config.h"


#include	"SPI_CPLD.h"
#include    "cs99xx_result.h"
#include 	"serve_test.h"
#include	"cs99xx_collect.h"
#include    "cs99xx_type.h"
#include    "cs99xx_relay_motion.h"
#include    <string.h>

/************************* 初始化全局标志 *************************/

/************************* 函数定义 *************************/


/*
 * 函数名：updata_time
 * 描述  ：更新测试显示时间
 * 输入  ：n 标记当前是偏移还是测试 0 测试 1偏移
 * 输出  ：无
 * 返回  ：无
 */
void updata_time(const int8_t n, uint16_t time)
{
    transform_test_time_string(time);
}

/*
 * 函数名：test_dis
 * 描述  ：服务于测试程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_dis(void)
{
    /* IR自动换挡 */
    if(cur_mode == IR)
    {
        ir_auto_find_gear();
    }
    
    /* 更新结果 */
    if(test_flag.gradation == STAGE_TEST)
    {
        if(tes_t && ((g_dis_time + 5) > tes_t))
        {
            updata_result(cur_mode);
        }
    }
    
    /* 显示限速 */
    if(test_flag.vol_change_flag == 0)
    {
        /* 在刚开始的时候要快速显示数据不能限速 */
        if(DIS_SPEED < 20)
        {
            DIS_SPEED++;
        }
        /* 控制显示的速度 */
        else
        {
            if(++DIS_SPEED > 38)
            {
                DIS_SPEED = 28;
            }
            else
            {
                return;
            }
        }
    }
    
	test_flag.re_times_per_second++;
    transform_test_vol_string();
    transform_test_loop_string();
	if(ERR_NUM)
	{
        disable_sample_task();/* 关闭采样任务 */
    }
	
	return;
}

/*
 * 函数名：offset_dis
 * 描述  ：服务于偏移测试
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
int8_t offset_dis(const int8_t n)
{
	int8_t err_num = 0;
    
    if(OFFSET_BBD == TEST_BBD)
    {
        bbd_auto_select_gear();
    }
    
    transform_test_vol_string();
    transform_test_loop_string();
	return err_num;
}

void dis_test_ui(void)
{
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
