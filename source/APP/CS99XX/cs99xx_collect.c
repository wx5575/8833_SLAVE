/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_collect.c
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include    <stdio.h>
#include    "cs99xx_collect.h"
#include	"SPI_CPLD.h"
#include    "cs99xx_result.h"
#include	"calibration.h"
#include    "stm32f10x.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "cs99xx_vref.h"
#include 	"DAC_VREF.h"
#include    "ADC_DMA.h"
#include 	"STM32_ADC.H"
#include    "dc_module.h"
#include	"string.h"

/*
 * 函数名：smoothing_filtering
 * 描述  ：dac平滑滤波
 * 输入  ：sf_n 区分vol cur, 0 vol， 1 cur
 * 输出  ：无
 * 返回  ：滤波的结果
 */

uint32_t smoothing_filtering(const u8 sf_n,const u32 val)
{
	int i = 0, j = 0;
	
	for(i = SF_DEPTH-1; i > 0; i--)
	{
		f_buf.sf_buf_u[sf_n][i] = f_buf.sf_buf_u[sf_n][i-1];
	}
    
	f_buf.sf_buf_u[sf_n][0] = val;
	
	for(i = 0; i < SF_DEPTH-1; i++)
	{
		for(j = i+1; j < SF_DEPTH; j++)
		{
			if(f_buf.sf_buf_u[sf_n][i] == f_buf.sf_buf_u[sf_n][j])
			{
				return f_buf.sf_buf_u[sf_n][i];
			}
		}
	}
	
	return val;
}

void set_high_err(void)
{
    test_flag.high_err = 1;
}

void reset_high_err(void)
{
    test_flag.high_err = 0;
}

uint8_t get_high_err(void)
{
    return test_flag.high_err;
}

#define DEEP_SLOW		120
#define DEEP_FAST		5
#define DEEP_MID		80
#define SLITHER_DEEP_	DEEP_SLOW
#define SLITHER_DEEP	 ((sys_par.ir_speed_sw == IR_FILTERING_DEEP_SLOW)? DEEP_SLOW : \
					(sys_par.ir_speed_sw == IR_FILTERING_DEEP_FAST)? DEEP_FAST : DEEP_MID)

typedef struct{
    uint32_t index;
    uint64_t sum;
    uint32_t c;
    uint16_t buf[SLITHER_DEEP_];/* 滤波缓冲 */
}SLITHER_AVE_INFO;

SLITHER_AVE_INFO slither_ave_info;

void clear_slither_data(void)
{
    memset(&slither_ave_info, 0, sizeof slither_ave_info);
}

uint16_t slither_f(uint16_t value)
{
    uint16_t res = 0;
	uint16_t deep = SLITHER_DEEP;
    
    slither_ave_info.index = (slither_ave_info.index + 1) % deep;
    
    slither_ave_info.sum -= slither_ave_info.buf[slither_ave_info.index];
    
    slither_ave_info.buf[slither_ave_info.index] = value;
    
    slither_ave_info.sum += slither_ave_info.buf[slither_ave_info.index];
    
    if(slither_ave_info.c < deep)
    {
        slither_ave_info.c++;
    }
    else
    {
        slither_ave_info.c = deep;
    }
    
    res = slither_ave_info.sum / slither_ave_info.c;
    
    return res;
}

/*
 * 函数名：count_dis_value
 * 描述  ：计算显示值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void count_dis_value(void)
{
	double cos_val = 0.0;
	double sin_val = 0.0;
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
	float temp_res = 0.0;
    
    /* 计算相位角的正弦值和余弦值 */
    if((cur_mode == ACW && cur_real_cur) || cur_mode == BBD)
    {
        cpld_count_angle(&sin_val, &cos_val);
    }
	
	if(cur_mode != GR)
	{
		temp_vol = sample_vol;
		temp_cur = sample_cur;
        cur_ad_vol = sample_cur * 3.3 / 4096;/* 计算出电流ad采样端的电压值 */
		
		if(temp_vol == 0)
		{
			vol_ave = 0;
		}
		else
		{
			vol_ave = temp_vol * cur_adc_k + cur_adc_b;
		}
		
        /* 测试阶段 */
        if(test_flag.gradation == STAGE_TEST)
        {
            //对电压小于设定值10个字的电压进行修正 2017.6.28
            if(vol_ave <= cur_vol)
            {
                if(vol_ave + 10 > cur_vol)
                {
                    vol_ave = cur_vol;
                }
            }
            else
            {
                if(cur_vol + 3 > vol_ave)
                {
                    vol_ave = cur_vol;
                }
            }
        }
        
		if(cur_mode == IR)
		{
			temp_cur = slither_f(temp_cur);
		}
		
		if(temp_cur == 0)
		{
			cur_ave = 0;
		}
		else
		{
			cur_ave = temp_cur;
		}
	}
	/* GR测试 */
	else
	{
		/* 直流 GR */
		if(DC_GR_EN)
		{
            float t_cur = 0;
            float t_vol = 0;
            
            if(test_flag.allow_dc_gr_fetch == 0)
            {
                return;
            }
            
			if(test_flag.uart_next == 0)
			{
				test_flag.uart_next = 1;
                
                send_dc_module_cmd(DC_MODULE_CMD_FETCH, 0);/* 获取测试数据 */
//                 send_dc_module_cmd(DC_MODULE_CMD_FETCH_SHORT, 0);/* 获取测试数据 */
			}
            
            if(0 == get_dc_gr_data(&t_cur, &t_vol))
//             if(0 == get_dc_gr_short_data(&t_cur, &t_vol))
            {
                test_flag.uart_next = 0;
                cur_ave = t_cur;
                vol_ave = t_vol;
            }
		}
		/* 交流 GR */
		else
		{
			temp_cur = sample_cur;/* 采集的电流值 */
			temp_vol = sample_vol;/* 采集电压值 */
            
            if(temp_cur == 0)
            {
                cur_ave = 0;
            }
            else
            {
                cur_ave = temp_cur * ratio_gr.adc_i_k[cur_gear] + ratio_gr.adc_i_b[cur_gear] + ROUND / 100;
            }
            
            if(temp_vol == 0)
            {
                vol_ave = 0;
            }
            else
            {
                vol_ave = temp_vol * ratio_gr.adc_v_k[cur_gear] + ratio_gr.adc_v_b[cur_gear] + ROUND / 100;
            }
		}
        
        //对电压小于设定值10个字的电压进行修正 2017.6.28
        if((cur_ave * 100) <= cur_vol)
        {
            if((cur_ave * 100) + 30 > cur_vol)
            {
                cur_ave = cur_vol / 100.0;
            }
        }
        else
        {
            if(cur_vol + 3 > (cur_ave * 100))
            {
                cur_ave = cur_vol / 100.0;
            }
        }
	}
    
    if(cur_ave < 0)
    {
        cur_ave = 0;
    }
    
    if(vol_ave < 0)
    {
        vol_ave = 0;
    }
	
	switch(cur_mode)
	{
		case ACW:
		{
			cur_ave = temp_cur * ratio_acw.adc_cur_k[cur_gear];
			real_ave = cur_ave * cos_val;
			
            /* 以电容形式显示真实电流 */
            if(type_spe.real_cur_format == CAP_FORMAT)
            {
                if(vol_ave <= 0)
                {
                    cap_ave = 0;
                }
                else
                {
                    /* nA/(v*2*PI*Hz) = nF */
                    switch(cur_gear)
                    {
                        case AC_200uA:
                            cap_ave = cur_ave * sin_val / 10000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                        case AC_2mA:
                            cap_ave = cur_ave * sin_val / 1000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                        case AC_10mA:
                        case AC_20mA:
                        case AC_50mA:
                            cap_ave = cur_ave * sin_val / 100 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                        case AC_100mA:
                            cap_ave = cur_ave * sin_val / 10 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                    }
                }
            }
			
			if(!OFFSET_BBD)
			{
				/* 引入偏移值 */
				if(cur_ave >= cur_offset)
				{
					cur_ave -= cur_offset;
				}
				else
				{
					cur_ave = 0;
				}
				
				if(real_ave >= cur_real_offset)
				{
					real_ave -= cur_real_offset;
				}
				else
				{
					real_ave = 0;
				}
			}
			
			/* 判断真实电流 */
			if(real_ave > cur_real_cur && cur_real_cur)
			{
				err_real = 1;
			}
			break;
		}
		case DCW:
		{
			cur_ave = temp_cur * ratio_dcw.adc_cur_k[cur_gear];
			
			if(!OFFSET_BBD)
			{
				/* 引入偏移值 */
				if(cur_ave >= cur_offset)
				{
					cur_ave -= cur_offset;
				}
				else
				{
					cur_ave = 0;
				}
			}
			
			/* 判断充电电流 */
			if(cur_ave > cur_charge_cur)
			{
				err_charge = 0;/* 不报警 */
			}
			break;
		}
		case IR:
		{
			/* 如果电流为0时就让他等于1 */
			if(cur_ave <= 0)
			{
				cur_ave = 1;
				res_ave = 1000*1000;
				res_ave_comm = ~(uint16_t)0;
				break;
			}
			
			temp_res = (float)vol_ave * cur_res_k / cur_ave;
			
            res_ave = temp_res;
			
			if(res_ave < 0)
			{
				res_ave = 0;
			}
			else
			{
				uint32_t temp = 0;
				temp = res_ave * 1000;
				res_ave = (float)temp / 1000.0; /* 保留3位小数 */
			}
			break;
		}
		case GR:/* mOhm = mV/mA*1000 */
		{
			/* 电流显示是两位小数点，乘以10就将其转换为mA */
// 			if(cur_ave*10 > cur_vol * 0.08)
            if(cur_ave)
			{
				res_ave = vol_ave * 10 / cur_ave;
			}
			
			if(!OFFSET_BBD)
			{
				/* 引入偏移值 */
				if(res_ave >= cur_offset)
				{
					res_ave -= cur_offset;
				}
				else
				{
					res_ave = 0;
				}
			}
			
			cur_ave_comm = cur_ave * 100;
			res_ave_comm = res_ave;
			break;
		}
		case BBD:
		{
			cur_ave = temp_cur * cur_adc_cur_k;
			
            if(vol_ave <= 0)
            {
                cap_ave = 0;
            }
            else
            {
                /* nA/(v*2*PI*Hz) = nF */
                switch(cur_gear)
                {
                    case AC_200uA:
                        cap_ave = cur_ave * sin_val / 10000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                        break;
                    case AC_2mA:
                        cap_ave = cur_ave * sin_val / 1000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                        break;
                    case AC_20mA:
                        cap_ave = cur_ave * sin_val / 100  * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                        break;
                }
            }
			
// 			real_ave = cur_ave*cos_val;
// 			T_DELAY();
// 			GUI_GotoXY(X+11*CW, Y);
// 			GUI_DispFloatFix ((float)temp_cap, 5, 3);
			
			if(!OFFSET_BBD)
			{
				/* 引入偏移值 */
				if(cap_ave >= g_cur_step->one_step.bbd.offset_cap)
				{
// 					cap_ave -= g_cur_step->one_step.bbd.offset_cap;
				}
				else
				{
// 					cap_ave = 0;
				}
			}
			
// 			g_cur_step->one_step.bbd.cap_value = cap_ave;
			break;
		}
		case CC:
// 			cur_ave = temp_cur * 1;
			break;
	}
    
    
    if(cur_mode == ACW || cur_mode == DCW)
    {
        /* 大电容测试只在测试阶段来判断上限报警*/
        if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
        {
            if(test_flag.gradation == STAGE_TEST)
            {
                /* 当电流值大于上限值时就将电流值强行改为上限值并设置上限报警 */
                if(cur_ave > cur_high)
                {
                    cur_ave = cur_high;
                    
                    set_high_err();
                }
                else
                {
                    reset_high_err();
                }
            }
        }
        /* 正常测试 */
        else
        {
            /* 当电流值大于上限值时就将电流值强行改为上限值并设置上限报警 */
            if(cur_ave > cur_high && app_flag.calibration == 0)
            {
				uint8_t buf[14]  ={1,1,1,1,2,2,3,3,4,4,1,1,2,2};
				uint8_t index[18]={0,1,2,16,17,3,9,10,4,14,15,5,6,7,8,11,12,13};
				static uint8_t c;
				
				sample_cur = g_ad_dog + buf[index[c++%18] % 14];
				
				cur_ave = sample_cur * cur_adc_cur_k;
                
				irq_stop_relay_motion();
				record_exception_scene();/* 记录异常现场 */
                set_high_err();
				test_flag.judge_err_en == DISABLE;
				ERR_NUM = ERR_HIGH;
            }
            else
            {
                reset_high_err();
            }
        }
    }
    
    /* 异常判断 */
    if(OFFSET_BBD != 2)
    if(test_flag.judge_err_en == ENABLE)
    {
        if(ERR_NUM == ERR_NONE)
        {
            ERR_NUM = judge_err();
        }
    }
}

/*
 * 函数名：judge_vol_exception
 * 描述  ：判断电压异常
 * 输入  ：无
 * 输出  ：无
 * 返回  ：错误码 cur_status
 */
static uint8_t judge_vol_exception(void)
{
    if(g_dis_time >= 5 || g_dis_time == tes_t)
    {
		/* 电压低于1V */
		if(vol_ave < 1)
		{
			record_exception_scene();/* 记录异常现场 */
			return ERR_SHORT;
		}
    }
    
    /* 测试时间到 */
    if(tes_t && g_dis_time + 1 >= tes_t)
    {
        return ERR_NONE;
    }
    
    if((vol_ave < cur_vol / 2 || vol_ave > cur_vol * 3 / 2))
    {
        if(++test_flag.vol_err_count > 4)
        {
            record_exception_scene();/* 记录异常现场 */
            ERR_NUM_OVER = ERR_VOL;
        }
    }
    else
    {
        test_flag.vol_err_count = 0;
        ERR_NUM_OVER = ERR_NONE;/* 测试时间到报警 */
    }
    
    return ERR_NONE;
}
/*
 * 函数名：set_dc_gr_open_err
 * 描述  ：设置开路报警
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void set_dc_gr_open_err(void)
{
    test_flag.dc_gr_open_err = ERR_OPEN;
}

/*
 * 函数名：get_dc_gr_open_err
 * 描述  ：获取直流模块开路报警
 * 输入  ：无
 * 输出  ：无
 * 返回  ：错误码
 */
uint8_t get_dc_gr_open_err(void)
{
    return test_flag.dc_gr_open_err;
}
/*
 * 函数名：set_dc_gr_amp_err
 * 描述  ：设置直流模块功放报警
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void set_dc_gr_amp_err(void)
{
    test_flag.dc_gr_amp_err = ERR_AMP;
}

/*
 * 函数名：get_dc_gr_open_err
 * 描述  ：获取直流模块功放报警
 * 输入  ：无
 * 输出  ：无
 * 返回  ：错误码
 */
uint8_t get_dc_gr_amp_err(void)
{
    return test_flag.dc_gr_amp_err;
}
/*
 * 函数名：judge_err
 * 描述  ：错误判断
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
int8_t judge_err(void)
{
	#define JUDGE_SHORT() {if(SHORT_PIN == RESET){if(SHORT_PIN == RESET){if(SHORT_PIN == RESET){err = ERR_SHORT;}}}}
	int8_t err = 0;
	
    /* 放电时不进行异常判断 */
    if(cur_mode == DCW)
    {
        if(test_flag.gradation == STAGE_DIS_CHA)
        {
            return err;
        }
    }
    
    if(cur_mode == CC)
    {
        /* 电流下限报警 */
        if(cur_ave < cur_low)
        {
            record_exception_scene();/* 记录异常现场 */
            ERR_NUM_OVER = ERR_OPEN;
        }
        else
        {
            ERR_NUM_OVER = ERR_NONE;
        }
        return err;
    }
    
    /* 稳压阶段 */
    if(test_flag.gradation == STAGE_STAB)
    {
        switch(cur_mode)
        {
            case DCW:
            {
				if(0 == cur_delay_t)
				{
					/* 电流上限报警 */
                    if(get_high_err())
					{
                        record_exception_scene();/* 记录异常现场 */
						err = ERR_HIGH;
						result_dcw();
					}
				}
                break;
            }
        }
    }
    /* 大电容测试充电阶段 */
	else if(test_flag.gradation == STAGE_CHARGE)
    {
        if(cur_ave > get_cur_upper(DCW, cur_gear))
        {
            record_exception_scene();/* 记录异常现场 */
            err = ERR_GEAR;
            result_dcw();
        }
    }
	/* 电压上升 阶段 缓变阶段 */
	else if(test_flag.gradation == STAGE_RISE || test_flag.gradation == STAGE_CHANGE)
	{
		switch(cur_mode)
		{
			case ACW:
            {
				/* 电流上限报警 */
// 				if(cur_ave > cur_high)
                if(get_high_err())
				{
                    record_exception_scene();/* 记录异常现场 */
					err = ERR_HIGH;
					result_acw();
				}
				break;
            }
			case DCW:
            {
                /* 延时时间为0时判断上限报警 */
                if(0 == cur_delay_t)
				{
					/* 电流上限报警 */
                    if(get_high_err())
					{
                        record_exception_scene();/* 记录异常现场 */
						err = ERR_HIGH;
						result_dcw();
					}
				}
                /* 延时时间为0时在电压上升阶段要判断档位报警来弥补不判断上限报警 */
				else
				{
                    if(get_high_err())
                    {
                        reset_high_err();
                    }
                    
                    if(ERR_NONE == err)
                    {
                        /* 档位报警 他的存在是为了弥补延时时间无法检测的上限报警 */
                        if(cur_ave > get_cur_upper(DCW, cur_gear))
                        {
                            record_exception_scene();/* 记录异常现场 */
                            err = ERR_GEAR;
                            result_dcw();
                        }
                    }
				}
				break;
            }
			case IR:
            {
				/* 短路电平判断三次 */
				if(!type_spe.ir_short_int_en)
				{
					JUDGE_SHORT();
				}
				break;
            }
		}
	}
	/* 测试阶段 */
	else if(test_flag.gradation == STAGE_TEST && (g_test_time>2 || cur_mode==BBD || cur_mode==CC))
	{
		static int low_times = 0;/* 记录下限报警次数 */
		
		switch(cur_mode)
		{
			case ACW:
			{
                /* 电压异常判断 */
                err = judge_vol_exception();
                
                if(ERR_NONE != err)
                {
                    return err;
                }
				
                if(ERR_NONE == err)
                {
                    /* 电流上限报警 */
                    if(get_high_err())
                    {
                        record_exception_scene();/* 记录异常现场 */
                        err = ERR_HIGH;
                        result_acw();
                    }
                }
				
                if(ERR_NONE == err)
                {
                    /* 真实电流报警 */
                    if(err_real)
                    {
                        record_exception_scene();/* 记录异常现场 */
                        err = ERR_REAL;
                    }
                }
				
                if(ERR_NONE == err)
                {
                    /* 电流下限报警 */
                    if(cur_ave < cur_low)
                    {
                        /* 下限报警判断三次 */
                        if(++low_times >= 2)
                        {
                            record_exception_scene();/* 记录异常现场 */
                            err = ERR_LOW;
                            result_acw();
                            
                            low_times = 0;
                        }
                    }
                }
				
                if(ERR_NONE == err)
                {
                    /* 测试时间到 */
                    if(tes_t && g_dis_time + 1 >= tes_t)
                    {
                        /* 电流下限报警 */
                        if(cur_ave < cur_low)
                        {
                            /* 下限报警判断两次 */
                            if(++low_times >= 2)
                            {
                                record_exception_scene();/* 记录异常现场 */
                                ERR_NUM_OVER = ERR_LOW;
                                result_acw();
                                
                                low_times = 0;
                            }
                        }
                    }
                }
				break;
			}
			case DCW:
			{
                /* 电压异常判断 */
                err = judge_vol_exception();
                
                /* 检查延时时间 如果大于测试时间应作修正 */
				if(cur_delay_t > tes_t && tes_t != 0)
				{
					cur_delay_t = tes_t;
				}
				
				/* 延时报警 */
				if(g_dis_time+1 >= cur_delay_t)
				{
                    if(ERR_NONE == err)
                    {
                        /* 电流上限报警 */
                        if(get_high_err())
                        {
                            record_exception_scene();/* 记录异常现场 */
                            err = ERR_HIGH;
                            result_dcw();
                        }
                    }
					
                    if(ERR_NONE == err)
                    {
                        /* 电流下限报警 */
                        if(cur_ave < cur_low)
                        {
                            record_exception_scene();/* 记录异常现场 */
                            err = ERR_LOW;
                            result_dcw();
                        }
                    }
                    
                    if(ERR_NONE == err)
                    {
                        /* 充电报警 测试时间到后才报 */
                        if(tes_t && g_dis_time + 2 >= tes_t)
                        {
                            /* 充电报警 */
                            if(err_charge && cur_charge_cur)
                            {
                                record_exception_scene();/* 记录异常现场 */
                                ERR_NUM_OVER = ERR_CHARGE;
                            }
                        }
                    }
				}
				else
				{
                    if(get_high_err())
                    {
                        reset_high_err();
                    }
                    
                    if(ERR_NONE == err)
                    {
                        /* 档位报警 他的存在是为了弥补延时时间无法检测的上限报警 */
                        if(cur_ave > get_cur_upper(DCW, cur_gear))
                        {
                            record_exception_scene();/* 记录异常现场 */
                            err = ERR_GEAR;
                            result_dcw();
                        }
                    }
				}
				break;
			}
			case IR:
			{
                /* 电压异常判断 */
                err = judge_vol_exception();
                if(ERR_NONE != err)
                {
                    return err;
                }
                
				if(cur_delay_t > tes_t && tes_t != 0)
				{
					cur_delay_t = tes_t;
				}
				
				/* 延时时间到 */
				if(g_dis_time >= cur_delay_t)
				{
					if(g_dis_time >= 3)
					{
                        /* 测试时间到前0.1s */
                        if(tes_t && g_dis_time + 1 >= tes_t)
                        {
                            if(ERR_NONE == err)
                            {
                                /* 电阻上限报警 */
                                if(res_ave > cur_high && cur_high)
                                {
                                    record_exception_scene();/* 记录异常现场 */
                                    ERR_NUM_OVER = ERR_HIGH;
                                    err_res_bak = res_ave;
                                    result_ir();
                                }
                                else
                                {
                                    ERR_NUM_OVER = 0;
                                }
                            }
                        }
						
						/* 换档延时没到就直接退出 不做判断 测试时间到除外 */
						if(g_ir_dly && g_dis_time != tes_t)
						{
							break;
						}
						
                        if(ERR_NONE == err)
                        {
                            /* 判断短路报警 */
                            if(res_ave <= 0.5)
                            {
                                record_exception_scene();/* 记录异常现场 */
                                err = ERR_SHORT;
                                result_ir();
                                return err;
                            }
                        }
						
                        if(ERR_NONE == err)
                        {
                            /* 电阻下限报警 */
                            if(res_ave < cur_low && vol_ave > 0)
                            {
                                if(++test_flag.ir_err_times > 0)
                                {
                                    record_exception_scene();/* 记录异常现场 */
                                    err = ERR_LOW;
                                    result_ir();
                                }
                            }
                            else
                            {
                                test_flag.ir_err_times = 0;
                            }
                        }
					}
				}
				break;
			}
			case GR:
			{
				if(g_test_time > 2)
				{
                    if(err == ERR_NONE)
                    {
						/* 直流 */
                        if(DC_GR_EN)
                        {
//                             err = get_dc_gr_open_err();//test_flag.dc_gr_open_err;
//                             if(err == ERR_OPEN)
//                             {
//                                 res_ave = 5100;
//                                 cur_ave = 0;
//                                 result_gr();
//                                 record_exception_scene();/* 记录异常现场 */
//                             }
                            /* 在直流模块中如果电流一直过小而并未收到模块的报警，此时需自行判断报警 
//                             else*/
							if(cur_ave*100 <= cur_vol*0.03)
                            {
                                err = ERR_OPEN;
                                res_ave = 5100;
                                cur_ave = 0;
                                result_gr();
                                record_exception_scene();/* 记录异常现场 */
                            }
                            
                            if(err == ERR_NONE)
                            {
                                err = get_dc_gr_amp_err();
                                if(err == ERR_AMP)
                                {
                                    result_gr();
                                    record_exception_scene();/* 记录异常现场 */
                                }
                            }
                        }
                        /* 交流 */
						else
                        {
                            /* 开路报警 电流小于设定值的3% 时认为开路 */
                            if(cur_ave*100 <= cur_vol*0.03)
                            {
                                if(++test_flag.vol_err_count > 1)
                                {
                                    err = ERR_OPEN;
                                    res_ave = 5100;
                                    cur_ave = 0;
                                    result_gr();
                                    record_exception_scene();/* 记录异常现场 */
                                }
                            }
                            else
                            {
                                test_flag.vol_err_count = 0;
                            }
                        }
                    }
					
                    if(err == ERR_NONE)
                    {
						/* 压降测试仪 */
                        if(GR_VOL_DROP_EN)
                        {
                            /* 电阻上限报警 */
                            if(res_ave	> cur_high && cur_high)
                            {
                                err = ERR_HIGH;
                                result_gr();
                                record_exception_scene();/* 记录异常现场 */
                            }
                        }
						/* 直流接地 */
                        else
                        {
                            /* 电阻上限报警 */
                            if(res_ave > cur_high && cur_high)
                            {
								/* 在电流正常的情况下才会判断上限报警 */
								if(++test_flag.hight_err_count > 1 && cur_ave > 1.0)
								{
									err = ERR_HIGH;
									result_gr();
									record_exception_scene();/* 记录异常现场 */
								}
                            }
							else
							{
								test_flag.hight_err_count = 0;
							}
                        }
                    }
					
                    if(err == ERR_NONE)
                    {
                        /* 测试时间到 */
//                         if(tes_t && g_dis_time+1 >= tes_t)
                        /* 0.5s后判断电阻下限报警 */
                        if(g_dis_time > 4)
                        {
                            if(GR_VOL_DROP_EN == 0)
                            {
                                /* 电阻下限报警 */
                                if(res_ave < cur_low)
                                {
                                    err = ERR_LOW;
                                    result_gr();
                                    record_exception_scene();/* 记录异常现场 */
                                }
                            }
                            else
                            {
                                /* 电阻下限报警 */
                                if(res_ave < cur_low)
                                {
                                    err = ERR_LOW;
                                    result_gr();
                                    record_exception_scene();/* 记录异常现场 */
                                }
                            }
                        }
                    }
					
					if(err == ERR_NONE)
					{
						/* 偏移测试 */
						if(OFFSET_BBD)
						{
							/* 压降模式 */
							if(GR_VOL_DROP_EN)
							{
								g_cur_step->one_step.gr.offset_res = vol_ave;/* 偏移电压 */
							}
							/* GR模式 */
							else
							{
								g_cur_step->one_step.gr.offset_res = res_ave;
							}
							
							g_cur_step->one_step.gr.offset_result = 1;	/* 1 fail 2pass */
						}
					}
				}
				break;
			}
			case BBD:
			{
				float temp;
                
				if(cur_ave > 2000)
				{
                    record_exception_scene();/* 记录异常现场 */
					err = ERR_SHORT;
					cap_ave = 99.99;
					sprintf((char*)loop_buf, "----");
					result_bbd();
					break;
				}
				
				if(g_test_time < 1)
				{
					break;
				}
				
				if(OFFSET_BBD == 0)
				{
					temp = cur_cap * cur_open_ratio / 100.0;
                    
					if(temp > cap_ave)
					{
						if(++test_flag.err_times > 1)
						{
                            record_exception_scene();/* 记录异常现场 */
							err = ERR_OPEN;
							result_bbd();
						}
					}
					else if(cur_cap * cur_short_ratio / 100.0 < cap_ave)
					{
                        record_exception_scene();/* 记录异常现场 */
						err = ERR_SHORT;
						result_bbd();
					}
				}
				break;
			}
		}//end switch()
	}//end if(test_flag.gradation == STAGE_TEST)
	
	/* 功放报警 */
	if(AMP_PIN == RESET)
	{
        /* 3s后进行功放报警判断 */
// 		if(test_flag.err_amp_timer > 2)
        {
            test_flag.err_amp_cont++;
            if(test_flag.err_amp_cont > 20)
            {
                disable_sample_task();/* 关闭采样任务  停止刷新屏幕数据 保持出错时的状态 */
                test_flag.err_amp_cont = 0;
                
                /* PWM功放不要报功放报警，而要报短路报警 */
                if(type_spe.amp_type == PWM_AMP)
                {
                    err = ERR_SHORT;
                }
                /* 线性功放正常处理 */
                else
                {
                    err = ERR_AMP;
                }
            }
        }
	}
    else
    {
        test_flag.err_amp_cont = 0;
    }
    
	if(ERR_NUM != ERR_NONE)
	{
		err = ERR_NUM;
	}
	
	return err;
}


void adc_sample(void)
{
	#define SM_DEEP 5
    
#if DMA_FOR_ADC_EN>0
	read_adc_from_dma();
	/* 目的是为了加快显示数据 */
	if(g_test_time < 3)
	{
		if(cur_mode == GR)
		{
			sample_vol = dma_buf[1];
			sample_cur = dma_buf[0];
			smoothing_filtering(0, sample_vol);	/* 滤波 */
			smoothing_filtering(1, sample_cur);
		}
		else
		{
			sample_vol = dma_buf[0];
			sample_cur = dma_buf[1];
			smoothing_filtering(0, sample_vol);	/* 滤波 */
			smoothing_filtering(1, sample_cur);
		}
	}
	else
	{
		if(cur_mode == GR)
		{
			sample_vol = smoothing_filtering(0, adc_value[1]);	/* 滤波 */
			sample_cur = smoothing_filtering(1, adc_value[0]);
		}
		else
		{
			sample_vol = smoothing_filtering(1, adc_value[0]);
			sample_cur = smoothing_filtering(0, adc_value[1]);	/* 滤波 */
		}
	}
#else
	/* 目的是为了加快显示数据 */
    if(app_flag.calibration == 0 && g_test_time < 3)
	{
		if(cur_mode == GR)
		{
			sample_vol = Get_Adc(13);
			sample_cur = Get_Adc(12);
			smoothing_filtering(0, sample_vol);
			smoothing_filtering(1, sample_cur);
		}
		else
		{
			sample_vol = Get_Adc(12);
			sample_cur = Get_Adc(13);
			smoothing_filtering(0, sample_vol);
			smoothing_filtering(1, sample_cur);
		}
	}
	else
	{
		if(cur_mode == GR)
		{
			sample_vol = smoothing_filtering(0, Get_Adc_Average(13,5));	/* 滤波 */
			sample_cur = smoothing_filtering(1, Get_Adc_Average(12,5));
		}
		else
		{
			sample_vol = smoothing_filtering(0, Get_Adc_Average(12,5));
			sample_cur = smoothing_filtering(1, Get_Adc_Average(13,5));	/* 滤波 */
		}
	}
#endif
    
    if(test_flag.gradation == STAGE_TEST)
    {
        if(tes_t && g_dis_time < tes_t + 2)
        {
            if(cur_mode == DCW)
            {
                if(sample_cur > cur_charge_cur)
                {
                    err_charge = 0;
                }
            }
        }
    }
	
    if(app_flag.calibration == 1)
    {
        count_dis_value();
    }
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
