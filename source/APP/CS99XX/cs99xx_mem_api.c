/*
 * Copyright(c) 2015,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_mem_api.c
 * 摘  要  ：为外部存储管理提供接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#define _99xxMEM_GLOBALS

#include	<string.h>
#include    <stdio.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include    "cs99xx_mem_api.h"
#include	"RTC_Config.h"
#include    "cs99xx_err_log.h"
#include    "dc_module.h"

/*
 * 函数名：get_used_res_num
 * 描述  ：获取已使用的结果总数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：已经使用的结果数目
 */
uint16_t get_used_res_num(void)
{
    return sys_par.used_res_num;
}

uint16_t get_pass_res_num(void)
{
    return sys_par.pass_res_num;
}

uint16_t get_res_string_size(void)
{
    return sizeof(RESULT_STRING_STR);
}

uint32_t get_step_used_flag_size(void)
{
    return sizeof(step_used_flag);
}

uint32_t get_group_table_size(void)
{
    return sizeof(cur_group_table);
}

void save_as_cur_group(const uint8_t file_num)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_as_cur_group_flash(file_num);
    }
    else
    {
        save_as_cur_group_eep(file_num);
    }
}
void save_group_table(const uint8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_group_table_flash(n);
    }
    else
    {
        save_group_table_eep(n);
    }
}

void save_cur_step(void)
{
	if(-1 == check_save_step_data(g_cur_step))
	{
		return;
	}
	
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_one_step_flash(g_cur_step, g_cur_file->num, g_cur_step->one_step.com.step);
    }
    else
    {
        save_one_step_to_eep(g_cur_step, g_cur_file->num, g_cur_step->one_step.com.step);
    }
}

void save_init_group(const uint8_t n, NODE_STEP *p_node)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_init_group_flash(n, p_node);
    }
    else
    {
        save_init_group_to_eep(n, p_node);
    }
}
void save_list(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_list_flash();
    }
    else
    {
        save_list_to_eep();
    }
}

void copy_cur_file_to_new_pos(const int8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        copy_cur_file_to_new_pos_flash(n);
    }
    else
    {
        copy_cur_file_to_new_pos_eep(n);
    }
}

void save_one_result_list_dis(RESULT_STR *res, uint16_t n)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
        save_one_result_list_dis_flash(res, n);
    }
    else
    {
        save_one_result_list_dis_eep(res, n);
    }
}

void save_one_result(RESULT_STR *res, uint16_t n)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
        save_one_result_flash(res, n);
    }
    else
    {
        save_one_result_eep(res, n);
    }
}
void read_result_list_dis_one_page(RESULT_STR *res, uint16_t page, uint16_t cur_page_num)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
        read_result_list_dis_one_page_flash(res, page, cur_page_num);
    }
    else
    {
        read_result_list_dis_one_page_eep(res, page, cur_page_num);
    }
}

void read_result_list_dis_one(RESULT_STR *res, uint16_t n)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
        read_result_list_dis_one_flash(res, n);
    }
    else
    {
        read_result_list_dis_one_eep(res, n);
    }
}

void read_one_result(RESULT_STR *res, uint16_t n)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
        read_one_result_flash(res, n);
    }
    else
    {
        read_one_result_eep(res, n);
    }
}
void read_n_results(RESULT_STR *res, uint32_t num, uint16_t n)
{
    int32_t i = 0;
    
    for(i = 0; i < n; i++)
    {
        if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
        {
            read_one_result_flash(res + i, num + i);
        }
        else
        {
            read_one_result_eep(res + i, num + i);
        }
    }
}

uint16_t get_cur_upper(uint8_t mode, uint8_t gear)
{
	switch(mode)
	{
		case ACW:
			switch(gear)
			{
				case AC_200uA:
				case AC_2mA:
				case AC_20mA:
				case AC_200mA:
					return 2000;
				case AC_10mA:
				case AC_100mA:
					return 1000;
				case AC_50mA:
					return 5000;
			}
			break;
		case DCW:
			switch(gear)
			{
				case DC_2uA:
				case DC_20uA:
				case DC_200uA:
				case DC_2mA:
				case DC_20mA:
					return 2000;
				case DC_10mA:
				case DC_100mA:
					return 1000;
				case DC_50mA:
					return 5000;
			}
			break;
		case IR:
			return 1000;
		case GR:
			break;
		case BBD:
			return 100;
	}
	return 0;
}

int32_t check_sys_par(void)
{
    if(sys_par.contrast > 9)
    {
        sys_par.contrast = 5;/* 液晶对比度(0-9) */
    }
    if(sys_par.allowance > 9)
    {
        sys_par.allowance = 5;/* 余量提示 (0-9)*/
    }
    if(sys_par.is_save_res > 1)
    {
        sys_par.allowance = 1;/* 是否保存结果，0不保存，1保存 */
    }
    if(sys_par.is_overflow_cover > 1)
    {
        sys_par.is_overflow_cover = 0;/* 是否溢出覆盖 */
    }
    /************************************/
    if(sys_par.is_gfi_protect > 1)
    {
        sys_par.is_gfi_protect = GFI_OFF;/* 是否GFI保护 */
    }
    if(sys_par.test_method > 1)
    {
        sys_par.test_method = FLOAT_MODE;/* 测试方法 1接地 0浮地 */
    }
    
    if(sys_par.test_method == GND_MODE)
    {
        sys_par.is_gfi_protect = GFI_OFF;
    }
    /*************************************/
    if(sys_par.is_self_check > 1)
    {
        sys_par.is_self_check = 0;/* 是否打开开机自检功能 */
    }
    if(sys_par.language > 1)
    {
        sys_par.language = CHINESE;/* 语言切换，1中文，0英文 */
    }
    if(sys_par.is_falt_continue > 1)
    {
        sys_par.is_falt_continue = 0;/* 失败继续是否失败继续，0不继续，1继续 */
    }
    
    {/* 失败模式 */
        int32_t i = 0;
        uint8_t options = 0;
        const uint8_t *fail_mode_buf[FAIL_MODE_END] = {0};/* 电流档位 */
        uint8_t flag[FAIL_MODE_END] = {0};/* 标志 */
        
        options = define_fail_mode_kinds(fail_mode_buf, flag);
        
        for(i = 0; i < options; i++)
        {
            if(sys_par.fail_mode == flag[i])
            {
                break;
            }
        }
        
        if(i == options)
        {
            sys_par.fail_mode = flag[0];
        }
        
        if(sys_par.is_falt_continue == 1)
        {
            sys_par.fail_mode = FAIL_MODE_CON;
        }
        
        menu_par[GUI_SYS_FAIL_MODE].unit[0]       = fail_mode_pool[sys_par.fail_mode];
        menu_par[GUI_SYS_FAIL_MODE].unit[1]       = fail_mode_pool[sys_par.fail_mode];
        //定制机CS9929AX_F只允许有这一个模式
        if(g_cur_type->num == CS9929AX_F)
        {
            sys_par.fail_mode = FAIL_MODE_FPDFC;
        }
    }
    
    if(sys_par.test_level > 1)
    {
        sys_par.test_level = 1;/* 测试电平 1电平 0触点 */
    }
    
    {
        uint8_t i = 0;
        uint32_t len = strlen("2014-12-12 12:12:12");
        uint8_t *p = (uint8_t*)sys_par.data_of_survey;
        
        if(strlen((const char*)p) != len)
        {
            strcpy(sys_par.data_of_survey, "2014-12-12 12:12:12");/* 检验时间 */
        }
        else
        {
            for(i = 0; i < len; i++, p++)
            {
                if(*p >= '0' || *p <= 9 || *p == '-' || *p == ' ' || *p == ':')
                {
                    continue;
                }
                
                strcpy(sys_par.data_of_survey, "2014-12-12 12:12:12");/* 检验时间 */
                break;
            }
        }
    }
    
    if(sys_par.is_table_dis > 1)
    {
        sys_par.is_table_dis = 1;/* 是否打开列表显示功能 0，关闭，1打开 */
    }
    if(sys_par.num_rule > 1)
    {
        sys_par.num_rule = 1;/* 编号规则 (0-2) */
    }
    if(sys_par.buzzer_sw > 1)
    {
        sys_par.buzzer_sw = 1;/* 蜂鸣器开关 */
    }
    if(sys_par.commu_on_off > 1)
    {
        sys_par.commu_on_off = 0;/* 通信开关 1打开 0关闭*/
    }
    if(sys_par.baud_rate_gear > 3 || !sys_par.baud_rate_gear)
    {
        sys_par.baud_rate_gear = 1;/* 通信开关 1打开 0关闭*/
    }
    if(sys_par.local_addr > 255 || !sys_par.local_addr)
    {
        sys_par.local_addr = 1;/* 通信开关 1打开 0关闭*/
    }
    if(sys_par.plc_on_off > 1)
    {
        sys_par.plc_on_off = 1;/* PLC开关 1打开，0关闭 */
    }
    if(sys_par.keyboard_lock > 1)
    {
        sys_par.keyboard_lock = 0;/* 键盘锁 */
    }
    
// 	char pass_word[PW_LON+1];	/* 密码最多8位，最少1位 */
// 	
// 	uint16_t used_res_num;		/* 已经记录的结果总数 用于统计结果*/
// 	uint16_t pass_res_num;		/* 合格的数 用于统计结果 */
// 	uint16_t cover_res_num;		/* 当used_res_num记满4500条后就如果允许覆盖就启动该计数变量计数 当其记满4500条后清零 */
// 	
//     char dut_name[NAME_LON+1];	/* 被测件名 */
    
    if(MUTE_EN == 0)
    {
        sys_par.silent_sw = DISABLE;
    }
    
    if(type_spe.plc_signal_en == DISABLE)
    {
        sys_par.plc_signal = ALL_STEP;/* PLC信号 控制最后做综合判断 */
    }
    
    if(sys_par.output_delay > 9999)
    {
        sys_par.output_delay = 0;
    }
	
	if(g_custom_sys_par.ir_gear_hold == SYS_SW_ON)
	{
		if(sys_par.ir_gear_hold > 9999 || sys_par.ir_gear_hold < 200)
		{
			sys_par.ir_gear_hold = 200;
		}
	}
	else
	{
		sys_par.ir_gear_hold = 300;
	}
	
    if(sys_par.ir_speed_sw > IR_FILTERING_DEEP_SLOW)
	{
		sys_par.ir_speed_sw = IR_FILTERING_DEEP_FAST;
	}
	
    save_sys_par();
	
    return 0;
}
#define DATA_OUT_RANGE_  -2
int32_t check_acw_par(UN_STRUCT *p)
{
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	int32_t i = 0;
	uint16_t *temp = (uint16_t*)&p->acw.port;
    
	l_kinds = define_cur_kinds(ACW, l_gear_buf, l_flag);
    
	if(p->acw.gear_i>l_flag[l_kinds>0?l_kinds-1:0] || p->acw.gear_i<l_flag[0])
	{
        RECORD_EXCEP("OUT_RANGE 1");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.upper_limit > get_cur_upper(ACW, p->acw.gear_i))
	{
        RECORD_EXCEP("OUT_RANGE 2");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.lower_limit > p->acw.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 3");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.ac_real_cur > p->acw.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 4");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.arc_sur > type_spe.acw_arc_h)
	{
        RECORD_EXCEP("OUT_RANGE 5");
		return DATA_OUT_RANGE_;
	}
	
	if(type_spe.hz_type == HZ_TYPE_GRADE)/* 等级模式 */
	{
		l_kinds = define_hz_kinds(ACW, l_gear_buf, l_flag);
		if(p->acw.output_freq<1 || p->acw.output_freq>l_flag[l_kinds>0?l_kinds-1:0])
		{
            RECORD_EXCEP("OUT_RANGE 6");
			return DATA_OUT_RANGE_;
		}
	}
	else
	{
		if(p->acw.output_freq<FREQ_HZ_L || p->acw.output_freq>FREQ_HZ_H)
		{
            RECORD_EXCEP("OUT_RANGE 7");
			return DATA_OUT_RANGE_;
		}
	}

	if(p->acw.rise_time>9999 || (p->acw.rise_time<3 && p->acw.rise_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 8");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.testing_time>9999 || (p->acw.testing_time<3 && p->acw.testing_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 9");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.fall_time>9999 || (p->acw.fall_time<3 && p->acw.fall_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 10");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.interval_time>9999)
	{
        RECORD_EXCEP("OUT_RANGE 11");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.steps_pass>1)
	{
        RECORD_EXCEP("OUT_RANGE 12");
		return DATA_OUT_RANGE_;
	}
	if(p->acw.steps_cont>1)
	{
        RECORD_EXCEP("OUT_RANGE 13");
		return DATA_OUT_RANGE_;
	}
	for(i = 0; i < type_spe.port_num; i++)
	{
		if((((*temp)>>(i*2)) & 3) > 2)
		{
            RECORD_EXCEP("OUT_RANGE 14");
			return DATA_OUT_RANGE_;
		}
	}
	return 0;
}
uint8_t get_cc_cur_intensity_upper(void)
{
    return 3;
}
int32_t check_cc_par(UN_STRUCT *p)
{
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	int32_t i = 0;
	uint16_t *temp = (uint16_t*)&p->cc.port;
	
	l_kinds = define_cur_kinds(CC, l_gear_buf, l_flag);
	
	if(p->cc.gear_i>l_flag[l_kinds>0?l_kinds-1:0] || p->cc.gear_i<l_flag[0])
	{
        RECORD_EXCEP("OUT_RANGE 1");
		return DATA_OUT_RANGE_;
	}
	if(p->cc.upper_limit > get_cur_upper(ACW, p->cc.gear_i))
	{
        RECORD_EXCEP("OUT_RANGE 2");
		return DATA_OUT_RANGE_;
	}
	if(p->cc.lower_limit > p->cc.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 3");
		return DATA_OUT_RANGE_;
	}
	if(p->cc.ac_real_cur > p->cc.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 4");
		return DATA_OUT_RANGE_;
	}
	if(p->cc.cur_intensity > get_cc_cur_intensity_upper())
	{
        RECORD_EXCEP("OUT_RANGE 5");
		return DATA_OUT_RANGE_;
	}
	
	if(type_spe.hz_type == HZ_TYPE_GRADE)/* 等级模式 */
	{
		l_kinds = define_hz_kinds(ACW, l_gear_buf, l_flag);
		if(p->cc.output_freq<1 || p->cc.output_freq>l_flag[l_kinds>0?l_kinds-1:0])
		{
            RECORD_EXCEP("OUT_RANGE 6");
			return DATA_OUT_RANGE_;
		}
	}
	else
	{
		if(p->cc.output_freq<FREQ_HZ_L || p->cc.output_freq>FREQ_HZ_H)
		{
            RECORD_EXCEP("OUT_RANGE 7");
			return DATA_OUT_RANGE_;
		}
	}
    
// 	if(p->cc.rise_time>9999 || (p->cc.rise_time<3 && p->cc.rise_time>0))
// 	{
//         RECORD_EXCEP("OUT_RANGE 8");
// 		return DATA_OUT_RANGE_;
// 	}
// 	if(p->cc.testing_time>9999 || (p->cc.testing_time<3 && p->cc.testing_time>0))
// 	{
//         RECORD_EXCEP("OUT_RANGE 9");
// 		return DATA_OUT_RANGE_;
// 	}
// 	if(p->cc.fall_time>9999 || (p->cc.fall_time<3 && p->cc.fall_time>0))
// 	{
//         RECORD_EXCEP("OUT_RANGE 10");
// 		return DATA_OUT_RANGE_;
// 	}
// 	if(p->cc.interval_time>9999)
// 	{
//         RECORD_EXCEP("OUT_RANGE 11");
// 		return DATA_OUT_RANGE_;
// 	}
	if(p->cc.steps_pass>1)
	{
        RECORD_EXCEP("OUT_RANGE 12");
		return DATA_OUT_RANGE_;
	}
	if(p->cc.steps_cont>1)
	{
        RECORD_EXCEP("OUT_RANGE 13");
		return DATA_OUT_RANGE_;
	}
	for(i = 0; i < type_spe.port_num; i++)
	{
		if((((*temp)>>(i*2)) & 3) > 2)
		{
            RECORD_EXCEP("OUT_RANGE 14");
			return DATA_OUT_RANGE_;
		}
	}
	return 0;
}
int32_t check_dcw_par(UN_STRUCT *p)
{
	uint8_t l_mode = DCW;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	int32_t i = 0;
	uint16_t *temp = (uint16_t*)&p->dcw.port;
	
	l_kinds = define_cur_kinds(l_mode, l_gear_buf, l_flag);
	
	if(p->dcw.gear_i>l_flag[l_kinds>0?l_kinds-1:0] || p->dcw.gear_i<l_flag[0])
	{
        RECORD_EXCEP("OUT_RANGE 1");
		return DATA_OUT_RANGE_;
	}
	if(p->dcw.upper_limit > get_cur_upper(DCW, p->dcw.gear_i))
	{
        RECORD_EXCEP("OUT_RANGE 2");
		return DATA_OUT_RANGE_;
	}
	if(p->dcw.lower_limit > p->dcw.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 3");
		return DATA_OUT_RANGE_;
	}
	
	if(p->dcw.arc_sur > type_spe.dcw_arc_h)
	{
        RECORD_EXCEP("OUT_RANGE 5");
		return DATA_OUT_RANGE_;
	}
	
	if(p->dcw.delay_time>9999 || (p->dcw.delay_time<3 && p->dcw.delay_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 6");
		return DATA_OUT_RANGE_;
	}
    
	if(p->dcw.rise_time>9999 || (p->dcw.rise_time<3 && p->dcw.rise_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 7");
		return DATA_OUT_RANGE_;
	}
    
	if(p->dcw.stab_time>9999 || (p->dcw.stab_time<3 && p->dcw.stab_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 8");
		return DATA_OUT_RANGE_;
	}
    
	if(p->dcw.testing_time>9999 || (p->dcw.testing_time<3 && p->dcw.testing_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 9");
		return DATA_OUT_RANGE_;
	}
	if(p->dcw.fall_time>9999 || (p->dcw.fall_time<3 && p->dcw.fall_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 10");
		return DATA_OUT_RANGE_;
	}
	if(p->dcw.interval_time>9999)
	{
        RECORD_EXCEP("OUT_RANGE 11");
		return DATA_OUT_RANGE_;
	}
	if(p->dcw.steps_pass>1)
	{
        RECORD_EXCEP("OUT_RANGE 12");
		return DATA_OUT_RANGE_;
	}
	if(p->dcw.steps_cont>1)
	{
        RECORD_EXCEP("OUT_RANGE 13");
		return DATA_OUT_RANGE_;
	}
	for(i = 0; i < type_spe.port_num; i++)
	{
		if((((*temp)>>(i*2)) & 3) > 2)
		{
            RECORD_EXCEP("OUT_RANGE 14");
			return DATA_OUT_RANGE_;
		}
	}
	return 0;
}
int32_t check_ir_par(UN_STRUCT *p)
{
	int32_t i = 0;
	uint16_t *temp = (uint16_t*)&p->ir.port;
	
	if(p->ir.auto_shift_gears>1)
	{
        RECORD_EXCEP("OUT_RANGE 1");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.upper_limit > type_spe.ir_res_h)
	{
        RECORD_EXCEP("OUT_RANGE 2");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.upper_limit!=0 && p->ir.lower_limit > p->ir.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 3");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.delay_time>9999 || (p->ir.delay_time<3 && p->ir.delay_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 4");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.rise_time>9999 || (p->ir.rise_time<3 && p->ir.rise_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 5");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.testing_time>9999 || (p->ir.testing_time<3 && p->ir.testing_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 6");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.interval_time>9999)
	{
        RECORD_EXCEP("OUT_RANGE 7");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.steps_pass>1)
	{
        RECORD_EXCEP("OUT_RANGE 8");
		return DATA_OUT_RANGE_;
	}
	if(p->ir.steps_cont>1)
	{
        RECORD_EXCEP("OUT_RANGE 9");
		return DATA_OUT_RANGE_;
	}
	for(i = 0; i < type_spe.port_num; i++)
	{
		if((((*temp)>>(i*2)) & 3) > 2)
		{
            RECORD_EXCEP("OUT_RANGE 10");
			return DATA_OUT_RANGE_;
		}
	}
	return 0;
}
int32_t check_gr_par(UN_STRUCT *p)
{
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	
	memset(l_gear_buf, 0, sizeof l_gear_buf);
	memset(l_flag, 0, sizeof l_flag);
	l_kinds = define_vol_kinds(GR, l_gear_buf, l_flag);/* 电流种类 */
	if(p->gr.testing_cur<GR_CUR_L || p->gr.testing_cur>GR_CUR_H)
	{
        RECORD_EXCEP("OUT_RANGE 1");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.voltage_gear<l_flag[0] || p->gr.voltage_gear>l_flag[l_kinds>0?l_kinds-1:0])
	{
        RECORD_EXCEP("OUT_RANGE 2");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.upper_limit > GR_RES_H(p->gr.testing_cur))
	{
        RECORD_EXCEP("OUT_RANGE 3");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.upper_limit != 0 && p->gr.lower_limit > p->gr.upper_limit)
	{
        RECORD_EXCEP("OUT_RANGE 4");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.testing_time>9999 || (p->gr.testing_time<3 && p->gr.testing_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 5");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.interval_time>9999)
	{
        RECORD_EXCEP("OUT_RANGE 6");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.steps_pass>1)
	{
        RECORD_EXCEP("OUT_RANGE 7");
		return DATA_OUT_RANGE_;
	}
	if(p->gr.steps_cont>1)
	{
        RECORD_EXCEP("OUT_RANGE 8");
		return DATA_OUT_RANGE_;
	}
	if(type_spe.gr_hz_type == HZ_TYPE_GRADE) /* 等级模式 */
	{
		memset(l_gear_buf, 0, sizeof l_gear_buf);
		memset(l_flag, 0, sizeof l_flag);
		l_kinds = define_hz_kinds(GR, l_gear_buf, l_flag);
		if(p->gr.output_freq<1 || p->gr.output_freq>l_flag[l_kinds>0?l_kinds-1:0])
		{
            RECORD_EXCEP("OUT_RANGE 9");
			return DATA_OUT_RANGE_;
		}
	}
	else
	{
		if(p->gr.output_freq<FREQ_HZ_L || p->gr.output_freq>FREQ_HZ_H)
		{
            RECORD_EXCEP("OUT_RANGE 10");
			return DATA_OUT_RANGE_;
		}
	}
	return 0;
}
int32_t check_bbd_par(UN_STRUCT *p)
{
	if(p->bbd.open_ratio<menu_par[GUI_BBD_OPEN_CHECK].lower || p->bbd.open_ratio>menu_par[GUI_BBD_OPEN_CHECK].upper)
	{
        RECORD_EXCEP("OUT_RANGE 1");
		return DATA_OUT_RANGE_;
	}
	if(p->bbd.short_ratio<menu_par[GUI_BBD_SHORT_CHECK].lower || p->bbd.short_ratio>menu_par[GUI_BBD_SHORT_CHECK].upper)
	{
        RECORD_EXCEP("OUT_RANGE 2");
		return DATA_OUT_RANGE_;
	}
	if(p->bbd.testing_time>9999 || (p->bbd.testing_time<3 && p->bbd.testing_time>0))
	{
        RECORD_EXCEP("OUT_RANGE 3");
		return DATA_OUT_RANGE_;
	}
	if(p->bbd.interval_time > 9999)
	{
        RECORD_EXCEP("OUT_RANGE 4");
		return DATA_OUT_RANGE_;
	}
	if(p->bbd.steps_pass > 1)
	{
        RECORD_EXCEP("OUT_RANGE 5");
		return DATA_OUT_RANGE_;
	}
	if(p->bbd.steps_cont > 1)
	{
        RECORD_EXCEP("OUT_RANGE 6");
		return DATA_OUT_RANGE_;
	}
	return 0;
}
int32_t check_save_step_data(NODE_STEP *p)
{
    int32_t err = 0;
    
	/* 检查数据正确性 */
	err = 0;
	if(p == NULL)
	{
		return -1;
	}
	
	if((p->one_step.com.mode < ACW) || (p->one_step.com.mode >= MODE_END))
	{
		return -1;
	}
	
	if(-1 == check_test_mode(p))
	{
		return -1;
	}
	
	/* 数据检查错误暂不处理 */
	switch(p->one_step.com.mode)
	{
		case ACW:
			if(check_acw_par(&p->one_step))
			{
				return -1;
			}
			break;
		case DCW:
			if(check_dcw_par(&p->one_step))
			{
				return -1;
			}
			break;
		case IR:
			if(check_ir_par(&p->one_step))
			{
				return -1;
			}
			break;
		case GR:
			if(check_gr_par(&p->one_step))
			{
				return -1;
			}
			break;
		case BBD:
			if(check_bbd_par(&p->one_step))
			{
				return -1;
			}
			break;
		case CC:
			if(check_cc_par(&p->one_step))
			{
				return -1;
			}
			break;
	}
	
	return err;
}
int32_t check_test_step_data(NODE_STEP *p, uint16_t size)
{
    uint32_t i = 0;
    int32_t err = 0;
    
	/* 检查数据正确性 */
	for(i = 0; i < size; i++, p = p->next)
	{
        err = 0;
		if(p == NULL)
		{
			err--;
            RECORD_EXCEP("g_cur_step null");
			break;
		}
		
		if((p->one_step.com.mode < ACW) || (p->one_step.com.mode >= MODE_END))
		{
            RECORD_EXCEP("mode false");
			err--;
		}
        
        if(-1 == check_test_mode(p))
        {
            RECORD_EXCEP("mode false");
            err--;
        }
		
// 		if(p->one_step.com.step != i+1)
// 		{
//             RECORD_EXCEP("step false");
// 			err--;
// 		}
		
        if(err < 0)
        {
            return err;
        }
        
        /* 数据检查错误暂不处理 */
		switch(p->one_step.com.mode)
		{
			case ACW:
				if(check_acw_par(&p->one_step))
				{
                    init_acw_step(p);
                    save_one_step(p, p->one_step.com.step);
                    err--;
				}
				break;
			case DCW:
				if(check_dcw_par(&p->one_step))
				{
                    init_dcw_step(p);
                    save_one_step(p, p->one_step.com.step);
                    err--;
				}
				break;
			case IR:
				if(check_ir_par(&p->one_step))
				{
                    init_ir_step(p);
                    save_one_step(p, p->one_step.com.step);
                    err--;
				}
				break;
			case GR:
				if(check_gr_par(&p->one_step))
				{
                    init_gr_step(p);
                    save_one_step(p, p->one_step.com.step);
                    err--;
				}
				break;
			case BBD:
				if(check_bbd_par(&p->one_step))
				{
                    init_bbd_step(p);
                    save_one_step(p, p->one_step.com.step);
                    err--;
				}
				break;
            case CC:
				if(check_cc_par(&p->one_step))
				{
                    init_cc_step(p);
                    save_one_step(p, p->one_step.com.step);
                    err--;
				}
                break;
		}
        
	}
	
	return err;
}

int32_t read_list(const int8_t m)
{
	int32_t err = 0;
	
	if(0 == file_table[m].name[0])
	{
        RECORD_EXCEP("FileName null");
		return ++err;
	}
	
	if(file_table[m].total > MAX_STEPS)
	{
        RECORD_EXCEP("step Total out range");
		return ++err;
	}
    
    load_steps_to_list(1);
    
	if(list_99xx.size == 0 || file_table[m].total == 0)
	{
        RECORD_EXCEP("list_99xx.size false");
		err++;
	}
    
    if(0 > check_test_step_data(list_99xx.head, list_99xx.size))
    {
        err++;
    }
	
	return err;
}
int32_t read_ratio(void)
{
    if(g_custom_sys_par.cal_medium == CAL_MEDIUM_FLASH)
    {
        return read_ratio_from_flash();/* 读校准系数 */
    }
    else
    {
        return read_ratio_from_eep();/* 读校准系数 */
    }
}

void save_group_info(const uint8_t file_num)
{
    save_file(file_num);
    save_group_table(file_num);
    save_step_used_flag(file_num);
}

void init_par(void)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* 放模式的名字 */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* 他里面放模式的排序 */
	int16_t kinds = 0;/* 放模式的数目 */
	
    init_file_table();
    init_default_file();
    file_table[0] = default_file;
    
	save_all_file();
	
    init_group_step_used_flag();
    init_file_step_table();
    
    g_cur_file = &file_table[0];
    
	define_modes(mode_buf, flag, &kinds);
    
    insert_step(0, flag[1]);
    save_group_info(g_cur_file->num);
    read_group_table(g_cur_file->num);/* 读出当前记忆组映射表 */
    
	init_sys_flag();/* 将最近一次使用的文件设为默认文件 */
    save_sys_flag();
    read_group_table(g_cur_file->num);
    init_sys_par(&sys_par);/* 初始化系统参数 */
    save_sys_par();
    
    read_group_table(g_cur_file->num);
    
    read_all_file();
}

int32_t check_table_flag(void)
{
    int32_t i = 0;
    uint16_t temp = TABLE_VALUE_NULL;
    uint8_t temp_flag_buf[MAX_STEPS/8 + 1];
    
    if(g_cur_file->total > MAX_STEPS)
    {
        return -1;
    }
    
    read_group_table(g_cur_file->num);/* 读出当前记忆组映射表 */
    
    for(i = 0; i < g_cur_file->total; i++)
    {
        if(cur_group_table[i] == TABLE_VALUE_NULL)
        {
            RECORD_EXCEP("lTABLE_VALUE_NULL err");
            return -1;
        }
    }
    
    memset(temp_flag_buf, 0, sizeof temp_flag_buf);
    
    for(i = 0; i < g_cur_file->total; i++)
    {
        temp = cur_group_table[i];
        
        if(temp == TABLE_VALUE_NULL)
        {
            return -1;
        }
        
        if((temp_flag_buf[temp/8] & (1<<(temp%8))))
        {
            return -1;
        }
        else
        {
            temp_flag_buf[temp/8] |= (1<<(temp%8));
        }
    }
    
    read_step_used_flag(g_cur_file->num);
    
    if(0 != memcmp(temp_flag_buf, step_used_flag, sizeof step_used_flag))
    {
        return -1;
    }
    
    return 0;
}

int32_t check_dc_gr_data_valid(void)
{
	uint32_t i = 0;
	uint8_t flag = 0;
	
	if(sys_par.dc_module_using_info.count == 0 || sys_par.dc_module_using_info.count > 15)
	{
		return -1;
	}
	
	for(i = 0; i < 16; i++)
	{
		if(sys_par.dc_module_using_info.addr[i] > 15)
		{
			return -1;
		}
		
		if(sys_par.dc_module_using_info.addr[i] > 0)
		{
			flag++;
		}
		
		if(sys_par.dc_module_using_info.addr[i] == 0 && flag == 0)
		{
			return -1;
		}
	}
	
	if(flag == 0)
	{
		return -1;
	}
	
	return 0;
}


int32_t read_paramenter(void)
{
	int32_t err = 0;
	int32_t n = 0;
	
    read_sys_flag();/* 读一下 初始化标志位 */
    read_sys_par();/* 加载系统参数 */
    check_sys_par();/* 检查系统参数 */
    
	if(get_cal_pin() && (KEY_ENTER != scan_keyboard()))
	{
		BUZZER = BUZZER_ON;
		cal_warning();
		while(1);
	}
	
	LED_ALL = LED_ON;
    if(BUZZER_EN)
    {
        BUZZER_SOUND(300);/* 开机蜂鸣300ms */
    }
    
	OSTimeDlyHMSM(0,0,0,200);/* 让灯亮一会儿 让子弹飞一会儿 */
    LED_ALL = LED_OFF;
    
    
	err = read_ratio();/* 读校准系数 */
    
	if(err == -1)
	{
        RECORD_EXCEP("eep cal. data err");
		g_sys_state.ratio_state = 1;/* 标记校准系数出现错误 */
	}
    
	serve_count(BOOT_COUNT);/* 对开机次数计数 */
	
	/* 设定屏幕的分辨率 */
// 	set_contrast(sys_par.contrast);/* 软件易受干扰改由硬件设置 */
	
// 	gui_draw_build_connect_to_board();/* 建立连接界面 */
    
    /* 检查DCGR模块 */
    if(check_gr_mode() && DC_GR_EN == ENABLE)
    {
		int32_t res = 0;
		
        read_sys_par();
		res = check_dc_gr_data_valid();//检查dc_gr数据的有效性
        
		/* 数据有效可以发送获取信息指令 */
		if(0 == res)
		{
			check_dc_gr_moduls();
		}
		/* 数据无效,需要自动进行模块的扫描 */
		else
		{
			auto_scan_modules();
		}
        OSTimeDlyHMSM(0,0,0,10);
    }
    
    /* 建立连接界面 */
	gui_draw_build_connect_to_board();
	
    /* 开机自检 */
    {
        uint8_t flag = 1;
        uint32_t count = 0;
        
        while((app_flag.power_check & 0xf0) != 0xf0) //app_flag
        {
            /* 通信超时 */
            OSTimeDlyHMSM(0,0,0,100);
            if(++count > 40)// 0X1FFFFF)
            {
                gui_draw_build_connect_fail();
                flag = 0;/* 通信失败跳过自检程序 */
                break;
            }
        }
        
        if(flag)
        {
            if(sys_par.is_self_check)
            {
                gui_draw_self_check();
            }
        }
    }
    
    /* 当系统初始化标志不为1时就执行以下初始化操作 */
	if(sys_flag.mem_init != 1)
	{
		init_par();
	}
	
	read_all_file();
    
	
	/* 读出最近使用的记忆组 */
	n = sys_flag.last_group;
    
	if(n > MAX_FILES)
	{
		init_par();
		n = sys_flag.last_group;
	}
	
	g_cur_file = &file_table[n];	/* 加载当前文件 */
    
    /* 检查数据 */
    if(-1 == check_table_flag())
    {
        err = 1;
    }
    else
    {
        err = read_list(n);/* 读测试步到链表中 */
    }
	
	/* 错误处理 */
	if(err)
	{
    //注释掉保存功能，看是否还会出错 苏州晟成测了1天多发现数据丢失
//         sys_flag.mem_init = 0;
// 		
//         save_sys_flag();
        
		while(1)
		{
            /* 如果数据出错就按下复位键就进入主界面 此时要马上重启 */
			if(!STOP_PIN)
			{
				return err;
			}
		}
	}
	
	g_cur_step = list_99xx.head;	/* 加载当前步 */
	return err;
}

int32_t save_file(const uint8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        return save_file_flash(n);
    }
    else
    {
        return save_file_to_eep(n);
    }
}

void save_all_file(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_all_file_flash();
    }
    else
    {
        save_all_file_to_eep();
    }
}

void read_file(const uint8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_file_flash(n);
    }
    else
    {
        read_file_to_eep(n);
    }
}

void read_all_file(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_all_file_flash();
    }
    else
    {
        read_all_file_to_eep();
    }
}

int32_t save_sys_flag(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_sys_flag_flash();
    }
    else
    {
        save_sys_flag_to_eep();
    }
    
    return 0;
}

void read_sys_flag(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_sys_flag_flash();
    }
    else
    {
        read_sys_flag_to_eep();
    }
}

int32_t save_sys_par(void)
{
    #ifdef BOOT_LOADER
    updata_sys_language_par();
    #endif
    
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_sys_par_flash();
    }
    else
    {
        save_sys_par_to_eep();
    }
    
    return 0;
}

void read_sys_par(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_sys_par_flash();
    }
    else
    {
        read_sys_par_from_eep();
    }
}

void save_ratio(const int8_t mode)
{
    if(g_custom_sys_par.cal_medium == CAL_MEDIUM_FLASH)
    {
        if(mode == 0)
        {
            save_ratio_from_flash(ACW);
            save_ratio_from_flash(DCW);
            save_ratio_from_flash(IR);
            save_ratio_from_flash(GR);
        }
        else
        {
            save_ratio_from_flash(mode);
        }
    }
    else
    {
        if(mode == 0)
        {
            save_ratio_to_eep(ACW);
            save_ratio_to_eep(DCW);
            save_ratio_to_eep(IR);
            save_ratio_to_eep(GR);
        }
        else
        {
            save_ratio_to_eep(mode);
        }
    }
}

void init_group_step_used_flag(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        init_group_step_used_flag_flash();
    }
    else
    {
        init_group_step_used_flag_eep();
    }
}

void init_file_step_table(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        init_file_step_table_flash();
    }
    else
    {
        init_file_step_table_eep();
    }
}

void save_step_used_flag(const uint8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_step_used_flag_flash(n);
    }
    else
    {
        save_step_used_flag_eep(n);
    }
}

void read_step_used_flag(const uint8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_step_used_flag_flash(n);
    }
    else
    {
        read_step_used_flag_eep(n);
    }
}

void read_group_table(const uint8_t n)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_group_table_flash(n);
    }
    else
    {
        read_group_table_eep(n);
    }
}

uint8_t get_first_step_mode(void)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        return get_first_step_mode_flash();
    }
    else
    {
        return get_first_step_mode_eep();
    }
}
void load_steps_to_list(const int16_t step)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        load_steps_to_list_flash(step);
    }
    else
    {
        load_steps_to_list_eep(step);
    }
}

void clear_step_used_flag(void)
{
    memset(step_used_flag, 0, sizeof step_used_flag);
}

void clear_group_table(void)
{
    memset(cur_group_table, 0xff, sizeof cur_group_table);
}

void save_one_step(NODE_STEP *node, const uint16_t step)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        save_one_step_flash(node, g_cur_file->num, step);
    }
    else
    {
        save_one_step_to_eep(node, g_cur_file->num, step);
    }
}

void read_one_step(NODE_STEP *node, const uint16_t step)
{
    if(g_custom_sys_par.par_medium == PAR_MEDIUM_FLASH)
    {
        read_one_step_flash(node, g_cur_file->num, step);
    }
    else
    {
        read_one_step_to_eep(node, g_cur_file->num, step);
    }
}
/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE*******************/
