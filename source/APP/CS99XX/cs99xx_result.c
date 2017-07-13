/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_result.c
 * 摘  要  ：结果信息管理与显示
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include	<string.h>
#include 	"stdlib.h"
#include	<stdio.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"RTC_Config.h"
#include    "cs99xx_result.h"
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "mb_cmd_define.h"
void updata_result(uint8_t mode)
{
    switch(mode)
    {
        case ACW:
            result_acw();
            break;
        case DCW:
            result_dcw();
            break;
        case IR:
            result_ir();
            break;
        case GR:
            result_gr();
            break;
        case BBD:
            result_bbd();
            break;
        case CC:
            result_cc();
            break;
    }
}

void result_acw(void)
{
	cur_result.un.acw.vol = vol_ave + ROUND;        /* 测试电压 */
	cur_result.un.acw.cur = cur_ave + ROUND;        /* 测试时的电流或电阻 */
    
	if(0 == cur_real_cur)
	{
		cur_result.un.acw.real_cur = 0xffff;
	}
	else
	{
		cur_result.un.acw.real_cur = real_ave + ROUND;
	}
}

void result_dcw(void)
{
	cur_result.un.dcw.vol = vol_ave + ROUND;        /* 测试电压 */
	cur_result.un.dcw.cur = cur_ave + ROUND;        /* 测试时的电流或电阻 */
}
// /*
//  * 函数名：result_ir
//  * 描述  ：更新ir的测试结果
//  * 输入  ：无
//  * 输出  ：无
//  * 返回  ：无
//  */
void result_ir(void)
{
	cur_result.un.ir.vol = vol_ave + ROUND;         /* 测试电压 */
	
    cur_result.un.ir.out_range = 0;
	cur_result.un.ir.absorptance = ir_absorptance;//吸收比
    
    if(ERR_NUM == ERR_SHORT)
    {
        cur_result.un.ir.res = type_spe.ir_res_l;
        cur_result.un.ir.out_range = 2;
    }
    else if((cur_auto == AUTO_SHIFT_OFF) && (res_ave > gear_up_limit))
    {
        cur_result.un.ir.res = gear_up_limit;
        cur_result.un.ir.out_range = 1;
    }
    else if(res_ave > cur_ir_rang_h)
    {
        cur_result.un.ir.res = cur_ir_rang_h;
        cur_result.un.ir.out_range = 1;
    }
    else
    {
        switch(cur_gear_comm)
        {
            case IR_1_GEAR_COMM:
                cur_result.un.ir.res = res_ave*1000;
                break;
            case IR_2_GEAR_COMM:
                cur_result.un.ir.res = res_ave*100;
                break;
            case IR_3_GEAR_COMM:
                cur_result.un.ir.res = res_ave*10;
                break;
            case IR_4_GEAR_COMM:
                cur_result.un.ir.res = res_ave*1;
                break;
            case IR_5_GEAR_COMM:
                cur_result.un.ir.res = res_ave/10;
                break;
            case IR_6_GEAR_COMM:
                cur_result.un.ir.res = res_ave/100;
                break;
        }
    }
    
	cur_result.un.ir.gear = cur_gear_comm;
}

void result_gr(void)
{
	/* 压降模式 */
	if(GR_VOL_DROP_EN)
	{
		cur_result.un.gr.cur = cur_ave*10 + ROUND;
		cur_result.un.gr.gear = cur_gear;
		switch(cur_gear)
		{
			case DC_GR_20mV:
				break;
			case DC_GR_200mV:
				break;
			case DC_GR_2000mV:
				break;
            case DC_GR_12V:
            case DC_GR_12V_100mA:
            case DC_GR_12V_1000mA:
				cur_result.un.gr.vol = (vol_ave+5)/10;      /* 测试时的电压 */
				break;
		}
	}
	/* GR模式 */
	else
	{
		cur_result.un.gr.cur = cur_ave*100 + ROUND;         /* 测试电流 */
		cur_result.un.gr.res = res_ave;                     /* 测试时的电阻 */
	}
}

void result_bbd(void)
{
	cur_result.un.bbd.vol = vol_ave + ROUND;                /* 测试电压 */
    cur_result.un.bbd.base_cap = cur_cap * 1000 + 0.5;      /* 测试时的 基准电容 单位pF */
    cur_result.un.bbd.open_r = cur_open_ratio;              /* 开路检测 单位% */
    cur_result.un.bbd.short_r = cur_short_ratio;            /* 短路检测 单位% */
    
	if(cap_ave > 0)
	{
		cur_result.un.bbd.cap = cap_ave * 1000 + 0.5;       /* 测试时的电容 单位pF */
	}
}

void result_cc(void)
{
	cur_result.un.cc.vol = vol_ave + ROUND;                /* 测试电压 */
	cur_result.un.cc.cur = cur_ave + ROUND;                /* 测试时的电流或电阻 */
}


void save_cur_result(RESULT_STR *res)
{
    RESULT_STR temp_res;
    
    if(CUR_FAIL)
    {
        res->test_time = g_dis_time;
        res->result = RES_FAIL;/* 测试结果 1 P,0 N */
    }
    else
    {
        res->result = RES_PASS;/* 测试结果 1 P,0 N */
    }
    
	/* 每次测试只能保存一次 */
	if(test_flag.save_only_one == 0)
	{
		test_flag.save_only_one++;
	}
    else
	{
		return;
	}
    
//////////////////////////////////////////////////////////////////////
	
    /* 当前结果无条件保存在结果列表里面 */
    save_one_result_list_dis(res, cur_step - 1);
    app_flag.res_list_num_count++;
	save_sys_flag();/* 保存结果列表中的结果编号最后一个值 每天自动清理一次 */
//////////////////////////////////////////////////////////////////////
    
    /* 结果保存关闭 */
	if(!sys_par.is_save_res)
	{
		return;
	}
    
	/* 保存结果 */
	if(sys_par.used_res_num >= get_res_total())
	{
		if(!sys_par.is_overflow_cover)
		{
			return;/* 如果不允许覆盖就直接返回 */
		}
        
		if(sys_par.cover_res_num >= get_res_total())
		{
			sys_par.cover_res_num = 0;/*如果覆盖计数达到n就清0 n为最大值 */
		}
		sys_par.cover_res_num++;
		
        read_one_result(&temp_res, sys_par.cover_res_num - 1);
        
        if(res->result != temp_res.result)
        {
            if(res->result == RES_PASS)
            {
                sys_par.pass_res_num += 1;
            }
            else
            {
                sys_par.pass_res_num -= 1;
            }
        }
        
        save_sys_par();
		save_result(res, SAVE_RESULT);
	}
	else
	{
		sys_par.used_res_num += 1;
        
        if(res->result == RES_PASS)
        {
            sys_par.pass_res_num += 1;
        }
        
        save_sys_par();
		
		save_result(res, SAVE_RESULT);
	}
}

uint32_t get_res_total(void)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
		return F_RES_TOTAL;
    }
    else
    {
        return E_RES_TOTAL;
    }
}

// /*
//  * 函数名：save_result
//  * 描述  ：保存测试结果
//  * 输入  ：stat 可以取INIT_RESULT，SAVE_RESULT 分别表示初始化结果数据 和 保存结果
//  * 输出  ：无
//  * 返回  ：无
//  */
void save_result(RESULT_STR *res, const int8_t stat)
{
	int32_t i = 0;
	
	i = i;
	
    if(res == NULL)
    {
        return;
    }
    
	if(stat == INIT_RESULT)
	{
		uint8_t buf1[20] = {0};
		uint8_t buf2[20] = {0};
		uint32_t cur_date;
		
		cur_date = Time_GetTime();/* 保存当前RTC时间 */
		
		transform_data_to_str(sys_flag.last_res_date, (char*)buf1);
		transform_data_to_str(cur_date, (char*)buf2);
		
		if(0 != strcmp((const char*)buf1, (const char*)buf2))
		{
			sys_flag.res_count = 0;
		}
		
		sys_flag.last_res_date = cur_date;
		
		g_serial_num = sys_flag.res_count;
        
		memset(res, 0, sizeof(RESULT_STR));
		res->step = g_cur_step->one_step.com.step;	/* 结果当前步 */
		res->total = g_cur_file->total;	/* 总步数 */
		res->mode = g_cur_step->one_step.com.mode;	/* 当前步模式 */
		res->work_mode = g_cur_file->work_mode;/* 工作模式 由测试时的文件工作模式决定 */
		res->time = cur_date;/* 保存当前RTC时间 */
		res->test_time = tes_t;
		
		if(res->step == 1)
		{
			app_flag.res_list_num_count = 0;
		}
		
		if(strlen((const char*)sys_par.dut_name) > NAME_LON)
		{
			sys_par.dut_name[NAME_LON] = 0;
			save_sys_par();
		}
        
		strcpy((char*)res->file_name, (const char*)g_cur_file->name);
		
		/* 编号规则 */
		switch(sys_par.num_rule)
		{
			case 0:
				sprintf((char*)res->dut_name, "%05d", ++g_serial_num);
				break;
			case 1:
				if(cur_step == 1)
				{
					++g_serial_num;
				}
				sprintf((char*)res->dut_name, "%05d", g_serial_num);
				break;
			case 2:
				strcpy((char*)res->dut_name, (const char*)sys_par.dut_name);
				break;
		}
		
		sys_flag.res_count = g_serial_num;
		
		res->result = RES_PASS;/* 测试结果 1 P,0 N */
		
		switch(cur_mode)
		{
			case ACW:
				res->un.acw.gear = cur_gear_comm;
				res->un.acw.real_gear = cur_gear;
                result_acw();
				break;
            case CC:
				res->un.cc.gear = cur_gear_comm;
				res->un.cc.real_gear = cur_gear;
                break;
			case DCW:
				res->un.dcw.real_gear = cur_gear;
				res->un.dcw.gear = cur_gear_comm;
				break;
			case IR:
				res->un.ir.gear = cur_gear_comm;
				break;
			case GR:
				res->un.gr.gear = cur_gear;
				break;
			case BBD:
				break;
		}
	}
	else if(stat == SAVE_RESULT)
	{
		uint16_t n = 0;
		uint16_t num = 0;
        
		if(res->step == 0)
		{
			return;
		}
		else if(res->mode >= MODE_END)
		{
			return;
		}
        
		if(!sys_par.is_save_res)
		{
			return;
		}
		
		if(sys_par.used_res_num >= get_res_total() && sys_par.cover_res_num)
		{
			num = sys_par.cover_res_num;
		}
		else
		{
			num = sys_par.used_res_num;
		}
        
        num = num;
        
        n = num;
        
        if(n < 1 || n > get_res_total())
        {
            return;
        }
        
        save_one_result(res, n - 1);
	}
}

void ch_out_result_to_string_comm(const RESULT_STR *res, char* buf)
{
	switch(res->mode)
	{
        default:
        case ACW:
		{
			switch(type_spe.acw_vol_range)
			{
                default:
                case ACW_500V:
				case ACW_1kV:
                case ACW_5kV:
                    mysprintf((uint8_t*)buf  , NULL, 153, res->un.acw.vol);
					break;
				case ACW_10kV:
                    if(res->un.acw.vol < 10000)
                    {
                        mysprintf((uint8_t*)buf  , NULL, 153, res->un.acw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)buf  , NULL, 152, res->un.acw.vol/10);
                    }
					break;
				case ACW_20kV:
				case ACW_30kV:
                    mysprintf((uint8_t*)buf  , NULL, 152, res->un.acw.vol/10);
                    break;
			}
			break;
		}
		case DCW:
		{
			switch(type_spe.dcw_vol_range)
			{
                default:
                case DCW_500V:
                case DCW_2kV:
				case DCW_6kV:
                    mysprintf((uint8_t*)buf  , NULL, 153, res->un.dcw.vol);
                    break;
				case DCW_10kV:
                    if(res->un.acw.vol < 10000)
                    {
                        mysprintf((uint8_t*)buf  , NULL, 153, res->un.dcw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)buf  , NULL, 152, res->un.dcw.vol/10);
                    }
					break;
				case DCW_20kV:
                    mysprintf((uint8_t*)buf  , NULL, 152, res->un.dcw.vol/10);
					break;
			}
			break;
		}
		case IR:
		{
            mysprintf((uint8_t*)buf  , NULL, 153, res->un.ir.vol);
            break;
		}
		case GR:
		{
            switch(type_spe.gr_cur_unit)
            {
                case CUR_U_A:
                    mysprintf((uint8_t*)buf  , NULL, 152, res->un.gr.cur);
                    break;
                case CUR_U_mA:
                    mysprintf((uint8_t*)buf  , NULL, 151, res->un.gr.cur);
                    break;
            }
			break;
		}
		case BBD:
		{
            mysprintf((uint8_t*)buf  , NULL, 153, res->un.bbd.vol);
            break;
		}
    }
}


void transfer_ir_res_out_range_loop_to_string(const RESULT_STR *res, char* buf)
{
    uint32_t t = 1;
    uint32_t b =  1;
    uint32_t decs = 0;
    
    switch(res->un.ir.gear)
    {
        case IR_1_GEAR_COMM:
            decs = 1;
            b = 1;
            break;
        case IR_2_GEAR_COMM:
            decs = 0;
            b = 1;
            break;
        case IR_3_GEAR_COMM:
            decs = 0;
            b = 1;
            break;
        case IR_4_GEAR_COMM:
            if(res->un.ir.res < 1000) // >1000M
            {
                decs = 0;
                b = 1;
            }
            else if(res->un.ir.res < 10 * 1000)
            {
                decs = 2;
                b = 1000; // > 1.00G
            }
            else
            {
                decs = 1;
                b = 1000; // > 10.0G
            }
            break;
        case IR_5_GEAR_COMM:
            decs = 0;
            b = 1000; // > 100G
            break;
//         case IR_6_GEAR_COMM:
//             decs = 0;
//             b = 1000;
//             break;
    }
    
    t = ten_power(decs);
    
    mysprintf((uint8_t*)buf, NULL, decs + 140, res->un.ir.res / b * t);
}
void ch_loop_result_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case ACW:
        {
            mysprintf((uint8_t*)buf, NULL, ac_gear[res->un.acw.real_gear].decs + 150, res->un.acw.cur);
            break;
        }
        case DCW:
        {
            mysprintf((uint8_t*)buf, NULL, dc_gear[res->un.dcw.real_gear].decs + 150, res->un.dcw.cur);
            break;
        }
        case IR:
        {
            if(res->un.ir.out_range == 0)
			{
                mysprintf((uint8_t*)buf, NULL, ir_gear[res->un.ir.gear].decs + 150, res->un.ir.res);
			}
			else if(res->un.ir.out_range == 1)
			{
                uint8_t t_buf[20] = {0};
                
                transfer_ir_res_out_range_loop_to_string(res, (char*)t_buf);
                mystrcat((uint8_t*)buf, ">", (uint8_t*)t_buf);
			}
            else
            {
                sprintf(buf  , "< %d"        , res->un.ir.res);
            }
            break;
        }
        case GR:
        {
			switch(res->un.gr.gear)
			{
				case DC_GR_20mV:
					break;
				case DC_GR_200mV:
					break;
				case DC_GR_2000mV:
					break;
				case DC_GR_12V:
                case DC_GR_12V_100mA:
                case DC_GR_12V_1000mA:
                    mysprintf((uint8_t*)buf, NULL, 152, res->un.gr.vol);
					break;
				default:
                    if(res->err_num == ERR_OPEN)
                    {
                        sprintf(buf  , "> %d"        , res->un.gr.res / 10);
                    }
                    else
                    {
                        mysprintf((uint8_t*)buf, NULL, 151, res->un.gr.res);
                    }
					break;
			}
            break;
        }
        case BBD:
        {
			float temp = res->un.bbd.cap/1000.0;
            uint8_t decs = 0;
			
			if(temp < 10)
			{
                decs = 3;
			}
			else if(temp < 100)
			{
                decs = 2;
			}
			else if(temp < 1000)
			{
                decs = 1;
			}
            
            mysprintf((uint8_t*)buf  , NULL, 150 + decs, temp * ten_power(decs));
            break;
        }
    }
}
void ch_real_result_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case ACW:
        {
            /* 表示真实电流功能关闭 */
            if(res->un.acw.real_cur == 0xffff)
            {
                sprintf(buf   , "-----");
            }
            else
            {
                mysprintf((uint8_t*)buf, NULL, ac_gear[res->un.acw.real_gear].decs + 150, res->un.acw.real_cur);
            }
            break;
        }
        case DCW:
        case IR:
        case GR:
        {
            sprintf(buf   , "-----");
            break;
        }
        /* BBD的基准电容 */
        case BBD:
        {
			float temp = res->un.bbd.base_cap/1000.0;
            uint8_t decs = 0;
			
			if(temp < 10)
			{
                decs = 3;
			}
			else if(temp < 100)
			{
                decs = 2;
			}
			else if(temp < 1000)
			{
                decs = 1;
			}
            
            mysprintf((uint8_t*)buf  , NULL, 150 + decs, temp * ten_power(decs));
            break;
        }
    }
}

void ch_open_ratio_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case BBD:
        {
            uint16_t temp = res->un.bbd.open_r;
            mysprintf((uint8_t*)buf  , NULL, 130, temp);
            break;
        }
    }
}

void ch_short_ratio_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case BBD:
        {
            uint16_t temp = res->un.bbd.short_r;
            mysprintf((uint8_t*)buf  , NULL, 130, temp);
            break;
        }
    }
}

void ch_range_result_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        default:
        case ACW:
            sprintf(buf  , "%1d"        , res->un.acw.gear);
            break;
        case DCW:
            sprintf(buf  , "%1d"        , res->un.dcw.gear);
            break;
        case IR:
        {
            uint8_t temp = res->un.ir.gear;
            if(res->un.ir.gear > 3)
            {
                temp = 5;
            }
            sprintf(buf  , "%1d"        , temp);
            break;
        }
        case GR:
            sprintf(buf  , "");
            break;
        case BBD:
            sprintf(buf  , "");
//             sprintf(buf  , "%1d"        , res->un.bbd.gear);
            break;
        case CC:
            sprintf(buf  , "%1d"        , res->un.cc.gear);
            break;
    }
}
void add_other_option(const RESULT_STR *res, char* buf)
{
	switch(res->mode)
	{
        case IR:
		{
			/* 吸收比不为0时添加 */
			if(res->un.ir.absorptance > 0)
			{
				mysprintf((uint8_t*)buf  , NULL, 141, res->un.ir.absorptance * 10);
			}
			break;
		}
	}
}

void change_result_to_string_comm(const RESULT_STR *res, char* res_buf)
{
    char buf[20] = {0};
    
    switch(sys_par.num_rule)
    {
        case 0:
        case 2:
            sprintf(buf   , "%s"        , res->dut_name);
            break;
        case 1:
            if(res->step == 1)
            {
                sprintf(buf   , "%s"        , res->dut_name);
            }
            else
            {
                sprintf(buf   , "");
            }
            break;
    }
    
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
    sprintf(buf       , "%02d,%02d,"     , res->step, res->total);
    strcat(res_buf, buf);
    
    sprintf(buf  , "%s,"        , work_mode_buf[res->work_mode]);
    strcat(res_buf, buf);
    
    sprintf(buf  , "%1d,"        , getTestMode(res->mode));
    strcat(res_buf, buf);
    
    sprintf(buf  , "\"%s\","        , res->file_name);
    strcat(res_buf, buf);
    
    ch_out_result_to_string_comm(res, buf);
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
    ch_range_result_to_string_comm(res, buf);
    strcat(res_buf, buf);
    if(res_buf[strlen(res_buf) - 1] != ',')
    {
        strcat(res_buf, ",");
    }
    
    ch_loop_result_to_string_comm(res, buf);
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
	/* BBD增加了 基准电容 */
	ch_real_result_to_string_comm(res, buf);
	strcat(res_buf, buf);
	strcat(res_buf, ",");
	
    /* BBD增加了 开路检测和短路检测 */
    if(res->mode == BBD)
    {
        ch_open_ratio_to_string_comm(res, buf);
        strcat(res_buf, buf);
        strcat(res_buf, ",");
        ch_short_ratio_to_string_comm(res, buf);
        strcat(res_buf, buf);
        strcat(res_buf, ",");
    }
    
    mysprintf((uint8_t*)buf  , NULL, 151, res->test_time);
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
    if(res->result == RES_PASS)
	{
        sprintf(buf  , "P");
	}
    else
    {
        sprintf(buf  , "F");
    }
    
    strcat(res_buf, buf);
//    #ifdef SUZHOU_MUTE
    strcat(res_buf, ",");
    
    sprintf(buf, "%s", transform_time_to_string(res->time, NULL));
    strcat(res_buf, buf);
	
	buf[0] = 0;
	add_other_option(res, buf);
	
	if(buf[0] != 0)
	{
		strcat(res_buf, ",");
		strcat(res_buf, buf);
	}
//    #endif
}




void ch_out_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str)
{
	switch(res->mode)
	{
		case ACW:
        case CC:
		{
			switch(type_spe.acw_vol_range)
			{
                default:
                case ACW_500V:
				case ACW_1kV:
                case ACW_5kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.acw.vol);
					break;
				case ACW_10kV:
                    if(res->un.acw.vol < 10000)
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.acw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.acw.vol/10);
                    }
					break;
				case ACW_20kV:
				case ACW_30kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.acw.vol/10);
                    break;
			}
			break;
		}
		case DCW:
		{
			switch(type_spe.dcw_vol_range)
			{
                default:
                case DCW_500V:
                case DCW_2kV:
				case DCW_6kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.dcw.vol);
                    break;
				case DCW_10kV:
                    if(res->un.dcw.vol < 10000)
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.dcw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.dcw.vol/10);
                    }
					break;
				case DCW_20kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.dcw.vol/10);
					break;
			}
			break;
		}
		case IR:
		{
            mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.ir.vol);
            break;
		}
		case GR:
		{
            switch(type_spe.gr_cur_unit)
            {
                case CUR_U_A:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[CUR_U_A], 52, res->un.gr.cur);
                    break;
                case CUR_U_mA:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[CUR_U_mA], 51, res->un.gr.cur);
                    break;
            }
			break;
		}
		case BBD:
		{
            mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.bbd.vol);
            break;
		}
    }
}


uint8_t get_ir_res_uint(const RESULT_STR *res)
{
    if(res->un.ir.gear <= IR_3_GEAR_COMM)
    {
        return RES_U_MOHM;
    }
//     else if(res->un.ir.gear == IR_4_GEAR_COMM)
//     {
//         if(res->un.ir.res < 1000) 
//         {
//             return RES_U_MOHM;
//         }
//         else
//         {
//             return RES_U_GOHM;
//         }
//     }
    else
    {
        return RES_U_GOHM;
    }
}

void transfer_ir_res_loop_to_string(const RESULT_STR *res, char* buf)
{
    uint32_t t = 1;
    uint8_t decs = 0;
    
    switch(res->un.ir.gear)
    {
        case IR_1_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 3;
                t = 1;
            }
            else
            {
                decs = 2;
                t = 10;
            }
            break;
        case IR_2_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 2;
                t = 1;
            }
            else
            {
                decs = 1;
                t = 10;
            }
            break;
        case IR_3_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 1;
                t = 1;
            }
            else
            {
                decs = 0;
                t = 10;
            }
            break;
        case IR_4_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 3;
                t = 1;
            }
            else
            {
                decs = 2;
                t = 10;
            }
            break;
        case IR_5_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 2;
                t = 1;
            }
            else
            {
                decs = 1;
                t = 10;
            }
            break;
//                     case IR_6_GEAR_COMM:
//                         mysprintf((uint8_t*)res_str->test_loop  , unit_pool[RES_U_GOHM], 51, res->un.ir.res);
//                         break;
    }
    
    mysprintf((uint8_t*)buf  , NULL, decs + 150, res->un.ir.res / t);
}

void ch_loop_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str)
{
    uint8_t buf[20] = {0};
    uint8_t c = 0;
    uint8_t unit = RES_U_MOHM;
    
    sprintf((char*)res_str->real_cur   , "--");
    
    switch(res->mode)
    {
        default:
        case ACW:
        {
            switch(res->un.acw.real_gear)
            {
                case AC_200uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 51, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool_[CUR_U_uA], 51, res->un.acw.real_cur);
                    break;
                case AC_2mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 53, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool[CUR_U_mA], 53, res->un.acw.real_cur);
                    break;
                case AC_10mA:
                case AC_20mA:
                case AC_50mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 52, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool[CUR_U_mA], 52, res->un.acw.real_cur);
                    break;
                case AC_100mA:
                case AC_200mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 51, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool[CUR_U_mA], 51, res->un.acw.real_cur);
                    break;
            }
            
            /* 表示真实电流功能关闭 */
            if(res->un.acw.real_cur == 0xffff)
            {
                    sprintf((char*)res_str->real_cur   , "--");
            }
            break;
        }
        case DCW:
        {
            switch(res->un.dcw.real_gear)
            {
                case DC_2uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 53, res->un.dcw.cur);
                    break;
                case DC_20uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 52, res->un.dcw.cur);
                    break;
                case DC_200uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 51, res->un.dcw.cur);
                    break;
                case DC_2mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 53, res->un.dcw.cur);
                    break;
                case DC_10mA:
                case DC_20mA:
                case DC_50mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 52, res->un.dcw.cur);
                    break;
                case DC_100mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 51, res->un.dcw.cur);
                    break;
            }
            break;
        }
        case IR:
        {
            /* 正常显示 */
            if(res->un.ir.out_range == 0)
			{
                transfer_ir_res_loop_to_string(res, (char*)buf);
                unit = get_ir_res_uint(res);
                mystrcat((uint8_t*)res_str->test_loop, (uint8_t*)div_str_pre_zero((char*)buf), (uint8_t *)unit_pool[unit]);
            }
            /* 超量程 */
			else if(res->un.ir.out_range == 1)
			{
                transfer_ir_res_out_range_loop_to_string(res, (char*)buf);
                
                unit = get_ir_res_uint(res);
                mystrcat((uint8_t*)buf, (uint8_t*)div_str_pre_zero((char*)buf), (uint8_t *)unit_pool[unit]);
                c = strlen((const char*)buf) - strlen((const char*)unit_pool[unit]);
                
                if(c < 4)
                {
                    front_add_space_at_string(buf, 4 - c);
                }
                
                mystrcat((uint8_t*)res_str->test_loop, ">", (uint8_t*)buf);
			}
            /* 短路报警 */
            else
            {
                sprintf((char*)res_str->test_loop  , "< %dMΩ", res->un.ir.res);
            }
            break;
        }
        case GR:
        {
			switch(res->un.gr.gear)
			{
				case DC_GR_20mV:
					break;
				case DC_GR_200mV:
					break;
				case DC_GR_2000mV:
					break;
				case DC_GR_12V:
                case DC_GR_12V_100mA:
                case DC_GR_12V_1000mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[VOL_U_V], 52, res->un.gr.vol);
					break;
				default:
                    if(res->err_num == ERR_OPEN)
                    {
                        sprintf((char*)res_str->test_loop  , "> %dmΩ"        , res->un.gr.res / 10);
                    }
                    else
                    {
                        mysprintf((uint8_t*)res_str->test_loop  , unit_pool[RES_U_mOHM], 51, res->un.gr.res);
                    }
					break;
			}
            break;
        }
        case BBD:
        {
			float temp = res->un.bbd.cap/1000.0;
            uint8_t decs = 0;
			
			if(res->err_num == ERR_SHORT || res->err_num == ERR_OPEN )
			{
				strcpy((char*)res_str->test_loop, "----nF");
			}
			else
			{
				if(temp < 10)
				{
					decs = 3;
				}
				else if(temp < 100)
				{
					decs = 2;
				}
				else
				{
					decs = 1;
				}
				
				mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CAP_U_nF], decs + 50, temp * ten_power(decs));
			}
			break;
        }
        case CC:
        {
            switch(res->un.cc.real_gear)
            {
                case AC_200uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 51, res->un.cc.cur);
                    break;
                case AC_2mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 53, res->un.cc.cur);
                    break;
                case AC_10mA:
                case AC_20mA:
                case AC_50mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 52, res->un.cc.cur);
                    break;
                case AC_100mA:
                case AC_200mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 51, res->un.cc.cur);
                    break;
            }
            break;
        }
    }
}


typedef struct{
    const char *gear;
}RES_GEAR_STR;

const RES_GEAR_STR res_ac_gear[]=
{
    {0},//0
    {"2uA"   },//2uA
    {"20uA"  },//20uA
    {"200uA" },//200uA
    {"2mA"   },
    {"10mA"  },
    {"20mA"  },
    {"50mA"  },
    {"100mA" },
    {"200mA" },
};

const RES_GEAR_STR res_dc_gear[]=
{
    {0},//0
    {"2uA"   },//2uA
    {"20uA"  },//20uA
    {"200uA" },//200uA
    {"2mA"   },
    {"10mA"  },
    {"20mA"  },
    {"50mA"  },
    {"100mA" },
    {"200mA" },
};

void ch_range_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str)
{
    switch(res->mode)
    {
        case ACW:
            sprintf((char*)res_str->range  , "%s"        , res_ac_gear[res->un.acw.real_gear].gear);
            break;
        case DCW:
            sprintf((char*)res_str->range  , "%s"        , res_dc_gear[res->un.dcw.real_gear].gear);
            break;
        case IR:
            sprintf((char*)res_str->range  , "--");
            break;
        case GR:
            sprintf((char*)res_str->range  , "--");
            break;
        case BBD:
//             sprintf((char*)res_str->range  , "%s"        , res_ac_gear[res->un.bbd.gear].gear);
            break;
        case CC:
            sprintf((char*)res_str->range  , "%s"        , res_ac_gear[res->un.cc.gear].gear);
            break;
    }
}


void change_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str, uint16_t n)
{
    sprintf((char*)res_str->num        , "%d"        , n);
    
    switch(sys_par.num_rule)
    {
        case 0:
        case 2:
            sprintf((char*)res_str->dut_name   , "%s"        , res->dut_name);
            break;
        case 1:
            if(res->step == 1)
            {
                sprintf((char*)res_str->dut_name   , "%s"        , res->dut_name);
            }
            else
            {
                sprintf((char*)res_str->dut_name   , "");
            }
            break;
    }
    
    sprintf((char*)res_str->file_name  , "%s"        , res->file_name);
    sprintf((char*)res_str->step       , "`%02d/%02d"     , res->step, res->total);
    sprintf((char*)res_str->work_mode  , "%s"        , work_mode_buf[res->work_mode]);
    sprintf((char*)res_str->test_mode  , "%s"        , mode_pool[GR_VOL_DROP_EN][res->mode]);
    mysprintf((uint8_t*)res_str->test_time  , "s", 51, res->test_time);
    
    ch_out_result_to_string(res, res_str);
    ch_loop_result_to_string(res, res_str);
    ch_range_result_to_string(res, res_str);
    
    if(res->result == RES_PASS)
	{
        sprintf((char*)res_str->result  , "P");
	}
    else
    {
        sprintf((char*)res_str->result  , "F");
    }
    
    sprintf((char*)res_str->time  , "%s", transform_time_to_string(res->time, NULL));
}

