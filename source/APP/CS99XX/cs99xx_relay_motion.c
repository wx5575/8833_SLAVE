/*
 * Copyright(c) 2014, 南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_vref.c
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#include    "cs99xx_relay_motion.h"
#include	<stdio.h>
#include    <string.h>
#include 	<math.h>
#include	"BSP_config.h"
#include 	"MC14094.h"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include 	"app.h"
#include    "cs99xx_result.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"cs99xx_it.h"
#include	"calibration.h"
#include    "cs99xx_collect.h"
#include    "cs99xx_vref.h"
#include    "cs99xx_mem_api.h"
#include    "cs99xx_err_log.h"
#include    "cs99xx_plc.h"
#include    "dc_module.h"
#include    "cs99xx_ir_shift_gear.h"



static void mc14094_set_gear(u8 status);

/*
 * 函数名：test_pass
 * 描述  ：测试通过调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_pass(void)
{
    /* 测试时间到报警 */
	if(/*FAIL || */ERR_NUM_OVER)
    {
        return;
    }
    
    /* 当前步报警 */
    if(CUR_FAIL)
    {
        return;
    }
    
    /* 此函数只处理一次 */
    if(test_flag.pass_only_one == 0)
    {
        test_flag.pass_only_one++;
    }
    else
    {
        return;
    }
    
    /* 更新PLC的状态 */
	if(g_cur_step->next == NULL || (steps_con && steps_pass /* && int_t*/) || !steps_con)
	{
        plc_signal_cmd(PLC_SIG_PASS);
	}
	
	if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
	{
        plc_signal_cmd(PLC_SIG_PASS);
	}
    
	LED_FAIL = LED_OFF;
	LED_TEST = LED_OFF;
    
	if(steps_pass)
	{
		LED_PASS = LED_ON;
	}
    
	if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
	{
		/* 蜂鸣 */
        if(BUZZER_EN>0)
        {
            BUZZER_SOUND(g_cur_file->pass_buzzing_time*50);
        }
	}
	
	if((g_cur_file->work_mode == G_MODE && g_cur_step->next == NULL)
		|| ((g_cur_file->work_mode == N_MODE) && !steps_con)
        || ((g_cur_file->work_mode == N_MODE)
             && steps_con  && g_cur_step->next == NULL && int_t == 0)
    )
	{
		/* 蜂鸣 */
        if(BUZZER_EN>0)
        {
            BUZZER_SOUND(g_cur_file->pass_buzzing_time*50);
        }
	}
}

/*
 * 函数名：test_fail
 * 描述  ：测试失败时调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_fail(void)
{
    exit_off();
    if(sys_par.plc_on_off == PLC_ON)
    {
        /* 向PLC发送测试失败的信号 */
        plc_signal_cmd(PLC_SIG_FAIL);
    }
	
	/* 点亮FAIL灯 */
	LED_FAIL = LED_ON;
	LED_TEST = LED_OFF;
	LED_PASS = LED_OFF;
    
	/* 偏移测试时不要保存数据 */
	if(OFFSET_BBD)
	{
		return;
	}
}

void relay_ready(void)
{
    if(cur_mode == CC)
    {
        MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_ON);/* 启动测试时置0 */
    }
    else
    {
        MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* 启动测试时置0 */
    }
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* 启动时置1 */
    
    MC14094_CMD(MC14094_B, MC14094_AC_DC, (cur_mode == DCW || cur_mode == IR));/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    
    on_switch_hv();/* 开高压 */
    
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}
/*
 * 函数名：on_dc_gr_control_relay
 * 描述  ：开直流模块控制继电器
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void on_dc_gr_control_relay(void)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_ON);/* ACW DCW IR */
	MC14094_Updata();// 更新输出缓冲区
}
/*
 * 函数名：test_ready
 * 描述  ：测试就绪
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_ready(void)
{
	test_flag.confirm_test_init_ok = 1;/* 确认测试前初始化OK */
    test_vref(cur_high);	/* 输出各基准 */
    
    if(cur_mode == GR && DC_GR_EN)
    {
        clear_dc_gr_st();
    }
    
	updata_port(&cur_port);/* 更新端口状态 */
	relay_motion();/* 继电器动作 */
    amp_relay_ctrl_on();/* 功放继电器 */
//    OSTimeDlyHMSM(0, 0, 0, 10);
    soft_delay_us(10);
	
    if(cur_mode == GR && DC_GR_EN)
    {
        wait_dc_gr_send_task_idle();
        
        if(-1 == ask_dc_gr_start_ok())
        {
            STOP = 1;
            clear_mdbus_send_queue();/* 清除modbus发送队列 */
            send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);/* 停止测试 */
            /* 启动失败 */
			gui_draw_dc_gr_module_comm_fail();
			clear_();
			test_flag.refresh_for_comm = 1;//重绘界面
            return;
        }
		
		if(g_cur_type->num == CS9933G_)
		{
			OSTimeDlyHMSM(0,0,0,200);//延时 等待各个模块输出稳定
		}
		
		wait_dc_gr_send_task_idle();
    }
    
	
    test_flag.allow_dc_gr_fetch = 1;
    
    if(ris_t == 0 || cur_mode == BBD || cur_mode == CC)
    {
        open_hv();//送基准
    }
    
    relay_ready();//开电子开关
    
    if(test_flag.test_status == TEST_STOPING)
    {
        open_sine(cur_frequency);/* 开正弦波 */
    }
    
    if(cur_mode == BBD || cur_mode == CC)
    {
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
    
	if(cur_mode == ACW)
	{
		err_real = 0;/* acw 真实电流报警  */
	}
	
	if(cur_mode == DCW)
	{
        err_charge = 1;/* dcw 充电报警置1  */
	}
    
    plc_signal_cmd(PLC_SIG_TESTING);/* 向PLC发送测试信号 */
    
	serve_count(AMP_COUNT);/* 功放继电器吸合次数更新 */
    
	TEST = 1;/* 进入测试状态 */
    HIGH = 1;
    test_flag.test_led_flag = 1;/* 开启闪灯 */
	test_flag.test_status = TEST_RUNING;
	test_flag.dc_discharge_time = 5;/* 10ms */ //直流放电时间
    
    g_test_time = 0;
    g_dis_time = 0;
    open_test_timer();/* 开定时器 */
    on_schedule_task();/* 打开调度任务 */
}


static void amp_relay_ctrl_on_8833(void)
{
	if(cur_mode == GR)
	{
		/* 直流 */
		if(DC_GR_EN)
		{
			app_flag.dc_gr_module_status = DC_GR_RUNING;
//             send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);/* 在启动前先发送停止命令，防止 */
            send_dc_module_cmd(DC_MODULE_CMD_START, 0);/* 启动测试 */
		}
		/* 交流 */
		else
		{
			/* 当1万伏PWM功放使能时 */
			if(type_spe.amp_type == PWM_AMP)
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY3, RELAY_ON);
			}
			else
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);/* 功放开关 1打开 */
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_ON);
			}
		}
	}
	else
	{
		/* 当1万伏PWM功放使能时 */
		if(type_spe.amp_type == PWM_AMP)
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);
			/* 大于5kv关闭 MC14094_AMP_RELAY2 */
			if(contact_segment == 0)
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_ON);
			}
			else
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
			}
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY3, RELAY_OFF);
		}
        /* 线性功放 */
		else
		{
            MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);/* 功放继电器 */
            
            /* 双触点变压器 contact */
            if(type_spe.transformer_point == DOUBLE_POINT)
            {
                if(contact_segment == 1)
                {
                    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_ON);/* 功放继电器 */
                }
                else
                {
                    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放继电器 */
                }
            }
            /* 单触点变压器 */
            else
            {
                MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放继电器 */
            }
		}
	}
    MC14094_Updata();
}

static void amp_relay_ctrl_off_8833(void)
{
    MC14094_Updata();
    soft_delay_us(10);
    
	if(cur_mode == GR)
	{
		/* 直流 */
		if(DC_GR_EN)
		{
            send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);
		}
		/* 交流 */
		else
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 1打开 */
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
		}
	}
	else
	{
		/* 当1万伏PWM功放使能时 */
		if(type_spe.amp_type == PWM_AMP)
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);
			/* 小于5kv关闭 MC14094_AMP_RELAY2 */
			if(contact_segment == 0)
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
			}
			else
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
			}
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY3, RELAY_OFF);
		}
		else
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放继电器 */
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放继电器 */
		}
	}
    
    MC14094_Updata();
}

static void amp_relay_ctrl_on_old(void)
{
	if(cur_mode == GR)
	{
		/* 直流 */
		if(DC_GR_EN)
		{
			app_flag.dc_gr_module_status = DC_GR_RUNING;
            send_dc_module_cmd(DC_MODULE_CMD_START, 0);/* 启动测试 */
		}
		/* 交流 */
		else
		{
			/* 当1万伏PWM功放使能时 */
			if(type_spe.amp_type == PWM_AMP)
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY3, RELAY_ON);
			}
			else
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);/* 功放开关 1打开 */
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_ON);
			}
		}
	}
	else
	{
		/* 当1万伏PWM功放使能时 */
		if(type_spe.amp_type == PWM_AMP)
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);
			/* 小于5kv关闭 MC14094_AMP_RELAY2 */
			if(contact_segment == 0)
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
			}
			else
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_ON);
			}
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY3, RELAY_OFF);
		}
		else
		{
            MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_ON);/* 功放继电器 */
            
            /* 双触点变压器 */
            if(type_spe.transformer_point == DOUBLE_POINT)
            {
                if(contact_segment == 1)
                {
                    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_ON);/* 功放继电器 */
                }
                else
                {
                    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放继电器 */
                }
            }
            /* 单触点变压器 */
            else
            {
                MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放继电器 */
            }
		}
	}
	
    MC14094_Updata();
}

static void amp_relay_ctrl_off_old(void)
{
    MC14094_Updata();
    soft_delay_us(10);
    
	if(cur_mode == GR)
	{
		/* 直流 */
		if(DC_GR_EN)
		{
			send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);
		}
		/* 交流 */
		else
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 1打开 */
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
		}
	}
	else
	{
		/* 当1万伏PWM功放使能时 */
		if(type_spe.amp_type == PWM_AMP)
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);
			/* 小于5kv关闭 MC14094_AMP_RELAY2 */
			if(cur_vol < 5000)
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
			}
			else
			{
				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
			}
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY3, RELAY_OFF);
		}
        /* 定制机专用 */
        else if(g_cur_type->num == CS9924_200 || g_cur_type->num == CS9914AM_H)
        {
            MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);
        }
		else
		{
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放继电器 */
		}
	}
    
    MC14094_Updata();
}


void amp_relay_ctrl_on(void)
{
    if(g_custom_sys_par.amp_select == AMP_OLD)
    {
        amp_relay_ctrl_on_old();
    }
    else
    {
        amp_relay_ctrl_on_8833();
    }
}
void amp_relay_ctrl_off(void)
{
    if(g_custom_sys_par.amp_select == AMP_OLD)
    {
        amp_relay_ctrl_off_old();
    }
    else
    {
        amp_relay_ctrl_off_8833();
    }
}
/*
 * 函数名：irq_stop_relay_motion
 * 描述  ：发生外部中断后要关闭高压，在中断服务程序中要调用该函数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 * 记录: 2017.5.13
 *          增加 close_hv();的调用 在测试停止后要将基准值清0
 */
void irq_stop_relay_motion(void)
{
    exit_off_all();//关闭中断
	MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_OFF);
	MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_ON);
	MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);
    MC14094_Updata();
    off_switch_hv();/* 关闭高压 */
    HIGH = 0;
    LED_TEST = LED_OFF;
    test_flag.test_led_flag = 0;
    close_sine();
    amp_relay_ctrl_off();
    close_hv();/* 关闭高压基准 2017.5.13 */
}

/*
 * 函数名：ir_set_gear
 * 描述  ：ir档位设置
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */

void ir_set_gear_s(const uint8_t gear)
{
	switch(gear)
	{
		case IR_10MOHM:
			mc14094_set_gear(MC14094_C_10M);
			break;
		case IR_100MOHM:
			mc14094_set_gear(MC14094_C_100M);
			break;
		case IR_1GOHM:
			mc14094_set_gear(MC14094_C_1G);
			break;
		case IR_10GOHM:
			mc14094_set_gear(MC14094_C_10G);
			break;
		case IR_100GOHM:
			mc14094_set_gear(MC14094_C_100G);
			break;
	}
}

void ir_set_gear(const uint8_t gear, uint8_t soft_gear)
{
    float temp = 0;
    
    ir_set_gear_s(gear);
    
    temp = ratio_ir.res_k[soft_gear][cur_vol_gear];
    
    SAFE_FLOAT_VOLU_CAL(cur_res_k, temp); /* 进行浮点数安全赋值处理 */
	
    
    cur_gear_comm = gear;
}
/*
 * 函数名：bbd_auto_select_gear
 * 描述  ：bbd自动换挡
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void bbd_auto_select_gear(void)
{
	uint32_t shift_gear_delay = 500;
	
// 	if(g_cur_step->one_step.bbd.get_cs_ok)
// 	{
// 		return;
// 	}
    
// 	if(((cur_vol - vol_ave) > 10) || ((vol_ave - cur_vol) > 10))
// 	{
// 		return;
// 	}
	
	if(g_ir_dly)
	{
		return;
	}
	
	switch(cur_gear)
	{
		case AC_2uA:
		{
			/* 向上换档 */
			if(cur_ave > 2000)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_20uA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_20uA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			else
			{
				test_flag.bbd_time++;
			}
			break;
		}
		case AC_20uA:
		{
			/* 向下换档 */
			if(cur_ave < 200)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_2uA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_2uA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			/* 向上换档 */
			else if(cur_ave > 2000)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_200uA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_200uA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			else
			{
				test_flag.bbd_time++;
			}
			break;
		}
		case AC_200uA:
		{
// 			/* 向下换档 */VOL_OPT
// 			if(cur_ave < 200)
// 			{
// 				test_flag.bbd_time = 0;
// 				cur_gear = AC_20uA;
// 				cur_adc_cur_k = ratio_bbd.adc_20uA_k;
// 				g_cur_step->one_step.bbd.gear = cur_gear;
// 				shift_cur_gear(BBD);
// 				g_ir_dly = 1000;
// 			}
// 			/* 向上换档 */
// 			else 
			if(cur_ave > 2000)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_2mA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_2mA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			else
			{
				test_flag.bbd_time++;
			}
			break;
		}
		case AC_2mA:
		{
// 			break;
			/* 向下换档 */
			if(cur_ave < 200)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_200uA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_200uA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			/* 向上换档 */
			else if(cur_ave > 2000)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_20mA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_20mA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			else
			{
				test_flag.bbd_time++;
			}
			break;
		}
		case AC_20mA:
		{
			/* 向下换档 */
			if(cur_ave < 200)
			{
				test_flag.bbd_time = 0;
				cur_gear = AC_2mA;
				cur_adc_cur_k = ratio_bbd.adc_cur_k[AC_2mA];
				g_cur_step->one_step.bbd.gear = cur_gear;
				shift_cur_gear(BBD);
				g_ir_dly = shift_gear_delay;
			}
			else
			{
				test_flag.bbd_time++;
			}
			break;
		}
	}
	
	if(test_flag.bbd_time > 10)
	{
		g_cur_step->one_step.bbd.get_cs_ok = 2;
        g_cur_step->one_step.bbd.gear = cur_gear;
	}
    
	MC14094_Updata();
}

void bbd_load_gear(void)
{
	cur_adc_cur_k = ratio_acw.adc_cur_k[cur_gear];
	
	shift_cur_gear(BBD);
}


static void mc14094_set_gear(uint8_t status)
{
	uint8_t data1 = status & (~0x80);
	uint8_t data2 = MC14094_BUFFER[MC14094_C] & (~0X7F);
	
	status = data1 + data2;/* 保护最高位 接地浮地的状态 */
	
	MC14094_CMD_BYTE(MC14094_C, status);
	MC14094_Updata();/* 写入硬件 */
}

void shift_cur_gear(uint8_t mode)
{
	switch(mode)
	{
		case ACW:
		case BBD:
        case CC:
		{
			switch(cur_gear)
			{
				case AC_2uA:
					mc14094_set_gear(MC14094_C_2uA);
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
					break;
				case AC_20uA:
					mc14094_set_gear(MC14094_C_20uA);
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
					break;
				case AC_200uA:
					mc14094_set_gear(MC14094_C_200uA);
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
					break;
				case AC_2mA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* 切换到2mA 20mA的电路 */
					MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_ON);/* 2mA 挡 */
					break;
				case AC_10mA:
				case AC_20mA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* 切换到2mA 20mA的电路 */
					MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_OFF);/* 20mA 挡 */
					break;
				case AC_50mA:
				case AC_100mA:
				case AC_200mA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
					mc14094_set_gear(MC14094_C_100mA);
					break;
			}
			break;
		}
		case DCW:
		{
			switch(cur_gear)
			{
				case DC_2uA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
					mc14094_set_gear(MC14094_C_2uA);
					break;
				case DC_20uA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
					mc14094_set_gear(MC14094_C_20uA);
					break;
				case DC_200uA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
					mc14094_set_gear(MC14094_C_200uA);
					break;
				case DC_2mA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);
					MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_ON);/* 2mA */
					break;
				case DC_10mA:
				case DC_20mA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);
					MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_OFF);/* 10mA */
					break;
				case DC_50mA:
				case DC_100mA:
					MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
					mc14094_set_gear(MC14094_C_100mA);
					break;
			}
			break;
		}
		default:
			break;
	}
}
void shift_gear(uint8_t mode)
{
    shift_cur_gear(mode);
    
    if(mode == IR)
    {
        init_set_ir_gear();
    }
    
	MC14094_Updata();// 更新输出缓冲区
}

/*
 * 函数名：relay_motion
 * 描述  ：进入测试前，继电器切换到对应的状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void relay_motion(void)
{
    /* 大电容测试 */
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        if(cur_mode == DCW)
        {
            /* 大电容测试前准备 */
            dcw_big_cap_start_charge();
        }
        else
        {
            no_dcw_big_cap_stop_charge();
        }
	}
    
    if(cur_mode != GR)
    {
        MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    }
    
	switch(cur_mode)
	{
		case ACW:
		{
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);
            
			g_short_delay = 10;
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 */
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 */
			MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
			MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
			MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
			
            MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
			shift_cur_gear(ACW);
			break;
		}
		case CC:
		{
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);
            
			g_short_delay = 10;
// 			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 */
// 			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 */
			MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
			MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
			MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
			MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
			shift_cur_gear(CC);
            break;
		}
		case DCW:
		{
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);/* DA电压段选择 */
            
			g_short_delay = 10;
// 			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 */
// 			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 1打开 */
			
			MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR 置1，DCW/IR置0 */
			
			MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
			
            MC14094_Updata();// 更新输出缓冲区
			soft_delay_us(test_flag.dc_discharge_time*50);
			MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 复位状态下切换到交流 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
			MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
            
            shift_cur_gear(DCW);
			break;
		}
		case IR:
		{
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);
            
// 			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 1打开 */
			MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
			MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR 置1，DCW/IR置0 */
			
            MC14094_Updata();// 更新输出缓冲区
// 			OSTimeDlyHMSM(0, 0, 0, test_flag.dc_discharge_time);
			soft_delay_us(test_flag.dc_discharge_time*50);/* 延时给直流回路放电 */
			MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 复位状态下切换到交流 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
			
			MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
            init_set_ir_gear();
			break;
		}
		case GR:
		{
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_OFF);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
			/* 直流 */
			if(DC_GR_EN)
			{
				if(app_flag.dc_gr_module_status == DC_GR_RUNING)
				{
// 					send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);/* 停止测试 */
                    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* 切换到GR采样继电器 */
					app_flag.dc_gr_module_status = DC_GR_STOPING;
				}
			}
			/* 交流 */
			else
			{
// 				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 1打开 */
// 				MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);
				
				MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR/BBD 置1，DCW/IR置0 */
				
				MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
				MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到外部扩展板采样 */
				MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_ON);
				MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_ON);/* 切换到GR采样继电器 */
			}
			break;
		}
		case BBD:
		{
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);
            MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
            
			MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 */
			MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR/BBD 置1，DCW/IR置0 */
			MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR/BBD */
// 			MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* 切换到2mA 20mA的电路 *//* 耐压绝缘接地电流采样切换 置1 ACW/DCW, 置0 IR/GR */
			MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR/BBD */
			
// 			MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);
// 			cpld_write(cpld_cmd);
// 			MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_ON);/* 2mA */
			
			shift_cur_gear(BBD);
			break;
		}
	}
    
	MC14094_Updata();// 更新输出缓冲区
}

static void serve_port_8_4094(uint8_t val, uint8_t port)
{
	if(val == 0)
	{
		MC14094_CMD_Port(MC14094_X, port, RELAY_OFF);
		MC14094_CMD_Port(MC14094_Y, 7 - port, RELAY_OFF);
	}
	else if(val == 1)
	{
		MC14094_CMD_Port(MC14094_X, port, RELAY_OFF);
		MC14094_CMD_Port(MC14094_Y, 7 - port, RELAY_ON);
	}
	else if(val == 2)
	{
		MC14094_CMD_Port(MC14094_X, port, RELAY_ON);
		MC14094_CMD_Port(MC14094_Y, 7 - port, RELAY_OFF);
	}
	else
	{
		MC14094_CMD_Port(MC14094_X, port, RELAY_OFF);
		MC14094_CMD_Port(MC14094_Y, 7 - port, RELAY_OFF);
	}
}

static void serve_port_16_4094(uint8_t val, uint8_t port)
{
    uint8_t id_h = 0;
    uint8_t id_l = 0;
    uint8_t port_h = 0;
    uint8_t port_l = 0;
    
    if(port < 8)
    {
        id_h = MC14094_H1;
        id_l = MC14094_L1;
        port_h = port;
        port_l = 7 - port;
    }
    else if(port < 16)
    {
        id_h = MC14094_H2;
        id_l = MC14094_L2;
        port_h = port - 8;
        port_l = 6 + 7 - port;
    }
    
    if(val == 0)
    {
        MC14094_CMD_Port(id_h, port_h, RELAY_OFF);
        MC14094_CMD_Port(id_l, port_l, RELAY_OFF);
    }
    else if(val == 1)
    {
        MC14094_CMD_Port(id_h, port_h, RELAY_OFF);
        MC14094_CMD_Port(id_l, port_l, RELAY_ON);
    }
    else if(val == 2)
    {
        MC14094_CMD_Port(id_h, port_h, RELAY_ON);
        MC14094_CMD_Port(id_l, port_l, RELAY_OFF);
    }
    else
    {
        MC14094_CMD_Port(id_h, port_h, RELAY_OFF);
        MC14094_CMD_Port(id_l, port_l, RELAY_OFF);
    }
}
/*
 * 函数名：serve_port
 * 描述  ： 服务于updata_port 更新端口的状态 该函数目前设计仅适用于8端口的
 * 			由于四端口的电路未知以后根据硬件添加
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void serve_port_4094(uint8_t val, uint8_t port)
{
    if(type_spe.port_num <= 8)
    {
        serve_port_8_4094(val, port);
    }
    else if(type_spe.port_num <= 16)
    {
        serve_port_16_4094(val, port);
    }
}
static void serve_port_cpld(uint8_t val, uint8_t port)
{
    port %= 8;
    
	if(val == 0)
	{
        cmd_cpld_hv_port(HV_PORT_L, 7 - port, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, 7 - port, RELAY_OFF);
	}
	else if(val == 1)
	{
        cmd_cpld_hv_port(HV_PORT_L, 7 - port, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, 7 - port, RELAY_ON);
	}
	else if(val == 2)
	{
        cmd_cpld_hv_port(HV_PORT_L, 7 - port, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, 7 - port, RELAY_OFF);
	}
	else
	{
        cmd_cpld_hv_port(HV_PORT_L, 7 - port, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, 7 - port, RELAY_OFF);
	}
}

void reset_ports(void)
{
    if(type_spe.ports_ctrl_type == CPLD_CTRL)
    {
        reset_posrts_cpld_ctrl();
    }
    else
    {
        reset_posrts_mc14094_ctrl();
    }
//     memset(MC14094_BUFFER_PORT, 0, sizeof MC14094_BUFFER_PORT);
//     MC14094_Updata_Port();
}
/*
 * 函数名：updata_port
 * 描述  ：更新端口的状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void updata_port(TEST_PORT *p)
{
	int32_t i = 0;
	uint16_t *p_val = (uint16_t *)p;
	
    if(type_spe.ports_ctrl_type == CPLD_CTRL)
    {
        for(i = 0; i < type_spe.port_num; i++)
        {
            serve_port_cpld((*(p_val + i / 8))>>(2 * (i % 8)) & 0x3, i);
        }
        
        hv_cpld_write(CMD_CPLD_HV_H);
        hv_cpld_write(CMD_CPLD_HV_L);
    }
    else
    {
        for(i = 0; i < type_spe.port_num; i++)
        {
            serve_port_4094((*(p_val + i / 8))>>(2 * (i % 8)) & 0x3, i);
        }
        
        MC14094_Updata_Port();
    }
}

/*
 * 函数名：dcw_big_cap_start_charge
 * 描述  ：进入测试前，继电器切换到对应的状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dcw_big_cap_start_charge(void)
{
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, !cur_output_impedan);
    cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L1, RELAY_ON);
    hv_cpld_write(CMD_CPLD_HV_H);
    hv_cpld_write(CMD_CPLD_HV_L);
//     hv_cpld_write(CMD_CPLD_HV_H);
//     hv_cpld_write(CMD_CPLD_HV_L);
}
void dcw_big_cap_stop_charge(void)
{
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
    cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L1, RELAY_OFF);
    hv_cpld_write(CMD_CPLD_HV_H);
    hv_cpld_write(CMD_CPLD_HV_L);
}
/*
 * 函数名：no_dcw_big_cap_stop_charge
 * 描述  ：非dcw大电容测试时应切换的继电器状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void no_dcw_big_cap_stop_charge(void)
{
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
    hv_cpld_write(CMD_CPLD_HV_H);
}
void dcw_big_cap_stop_dis_charge(void)
{
    cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L1, RELAY_OFF);
    
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    hv_cpld_write(CMD_CPLD_HV_H);
}

void dcw_big_cap_cal(void)
{
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
    hv_cpld_write(CMD_CPLD_HV_H);
}
void dcw_big_cap_keep_dis_charge(void)
{
    cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L1, RELAY_OFF);
    
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    
    hv_cpld_write(CMD_CPLD_HV_H);
    hv_cpld_write(CMD_CPLD_HV_L);
}
void dcw_big_cap_execute_dis_charge(void)
{
    if(cur_mode != DCW)
    {
        return;
    }
    
    irq_stop_relay_motion();/* 关闭电子开关 高压 */
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
    cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L1, RELAY_OFF);
    
    if(vol_ave == 0)
    {
        test_flag.dis_charge_end = 1;
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    }
    else if(vol_ave < 1000)
    {
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_ON);
    }
    else if(vol_ave < 2500)
    {
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    }
    else if(vol_ave < 4000)
    {
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    }
    else if(vol_ave < 5000)
    {
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    }
    else if(vol_ave < 6000)
    {
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    }
    else
    {
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H1, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H3, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H4, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H5, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H6, RELAY_OFF);
        cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H7, RELAY_OFF);
    }
    
    hv_cpld_write(CMD_CPLD_HV_H);
    hv_cpld_write(CMD_CPLD_HV_L);
}

void dcw_big_cap_output_Impedan_sel(uint8_t n)
{
    if(type_spe.dcw_big_cap_en != ENABLE && cur_mode != DCW)
    {
        return;
    }
    
    cmd_cpld_hv_port(HV_PORT_H, CPLD_HV_H2, RELAY_ON);
    cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L1, RELAY_OFF);
    
    switch(n)
    {
        case 0:
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L2, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L3, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L4, RELAY_OFF);
            break;
        case 1:
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L2, RELAY_ON);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L3, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L4, RELAY_OFF);
            break;
        case 2:
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L2, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L3, RELAY_ON);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L4, RELAY_OFF);
            break;
        case 3:
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L2, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L3, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L4, RELAY_ON);
            break;
        default:
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L2, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L3, RELAY_OFF);
            cmd_cpld_hv_port(HV_PORT_L, CPLD_HV_L4, RELAY_OFF);
            break;
    }
    
    hv_cpld_write(CMD_CPLD_HV_H);
    hv_cpld_write(CMD_CPLD_HV_L);
}

/*
 * 函数名：de_reset
 * 描述  ：测试程序默认初始化值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void de_reset(void)
{
	MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_OFF);/* 复位状态必须置0 */
	MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_ON);/* 复位时置1 */
	MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR 置1，DCW/IR置0 */
	
	MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 高压接地电源选择 1选择接地电源 0选择高压电源 */
	MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
	MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 耐压绝缘接地电流采样切换 置1 ACW/DCW, 置0 IR/GR */
	MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
	
	MC14094_Updata();// 更新输出缓冲区
	
    plc_signal_cmd(PLC_SIG_RESET);/* 在复位状态下PLC这三个继电器都不吸合 */
    
	close_sine();/* 关闭正弦波 */
	memset((void*)&test_flag, 0, sizeof test_flag);/* 清空标志位 */
	LED_ALL = LED_OFF;/* 关闭LED指示灯 */
}

/*
 * 函数名：test_reset
 * 描述  ：测试复位
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_reset(void)
{
    if(cur_mode != BBD)
        DAC_Vol[DAC_OUTPUT_VREF] = HV_VREF_RESET;
	DAC_Vol[DAC_SHORT_VREF] = SHORT_VREF_RESET;
	DAC_Vol[DAC_ARC_VREF] = ARC_VREF_RESET;
	
	/* 测试模式选择 */
	if(sys_par.test_method == GND_MODE)
	{
		MC14094_CMD(MC14094_B, MC14094_GFI_RELAY, RELAY_ON); /* k8 */
		MC14094_CMD(MC14094_C, MC14094_C_GND_FLOAT, RELAY_ON);/* 接地模式 */
	}
	else
	{
		MC14094_CMD(MC14094_B, MC14094_GFI_RELAY, RELAY_OFF); /* k8 */
		MC14094_CMD(MC14094_C, MC14094_C_GND_FLOAT, RELAY_OFF);/* 浮地模式 */
	}
	
//     MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
	
	/* reset CD4053; */
	MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_OFF);/* 复位状态必须置0 */
	MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_ON);/* 复位时置1 */
	
	MC14094_Updata();// 更新输出缓冲区
    
    irq_stop_relay_motion();
    
    updata_port(&cur_port);/* 更新端口状态 */
    
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            dcw_big_cap_keep_dis_charge();
        }
    }
    
	relay_motion();/* 继电器进入就绪状态 */
	
	close_sine();/* 关闭正弦波 */
	
	LED_ALL = LED_OFF;/* 关闭LED指示灯 */
//	BUZZER = BUZZER_OFF;
	
	if(!(cur_step == 1 && sys_par.output_delay > 0))
	{
		plc_signal_cmd(PLC_SIG_RESET);
	}
	HIGH = 0;/* 高压输出表示置0 */
}

void set_sine_hz(const int8_t n)
{
	#define COUNT_BASE(F) (10000*(F)/50)
	
	uint32_t temp_w = 0;
	uint16_t freq_buf[10] = {50, 50, 60, 100, 150, 200, 250, 300, 350, 400};
	
	temp_w = COUNT_SIN_FREQ_WORD(freq_buf[n%10]);
	
	cpld_base = COUNT_BASE(freq_buf[n%10]);
	temp_w |= CPLD_W;
	cpld_write(temp_w);
	cpld_write(CPLD_START_SINEWAVE);
	cpld_write(CPLD_START_SINEWAVE);
}
void open_sine(float freq)
{
	#define COUNT_BASE(F) (10000*(F)/50)
	
	uint32_t temp_w = 0;
	
	temp_w = COUNT_SIN_FREQ_WORD(freq) + ROUND;
	
	cpld_base = COUNT_BASE(freq);
	temp_w |= CPLD_W;
	cpld_write(temp_w);
	cpld_write(CPLD_START_SINEWAVE);
	cpld_write(CPLD_START_SINEWAVE);
}
void close_sine(void)
{
	cpld_write(CPLD_STOP_SINEWAVE);
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
