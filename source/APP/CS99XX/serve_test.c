/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：serve_test.c
 * 摘  要  ：测试服务子程序，提供测试程序的入口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#define _SERVE_TEST_GLOBALS
#include    <string.h>
#include    <math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include 	"mb_server.h"
#include    "cs99xx_vref.h"
#include 	"MC14094.h"
#include	"RTC_Config.h"
#include    "stdio.h"
#include    "cs99xx_result.h"
#include 	"SysTemConfig.h"
#include    "on_chip_flash_api.h"
#include    "cs99xx_err_log.h"
#include	"Timer_Config.h"
#include    "cs99xx_it.h"
#include    "cs99xx_buzzer.h"
#include    "cs99xx_plc.h"
#include    "irq.h"
#include    "dc_module.h"
#include    "cs99xx_collect.h"

/*
 * 函数名：record_exception_scene
 * 描述  ：记录异常现场
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void record_exception_scene(void)
{
    err_vol_bak = vol_ave;
    err_cur_bak = cur_ave;
    err_res_bak = res_ave;
    
    err_real_bak = real_ave;
    err_cap_bak  = cap_ave;
}
/*
 * 函数名：record_exception_scene
 * 描述  ：恢复异常现场
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void recover_exception_scene(void)
{
    vol_ave = err_vol_bak;
    cur_ave = err_cur_bak;
    res_ave = err_res_bak;
    
    real_ave = err_real_bak;
    cap_ave = err_cap_bak;
}

/*
 * 函数名：open_hv
 * 描述  ：设置高压输出基准
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void open_hv(void)
{
    if(DC_GR_EN && (g_cur_step->one_step.com.mode==GR))
    {
        return;
    }
    
    DAC_Vol[0] =  (u16)(cur_vol * cur_dac_k + cur_dac_b);
}

/*
 * 函数名：close_hv
 * 描述  ：清零高压输出基准
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void close_hv(void)
{
    disable_sample_task();/* 关闭采样任务 */
    
    if(cur_mode != BBD)
    {
        DAC_Vol[DAC_OUTPUT_VREF] = 0;
    }
    else
    {
        DAC_Vol[DAC_OUTPUT_VREF] = 70;
    }
}

void cur_cylinder_ctrl_stop(void)
{
    CYLINDER_CTRL(2000);
}

/*
 * 函数名： dis_exception_info
 * 描述  ：显示异常信息
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_exception_log(void)
{
}

/*
 * 函数名：set_offset_mode
 * 描述  ：设置偏移模式
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
//static void set_offset_mode(void)
//{
//}



/*
 * 函数名：set_offset_mode
 * 描述  ：设置偏移模式
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
//static void serve_test_more(void)
//{
//}
/*
 * 函数名：serve_test_shortcut
 * 描述  ：设置偏移模式
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void serve_test_shortcut(void)
{
}




/*
 * 函数名：serve_count
 * 描述  ：系统事件计数
 * 输入  ：n 事件分类
 * 输出  ：无
 * 返回  ：无
 */
void serve_count(const int8_t n)
{
	switch(n)
	{
		case KEY_START_COUNT:/* 对START键计数 */
			sys_flag.key_start_times++;
			break;
		case KEY_STOP_COUNT:/* 对STOP键计数 */
			sys_flag.key_stop_times++;
			break;
		case AMP_COUNT:/* 对AMP计数 */
			sys_flag.amp_times++;
			break;
		case BOOT_COUNT:/* 对BOOT计数 */
			sys_flag.boot_times++;
			break;
	}
    //注释掉保存功能，看是否还会出错 苏州晟成测了1天多发现数据丢失
//     save_sys_flag();
}

/*
 * 函数名：count_gear_vol
 * 描述  ：计算IR的电压档位
 * 输入  ：cur 没有用
 * 输出  ：无
 * 返回  ：无
 */
void count_gear_vol(uint32_t cur)
{
    switch(type_spe.ir_vol_range)
    {
        case IR_1kV:
        case IR_1_5kV:
        {
            if(cur_vol < 150)
            {
                cur_vol_gear = 0;
            }
            else if(cur_vol < 250)
            {
                cur_vol_gear = 1;
            }
            else if(cur_vol < 350)
            {
                cur_vol_gear = 2;
            }
            else if(cur_vol < 450)
            {
                cur_vol_gear = 3;
            }
            else if(cur_vol < 550)
            {
                cur_vol_gear = 4;
            }
            else if(cur_vol < 650)
            {
                cur_vol_gear = 5;
            }
            else if(cur_vol < 750)
            {
                cur_vol_gear = 6;
            }
            else if(cur_vol < 850)
            {
                cur_vol_gear = 7;
            }
            else if(cur_vol < 950)
            {
                cur_vol_gear = 8;
            }
            else if(cur_vol < 1050)
            {
                cur_vol_gear = 9;
            }
            else if(cur_vol < 1150)
            {
                cur_vol_gear = 10;
            }
            else if(cur_vol < 1250)
            {
                cur_vol_gear = 11;
            }
            else if(cur_vol < 1350)
            {
                cur_vol_gear = 12;
            }
            else if(cur_vol < 1450)
            {
                cur_vol_gear = 13;
            }
            else
            {
                cur_vol_gear = 14;
            }
            
            break;
        }
        case IR_2_5kV:
        {
            if(cur_auto == AUTO_SHIFT_OFF)
            {
                gear_up_limit = 0;
            }
            
            if(cur_vol < 1500)
            {
				if(cur_vol < 100)
				{
					cur_vol_gear = 0;
				}
				else
				{
					cur_vol_gear = ((cur_vol - 100) / 100 + 0) % 15;
				}
            }
            else
            {
                cur_vol_gear = 14 + (cur_vol - 1500) / 500;
            }
            break;
        }
        case IR_5kV:
        {
            if(cur_auto == AUTO_SHIFT_OFF)
            {
                gear_up_limit = 0;
            }
            
            if(type_spe.amp_type == PWM_AMP)
            {
				if(cur_vol < 500)
				{
					cur_vol_gear = 0;
				}
				else
				{
					cur_vol_gear = (cur_vol - 500) / 500;
				}
            }
            else
            {
                if(cur_vol < 1500)
                {
					if(cur_vol < 100)
					{
						cur_vol_gear = 0;
					}
					else
					{
						cur_vol_gear = (cur_vol - 100) / 100;
					}
                }
                else
                {
                    cur_vol_gear = 14 + (cur_vol - 1500) / 500;
                }
            }
            
            if(cur_vol_gear < 2)
            {
                cur_vol_gear = 2;
            }
            break;
        }
        case IR_10kV:
        {
            if(cur_auto == AUTO_SHIFT_OFF)
            {
                gear_up_limit = 0;
            }
            
            if(cur_vol < 5250)
            {
                cur_vol_gear = 0;
            }
            else if(cur_vol < 5750)
            {
                cur_vol_gear = 1;
            }
            else if(cur_vol < 6250)
            {
                cur_vol_gear = 2;
            }
            else if(cur_vol < 6750)
            {
                cur_vol_gear = 3;
            }
            else if(cur_vol < 7250)
            {
                cur_vol_gear = 4;
            }
            else if(cur_vol < 7750)
            {
                cur_vol_gear = 5;
            }
            else if(cur_vol < 8250)
            {
                cur_vol_gear = 6;
            }
            else if(cur_vol < 8750)
            {
                cur_vol_gear = 7;
            }
            else if(cur_vol < 9250)
            {
                cur_vol_gear = 8;
            }
            else
            {
                cur_vol_gear = 9;
            }
            
            break;
        }
    }
    
    if(cur_auto == AUTO_SHIFT_OFF)// && cur_ir_dis_gear == 1)
    {
        switch(cur_gear)
        {
            case IR_10MOHM:
                gear_up_limit = 10;//10 * 10;
                break;
            case IR_100MOHM:
                gear_up_limit = 100;//10 * 100;
                break;
            case IR_1GOHM:
                gear_up_limit = 1 * 1000; // 999M
                break;
            case IR_10GOHM:
                gear_up_limit = 10 * 1000;
                break;
            case IR_100GOHM:
                gear_up_limit = 1 * 1000 * 100;
                break;
        }
        
        if(gear_up_limit > IR_RES_H)
        {
            gear_up_limit = IR_RES_H;
        }
    }
}

/*
 * 函数名：transform_gr_cur_string
 * 描述  ：将GR电流转换为字符串
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void transform_gr_cur_string(void)
{
	switch(type_spe.gr_cur_unit)
	{
		case CUR_U_A:
            if(cur_ave < 100.00)
			{
                mysprintf((uint8_t*)output_buf, NULL, 2 + 150, cur_ave * 100);
            }
            else
            {
                mysprintf((uint8_t*)output_buf, NULL, 1 + 150, cur_ave * 10);
            }
			break;
		case CUR_U_mA:
            if(cur_ave < 1000)
			{
				mysprintf((uint8_t*)output_buf, NULL, 1 + 150, cur_ave*10);
			}
			else
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, cur_ave*100);
			}
			break;
	}
}

/*
 * 函数名：transform_test_vol_string
 * 描述  ：将ACW DCW IR CC BBD ...电压转换为字符串
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void transform_test_vol_string(void)
{
	switch(cur_mode)
	{
		case ACW:
		{
			if(type_spe.acw_vol_range == ACW_20kV
				|| type_spe.acw_vol_range == ACW_30kV)
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave / 10.0);
			}
			else
			{
				if(vol_ave < 10000)
				{
					mysprintf((uint8_t*)output_buf, NULL, 3 + 150, vol_ave);
				}
				else
				{
					mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave/10.0);
				}
			}
			break;
		}
		case DCW:
		{
			if(type_spe.dcw_vol_range == DCW_20kV)
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave / 10.0);
			}
			else
			{
				if(vol_ave < 10000)
				{
					mysprintf((uint8_t*)output_buf, NULL, 3 + 150, vol_ave);
				}
				else
				{
					mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave/10.0);
				}
			}
			break;
		}
        case CC:
		case IR:
		case BBD:
		{
            if(vol_ave < 10000)
            {
                mysprintf((uint8_t*)output_buf, NULL, 3 + 150, vol_ave);
            }
            else
            {
                mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave/10.0);
            }
			break;
		}
		case GR:
		{
			transform_gr_cur_string();
			break;
		}
	}
}

/*
 * 函数名：transform_test_time_string
 * 描述  ：将测试时间转换为字符串
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void transform_test_time_string(uint16_t time)
{
    mysprintf((uint8_t*)time_buf, NULL, 1 + 150, time);
}
/*
 * 函数名：transform_test_time_string
 * 描述  ：将测试电流 电阻 电容 转换为字符串
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void transform_test_loop_string(void)
{
    if(OFFSET_BBD == 0)
    {
        real_buf[0] = 0;
    }
    else
    {
        strcpy((char*)real_buf, "-----");
    }
    
    switch(cur_mode)
    {
        case ACW:
        {
            mysprintf((uint8_t*)loop_buf, NULL, ac_gear[cur_gear].decs + 150, cur_ave + ROUND);
            if(cur_real_cur > 0)
            {
                mysprintf((uint8_t*)real_buf, NULL, ac_gear[cur_gear].decs + 150, real_ave + ROUND);
            }
            break;
        }
        case DCW:
        {
            mysprintf((uint8_t*)loop_buf, NULL, dc_gear[cur_gear].decs + 150, cur_ave + ROUND);
            break;
        }
        case IR:
        {
            if(OFFSET_BBD == 0)
            {
                transform_test_ir_res_loop();
            }
            else
            {
                strcpy((char*)loop_buf, "-----");
            }
            break;
        }
        case GR:
        {
            transform_test_gr_vol();
            break;
        }
        case BBD:
        {
            /* nA/(v*2*PI*Hz) = nF */
// 			cap_ave = cur_ave*1000/(vol_ave*2*PI*50);
            
            uint8_t temp = 0;
            uint8_t temp1 = 0;
            
            temp = COUNT_FLOAT_DECS(cap_ave);
            temp1 = COUNT_FLOAT_DECS(cur_cap);
            
			if(ERR_NUM != ERR_NONE)
			{
				sprintf((char*)loop_buf, "----");
			}
			else
			{
				mysprintf((uint8_t*)loop_buf, NULL, temp + 150 , cap_ave * pow(10, temp)  + ROUND);
			}
			
            if(OFFSET_BBD == 0)
            {
                mysprintf((uint8_t*)real_buf, NULL, temp1 + 150, cur_cap * pow(10, temp1) + ROUND);
            }
            break;
        }
        case CC:
        {
            mysprintf((uint8_t*)loop_buf, NULL, ac_gear[cur_gear].decs + 150, cur_ave + ROUND);
            break;
        }
    }
}

void confirm_contact_segment_s(void)
{
	if(cur_vol <= type_spe.contact_segment_point)
	{
		contact_segment = 0;
	}
	else
	{
		contact_segment = 1;
	}
}
/*
 *name: confirm_vol_segment
 *fun : 确定电压段
 * 
 */
void confirm_vol_segment(void)
{
	//双触点变压器基准不分段 触点选择根据触点分界电压决定
	if(type_spe.transformer_point == DOUBLE_POINT)
	{
		confirm_contact_segment_s();
		vol_segment = 1;
	}
	/* 单触点变压器 */
	else
	{
		if(g_cur_file->work_mode == G_MODE)
		{
			vol_segment = 1;
		}
		else
		{
			confirm_vol_segment_s();
		}
	}
}
/*
 * 函数名：load_cur_frequency
 * 描述  ：计算当前步输出频率
 * 输入  ：frequency 频率设置值
 * 输出  ：无
 * 返回  ：返回对应的频率值 50hz--400hz 或 40.0hz - 400.0hz
 */
uint16_t load_cur_frequency(uint16_t frequency)
{
    uint16_t freq_buf[10] = {50,50,60,100,150,200,250,300,350,400};
    uint16_t temp = 0;
    
    if(cur_mode == GR)
    {
        temp = freq_buf[frequency % 10];/* 开正弦波 */
    }
    else
    {
        if(type_spe.hz_type == HZ_TYPE_GRADE)
        {
            temp = freq_buf[frequency % 10];/* 开正弦波 */
        }
        else
        {
            temp = frequency/10.0;
        }
    }
    
    return temp;
}
uint8_t get_gr_gear(void)
{
    uint8_t gear = 0;
    
    if(GR_VOL_DROP_EN == DISABLE)
    {
        if(DC_GR_EN == ENABLE)
        {
            gear = 1;
        }
        else
        {
            gear = 0;
        }
    }
    else
    {
        if(type_spe.gr_cur_divide_gear_en == ENABLE)
        {
            if(cur_vol > 1000)
            {
                gear = DC_GR_12V_1000mA;
            }
            else
            {
                gear = DC_GR_12V_100mA;
            }
        }
        else
        {
            gear = DC_GR_12V;
        }
    }
    
    return gear;
}

void load_ratio(uint8_t mode)
{
    switch(mode)
    {
        case ACW:
            cur_dac_k = ratio_acw.dac_k[vol_segment];
            cur_dac_b = ratio_acw.dac_b[vol_segment];
            cur_adc_k = ratio_acw.adc_v_k[vol_segment];
            cur_adc_b = ratio_acw.adc_v_b[vol_segment];
            cur_arc_k = ratio_acw.arc_k;
			cur_adc_cur_k = ratio_acw.adc_cur_k[cur_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            SAFE_FLOAT_VOLU_CAL(cur_arc_k, cur_arc_k);
            break;
        case DCW:
            cur_dac_k = ratio_dcw.dac_k[vol_segment];
            cur_dac_b = ratio_dcw.dac_b[vol_segment];
            cur_adc_k = ratio_dcw.adc_v_k[vol_segment];
            cur_adc_b = ratio_dcw.adc_v_b[vol_segment];
            cur_arc_k = ratio_dcw.arc_k;
			cur_adc_cur_k = ratio_dcw.adc_cur_k[cur_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            SAFE_FLOAT_VOLU_CAL(cur_arc_k, cur_arc_k);
            break;
        case IR:
            cur_dac_k = ratio_ir.dac_k[vol_segment];
            cur_dac_b = ratio_ir.dac_b[vol_segment];
            cur_adc_k = ratio_ir.adc_v_k[vol_segment];
            cur_adc_b = ratio_ir.adc_v_b[vol_segment];
            cur_res_k = ratio_ir.res_k[cur_gear-1][cur_vol_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            SAFE_FLOAT_VOLU_CAL(cur_res_k, cur_res_k);
            break;
        case GR:
            cur_dac_k = ratio_gr.dac_k[cur_gear];
            cur_dac_b = ratio_gr.dac_b[cur_gear];
            cur_adc_k = ratio_gr.adc_i_k[cur_gear];
            cur_adc_b = ratio_gr.adc_i_b[cur_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            break;
        case BBD:
            /* bbd的系数使用acw的 */
            cur_dac_k = ratio_acw.dac_k[vol_segment];
            cur_dac_b = ratio_acw.dac_b[vol_segment];
            cur_adc_k = ratio_acw.adc_v_k[vol_segment];
            cur_adc_b = ratio_acw.adc_v_b[vol_segment];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            break;
        case CC:
            /* 人为的调到500v */
            cur_dac_k = 3.5;
            cur_dac_b = 42;
            cur_adc_k = 1.8;
            cur_adc_b = 0;
            
            if(cur_intensity == CC_CUR_HIGH)
            {
                cur_dac_k = 3.5;
                cur_dac_b = 42;
            }
            else if(cur_intensity == CC_CUR_MID)
            {
                cur_dac_k = 3;
                cur_dac_b = 42;
            }
            else if(cur_intensity == CC_CUR_LOW)
            {
                cur_dac_k = 2;
                cur_dac_b = 42;
            }
            else
            {
                cur_dac_k = 2;
                cur_dac_b = 42;
            }
        break;
    }
}

#include "dc_module_bridge.h"
void set_dc_gr_module_info(void)
{
    int32_t i = 0;
    uint16_t t_m_output[4] = {0,0,0,0};/* 每个成员都表示单个模块的输出电流值 */
    uint8_t n = 0;/* 选择模块的工作数目 */
    uint16_t temp_c = 0;
    
    if(cur_mode == GR)
    {
        if(DC_GR_EN)
        {
            /* 根据设置的输出电流计算需要几个模块参与测试 */
            if(cur_vol > ONE_DCGR_MAX_CUR)
            {
                n = sys_par.dc_module_using_info.count;
//                 if(cur_vol % ONE_DCGR_MAX_CUR != 0)
//                 {
//                     n = cur_vol / ONE_DCGR_MAX_CUR + 1;
//                 }
//                 else
//                 {
//                     n = cur_vol / ONE_DCGR_MAX_CUR;
//                 }
            }
            else
            {
                n = 1;
            }
            
            select_dc_gr_n_moduls(n);
            
            temp_c = cur_vol / n;
            
            /* 计算每个模块的输出电流值 */
            for(i = 0; i < n - 1; i++)
            {
                t_m_output[i] = temp_c;
            }
            
            t_m_output[i] = cur_vol - (temp_c * (n - 1));
            
            for(i = 0; i < n; i++)
            {
                set_dc_gr_module_output_da(sys_par.dc_module_using_info.addr[i], t_m_output[i]);
            }
            
            /* 设置电压档位，测试方法 */
            set_dc_gr_vol_gear(cur_gear);
            set_dc_gr_test_method(cur_method);
            
            /* 将设置值发送给模块 */
            send_dc_module_cmd(DC_MODULE_CMD_SET_DATA, 0);
        }
    }
}
/*
 * 函数名：load_data
 * 描述  ：加载测试数据
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void load_data(void)
{
	UN_STRUCT *pun = &g_cur_step->one_step;
	
	cur_step = pun->com.step;/* 更新当前步号 */
	cur_mode = pun->com.mode;/* 更新当前步模式 */
    
    /* 偏移测试无输出延时 */
    zeo_t = 0;
    
    dly_t = sys_par.output_delay;/* 输出延时 */
    
    result_r_queue[result_r_queue_h++] = cur_step;
	
    if(result_r_queue_h >= RES_QUEUE_MAX)
    {
        result_r_queue_h = 0;
    }
    
	switch(cur_mode)
	{
		case ACW:
		{
			if(g_cur_file->work_mode == N_MODE)
			{
				one_t = pun->acw.rise_time + zeo_t;
				two_t = pun->acw.rise_time + pun->acw.testing_time + zeo_t;
				thr_t = pun->acw.rise_time + pun->acw.testing_time + pun->acw.fall_time + zeo_t;
				for_t = pun->acw.rise_time + pun->acw.testing_time + pun->acw.fall_time + pun->acw.interval_time + zeo_t;
                
                int_t = pun->acw.interval_time;
                cha_t = 0;
			}
			else if(g_cur_file->work_mode == G_MODE)
			{
                if(cur_step == 1)
                {
                    one_t = pun->acw.rise_time + zeo_t;
                }
                else
                {
                    one_t = 0 + zeo_t;
                }
                
				two_t = one_t + pun->acw.interval_time;/* 缓变时间 */
				thr_t = two_t + pun->acw.testing_time;
                
                if(cur_step == g_cur_file->total)
                {
                    for_t = thr_t + pun->acw.fall_time;
                }
                else
                {
                    for_t = thr_t + 0;
                }
                
                cha_t = pun->acw.interval_time;
                int_t = cha_t;
			}
            
            tes_t = pun->acw.testing_time;
            ris_t = pun->acw.rise_time;
            fal_t = pun->acw.fall_time;
			
			steps_con = pun->acw.steps_cont;
			steps_pass = pun->acw.steps_pass;
			
			cur_vol = pun->acw.testing_voltage;
			cur_gear = pun->acw.gear_i;
			cur_high = pun->acw.upper_limit;
			cur_low = pun->acw.lower_limit;
			cur_frequency_gear = pun->acw.output_freq;
			cur_real_cur = pun->acw.ac_real_cur; /* 当前真实电流 */
			cur_arc_gear = pun->acw.arc_sur;/* 电弧侦测 */
			cur_port = pun->acw.port;
            
            cur_frequency = load_cur_frequency(cur_frequency_gear);
            
			cur_offset_result = pun->acw.offset_result;
			/* 如果偏移测试成功 或 手动偏移打开 */
			if(pun->acw.offset_result == OFFSET_PASS
				|| g_custom_sys_par.offset_set_en)
			{
				cur_offset = pun->acw.offset_cur;/* 电流或电阻偏移值 */
				cur_real_offset = pun->acw.offset_real;/* 真实电流偏移 */
			}
			/* 如果偏移测试未做或失败 */
			else
			{
				cur_offset = 0;
				cur_real_offset = 0;
			}
            
			if(type_spe.acw_vol_range == ACW_20kV
				|| type_spe.acw_vol_range == ACW_30kV)
			{
				cur_vol *= 10;
			}
			
            vol_ave = cur_vol;
            /* 偏移测试 */
            if(OFFSET_BBD > 0)
            {
                cur_ave = cur_offset;
                real_ave = cur_real_offset;
            }
            /* 正常测试 */
            else
            {
                cur_ave = cur_high;
                real_ave = cur_real_cur;
            }
            
            g_dis_time = tes_t;
			
            
			switch(cur_gear)
			{
				case AC_200uA:
					cur_gear_comm = AC_0_GEAR_COMM;
					break;
				case AC_2mA:
					cur_gear_comm = AC_1_GEAR_COMM;
					break;
				case AC_10mA:
				case AC_20mA:
					cur_gear_comm = AC_2_GEAR_COMM;
					break;
				case AC_50mA:
				case AC_100mA:
					cur_gear_comm = AC_3_GEAR_COMM;
					break;
			}
            
            confirm_vol_segment();/* 确定电压段 */
			
			if(cur_adc_cur_k != 0)
			{
				g_ad_dog = (cur_high + 1) / cur_adc_cur_k;
//				ADC_WatchdogConfig(g_ad_dog);
			}
			break;
		}
		case DCW:
		{
			if(g_cur_file->work_mode == N_MODE)
			{
                if(type_spe.dcw_big_cap_en == DISABLE)
                {
                    one_t = pun->dcw.rise_time + zeo_t;
                    two_t = one_t + pun->dcw.testing_time;
                    thr_t = two_t + pun->dcw.fall_time;
                    for_t = thr_t + pun->dcw.interval_time;
                }
                /* 大电容测试 */
                else
                {
                    one_t = pun->dcw.rise_time + zeo_t;
                    two_t = one_t + pun->dcw.stab_time;
                    thr_t = two_t + pun->dcw.testing_time;
                    for_t = thr_t + pun->dcw.fall_time;
                    fiv_t = for_t + pun->dcw.interval_time;
                }
                
				int_t = pun->dcw.interval_time;
                cha_t = 0;
			}
			else if(g_cur_file->work_mode == G_MODE)
			{
                if(cur_step == 1)
                {
                    one_t = pun->dcw.rise_time + zeo_t;
                }
                else
                {
                    one_t = 0 + zeo_t;
                }
				two_t = one_t + pun->dcw.interval_time;/* 缓变时间 */
				thr_t = two_t + pun->dcw.testing_time;/* 测试 */
                
                if(cur_step == g_cur_file->total)
                {
                    for_t = thr_t + pun->dcw.fall_time;/* 下降时间 */
                }
                else
                {
                    for_t = thr_t + 0;
                }
                
                cha_t = pun->dcw.interval_time;
                int_t = cha_t;
			}
            
			tes_t = pun->dcw.testing_time;
            ris_t = pun->dcw.rise_time;
            fal_t = pun->dcw.fall_time;
            sta_t = pun->dcw.stab_time;
			
			steps_con = pun->dcw.steps_cont;
			steps_pass = pun->dcw.steps_pass;
			
			cur_vol  = pun->dcw.testing_voltage;
			cur_gear = pun->dcw.gear_i;
			cur_high = pun->dcw.upper_limit;
			cur_low  = pun->dcw.lower_limit;
			cur_charge_cur = pun->dcw.charge_cur;	/* 当前充电电流 */
			cur_delay_t    = pun->dcw.delay_time;
			cur_arc_gear   = pun->dcw.arc_sur;/* 电弧侦测 */
			cur_port = pun->dcw.port;
            cur_output_impedan = pun->dcw.output_impedance;/* 输出阻抗 */
			cur_frequency = 400;/* 选择400赫兹 */
            
			cur_offset_result = pun->dcw.offset_result;
			
			/* 如果偏移测试成功 */
			if(pun->dcw.offset_result == OFFSET_PASS
				|| g_custom_sys_par.offset_set_en)
			{
				cur_offset = pun->dcw.offset_cur;/* 电流或电阻偏移值 */
			}
			/* 如果偏移测试未做或失败 */
			else
			{
				cur_offset = 0;
			}
			
			if(type_spe.dcw_vol_range == DCW_20kV)
			{
				cur_vol *= 10;
			}
			
			vol_ave = cur_vol;
            /* 偏移测试 */
            if(OFFSET_BBD > 0)
            {
                cur_ave = cur_offset;
            }
            /* 正常测试 */
            else
            {
                cur_ave = cur_high;
            }
            g_dis_time = tes_t;
            
			switch(cur_gear)
			{
				case DC_2uA:
					cur_gear_comm = DC_0_GEAR_COMM;
					break;
				case DC_20uA:
					cur_gear_comm = DC_1_GEAR_COMM;
					break;
				case DC_200uA:
					cur_gear_comm = DC_2_GEAR_COMM;
					break;
				case DC_2mA:
					cur_gear_comm = DC_3_GEAR_COMM;
					break;
				case DC_10mA:
				case DC_20mA:
					cur_gear_comm = DC_4_GEAR_COMM;
					break;
				case DC_50mA:
					cur_gear_comm = DC_5_GEAR_COMM;
					break;
			}
            
            cur_real_gear_comm = cur_gear;
			
            confirm_vol_segment();/* 确定电压段 */
			
			if(cur_adc_cur_k != 0)
			{
				g_ad_dog = (cur_high + 1) / cur_adc_cur_k;
//				ADC_WatchdogConfig(g_ad_dog);
			}
			break;
		}
		case IR:
		{
			one_t = pun->ir.rise_time + zeo_t;
			two_t = one_t + pun->ir.testing_time;
			thr_t = two_t + 0;
			for_t = thr_t + pun->ir.interval_time;
            
			tes_t = pun->ir.testing_time;
            ris_t = pun->ir.rise_time;
            fal_t = 0;
            int_t = pun->ir.interval_time;
            cha_t = 0;
			
			steps_con = pun->ir.steps_cont;
			steps_pass = pun->ir.steps_pass;
			cur_frequency_gear = 9;/* 选择400赫兹 */
			cur_frequency = 400;/* 选择400赫兹 */
			cur_vol = pun->ir.testing_voltage;
			cur_auto = pun->ir.auto_shift_gears;
			cur_high = pun->ir.upper_limit;
			cur_low = pun->ir.lower_limit;
			cur_delay_t = pun->ir.delay_time;
			cur_port = pun->ir.port;
			cur_ir_rang_h = IR_RES_H;
            
            vol_ave = cur_vol;
            res_ave = cur_low;
            res_ave_comm = cur_low * 1000;
            g_dis_time = tes_t;
            cur_offset_result = 3;
            
            if(g_cur_type->num == CS9922BX_DZ)/* 加了一个小数点 */
            {
                cur_high /= 10.0;
                cur_low /= 10.0;
                cur_ir_rang_h = IR_RES_H / 10.0;
            }
            
			/* 计算出电阻的档位 */
			if(cur_low < 10)
			{
				cur_gear = 1;
			}
			else if(cur_low < 100)
			{
				cur_gear = 2;
			}
			else if(cur_low < 1000)
			{
				cur_gear = 3;
			}
			else if(cur_low < 10*1000)
			{
				cur_gear = 4;
			}
			else
			{
				cur_gear = 5;
			}
            
            /* 10kV的档位从3开始 */
            if(type_spe.ir_vol_range == IR_10kV)
            {
                cur_gear_min = 3;
            }
            else
            {
                cur_gear_min = 1;
            }
            
            if(cur_gear < cur_gear_min)
            {
                cur_gear = cur_gear_min;
            }
            
            if(type_spe.ir_res_h <= 10*1000.0)
            {
                cur_gear_max = 4;
            }
            else
            {
                cur_gear_max = 5;
            }
            
            if(cur_gear > cur_gear_max)
            {
                cur_gear = cur_gear_max;
            }
            
            /* 当电压小于200V时电阻档位最大值为4 */
            if(cur_vol < 200)
            {
                if(cur_gear_max == 5)
                {
                    cur_gear_max = 4;
                }
            }
            
			cur_gear_comm = cur_gear;
			cur_ir_dis_gear = cur_gear;/* ir显示档位 只有在非自动挡才用到 */
            cur_ad_vol = 0;
            g_ir_dly = 500;/* 启动后首先要进入延时 */
			
			cur_ir_dis_gear = cur_gear;/* ir显示档位 只有在非自动挡才用到 */
            cur_ir_dis_gear_0 = cur_ir_dis_gear - 1;
            cur_ir_dis_gear_1 = cur_ir_dis_gear + 1;
            
            count_gear_vol(cur_vol);/* 确定电压的档位 */
            
            confirm_vol_segment();/* 确定电压段 */
			g_ir_gear_hold = sys_par.ir_gear_hold;
			
			if(tes_t >= 600)
			{
				app_flag.absorptance = 1;
			}
			else
			{
				app_flag.absorptance = 0;
			}
			
			ir_15s_res = 0;
			ir_60s_res = 0;
			ir_absorptance = 0;
			break;
		}
		case GR:
		{
			one_t = 0 + zeo_t;
			two_t = one_t + pun->gr.testing_time;
			thr_t = two_t + 0;
			for_t = thr_t + pun->gr.interval_time;
            
			tes_t = pun->gr.testing_time;
            ris_t = 0;
            fal_t = 0;
            int_t = pun->gr.interval_time;
            cha_t = 0;
			
			steps_con = pun->gr.steps_cont;
			steps_pass = pun->gr.steps_pass;
			
			cur_vol = pun->gr.testing_cur;
			cur_gear = pun->gr.voltage_gear;
			cur_high = pun->gr.upper_limit;
			cur_low = pun->gr.lower_limit;
			cur_frequency_gear = pun->gr.output_freq;
			cur_method = pun->gr.test_method;
            cur_frequency = load_cur_frequency(cur_frequency_gear);
			
			cur_offset_result = pun->gr.offset_result;
			/* 如果偏移测试成功 */
			if(pun->gr.offset_result == OFFSET_PASS
				|| g_custom_sys_par.offset_set_en)
			{
				cur_offset = pun->gr.offset_res;/* 电流或电阻偏移值 */
			}
			/* 如果偏移测试未做或失败 */
			else
			{
				cur_offset = 0;
			}
			
            cur_ave = cur_vol/100.0;
            /* 偏移测试 */
            if(OFFSET_BBD > 0)
            {
                res_ave = cur_offset;
            }
            /* 正常测试 */
            else
            {
                res_ave = cur_high;
            }
			
            res_ave_comm = cur_high;
            g_dis_time = tes_t;
            
			cur_gear = get_gr_gear();
            set_dc_gr_module_info();
			break;
		}
		case BBD:
		{
            //BBD在测试GETC时使用3s
            if(OFFSET_BBD)
            {
                one_t = 0;
                two_t = 30;
                thr_t = two_t;
                for_t = thr_t;
            }
            /* BBD正常测试时使用1s */
            else
            {
                one_t = 0;
                two_t = 10;
                thr_t = two_t;
                for_t = thr_t;
            }
            
			tes_t = two_t - one_t;
            ris_t = 0;
            fal_t = 0;
            int_t = 0;
            cha_t = 0;
			
			steps_con = pun->bbd.steps_cont;
			steps_pass = pun->bbd.steps_pass;
			
			cur_vol = pun->ir.testing_voltage;
			cur_cap = g_cur_step->one_step.bbd.cap_value;
            cur_getc_result = g_cur_step->one_step.bbd.get_cs_ok;
			cur_gear = g_cur_step->one_step.bbd.gear;
            cur_open_ratio = g_cur_step->one_step.bbd.open_ratio;
            cur_short_ratio = g_cur_step->one_step.bbd.short_ratio;
			cur_port = pun->bbd.port;
// 			cur_frequency = 400;/* 400Hz */
			cur_frequency_gear = g_cur_step->one_step.bbd.output_freq;/* 400Hz */
			
            cur_frequency = 100;/* 100Hz *///load_cur_frequency(cur_frequency_gear);
            vol_ave = cur_vol;
            confirm_vol_segment();/* 确定电压段 */
			bbd_load_gear();
			break;
		}
		case CC:
		{
            if(zeo_t <= 1)
            {
                zeo_t = 1;
            }
            
			if(g_cur_file->work_mode == N_MODE)
			{
				one_t = zeo_t + 0;
				two_t = one_t + 1;
				thr_t = two_t + 0;
				for_t = thr_t + 0;
                
                int_t = 0;
                cha_t = 0;
			}
            
            tes_t = two_t - one_t;
            ris_t = 0;
            fal_t = 0;
			
			steps_con = pun->cc.steps_cont;
			steps_pass = pun->cc.steps_pass;
			
			cur_vol = pun->cc.testing_voltage;
			cur_gear = pun->cc.gear_i;
			cur_high = pun->cc.upper_limit;
			cur_low = pun->cc.lower_limit;
			cur_frequency_gear = pun->cc.output_freq;
			cur_real_cur = pun->cc.ac_real_cur; /* 当前真实电流 */
			cur_intensity = pun->cc.cur_intensity;/* 电流强度 */
			cur_port = pun->cc.port;
            
			cur_frequency = load_cur_frequency(cur_frequency_gear);
            
            vol_ave = cur_vol;
            cur_ave = cur_low;
            
			switch(cur_gear)
			{
				case AC_200uA:
					cur_gear_comm = AC_0_GEAR_COMM;
					break;
				case AC_2mA:
					cur_gear_comm = AC_1_GEAR_COMM;
					break;
				case AC_10mA:
				case AC_20mA:
					cur_gear_comm = AC_2_GEAR_COMM;
					break;
				case AC_50mA:
				case AC_100mA:
					cur_gear_comm = AC_3_GEAR_COMM;
					break;
			}
            
            if(cur_vol > 500)
            {
                vol_segment = 1;
            }
            else
            {
                vol_segment = 0;
            }
			break;
		}
	}
	
	cur_gear_bak = cur_gear;
    load_ratio(cur_mode);
	
    
    transform_test_vol_string();
    transform_test_loop_string();
    transform_test_time_string(tes_t);
}
///////////////////////////////////////////////////////////
//                                                       //
//				    测试程序内部接口                     //
//                                                       //
///////////////////////////////////////////////////////////
/*
 * 函数名： install_test_irq_fun
 * 描述  ：初始化测试状态机函数 N模式 G模式 大电容 由于每种模式都有不同的测试流程 因此需要单独
 *         实现不同的状态机。
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void install_test_irq_fun(void)
{
    /* 大电容测试 */
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            test_irq_fun = test_big_cap_irq;
            cs99xx_test_fun = cs99xx_big_cap_test;
            return;
        }
    }
    
	/* N模式 */
	if(g_cur_file->work_mode == N_MODE)
	{
		test_irq_fun = test_irq;
		cs99xx_test_fun = cs99xx_test;
	}
	/* G模式 */
	else if(g_cur_file->work_mode == G_MODE)
	{
        test_irq_fun = test_g_irq;
		cs99xx_test_fun = cs99xx_g_test;
	}
}

/*
 * 函数名：test_details
 * 描述  ：测试细节
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void test_details(void)
{
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
			cur_status = ST_TESTING;
			if(test_flag.do_only_one)
			{
				test_flag.do_only_one = 0;
				test_ready();
			}
            break;
        }
		case STAGE_RISE:/* 第一阶段 电压上升 */
			cur_status = ST_VOL_RISE;
			HIGH = 1;
			
			test_dis();
			break;
		case STAGE_TEST:/* 第二阶段 正在测试 */
			cur_status = ST_TESTING;
            cur_cylinder_ctrl_status = CY_TESTING;
			test_dis();
			break;
		case STAGE_FALL:/* 第三阶段 电压下降 */
			cur_status = ST_VOL_FALL;
            count_fall_vol_step_value();/* 计算电压下降步进值 */
			test_dis();
			break;
		case STAGE_INTER:/* 第四阶段 间隔等待 */
			cur_status = ST_INTER_WAIT;
			WAIT = 1;
			
			if(TEST)
			{
				TEST = 0;
				HIGH = 0;
// 				test_reset();
                irq_stop_relay_motion();
				relay_motion();/* 继电器进入就绪状态 */
				
				if(!CUR_FAIL)
				{
					test_pass();
                    save_cur_result(&cur_result);
					if(steps_pass)/* 如果步间pass打开就点亮 pass灯 */
					{
						LED_PASS = LED_ON;/* 点亮pass灯 */
					}
				}
				else
				{
					LED_FAIL = LED_ON;
				}
				
				if(fal_t > 0)
				{
                    vol_ave = 0;
                    cur_ave = 0;
                    test_flag.vol_change_flag = 1;
                    test_dis();
                    test_flag.vol_change_flag = 0;
				}
			}
			break;
	}
}

/*
 * 函数名：test_big_cap_details
 * 描述  ：测试细节
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void test_big_cap_details(void)
{
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* 就绪 */
			if(test_flag.do_only_one)
			{
				test_flag.do_only_one = 0;
				test_ready();
			}
			break;
		case STAGE_CHARGE:/* 充电 */
            test_vref(cur_high);	/* 输出各基准 */
            dcw_big_cap_start_charge();
			cur_status = ST_VOL_RISE;/* 电压上升 */
            
			HIGH = 1;
			test_dis();
			break;
        case STAGE_STAB:/* 稳压 */
			test_dis();
            break;
		case STAGE_TEST:/* 测试 */
            test_vref(cur_high);	/* 输出各基准 */
			cur_status = ST_TESTING;
            cur_cylinder_ctrl_status = CY_TESTING;
            
			test_dis();
			break;
		case STAGE_FALL:/* 下降 */
			cur_status = ST_VOL_FALL;
            count_rise_vol_step_value();/* 计算电压上升步进值 */
			test_dis();
			break;
        case STAGE_DIS_CHA:/* 放电 */
            SHORT_INT(DISABLE);	/* 关闭短路中断 */
            GFI_INT(DISABLE);	/* 关闭GFI中断 */
            ARC_INT(DISABLE);	/* 关闭ARC中断 */
			test_dis();
            set_sample_rate(20);
            on_sample_task();
            resume_sample_task();
            dcw_big_cap_execute_dis_charge();
            break;
		case STAGE_INTER:/* 间隔等待 */
			cur_status = ST_INTER_WAIT;
			WAIT = 1;
			
			if(TEST)
			{
				TEST = 0;
				HIGH = 0;
// 				test_reset();
				relay_motion();/* 继电器进入就绪状态 */
				
				if(!CUR_FAIL)
				{
					test_pass();
                    
					if(steps_pass)/* 如果步间pass打开就点亮 pass灯 */
					{
						LED_PASS = LED_ON;/* 点亮pass灯 */
					}
				}
				else
				{
					LED_FAIL = LED_ON;
				}
				
				if(fal_t > 0)
				{
                    vol_ave = 0;
                    cur_ave = 0;
                    test_flag.vol_change_flag = 1;
                    test_dis();
                    test_flag.vol_change_flag = 0;
				}
			}
			break;
	}
}

/*
 * 函数名：test_state_dis
 * 描述  ：显示测试时的状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_status_dis(void)
{
}

/*
 * 函数名：cs99xx_test
 * 描述  ：测试程序的主框架
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cs99xx_test(void)
{
	/* 显示测试状态 */
	test_status_dis();
	
	/* 测试细节 */
	test_details();
	
	/* 更新时间 */
	updata_time(U_TEST_TIME, g_dis_time);
}
/*
 * 函数名：cs99xx_big_cap_test
 * 描述  ：测试程序的主框架
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cs99xx_big_cap_test(void)
{
	/* 显示测试状态 */
	test_status_dis();
	
	/* 测试细节 */
	test_big_cap_details();
	
	/* 更新时间 */
	updata_time(U_TEST_TIME, g_dis_time);
}

///////////////////////////////////////////////////////////
//                                                       //
//				        G模式                            //
//                                                       //
///////////////////////////////////////////////////////////
/*
 * 函数名：count_vol_step_value
 * 描述  ：计算当前步的缓变电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
// void count_vol_step_value1(void)
// {
// 	int16_t vol_1 = 0;/* 初始电压值 */
// 	int16_t vol_2 = 0;/* 目标电压值 */
// 	uint16_t temp_inter_t = 0;/* 间隔时间 */
// 	
// 	if(g_cur_step->next != NULL)
// 	{
// 		switch(cur_mode)
// 		{
// 			case ACW:
// 				vol_1        = g_cur_step->one_step.acw.testing_voltage;
// 				vol_2        = g_cur_step->next->one_step.acw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.acw.interval_time;
// 				break;
// 			case DCW:
// 				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
// 				vol_2        = g_cur_step->next->one_step.dcw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.dcw.interval_time;
// 				break;
// 		}
// 	}
// 	else
// 	{
// 		switch(g_cur_step->one_step.com.mode)
// 		{
// 			case ACW:
// 				vol_1        = g_cur_step->one_step.acw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.acw.interval_time;
// 				break;
// 			case DCW:
// 				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.dcw.interval_time;
// 				break;
// 		}
// 		vol_2 = 0;
// 	}
//     
// 	vol_ch_base = vol_1;
//     vol_ch_target = vol_2;
// 	
// 	if(temp_inter_t != 0)
// 	{
// 		vol_ch_step = (float)(vol_2 - vol_1)/temp_inter_t;
// 	}
// }
/*
 * 函数名：count_vol_step_value
 * 描述  ：计算下一步的缓变电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void count_vol_step_value(void)
{
	int16_t vol_1 = 0;/* 初始电压值 */
	int16_t vol_2 = 0;/* 目标电压值 */
	uint16_t temp_inter_t = 0;/* 缓变时间 */
	
    /* 不是最后一步 */
	if(g_cur_step->next != NULL)
	{
		switch(cur_mode)
		{
			case ACW:
				vol_1        = g_cur_step->one_step.acw.testing_voltage;
				vol_2        = g_cur_step->next->one_step.acw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.acw.interval_time;
				break;
			case DCW:
				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
				vol_2        = g_cur_step->next->one_step.dcw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.dcw.interval_time;
				break;
		}
	}
    /* 是最后一步 */
	else
	{
		switch(g_cur_step->one_step.com.mode)
		{
			case ACW:
				vol_1        = g_cur_step->one_step.acw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.acw.fall_time;
				break;
			case DCW:
				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.dcw.fall_time;
				break;
		}
		vol_2 = 0;
	}
    
	vol_ch_base = vol_1;
    vol_ch_target = vol_2;
    
	if(temp_inter_t != 0)
	{
		vol_ch_step = (float)(vol_2 - vol_1) / temp_inter_t;
	}
}
/*
 * 函数名：count_rise_vol_step_value
 * 描述  ：计算当前步电压上升的电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void count_rise_vol_step_value(void)
{
	vol_ch_base = 0;
    vol_ch_target = cur_vol;
    
	if(ris_t != 0)
	{
		vol_ch_step = (float)(vol_ch_target - vol_ch_base) / ris_t;
	}
}
/*
 * 函数名：count_vol_step_value
 * 描述  ：计算当前步电压下降的电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void count_fall_vol_step_value(void)
{
	vol_ch_base = cur_vol;
    vol_ch_target = 0;
    
	if(fal_t != 0)
	{
		vol_ch_step = (float)(vol_ch_target - vol_ch_base) / fal_t;
	}
}

void count_vol_ch_step(void)
{
    test_flag.vol_rise_step_t = 0;
    
    switch(test_flag.gradation)
    {
        case STAGE_CHARGE://充电
        case STAGE_RISE://上升
            count_rise_vol_step_value();
            break;
        case STAGE_FALL://下降
            count_fall_vol_step_value();
            break;
    }
}
/*
 * 函数名：vol_change
 * 描述  ：电压缓变
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
uint16_t ddug_led = 0;
void vol_change(void)
{
    float temp_vol = 0.0;
    
	temp_vol = vol_ch_base + vol_ch_step * g_dis_time;
    
	if(vol_ch_step > 0)
	{
		if(temp_vol > vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	else
	{
		if(temp_vol < vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	
	DAC_Vol[0] = temp_vol * cur_dac_k + cur_dac_b;
    if(DAC_Vol[0] < 300)
    {
        ddug_led = DAC_Vol[0];
    }
}

void vol_change_200hz(uint32_t time)
{
    float temp_vol = 0.0;
    
    float one_step_vol = vol_ch_step / 20.0;
    
	temp_vol = vol_ch_base + one_step_vol * time;
    
	if(vol_ch_step > 0)
	{
		if(temp_vol > vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	else
	{
		if(temp_vol < vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	
	DAC_Vol[0] = temp_vol * cur_dac_k + cur_dac_b;
    
    if(DAC_Vol[0] < 300)
    {
        ddug_led = DAC_Vol[0];
    }
}
void vol_change_1000hz(uint32_t time)
{
    float temp_vol = 0.0;
    
    float one_step_vol = vol_ch_step / 100.0;
    
	temp_vol = vol_ch_base + one_step_vol * time;
    
	if(vol_ch_step > 0)
	{
		if(temp_vol > vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	else
	{
		if(temp_vol < vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	
	DAC_Vol[0] = temp_vol * cur_dac_k + cur_dac_b;
    
    if(DAC_Vol[0] < 300)
    {
        ddug_led = DAC_Vol[0];
    }
}
/*
 * 函数名：test_g_details
 * 描述  ：g模式测试细节
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_g_details(void)
{
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
			if(test_flag.do_only_one)
			{
				test_flag.do_only_one = 0;
				test_ready();
			}
            break;
        }
		case STAGE_RISE:/* 第一阶段 电压上升 */
			cur_status = ST_VOL_RISE;
			HIGH = 1;
            count_rise_vol_step_value();/* 计算电压上升步进值 */
			test_dis();
			break;
		case STAGE_CHANGE:/* 第二阶段 电压缓变 */
			cur_status = ST_INTER_WAIT;
            cur_cylinder_ctrl_status = CY_TESTING;
            test_dis();
			break;
		case STAGE_TEST:/* 第三阶段  正在测试 */
			cur_status = ST_TESTING;
            cur_cylinder_ctrl_status = CY_TESTING;
            
            test_dis();
			break;
		case STAGE_FALL:/* 第一阶段 电压下降 */
			cur_status = ST_VOL_FALL;
            count_fall_vol_step_value();/* 计算电压下降步进值 */
			test_dis();
			break;
	}
}

/*
 * 函数名：cs99xx_g_test
 * 描述  ：G模式测试程序的主框架
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cs99xx_g_test(void)
{
    /* 显示测试状态 */
	test_status_dis();
	
	/* g模式测试细节 */
	test_g_details();
	
    /* 更新时间 */
	updata_time(U_TEST_TIME, g_dis_time);
}

///////////////////////////////////////////////////////////
//                                                       //
//				        偏移测量                         //
//                                                       //
///////////////////////////////////////////////////////////
/*
 * 函数名：offset_details
 * 描述  ：offset模式测试细节
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void offset_details(void)
{
	uint8_t err = 0;
	
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* 第〇阶段 状态准备 */
            if(test_flag.do_only_one)
            {
                test_flag.do_only_one = 0;
                test_ready();
                
                g_test_time = 0;
                g_dis_time = 0;
                open_test_timer();/* 开定时器 */
                on_schedule_task();
            }
			break;
		case STAGE_RISE:/* 第一阶段 电压上升 */
            count_rise_vol_step_value();/* 计算电压上升步进值 */
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_TEST:/* 第二阶段 进入测试阶段 这里调用了acw的 test_acw_two */
			err = offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_FALL:/* 电压下降 */
            count_fall_vol_step_value();/* 计算电压下降步进值 */
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
	}
	
	if(err != ERR_NONE)
	{
		exception_handling(err);
	}
}

static void offset_details_g(void)
{
	uint8_t err = 0;
	
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* 第〇阶段 状态准备 */
			
            if(test_flag.do_only_one)
            {
                test_flag.do_only_one = 0;
                test_ready();
                
                g_test_time = 0;
                g_dis_time = 0;
                open_test_timer();/* 开定时器 */
                on_schedule_task();
            }
			break;
		case STAGE_RISE:/* 第一阶段 电压上升 */
            count_rise_vol_step_value();/* 计算电压上升步进值 */
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_TEST:/* 第二阶段 进入测试阶段 这里调用了acw的 test_acw_two */
			err = offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_FALL:/* 电压下降 */
            count_fall_vol_step_value();/* 计算电压下降步进值 */
			vol_change();
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
	}
	
	if(err != ERR_NONE)
	{
		exception_handling(err);
	}
}
/*
 * 函数名：offset_state_dis
 * 描述  ：offset测试状态显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void offset_state_dis(void)
{
}
/*
 * 函数名：offset_test
 * 描述  ：offset测试流程
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void offset_test(void)
{
	/* 显示状态 */
	offset_state_dis();
	
	/* 测试细则 */
    if(g_cur_file->work_mode == G_MODE)
    {
        offset_details_g();
    }
    else
    {
        offset_details();
    }
    
    /* 刷新时间 */
	updata_time(U_OFFSET_TIME, g_dis_time);
}
//////////////////////////////////////////////////////////////////////////
void clear_test_flag(void)
{
	memset((void*)&test_flag, 0, sizeof test_flag);/* 清空程序运行的标志位 */
}
/*
 * 函数名：clear_par
 * 描述  ：清空全局参数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void clear_par(void)
{
	vol_ave = 0;
	cur_ave = 0;
	res_ave = 0;
	real_ave = 0;
	
    err_vol_bak = 0;
    err_cur_bak = 0;
    err_res_bak = 0;
    
    clear_test_flag();
	memset(&f_buf, 0, sizeof f_buf);/* 清空滤波缓冲区 */
    
    sample_vol = 0;
    sample_cur = 0;
}

void set_sample_rate(uint32_t rate)
{
    if(rate == 0)
    {
        rate = ~0;
        app_flag.sample_cycle = rate;
    }
    else if(rate > 1000)
    {
        app_flag.sample_cycle = 1000;
    }
    else
    {
        app_flag.sample_cycle = 1000 / rate; /* 单位ms */
    }
    
	app_flag.sample_rate = rate;
}

void wait_dc_gr_send_task_idle(void)
{
	uint32_t time_out = 0;
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,10);
		if(app_flag.dc_gr_task_send_idle)
		{
			if(time_out++ > 10)
			{
				break;
			}
		}
		else
		{
			time_out = 0;
		}
	}
}
/*
 * 函数名：startup
 * 描述  ：启动测试
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void startup(void)
{
	int8_t fail_bak = FAIL;/* 备份测试失败标志 */
    uint8_t err = 0;
	
	clear_par();/* 清空全局变量的值 */
	FAIL = fail_bak;/* 恢复测试失败标志 */
	LED_ALL = LED_OFF;
	BUZZER = BUZZER_OFF;
	g_test_time = 0;/* 清空定时器 */
	g_dis_time = 0;
    g_plc_dly = 0;
	
	off_keyboard();/* 关闭键盘 */
	save_result(&cur_result, INIT_RESULT);/* 初始化结构数据信息 */
    OSMutexPend(MdbusMutex, 0, &err);
    clear_mdbus_send_queue();/* 清空modbus发送队列 */
	clear_slither_data();/* 清空滑动平均缓冲区数据 */
    OSMutexPost(MdbusMutex);
	cpld_write(CPLD_START);/* 启动CPLD计数 */
	
	test_flag.do_only_one = 1;/* 只调1次 test_ready */
    test_flag.vol_change_flag = 1;
    test_flag.allow_dc_gr_fetch = 0;
	test_flag.uart_next = 0;
	cur_status = ST_TESTING;
	
	/*******************************************************/
    on_sample_task();/* 启动采样任务 */
    
	if(cur_mode == GR && DC_GR_EN)
	{
        clear_dc_gr_data();
		set_sample_rate(0);
        on_dc_gr_control_relay();
		wait_dc_gr_send_task_idle();//延时避开继电器冲击
	}
	else
	{
		set_sample_rate(100);
        resume_sample_task();
	}
    
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            dcw_big_cap_stop_dis_charge();
        }
    }
    
    exit_sw();
    plc_signal_cmd(PLC_SIG_TESTING);/* 向PLC发送测试信号 */
    test_flag.judge_err_en = 1;
}

/*
 * 函数名：close_test_timer
 * 描述  ：关闭测试定时器
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void close_test_timer(void)
{
    bsp_tim2_close();/* 关定时器 */
}
/*
 * 函数名：open_test_timer
 * 描述  ：打开测试定时器
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void open_test_timer(void)
{
    bsp_tim2_open();/* 开定时器 */
}
/*
 * 函数名：stop_test
 * 描述  ：停止测试 无论是测试成功还是失败 需要停止时都会调用这个函数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void stop_test(void)
{
    test_flag.judge_err_en = 0;
    close_test_timer();
	close_hv();
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);/* 清定时器中断标志 */
	
    relay_motion();/* 继电器进入就绪状态 */
    
	/* 关中断 */
	RTC_INI(ENABLE);
// 	STOP_INT(DISABLE);	/* 关闭复位中断 */
	SHORT_INT(DISABLE);	/* 关闭短路中断 */
	GFI_INT(DISABLE);	/* 关闭GFI中断 */
	ARC_INT(DISABLE);	/* 关闭ARC中断 */
	
	g_test_time = 0;/* 清空时间片 */
	GR_OVER = 1;
	
	TEST = 0;	/* 测试标志清0 */
	LED_TEST = 0;/* 关测试灯 */
	HIGH = 0; 	/* 高压标志 */
	
	disable_sample_task();/* 停止采样任务 */
	
	cpld_write(CPLD_STOP);/* 停止CPLD计数 */
	
	if(!(sys_par.fail_mode == FAIL_MODE_CON))
	{
		if(g_comm_status == LOCAL_COMM)
		{
			on_keyboard();/* 打开键盘 */
		}
	}
}

/*
 * 函数名：serve_test_poll_task
 * 描述  ：为系统提供定时任务
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void serve_test_poll_task(void)
{
    static uint32_t count = 0;
    
    /* IR换挡延时 */
	if(g_ir_dly)
	{
		g_ir_dly--;
	}
    
    buzzer_delay_off();/* 蜂鸣器延时关闭 */
    buzzer_flicker();/* 蜂鸣器闪响 */
    
    /* 气缸控制延时 */
    if(g_cylinder_ctrl_t)
    {
        if(0 == --g_cylinder_ctrl_t)
        {
            cur_cylinder_ctrl_status = CY_WAIT;
        }
    }
    
	/* PLC延时关闭 */
    plc_delay_off();
    
    /* 清除gfi的延时计数 */
    gfi_cycle_clear_count();
    
    /* 短路中断以轮询的方式判断 */
    if(cur_mode == IR)
    {
        if(!type_spe.ir_short_int_en && HIGH)
        {
            if(SHORT_PIN == RESET)
            {
                if(SHORT_PIN == RESET)
                {
                    if(SHORT_PIN == RESET)
                    {
                        short_irq();
                    }
                }
            }
        }
    }
    
    if(++count >= 1000)
    {
        test_flag.record_per_second_re = test_flag.re_times_per_second;
        test_flag.re_times_per_second = 0;
        count = 0;
        
        test_flag.err_amp_timer++;//功放报警定时器
    }
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/

