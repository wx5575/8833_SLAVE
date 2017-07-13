/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_exception.c
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include    "cs99xx_exception.h"
#include	<string.h>
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"cs99xx_it.h"
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_result.h"
#include 	"cs99xx_plc.h"



static void fail_mode_stop_dispose(void)
{
    LED_TEST = LED_OFF;
	LED_FAIL = LED_ON;/* 点亮FAIL灯 */
    
    if(BUZZER_EN)
    {
        BUZZER_SOUND(0);/* 不受系统开关控制 */
    }
    
	
    while(1)
    {
		if(RESET == STOP_PIN || TERMINATE)/* 按下复位键才能退出 */
		{
            app_flag.buzzer_flicker_en = 0;
			LED_FAIL = 0;
			STOP = 1;
			g_return = 1;
			LED_FAIL = LED_OFF;/* 熄灭FAIL灯 */
            if(BUZZER_EN)
            {
                BUZZER = BUZZER_OFF;/* 不受系统开关控制 */
            }
			return;
		}
        
        OSTimeDlyHMSM(0,0,0,10);
    }
}

static void fail_mode_halt_dispose(void)
{
    fail_mode_stop_dispose();
    
    /* 如果当前步不是最后一步下次启动就会从下一步开始 */
    if(cur_step != g_cur_file->total)
    {
        NEXT  = 1;
    }
}

static void fail_mode_con_dispose(void)
{
    if(g_cur_step->next == NULL)
    {
        OVER = 1;
        if(sys_par.plc_signal != EACH_STEP)
        {
            return;/* 返回去显示失败继续 */
        }
    }
    
	/* 失败继续并且 当前步打开了步间连续 工作模式为N */
	if(g_cur_step->next != NULL && steps_con && (g_cur_file->work_mode == N_MODE))
	{
		if(tes_t == 0)
		{
			FAIL = 1;/* 测试失败 */
			
			BUZZER_SOUND(300);/* 蜂鸣300ms */
			LED_FAIL = LED_ON;
            LED_TEST = LED_OFF;
			
            /* 当前步间隔时间为0 */
			if(int_t == 0)
			{
				stop_test();
				OSTimeDlyHMSM(0,0,0,800);
				g_test_time = 0;
                
                CONT = 1;
			}
            /* 间隔时间不为0 进入间隔时间 */
			else
            {
                close_test_timer();/* 关定时器 */
                g_test_time = thr_t + 1;
                open_test_timer();/* 开定时器 */
            }
			return;
		}
		else
		{
			FAIL = 1;/* 测试失败 */
// 			if(g_cur_step->next == NULL)
// 			{
// 				OSTimeDlyHMSM(0,0,0,100);
// 				OVER = 1;
// 				return;
// 			}
			
			close_test_timer();/* 关定时器 */
			BUZZER_SOUND(100);/* 开机蜂鸣100ms */
			LED_FAIL = LED_ON;
			OSTimeDlyHMSM(0,0,0,800);
            
			g_test_time = thr_t + 1;/* 进入间隔等待 */
			exit_sw();/* 打开外部中断 */
			open_test_timer();/* 开定时器 */
			return;
		}
	}
    /* 停止测试 */
    else
    {
        fail_mode_stop_dispose();
    }
}
static void fail_mode_restart_dispose(void)
{
    LED_TEST = LED_OFF;
	LED_FAIL = LED_ON;/* 点亮FAIL灯 */
    if(BUZZER_EN)
    {
        BUZZER_SOUND(0);/* 不受系统开关控制 */
    }
    
	
    while(1)
    {
        if(RESET == START_PIN || test_flag.start_comm)
        {
            test_flag.start_comm = 0;
            FAIL_RE_START = FAIL_RE_START_FIRST;
            return;
        }
        if(RESET == STOP_PIN || TERMINATE)
        {
            STOP = 1;
            return;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
    }
}
static void fail_mode_next_dispose(void)
{
	LED_FAIL = LED_ON;/* 点亮FAIL灯 */
    if(BUZZER_EN)
    {
        BUZZER_SOUND(0);/* 不受系统开关控制 */
    }
    
	
    while(1)
    {
        if(RESET == START_PIN || test_flag.start_comm)
        {
            test_flag.start_comm = 0;
            FAIL_RE_START = FAIL_RE_START_NEXT;
			BUZZER = BUZZER_OFF;
            return;
        }
        if(RESET == STOP_PIN || TERMINATE)
        {
            STOP = 1;
			BUZZER = BUZZER_OFF;
            return;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
    }
}

static void fail_mode_fpdfc_dispose(void)
{
    fail_mode_con_dispose();
}

/*
 * 函数名：exception_handling
 * 描述  ：异常处理函数
 * 输入  ：errnum 错误码
 * 输出  ：无
 * 返回  ：无
 */
void exception_handling(int8_t errnum)
{
	int8_t l_err_num = errnum;
	
	uint8_t err_status_bool[12]=
	{
		0            , ST_ERR_H   , ST_ERR_L   , ST_ERR_REAL , ST_ERR_CHAR , ST_ERR_FAIL         ,
        ST_ERR_SHORT , ST_ERR_ARC , ST_ERR_GFI , ST_ERR_AMP  , ST_ERR_GEAR , ST_ERR_VOL_ABNORMAL ,
	};/* 给上位机发的错误状态 */
	
	irq_stop_relay_motion();
    close_test_timer();/* 关定时器 */
    
	if(STOP)
	{
		return;
	}
    
    
    relay_motion();/* 复位放电 */
	res_ave = err_res_bak;
    test_flag.vol_change_flag = 1;
    test_dis();/* 更新数据 */
    
	CUR_FAIL = 1;/* 当前步测试失败 */
    FAIL = 1;/* 测试失败 */
	cur_result.err_num = l_err_num;
    recover_exception_scene();
    updata_result(cur_mode);
    save_cur_result(&cur_result);/* 保存结果 */
    
    test_flag.judge_err_en == DISABLE;
    
	cur_status = err_status_bool[l_err_num%12];/* 给上位机发的错误状态 */
    cur_cylinder_ctrl_stop();
    
    test_flag.test_led_flag = 0;
	test_fail();
    
    updata_time(U_TEST_TIME, g_dis_time);
	
    
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            STOP = 1;
            /* 蜂鸣器长鸣 */
            if(BUZZER_EN)
            {
                BUZZER = BUZZER_ON;/* 不受系统开关控制 */
            }
            
            ERR_NUM = ERR_NONE;
            ERR_NUM_OVER = ERR_NONE;
            return;
        }
    }
    
	if(OFFSET_BBD)
	{
		goto l;
	}
    
	if(cur_mode == BBD)
	{
		if(errnum == ERR_SHORT)
		{
			cur_plc_err_st = PLC_HIGH_FAIL;
		}
		else if(errnum == ERR_OPEN)
		{
			cur_plc_err_st = PLC_LOW_FAIL;
		}
	}
	else
	{
		if(errnum == ERR_HIGH)
		{
			cur_plc_err_st = PLC_HIGH_FAIL;
		}
		else if(errnum == ERR_LOW)
		{
			cur_plc_err_st = PLC_LOW_FAIL;
		}
	}
	
    if(cur_mode == CC)
    {
        if(FAIL_MODE_CON == sys_par.fail_mode)
        {
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
            goto l;
        }
        else if(FAIL_MODE_CON != sys_par.fail_mode && FAIL_MODE_FPDFC != sys_par.fail_mode)
        {
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
        }
    }
	
    switch(sys_par.fail_mode)
    {
        case FAIL_MODE_STOP:
    l:
            fail_mode_stop_dispose();
            break;
        case FAIL_MODE_HALT:
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
            fail_mode_halt_dispose();
            break;
        case FAIL_MODE_CON:
            fail_mode_con_dispose();
            break;
        case FAIL_MODE_RESTART:
            fail_mode_restart_dispose();
            break;
        case FAIL_MODE_NEXT:
            fail_mode_next_dispose();
            break;
        case FAIL_MODE_FPDFC:
            fail_mode_fpdfc_dispose();
            break;
    }
    
    app_flag.buzzer_flicker_en = 0;
	ERR_NUM = ERR_NONE;
    ERR_NUM_OVER = ERR_NONE;
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
