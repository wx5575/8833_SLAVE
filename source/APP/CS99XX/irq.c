/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：irq.c
 * 摘  要  ：外部中断服务程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
/********************* 包含头文件 ************************/
#include    "DAC_VREF.h"
#include    "cs99xx_vref.h"
#include    "app.h"
#include 	"serve_test.h"
#include	"cs99xx_relay_motion.h"
#include 	"mb_server.h"
#include	"cs99xx_type.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_it.h"


#define GFI_DELAY_COUNT_MAX      10  /* GFI 累积报警 */
#define GFI_CLEAR_COUNT_CYCLE    100 /* GFI 清除计数值周期 单位ms */


/*******************************************************************************
*				中断服务程序 钩子函数 在中断程序中做的事情越少越好             *
*******************************************************************************/

/*
 * 函数名：gfi_cycle_clear_count
 * 描述  ：GFI中期性清除中断计数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gfi_cycle_clear_count(void)
{
    static uint32_t c = 0;
    
    /* 80不报警 90报警 设为100 */
    if(++c > GFI_CLEAR_COUNT_CYCLE)
    {
        test_flag.gfi_delay_count = 0;
        c = 0;
    }
}
/*
 * 函数名：stop_irq
 * 描述  ：复位中断服务程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void stop_irq(void)
{
	irq_stop_relay_motion();/* 关闭电子开关 高压 */
    close_hv();/* 关闭高压基准 */
	STOP = 1;	/* 置位测试标志 */
    FAIL = 0;/* 按下复位键后清空测试失败标记 */
    TERMINATE = 1;/* 终止测试标记置位 */
	cur_status = ST_STOP;
    CYLINDER_CTRL(1000);
    
    clear_keyboard();/* 清空键盘 */
    STOP_INT(DISABLE);/* 关闭中断 */
    BUZZER = BUZZER_OFF;/* 关闭蜂鸣 */
    LED_FAIL = LED_OFF;/* 关闭LED_FAIL灯 */
}
/*
 * 函数名：short_irq
 * 描述  ：短路中断服务程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void short_irq(void)
{
	ERR_NUM = ERR_SHORT;
    
    disable_sample_task();/* 关闭采样任务 *//* 停止刷新屏幕数据 保持出错时的状态 */
    
    /* 耐压测试时短路报警电流显示设置值 */
    if(cur_mode == ACW || cur_mode == DCW)
    {
        cur_ave = cur_high;
    }
    
    record_exception_scene();/* 记录异常现场 */
	irq_stop_relay_motion();
	SHORT_INT(DISABLE);
}
/*
 * 函数名：arc_irq
 * 描述  ：arc中断服务程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void arc_irq(void)
{
	ERR_NUM = ERR_ARC;
    
    disable_sample_task();/* 关闭采样任务 */
    record_exception_scene();/* 记录异常现场 */
	irq_stop_relay_motion();
	ARC_INT(DISABLE);
}
/*
 * 函数名：gfi_irq
 * 描述  ：gfi中断服务程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gfi_irq(void)
{
    if(++test_flag.gfi_delay_count < GFI_DELAY_COUNT_MAX)
    {
        return;
    }
    
	ERR_NUM = ERR_GFI;
    disable_sample_task();/* 关闭采样任务 */
    record_exception_scene();/* 记录异常现场 */
	irq_stop_relay_motion();
	GFI_INT(DISABLE);
}

/*
 * 函数名：offset_res
 * 描述  ：offset保存偏移测试结果
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void offset_res_f(const int8_t res)
{
	cur_offset_result = res;
	switch(cur_mode)
	{
		case ACW:
			g_cur_step->one_step.acw.offset_result = res;
            g_cur_step->one_step.acw.offset_cur = cur_ave + ROUND;
            g_cur_step->one_step.acw.offset_real = real_ave + ROUND;
			break;
		case DCW:
			g_cur_step->one_step.dcw.offset_result = res;
            g_cur_step->one_step.dcw.offset_cur = cur_ave + ROUND;
			break;
		case GR:
			g_cur_step->one_step.gr.offset_result = res;
            g_cur_step->one_step.gr.offset_res = res_ave + ROUND;/* 测试时的电流或电阻 */
			break;
        case BBD:
			g_cur_step->one_step.bbd.offset_result = res;
            if(cap_ave > 0)
            {
                g_cur_step->one_step.bbd.offset_cap = cap_ave + ROUND;/* 测试时的电流或电阻 或电容*/
            }
            break;
	}
}

static void serve_test_over(void)
{
    HIGH = 0;
    close_test_timer();
    irq_stop_relay_motion();//关闭高压 2017.5.24
    
    /* 如果fail_mode设为FAIL_MODE_FPDFC 并且是第一步 正常结束就结束整个测试 */
    if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1 && !CUR_FAIL)
    {
        CUR_OVER = 1;
        OVER = 1;/* 测试步全部完成 */
    }
    /* 继续下一步 */
    else
    {
        if(steps_con)
        {
            CUR_OVER = 1;
            CONT = 1;
            
            if(cur_step == g_cur_file->total)
            {
                if(FAIL)
                {
                    OVER = 1;/* 测试步全部完成 */
                    CONT = 0;
                }
            }
            return;
        }
        /* 停止测试 */
        else
        {
            CUR_OVER = 1;
            
            /* 当前步是最后一步 */
            if(cur_step == g_cur_file->total)
            {
                OVER = 1;/* 测试步全部完成 */
                cur_cylinder_ctrl_status = CY_OVER;
            }
        }
    }
}

static void sever_offset_test_over(void)
{
    /* 连续模式 */
    if(app_flag.offset_mode == OFFSET_CON)
    {
        if(g_cur_step->one_step.com.step < g_cur_file->total)
        {
            close_test_timer();/* 关定时器 */
            stop_test();/* 停止测试 */
            g_test_time = 0;/* 跳过第1阶段 */
            test_flag.gradation = STAGE_READY;/* 这个很重要 */
            OFFSET_NEXT = 1;/* 标记下一个步 */
            offset_res_f(OFFSET_PASS);/* 标记当前步测试合格 */
            return;
        }
    }
    
    stop_test();/* 停止测试 */
    offset_res_f(OFFSET_PASS);
    PASS = 1;
}

void report_absorptance_res_value(void)
{
	if(g_dis_time < 150)
	{
		ir_15s_res = 0;
		ir_60s_res = 0;
		ir_absorptance = 0;
		return;
	}
	
	if(g_dis_time == 150)
	{
		ir_15s_res = res_ave;/* IR测试 15s时的电阻值 */
	}
	else if(g_dis_time == 600)
	{
		ir_60s_res = res_ave;/* IR测试 60s时的电阻值 */
	}
	
	if(g_dis_time >= 600)
	{
		if(ir_15s_res > 0 && ir_60s_res > 0)
		{
			ir_absorptance = ir_60s_res / ir_15s_res;
		}
	}
}

/*
 * 函数名：test_irq
 * 描述  ：测试时钟控制，被调度任务调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_irq(void)
{
	/* 当测试时间为0时 测试时间无穷大 */
	if(0 == tes_t)
	{
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
		if(TESTING)
		{
			test_flag.gradation = STAGE_TEST;
			g_dis_time = g_test_time % 10000;
            if(g_dis_time == 0)
            {
                g_test_time = 0;
            }
			return;
		}
		
		if(g_test_time <= zeo_t)
		{
			g_dis_time = g_test_time - 0;
			test_flag.gradation = STAGE_READY;
			test_flag.dis_status = TEST_WAIT_INDEX;
			test_flag.vol_change_flag = 1;
			return;
		}
		/* 使程序进入第一阶段电压上升 */
		else if(g_test_time <= one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
		{
            count_rise_vol_step_value();/* 计算电压上升步进值 */
			g_dis_time = g_test_time - zeo_t;
			test_flag.gradation = STAGE_RISE;
			test_flag.dis_status = TEST_RISE_INDEX;
			test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 1)
            {
                count_vol_ch_step();
            }
			return;
		}
		
		report_absorptance_res_value();
		
        if(g_test_time == one_t + 1)
        {
            open_hv();
        }
        
		if(CUR_FAIL)
		{
			if(g_test_time < for_t)
			{
				/* 步间连续关闭 */
				if(!steps_con)
				{
                    serve_test_over();
                    return;
				}
				g_dis_time = g_test_time - thr_t + 1;
				test_flag.gradation = STAGE_INTER;
				test_flag.dis_status = TEST_INTER_INDEX;
				return;
			}
			else
			{
                serve_test_over();
                return;
			}
		}
		
		test_flag.vol_change_flag = 0;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		g_dis_time = g_test_time - one_t;
		
		if(g_dis_time >= 9999)
		{
            g_test_time = 0;
			TESTING = 1;/* 在启动时被清空 */
		}
		return;
	}
	
	/* 测试就绪 */
	if(g_test_time <= zeo_t)
	{
		g_dis_time = g_test_time;
		test_flag.gradation = STAGE_READY;
		test_flag.dis_status = TEST_WAIT_INDEX;
        test_flag.vol_change_flag = 1;
	}
	/* 使程序进入第一阶段电压上升 */
	else if(g_test_time <= one_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - zeo_t;
		test_flag.gradation = STAGE_RISE;
		test_flag.dis_status = TEST_RISE_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* 进入第二阶段 测试 */
	else if(g_test_time <= two_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - one_t;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		test_flag.vol_change_flag = 0;
        
		report_absorptance_res_value();
		
        if(g_test_time == one_t + 1)
        {
            open_hv();
        }
	}
	/* 第三阶段 电压下降 */
	else if(g_test_time <= thr_t)
	{
		HIGH = 1;
		test_flag.vol_change_flag = 1;
		test_flag.dis_status = TEST_FALL_INDEX;
		g_dis_time = g_test_time - two_t;
		test_flag.gradation = STAGE_FALL;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* 第四阶段 间隔等待 */
	else if(g_test_time <= for_t)
	{
		HIGH = 0;
		
		if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
		{
			if(!CUR_FAIL)
			{
				serve_test_over();
				return;
			}
		}
		/* 步间连续关闭 */
		if(!steps_con || (FAIL && cur_step == g_cur_file->total))
		{
            serve_test_over();
            return;
		}
        
		g_dis_time = g_test_time - thr_t;
		test_flag.gradation = STAGE_INTER;
		test_flag.dis_status = TEST_INTER_INDEX;
	}
	/* 当前步测试结束 */
	else
	{
		/* 正常测试 */
		if(!OFFSET_BBD)
		{
            serve_test_over();
        }
        else
        {
            sever_offset_test_over();
        }
	}
}

/*
 * 函数名：test_big_cap_irq
 * 描述  ：测试时钟控制，被调度任务调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_big_cap_irq(void)
{
    static uint16_t dis_time_bak;
    
	/* 当测试时间为0时 测试时间无穷大 */
	if(0 == tes_t)
	{
        /* 正在放电 */
        if(TERMINATE)
        {
            test_flag.gradation = STAGE_DIS_CHA;
            g_dis_time = g_test_time - for_t;
            test_flag.dis_status = TEST_DISCHARGE_INDEX;
            test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 1)
            {
                amp_relay_ctrl_off();
                on_sample_task();/* 启动采样任务 */
            }
            return;
        }
        
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
		if(TESTING)
		{
			test_flag.gradation = STAGE_TEST;
			g_dis_time = g_test_time % 10000;
            if(g_dis_time == 0)
            {
                g_test_time = 0;
            }
			return;
		}
		
		if(g_test_time <= zeo_t)
		{
			g_dis_time = g_test_time - 0;
			test_flag.gradation = STAGE_READY;
			test_flag.dis_status = TEST_WAIT_INDEX;
			return;
		}
		/* 使程序进入第一阶段正在充电 */
		else if(g_test_time <= one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
		{
			g_dis_time = g_test_time - zeo_t;
			test_flag.gradation = STAGE_CHARGE;
			test_flag.dis_status = TEST_RISE_INDEX;
			test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 0)
            {
                count_vol_ch_step();
            }
			return;
		}
		
        if(g_test_time == one_t + 1)
        {
            open_hv();
        }
        
		if(CUR_FAIL)
		{
			if(g_test_time < for_t)
			{
				/* 步间连续关闭 */
				if(!steps_con)
				{
					goto l;/* 结束测试 */
				}
				g_dis_time = g_test_time - thr_t + 1;
				test_flag.gradation = STAGE_INTER;
				test_flag.dis_status = TEST_INTER_INDEX;
				return;
			}
			else
			{
				goto l;/* 结束测试 */
			}
		}
		
		test_flag.vol_change_flag = 0;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		g_dis_time = g_test_time - one_t;
		
		if(g_dis_time >= 9999)
		{
            g_test_time = 0;
			TESTING = 1;/* 在启动时被清空 */
		}
        
		return;
	}
	
	/* 测试就绪 */
	if(g_test_time <= zeo_t)
	{
		g_dis_time = g_test_time;
		test_flag.gradation = STAGE_READY;
		test_flag.dis_status = TEST_WAIT_INDEX;
	}
	/* 使程序进入第一阶段正在充电 */
	else if(g_test_time <= one_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - zeo_t;
		test_flag.gradation = STAGE_CHARGE;
		test_flag.dis_status = TEST_CHARGE_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
    /* 进入第二阶段 稳压 */
	else if(g_test_time <= two_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - one_t;
		test_flag.gradation = STAGE_STAB;
		test_flag.dis_status = TEST_STAB_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_test_time == one_t + 1)
        {
            open_hv();
        }
	}
	/* 进入第三阶段 测试 */
	else if(g_test_time <= thr_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - two_t;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
        
        if(g_dis_time > 10)
        {
            test_flag.vol_change_flag = 0;
        }
		
        if(g_test_time == two_t + 1)
        {
            open_hv();
        }
	}
	/* 第四阶段 电压下降 */
	else if(g_test_time <= for_t)
	{
		HIGH = 1;
		test_flag.vol_change_flag = 1;
		g_dis_time = g_test_time - thr_t;
		test_flag.dis_status = TEST_FALL_INDEX;
		test_flag.gradation = STAGE_FALL;
        
        if(g_dis_time == 0)
        {
            count_vol_ch_step();
        }
	}
    /* 第五阶段 正在放电 */
    else if(0 == test_flag.dis_charge_end)
    {
        test_flag.gradation = STAGE_DIS_CHA;
        g_dis_time = g_test_time - for_t;
        dis_time_bak = g_dis_time;
        test_flag.dis_status = TEST_DISCHARGE_INDEX;
        test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            amp_relay_ctrl_off();
            
            on_sample_task();
        }
    }
	/* 第六阶段 间隔等待 */
	else if(g_test_time <= fiv_t + dis_time_bak)
	{
		HIGH = 0;
        
        if(TERMINATE)
        {
            goto l;/* 结束测试 */
        }
        
		g_dis_time = g_test_time - thr_t - dis_time_bak;
		test_flag.gradation = STAGE_INTER;
		test_flag.dis_status = TEST_INTER_INDEX;
        
		/* 步间连续关闭 */
		if(!steps_con)
		{
			goto l;/* 结束测试 */
		}
	}
	/* 当前步测试结束 */
	else
	{
		HIGH = 0;
        close_test_timer();
        
		/* 继续下一步 */
		if(steps_con)
		{
            CUR_OVER = 1;
			CONT = 1;
			return;
		}
        /* 停止测试 */
		else
		{
	l:
            close_test_timer();/* 关定时器 */
			irq_stop_relay_motion();
            test_flag.dis_status = TEST_DISCHARGE_OVER_INDEX;
            
            CUR_OVER = 1;
            
			/* 当前步是最后一步 */
			if(g_cur_step == list_99xx.tail)
			{
				OVER = 1;/* 测试步全部完成 */
			}
		}
	}
}

/*
 * 函数名： test_g_irq
 * 描述  ：G模式测试时钟控制，被调度任务调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_g_irq(void)
{
	/* 当测试时间为0时 测试时间无穷大 */
	if(0 == tes_t)
	{
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
		if(TESTING)
		{
			test_flag.gradation = STAGE_TEST;
			g_dis_time = g_test_time % 10000;
            if(g_dis_time == 0)
            {
                g_test_time = 0;
            }
			test_flag.vol_change_flag = 0;
			return;
		}
		
		if(g_test_time <= zeo_t)
		{
			g_dis_time = g_test_time - 0;
			test_flag.gradation = STAGE_READY;
			test_flag.dis_status = TEST_WAIT_INDEX;
            test_flag.vol_change_flag = 1;
			return;
		}
		/* 使程序进入第一阶段电压上升 */
		else if(g_test_time <= one_t)
		{
			g_dis_time = g_test_time - zeo_t;
			test_flag.gradation = STAGE_RISE;
			test_flag.dis_status = TEST_RISE_INDEX;
			test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 1)
            {
                count_vol_ch_step();
            }
			return;
		}
		
        if(g_test_time == one_t + 1)
        {
            open_hv();
        }
        
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		test_flag.vol_change_flag = 0;
		g_dis_time = g_test_time - one_t;
		
		if(g_dis_time >= 9999)
		{
            g_test_time = 0;
			TESTING = 1;/* 在启动时被清零 */
		}
		return;
	}
	
	/* 进入第0阶段 测试准备 */
	if(g_test_time <= zeo_t)
	{
		g_dis_time = g_test_time;
		test_flag.gradation = STAGE_READY;
        test_flag.dis_status = TEST_WAIT_INDEX;
		test_flag.vol_change_flag = 1;
	}
	/* 进入第1阶段 电压上升 */
	else if(g_test_time <= one_t)
	{
		g_dis_time = g_test_time - zeo_t;
		test_flag.gradation = STAGE_RISE;
		test_flag.dis_status = TEST_RISE_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* 进入第2阶段 电压缓变 */
	else if(g_test_time <= two_t)
	{
		g_dis_time = g_test_time - one_t;
		test_flag.gradation = STAGE_CHANGE;
		test_flag.dis_status = TEST_CHANGE_INDEX;
		test_flag.vol_change_flag = 1;
	}
	/* 进入第3阶段 测试 */
	else if(g_test_time <= thr_t)
	{
		g_dis_time = g_test_time - two_t;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		
        if(g_dis_time > 10)
        {
            test_flag.vol_change_flag = 0;
        }
        
        if(g_test_time == one_t + 1)
        {
            open_hv();
        }
	}
	/* 进入第4阶段 电压下降*/
	else if(g_test_time <= for_t)
	{
		test_flag.vol_change_flag = 0;
		g_dis_time = g_test_time - thr_t;
		test_flag.dis_status = TEST_FALL_INDEX;
		test_flag.gradation = STAGE_FALL;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	else
	{
		/* 正常测试 */
		if(!OFFSET_BBD)
		{
            close_test_timer();
            
            if(g_cur_step->next != NULL)
            {
                CONT = 1;
                return;
            }
            
            irq_stop_relay_motion();
            OVER = 1;
        }
		/* 偏移测试 */
        else
        {
            if(app_flag.offset_mode == OFFSET_CON)
            {
                if(g_cur_step->next != NULL)
                {
                    close_test_timer();/* 关定时器 */
                    stop_test();/* 停止测试 */
                    test_pass();/* 保存结果 */
                    offset_res_f(OFFSET_PASS);
                    OFFSET_NEXT = 1;/* 标记下一个步 */
                    return;
                }
            }
            
            stop_test();/* 停止测试 */
            test_pass();
            offset_res_f(OFFSET_PASS);
        }
        
		PASS = 1;/* 当不再进行测试时就将pass标志写1 */
	}
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
