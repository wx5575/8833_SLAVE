/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_for_comm.h
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
/******************** 加载文件 ************************/

#include	<string.h>
#include	"RTC_Config.h"
#include 	"app.h"
#include	"irq.h"
#include	"cs99xx_struct.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_exception.h"
#include	"cs99xx_type.h"
#include 	"SysTemConfig.h"
#include	"keyboard.h"
#include    "mb_server.h"
#include    "mb_cmd_define.h"
#include    "cs99xx_result.h"
#include 	"auto_cal_comm.h"


/******************** 函数定义 ************************/

uint8_t get_cur_gear_par_form(uint8_t mode)
{
    uint8_t type = 0;
    
    /* 注意:15x,1标示数据带前导零 5表示最大长度为5 x表示小数位数 */
    switch(mode)
    {
        case ACW:
            type = ac_gear[cur_gear].decs + 150;
            break;
        case DCW:
            type = dc_gear[cur_gear].decs + 150;
            break;
        case IR:
            type = ir_gear[cur_gear].decs + 150;
            break;
        case GR:
            type = 1 + 150;
            break;
    }
    
    return type;
}
uint8_t get_vol_gear_par_form(uint8_t mode)
{
    uint8_t type = 0;
    
    /* 注意:15x,1标示数据带前导零 5表示最大长度为5 x表示小数位数 */
    switch(mode)
    {
        case ACW:
            type = acw_vol_info[type_spe.acw_vol_range].decs + 150;
            break;
        case DCW:
            type = dcw_vol_info[type_spe.dcw_vol_range].decs + 150;
            break;
        case IR:
            type = ir_vol_info[type_spe.ir_vol_range].decs + 150;
            break;
        case GR:
            type = 2 + 150;
            break;
        case BBD:
            type = acw_vol_info[type_spe.acw_vol_range].decs + 150;
            break;
    }
    
    return type;
}
static FILE_WORK_MODE_TYPE getFileModeType(uint8_t mode)
{
	FILE_WORK_MODE_TYPE mode_type;
    
	if(mode == G_MODE)
    {
		mode_type = G_TYPE;
	}
	else if(mode == N_MODE)
    {
		mode_type = N_TYPE;
	}
	
	return mode_type;
}
static FILE_ARC_TYPE getFileArcType(uint8_t mode)
{
	FILE_ARC_TYPE arc_type;
    
	if(mode == ARC_GRADE_MODEL)
    {
		arc_type = SCALE_TYPE;
	}
	else if(mode == ARC_CUR_MODEL)
    {
		arc_type = CURRENT_TYPE;
	}
	
	return arc_type;
}
static uint8_t getFileModeType_f(FILE_WORK_MODE_TYPE mode_type)
{
	uint8_t mode = 0;
    
	if(mode_type == G_TYPE)
    {
		mode = G_MODE;
	}
	else
    {
		mode = N_MODE;
	}
	
	return mode;
}
static uint8_t getFileArcType_f(FILE_ARC_TYPE mode)
{
	 uint8_t arc_type;
    
	if(mode == SCALE_TYPE)
    {
		arc_type = ARC_GRADE_MODEL;
	}
	else
    {
		arc_type = ARC_CUR_MODEL;
	}
	
	return arc_type;
}
uint8_t getTestMode(uint8_t mode_num)
{
	uint8_t mode;
	switch(mode_num)
	{
		case ACW:
			mode = ACW_MODE;
			break;
		case DCW:
			mode = DCW_MODE;
			break;
		case IR:
			mode = IR_MODE;
			break;
		case GR:
			mode = GR_MODE;
			break;
		case BBD:
			mode = BBD_MODE;
			break;
	}
	return mode;
}

static uint8_t getTestMode_f(uint8_t mode_num)
{
	uint8_t mode;
	switch(mode_num)
	{
		case ACW_MODE:
			mode = ACW;
			break;
		case DCW_MODE:
			mode = DCW;
			break;
		case IR_MODE:
			mode = IR;
			break;
		case GR_MODE:
			mode = GR;
			break;
		case BBD_MODE:
			mode = BBD;
			break;
	}
	return mode;
}


int16_t check_step_exist(const uint16_t n)
{
// 	return position_step(n)!=NULL?1:0;
    return (check_step_exist_for_comm(n) == 0? 1 : 0);
}
void init_file_group(const uint8_t n)
{
// 	NODE_STEP temp_node;
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* 放模式的名字 */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* 他里面放模式的排序 */
	int16_t kinds = 0;/* 放模式的数目 */
    TEST_FILE * f_back = g_cur_file;
	
	define_modes(mode_buf, flag, &kinds);
	
// 	temp_node.one_step.com.step = 1;
// 	temp_node.one_step.com.mode = flag[1];
// 	init_mode(&temp_node.one_step);
    g_cur_file = pos_file(n);
    g_cur_file->total = 0;
    
    clear_step_used_flag();
    clear_group_table();
    
    insert_step(0, flag[1]);
    
    save_group_info(n);
    
    g_cur_file = f_back;
    
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
}

// int16_t set_cur_file_for_comm(const int16_t file_num)
// {
// 	if(0 == check_file_exist(file_num))
// 	{
// 		return DATA_OUT_RANGE;/* 文件不存在 */
// 	}
// 	
// 	g_cur_file = pos_file(file_num);/* 重新定位当前文件 */
// 	sys_flag.last_group = g_cur_file->num;/* 更新最后使用的文件标志 */
// 	save_sys_flag();
// 	/* 读出最近使用的记忆组 */
// 	if(read_list(g_cur_file->num))
// 	{
// 		sys_flag.mem_init = 0;
// 		save_sys_flag();
// 		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
// 	}
// 	
// 	g_cur_step = list_99xx.head;
// 	test_flag.refresh_for_comm = 1;/* 刷新屏幕 */
// 	
// 	return NO_ERR;
// }

// int16_t set_cur_step_for_comm(const int16_t step_num)
// {
// 	if(0 == check_step_exist(step_num))
// 	{
// 		return DATA_OUT_RANGE;/* 文件不存在 */
// 	}
//     
//     load_steps_to_list(step_num);
//     g_cur_step = list_99xx.head;
//     
// 	test_flag.refresh_for_comm = 1;/* 刷新屏幕 */
// 	
// 	return NO_ERR;
// }

/* 文件另存为 */
int16_t app_save_file_for_comm(FILE_MSG * file)
{
    /*保存文件的时候文件编号和文件名是自身的，其余数据均是活动文件的数据*/
    /*active_file当前活动文件*/
	TEST_FILE *p = &file_table[file->file_num];
//     FILE_MSG active_file;
    
    if(p->num != file->file_num)
    {
        return PAR_NOT_ALLOWED;
    }
    if(file->file_name[0] == 0)
    {
        return PAR_NOT_ALLOWED;
    }
    
    *p = *g_cur_file;//拷贝当前文件信息
    p->num = file->file_num;
    strncpy(p->name, (const char*)file->file_name, NAME_LON);
    
    save_file(file->file_num);
    /* 拷贝记忆组到指定文件位置 */
	save_as_cur_group(file->file_num);
	return NO_ERR;
}
int16_t app_edit_file_for_comm(FILE_MSG * file)
{
    /*文件编号、文件名、pass保持时间、蜂鸣保持时间*/
    
	TEST_FILE *p = &file_table[file->file_num];
    
    if(p->num != file->file_num)
    {
        return PAR_NOT_ALLOWED;
    }
    if(file->file_name[0] == 0)
    {
        return PAR_NOT_ALLOWED;
    }
    
    strncpy(p->name, (const char*)file->file_name, NAME_LON);
	p->hold_pass_time = file->pass_keep_time;
	p->pass_buzzing_time = file->pass_beep_time;
    
    p->work_mode = getFileModeType_f(file->mode_type);
    p->arc_mode = getFileArcType_f(file->arc_type);
    
    save_file(file->file_num);
	return NO_ERR;
}
int16_t app_new_file_for_comm(FILE_MSG * file)
{
    /*文件编号、文件名、pass保持时间、蜂鸣保持时间*/
    
	TEST_FILE *p = &file_table[file->file_num];
	TEST_FILE *back = g_cur_file;
    
    if(p->num != file->file_num)
    {
        return PAR_NOT_ALLOWED;
    }
    if(file->file_name[0] == 0)
    {
        return DATA_OUT_RANGE;
    }
    
    strncpy(p->name, (const char*)file->file_name, NAME_LON);
	p->hold_pass_time = file->pass_keep_time;
	p->pass_buzzing_time = file->pass_beep_time;
    
    p->work_mode = getFileModeType_f(file->mode_type);
    p->arc_mode = getFileArcType_f(file->arc_type);
    p->create_time = Time_GetTime();/* 保存当前RTC时间 */
    
    g_cur_file = p;
    
    {
        const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* 放模式的名字 */
        uint8_t flag[MODEL_KINDS_MAX] = {0};/* 他里面放模式的排序 */
        int16_t kinds = 0;/* 放模式的数目 */
        
        define_modes(mode_buf, flag, &kinds);
        
        clear_step_used_flag();
        clear_group_table();
        g_cur_file->total = 0;
        
        insert_step(0, flag[1]);
        
        save_group_info(g_cur_file->num);
    }
    
    g_cur_file = back;
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
    
	return NO_ERR;
}
int16_t del_file_for_comm(const uint8_t file_num)
{
	if(file_num == 0)
	{
		return DATA_OUT_RANGE;
	}
	
    del_file(file_num);
    
    /* 读出最近使用的记忆组 */
    if(0 != read_list(g_cur_file->num))
    {
        sys_flag.mem_init = 0;
        save_sys_flag();
        return EXE_NOT_ALLOWED;/* 仪器未准备好 load_data*/
    }
    
    g_cur_step = list_99xx.head;
	
	test_flag.refresh_for_comm = 1;/* 刷新屏幕 */
	
	return NO_ERR;
}
int16_t del_all_file_for_comm(void)
{
    del_all_file();
    return NO_ERR;
}

int16_t get_idn_msg_for_comm(IDN_MSG * idn_msg)
{
	strcpy(idn_msg->idn_data, "Allwin Technologies");
    
	strcat(idn_msg->idn_data, ",");
	strcat(idn_msg->idn_data, g_cur_type->name);
    
	strcat(idn_msg->idn_data, ",xxxxxxxx,");
	strcat(idn_msg->idn_data, g_cur_type->ver);
	
    return NO_ERR;
}

int16_t tester_reset_for_comm(void)
{
    stop_irq();
    STOP_COMM = 1;
    return NO_ERR;
}
int16_t inster_one_step_for_comm(const uint8_t test_mode)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int32_t i = 0;
    uint8_t m = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	define_modes(mode_buf, flag, &kinds);/* 获取当前机型的模式信息 */
	
	for(i = 1; i <= kinds; i++)
	{
		if(flag[i] == mode)
		{
            save_cur_step();	/* 在更新g_cur_step之前要将之前的数据写入到eep中 */
            
            m = get_first_step_mode();/* 备份第一步的模式 */
            
            insert_step(g_cur_step->one_step.com.step, flag[1]);/* 刚插入的新步的模式应该是型号序列的第一个型号 */
            
            load_steps_to_list(g_cur_step->one_step.com.step+1);
            
            g_cur_step = list_99xx.head;
            
            if(g_cur_file->work_mode == G_MODE)
            {
                g_cur_step->one_step.com.mode = m;/* G模式下插入的新步要与第一步的模式保持一致 */
            }
            else
            {
                g_cur_step->one_step.com.mode = mode;
            }
            
			load_data();
			init_mode(g_cur_step);
            save_group_info(g_cur_file->num);
			save_list();/* 将改变后的结果保存 */
            test_flag.refresh_for_comm = 1;
			return NO_ERR;
		}
	}
	
	return PAR_NOT_ALLOWED;
}
int16_t del_cur_step_for_comm(void)
{
	if(g_cur_file->total == 1)
	{
		return EXE_NOT_ALLOWED;
	}
    
    del_step(g_cur_step->one_step.com.step);
    
    if(g_cur_file->total < g_cur_step->one_step.com.step)
    {
        load_steps_to_list(g_cur_file->total);
    }
    else
    {
        load_steps_to_list(g_cur_step->one_step.com.step);
    }
    
    g_cur_step = list_99xx.head;
    
	load_data();
    save_group_info(g_cur_file->num);
	save_list();/* 将改变后的结果保存的eep中 */
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
int16_t advance_cur_step_for_comm(void)
{
	if(g_cur_step->one_step.com.step <= 1)
	{
		return EXE_NOT_ALLOWED;
	}
	
    swap_step(g_cur_step->one_step.com.step, g_cur_step->one_step.com.step - 1);
    load_steps_to_list(g_cur_step->one_step.com.step - 1);
    g_cur_step = list_99xx.head;
    
	load_data();
    save_group_info(g_cur_file->num);
	save_list();/* 将改变后的结果保存的eep中 */
	return NO_ERR;
}
int16_t retreat_cur_step_for_comm(void)
{
	if(g_cur_step->one_step.com.step >= g_cur_file->total)
	{
		return EXE_NOT_ALLOWED;
	}
	
    swap_step(g_cur_step->one_step.com.step, g_cur_step->one_step.com.step + 1);
    load_steps_to_list(g_cur_step->one_step.com.step + 1);
    g_cur_step = list_99xx.head;
    
	load_data();
    save_group_info(g_cur_file->num);
	save_list();/* 将改变后的结果保存的eep中 */
	return NO_ERR;
}
int16_t swap_two_step_for_comm(const uint16_t step)
{
	if(check_step_exist_for_comm(step) != 0)
	{
		return EXE_NOT_ALLOWED;
	}
	
    swap_step(g_cur_step->one_step.com.step, step);
    load_steps_to_list(g_cur_step->one_step.com.step + 1);
    g_cur_step = list_99xx.head;
    
	load_data();
    save_group_info(g_cur_file->num);
	save_list();/* 将改变后的结果保存的eep中 */
	return NO_ERR;
}

int16_t set_bbd_open_check_for_comm(const uint16_t test_mode, uint16_t open_ratio)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
    
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode || p->com.mode != BBD)
	{
		return PAR_NOT_ALLOWED;
	}
    
    if(open_ratio >= BBD_OPEN_L && open_ratio <= BBD_OPEN_H)
    {
        p->bbd.open_ratio = open_ratio;
    }
    else
    {
        return DATA_OUT_RANGE;
    }
    
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}

int16_t get_bbd_open_check_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(*data);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode && p->com.mode != BBD)
	{
		return PAR_NOT_ALLOWED;
	}
    
	mysprintf((uint8_t*)data, NULL, 130, cur_open_ratio);
	return cur_open_ratio;
}
int16_t set_bbd_short_check_for_comm(const uint16_t test_mode, uint16_t short_ratio)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
    
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode || p->com.mode != BBD)
	{
		return PAR_NOT_ALLOWED;
	}
    
    if(short_ratio >= BBD_SHORT_L && short_ratio <= BBD_SHORT_H)
    {
        p->bbd.short_ratio = short_ratio;
    }
    else
    {
        return DATA_OUT_RANGE;
    }
    
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
int16_t get_bbd_short_check_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(*data);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode && p->com.mode != BBD)
	{
		return PAR_NOT_ALLOWED;
	}
    
	mysprintf((uint8_t*)data, NULL, 130, cur_short_ratio);
	return NO_ERR;
}
int16_t set_mode_for_comm(const uint16_t test_mode)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int32_t i = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	define_modes(mode_buf, flag, &kinds);/* 获取当前机型的模式信息 */
    
	for(i = 1; i <= kinds; i++)
	{
		if(flag[i] == mode)
		{
			g_cur_step->one_step.com.mode = mode;
			init_mode(g_cur_step);
			save_list();/* 将改变后的结果保存的eep中 */
			load_data();
            test_flag.refresh_for_comm = 1;
			return NO_ERR;
		}
	}
	
	return PAR_NOT_ALLOWED;
}


int16_t get_mode_for_comm(void)
{
    return getTestMode(cur_mode);
}
int16_t set_vol_for_comm(const uint16_t voltage, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
			if(voltage >= ACW_VOL_L && voltage <= ACW_VOL_H)
			{
				p->acw.testing_voltage = voltage;
				break;
			}
			return DATA_OUT_RANGE;
		case DCW:
			if(voltage >= DCW_VOL_L && voltage <= DCW_VOL_H)
			{
				p->dcw.testing_voltage = voltage;
				break;
			}
			return DATA_OUT_RANGE;
		case IR:
			if(voltage >= IR_VOL_L && voltage <= IR_VOL_H)
			{
				p->ir.testing_voltage = voltage;
				break;
			}
			return DATA_OUT_RANGE;
		case GR:
			if(voltage >= GR_CUR_L && voltage <= GR_CUR_H)
			{
				p->gr.testing_cur = voltage;
				break;
			}
			return DATA_OUT_RANGE;
		case BBD:
			if(voltage >= BBD_VOL_L && voltage <= BBD_VOL_H)
			{
				p->bbd.testing_voltage = voltage;
				break;
			}
			return DATA_OUT_RANGE;
		default:
			return PAR_NOT_ALLOWED;
	}
    
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
int16_t get_vol_for_comm(uint16_t *data, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
    uint8_t type = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
    type = get_vol_gear_par_form(mode);
	mysprintf((uint8_t*)data, NULL, type, cur_vol);
    return NO_ERR;
}
/*
 * 函数名：set_gear_for_comm
 * 描述  ：通信设置档位
 * 输入  ：range 通信传入的档位 test_mode 测试模式
 * 输出  ：无
 * 返回  ：错误码
 * 记录  : 2017.6.13 对此函数重新实现
 */
int16_t set_gear_for_comm(const uint16_t range, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp = range;/* 通信时的档位从0开始 本机的档位是从1开始 */
    uint8_t mode = getTestMode_f(test_mode);
	
	uint32_t cur_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[CUR_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[CUR_KINDS_MAX] = {0};/* 标志 */
    int32_t i = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
			cur_kinds = define_cur_kinds(mode, gear_buf, flag);
			
            switch(temp)
            {
                case AC_0_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == AC_200uA)
                        {
                            p->acw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case AC_1_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == AC_2mA)
                        {
                            p->acw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case AC_2_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == AC_20mA || flag[i] == AC_10mA)
                        {
                            p->acw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case AC_3_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == AC_50mA || flag[i] == AC_100mA || flag[i] == AC_200mA)
                        {
                            p->acw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                default:
                    return DATA_OUT_RANGE;
            }
            break;
		case DCW:
			cur_kinds = define_cur_kinds(mode, gear_buf, flag);
			
            switch(temp)
            {
                case DC_0_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == DC_2uA)
                        {
                            p->dcw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case DC_1_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == DC_20uA)
                        {
                            p->dcw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case DC_2_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == DC_200uA)
                        {
                            p->dcw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case DC_3_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == DC_2mA)
                        {
                            p->dcw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case DC_4_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == DC_10mA || flag[i] == DC_20mA)
                        {
                            p->dcw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                case DC_5_GEAR_COMM:
                    for(i = 0; i < cur_kinds; i++)
                    {
                        if(flag[i] == DC_50mA)
                        {
                            p->dcw.gear_i = flag[i];
                            break;
                        }
                    }
                    
                    if(i == cur_kinds)
                    {
                        return DATA_OUT_RANGE;
                    }
                    break;
                default:
                    return DATA_OUT_RANGE;
            }
            break;
		default:
			return PAR_NOT_ALLOWED;
	}
	
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
/*
 * 函数名：set_gear_for_comm
 * 描述  ：通信设置档位
 * 输入  ：range 通信传入的档位 test_mode 测试模式
 * 输出  ：无
 * 返回  ：错误码
 * 记录  : 2017.6.13 修改函数 
 *           case ACW:
 *               return cur_gear_comm;
 *           case DCW:
 *               return cur_gear_comm;
 *   
 **/
int16_t get_gear_for_comm(const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
			return cur_gear_comm;
		case DCW:
			return cur_gear_comm;
		default:
			return PAR_NOT_ALLOWED;
	}
}
int16_t set_high_for_comm(uint16_t high_data, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 0;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
			temp_l = 1;
			switch(cur_gear)
			{
				case AC_200uA:
				case AC_2mA:
				case AC_20mA:
				case AC_200mA:
					temp_h = 2000;
					break;
				case AC_10mA:
				case AC_100mA:
					temp_h = 1000;
					break;
				case AC_50mA:
					temp_h = 5000;
					break;
			}
			if(high_data >= temp_l && high_data <= temp_h)
			{
				p->acw.upper_limit = high_data;
				break;
			}
			return DATA_OUT_RANGE;
		case DCW:
			temp_l = 1;
			switch(cur_gear)
			{
				case DC_2uA:
				case DC_20uA:
				case DC_200uA:
				case DC_2mA:
				case DC_20mA:
					temp_h = 2000;
					break;
				case DC_10mA:
				case DC_100mA:
					temp_h = 1000;
					break;
				case DC_50mA:
					temp_h = 5000;
					break;
			}
			if(high_data >= temp_l && high_data <= temp_h)
			{
				p->dcw.upper_limit = high_data;
				break;
			}
			return DATA_OUT_RANGE;
		case IR:
			temp_l = 0;
			temp_h = IR_RES_H;
			if(high_data >= temp_l && high_data <= temp_h)
			{
				p->ir.upper_limit = high_data;
				break;
			}
			return DATA_OUT_RANGE;
		case GR:
			temp_h = GR_RES_H(p->gr.testing_cur);
			temp_l = GR_RES_L;
			if(high_data >= temp_l && high_data <= temp_h)
			{
				p->gr.upper_limit = high_data;
				break;
			}
			return DATA_OUT_RANGE;
		default:
			return PAR_NOT_ALLOWED;
	}
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
int32_t get_high_for_comm(uint16_t *data, uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
    uint8_t type = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
    
    type = get_cur_gear_par_form(mode);
    
	mysprintf((uint8_t*)data, NULL, type, cur_high);
    
	return NO_ERR;
}
int16_t set_low_for_comm(uint16_t low_data, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 0;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	temp_h = cur_high;
	
	switch(mode)
	{
		case ACW:
			temp_l = 0;
			if(low_data >= temp_l && low_data <= temp_h)
			{
				p->acw.lower_limit = low_data;
				break;
			}
			return DATA_OUT_RANGE;
		case DCW:
			temp_l = 0;
			if(low_data >= temp_l && low_data <= temp_h)
			{
				p->dcw.lower_limit = low_data;
				break;
			}
			return DATA_OUT_RANGE;
		case IR:
			temp_l = 1;
			if(low_data >= temp_l && low_data <= temp_h)
			{
				p->ir.lower_limit = low_data;
				break;
			}
			else if(temp_h==0 && low_data >= temp_l)
			{
				p->ir.lower_limit = low_data;
				break;
			}
			return DATA_OUT_RANGE;
		case GR:
			temp_l = 0;
			if(low_data >= temp_l && low_data <= temp_h)
			{
				p->gr.lower_limit = low_data;
				break;
			}
			return DATA_OUT_RANGE;
		default:
			return PAR_NOT_ALLOWED;
	}
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
int16_t set_offset_value_for_comm(uint16_t offset_data, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 0;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	temp_h = cur_high;
	
	switch(mode)
	{
		case ACW:
			temp_l = 0;
			if(offset_data >= temp_l && offset_data <= temp_h)
			{
				p->acw.offset_cur = offset_data;
				break;
			}
			return DATA_OUT_RANGE;
		case DCW:
			temp_l = 0;
			if(offset_data >= temp_l && offset_data <= temp_h)
			{
				p->dcw.offset_cur = offset_data;
				break;
			}
			return DATA_OUT_RANGE;
		case IR:
			return DATA_OUT_RANGE;
		case GR:
			temp_l = 0;
			if(offset_data >= temp_l && offset_data <= temp_h)
			{
				p->gr.offset_res = offset_data;
				break;
			}
			return DATA_OUT_RANGE;
		default:
			return PAR_NOT_ALLOWED;
	}
	save_cur_step();
	test_flag.refresh_for_comm = 1;
	return NO_ERR;
}
int32_t get_low_for_comm(uint16_t *data, uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
    uint8_t type = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
    type = get_cur_gear_par_form(mode);
	mysprintf((uint8_t*)data, NULL, type, cur_low);
	return NO_ERR;
}
int32_t get_offset_value_for_comm(uint16_t *data, uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
    uint8_t type = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
// 	if(mode == GR)
// 	{
// 		type = 151;
// 	}
// 	else
// 	{
// 		type = 50;
// 	}
    type = get_cur_gear_par_form(mode);
	
	mysprintf((uint8_t*)data, NULL, type, cur_offset);
	return NO_ERR;
}
int16_t set_real_cur_for_comm(uint16_t real_cur)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 0;
	uint16_t temp_l = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != ACW)
	{
		return PAR_NOT_ALLOWED;
	}
	
	temp_h = cur_high;
	temp_l = 0;
	
    if(real_cur >= temp_l && real_cur <= temp_h)
    {
        p->acw.ac_real_cur = real_cur;
    }
    else
    {
        return DATA_OUT_RANGE;
    }
    
	save_cur_step();
	test_flag.refresh_for_comm = 1;
    
	return NO_ERR;
}
int16_t get_real_cur_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = ACW;
    uint8_t type = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != ACW)
	{
		return PAR_NOT_ALLOWED;
	}
    
    type = get_cur_gear_par_form(mode);
	mysprintf((uint8_t*)data, NULL, type, cur_real_cur);
	return NO_ERR;
}
int16_t set_charge_cur_for_comm(uint16_t charge_cur, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 0;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	temp_h = cur_high;
	temp_l = 0;
	
	switch(mode)
	{
		case DCW:
			if(charge_cur >= temp_l && charge_cur <= temp_h)
			{
				p->dcw.charge_cur = charge_cur;
				break;
			}
			return DATA_OUT_RANGE;
		default:
			return PAR_NOT_ALLOWED;
	}
	save_cur_step();
	return NO_ERR;
}
int16_t get_charge_cur_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(*data);
    uint8_t type = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
    type = get_cur_gear_par_form(mode);
	mysprintf((uint8_t*)(data + 1), NULL, type, cur_charge_cur);
	return NO_ERR;
}
int16_t set_arc_for_comm(const uint16_t current, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 0;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
			if(g_cur_file->arc_mode == ARC_GRADE_MODEL)
			{
				temp_h = 9;
				temp_l = 0;
			}
			else
			{
				temp_h = ACW_ARC_H;
				temp_l = 0;
			}
			
			if(current >= temp_l && current <= temp_h)
			{
				p->acw.arc_sur = current;
				break;
			}
			return DATA_OUT_RANGE;
		case DCW:
			if(g_cur_file->arc_mode == ARC_GRADE_MODEL)
			{
				temp_h = 9;
				temp_l = 0;
			}
			else
			{
				temp_h = DCW_ARC_H;
				temp_l = 0;
			}
			
			if(current >= temp_l && current <= temp_h)
			{
				p->dcw.arc_sur = current;
				break;
			}
			return DATA_OUT_RANGE;
		default:
			return PAR_NOT_ALLOWED;
	}
	save_cur_step();
	return NO_ERR;
}
int16_t get_arc_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(*data);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	if(g_cur_file->arc_mode == ARC_GRADE_MODEL)
	{
        mysprintf((uint8_t*)data, NULL, 10, cur_arc_gear);
	}
    else
    {
        mysprintf((uint8_t*)data, NULL, 152, cur_arc_gear);
    }
	
	return cur_arc_gear;
}
int16_t set_hz_for_comm(const uint16_t frequency, const uint8_t test_mode)
{
    const uint8_t *gear_buf[10] = {0};
    uint8_t flag[10] = {0};
    uint8_t kinds = 0;
    uint32_t i = 0;
    uint8_t temp = 0;
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
    if(type_spe.gr_hz_type == HZ_TYPE_GRADE)
    {
        switch(mode)
        {
            case GR:
                /* 当直流模块使能后就不必设置频率 */
                if(DC_GR_EN)
                {
                    return NO_ERR;
                }
                
                kinds = define_hz_kinds(GR, gear_buf, flag);
                switch(frequency)
                {
                    case 500:
                        temp = AC_50HZ;
                        break;
                    case 600:
                        temp = AC_60HZ;
                        break;
                    case 1000:
                        temp = AC_100HZ;
                        break;
                    case 1500:
                        temp = AC_150HZ;
                        break;
                    case 2000:
                        temp = AC_200HZ;
                        break;
                    case 2500:
                        temp = AC_250HZ;
                        break;
                    case 3000:
                        temp = AC_300HZ;
                        break;
                    case 3500:
                        temp = AC_350HZ;
                        break;
                    case 4000:
                        temp = AC_400HZ;
                        break;
                }
                for(i = 0; i < kinds; i++)
                {
                    if(flag[i] == temp)
                    {
                        p->acw.output_freq = temp;
                        break;
                    }
                }
                if(i < kinds)
                {
                    save_cur_step();
                    return NO_ERR;;
                }
                return DATA_OUT_RANGE;
        }
    
    }
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		switch(mode)
		{
			case ACW:
			{
				kinds = define_hz_kinds(ACW, gear_buf, flag);
				switch(frequency)
				{
					case 500:
						temp = AC_50HZ;
						break;
					case 600:
						temp = AC_60HZ;
						break;
					case 1000:
						temp = AC_100HZ;
						break;
					case 1500:
						temp = AC_150HZ;
						break;
					case 2000:
						temp = AC_200HZ;
						break;
					case 2500:
						temp = AC_250HZ;
						break;
					case 3000:
						temp = AC_300HZ;
						break;
					case 3500:
						temp = AC_350HZ;
						break;
					case 4000:
						temp = AC_400HZ;
						break;
				}
                
				for(i = 0; i < kinds; i++)
				{
					if(flag[i] == temp)
					{
						p->acw.output_freq = temp;
						break;
					}
				}
				
				if(i != kinds)
				{
					break;
				}
				return DATA_OUT_RANGE;
			}
			case GR:
			{
            }
			default:
				return PAR_NOT_ALLOWED;
		}
	}
	else
	{
		uint16_t temp_h = 4000;
		uint16_t temp_l = 400;
		if(frequency<temp_l || frequency>temp_h)
		{
			return DATA_OUT_RANGE;
		}
		switch(mode)
		{
			case ACW:
				p->acw.output_freq = frequency;
				break;
			case GR:
				p->gr.output_freq = frequency;
				break;
			default:
				return PAR_NOT_ALLOWED;
		}
	}
	save_cur_step();
	return NO_ERR;
}
int16_t get_hz_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(*data);
    uint16_t temp = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
    if(type_spe.gr_hz_type == HZ_TYPE_GRADE  && mode == GR)
    {
		switch(cur_frequency_gear)
		{
			case AC_50HZ:
				temp = 500;
                break;
			case AC_60HZ:
				temp = 600;
                break;
			case AC_100HZ:
				temp = 1000;
                break;
			case AC_150HZ:
				temp = 1500;
                break;
			case AC_200HZ:
				temp = 2000;
                break;
			case AC_250HZ:
				temp = 2500;
                break;
			case AC_300HZ:
				temp = 3000;
                break;
			case AC_350HZ:
				temp = 3500;
                break;
			case AC_400HZ:
				temp = 4000;
                break;
			default:
				return DATA_OUT_RANGE;
        }
        
        mysprintf((uint8_t*)data, NULL, 151, temp);
        return temp;
    }
    
	if(type_spe.hz_type == HZ_TYPE_GRADE  && mode == ACW)
	{
		switch(cur_frequency_gear)
		{
			case AC_50HZ:
				temp = 500;
                break;
			case AC_60HZ:
				temp = 600;
                break;
			case AC_100HZ:
				temp = 1000;
                break;
			case AC_150HZ:
				temp = 1500;
                break;
			case AC_200HZ:
				temp = 2000;
                break;
			case AC_250HZ:
				temp = 2500;
                break;
			case AC_300HZ:
				temp = 3000;
                break;
			case AC_350HZ:
				temp = 3500;
                break;
			case AC_400HZ:
				temp = 4000;
                break;
			default:
				return DATA_OUT_RANGE;
        }
        
        mysprintf((uint8_t*)data, NULL, 151, temp);
        return temp;
	}
	else
	{
        mysprintf((uint8_t*)data, NULL, 151, cur_frequency_gear);
        return cur_frequency_gear;
	}
}

int16_t get_hz_for_comm_f(uint16_t *data)
{
    uint8_t mode = 0;
    uint16_t temp = 0;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
    mode = cur_mode;
	
    if(type_spe.gr_hz_type == HZ_TYPE_GRADE  && mode == GR)
    {
		switch(cur_frequency_gear)
		{
			case AC_50HZ:
				temp = 500;
                break;
			case AC_60HZ:
				temp = 600;
                break;
			case AC_100HZ:
				temp = 1000;
                break;
			case AC_150HZ:
				temp = 1500;
                break;
			case AC_200HZ:
				temp = 2000;
                break;
			case AC_250HZ:
				temp = 2500;
                break;
			case AC_300HZ:
				temp = 3000;
                break;
			case AC_350HZ:
				temp = 3500;
                break;
			case AC_400HZ:
				temp = 4000;
                break;
			default:
				return DATA_OUT_RANGE;
        }
        
        mysprintf((uint8_t*)data, NULL, 151, temp);
        return temp;
    }
    
	if(type_spe.hz_type == HZ_TYPE_GRADE  && mode == ACW)
	{
		switch(cur_frequency_gear)
		{
			case AC_50HZ:
				temp = 500;
                break;
			case AC_60HZ:
				temp = 600;
                break;
			case AC_100HZ:
				temp = 1000;
                break;
			case AC_150HZ:
				temp = 1500;
                break;
			case AC_200HZ:
				temp = 2000;
                break;
			case AC_250HZ:
				temp = 2500;
                break;
			case AC_300HZ:
				temp = 3000;
                break;
			case AC_350HZ:
				temp = 3500;
                break;
			case AC_400HZ:
				temp = 4000;
                break;
			default:
				return DATA_OUT_RANGE;
        }
        
        mysprintf((uint8_t*)data, NULL, 151, temp);
        return temp;
	}
	else
	{
        mysprintf((uint8_t*)data, NULL, 151, cur_frequency_gear);
        return cur_frequency_gear;
	}
}
int16_t set_pass_signal_for_comm(const uint16_t Psignal, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 1;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	if(Psignal < temp_l || Psignal>temp_h)
	{
		return DATA_OUT_RANGE;
	}
	
	switch(mode)
	{
		case ACW:
			p->acw.steps_pass = Psignal;
			break;
		case DCW:
			p->dcw.steps_pass = Psignal;
			break;
		case IR:
			p->ir.steps_pass = Psignal;
			break;
		case GR:
			p->gr.steps_pass = Psignal;
			break;
		case BBD:
			p->bbd.steps_pass = Psignal;
			break;
		default:
			return PAR_NOT_ALLOWED;
	}
    
	save_cur_step();
	return NO_ERR;
}
int16_t get_pass_signal_for_comm(const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	return steps_pass;
}
int16_t set_steps_cont_for_comm(const uint16_t cnext, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 1;
	uint16_t temp_l = 0;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	if(cnext < temp_l || cnext>temp_h)
	{
		return DATA_OUT_RANGE;
	}
	
	switch(mode)
	{
		case ACW:
			p->acw.steps_cont = cnext;
			break;
		case DCW:
			p->dcw.steps_cont = cnext;
			break;
		case IR:
			p->ir.steps_cont = cnext;
			break;
		case GR:
			p->gr.steps_cont = cnext;
			break;
		case BBD:
			p->bbd.steps_cont = cnext;
			break;
		default:
			return PAR_NOT_ALLOWED;
	}
	save_cur_step();
	return NO_ERR;
}
int16_t get_steps_cont_for_comm(const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	return steps_con;
}

int16_t set_port_for_comm(const TEST_PORT port_data, const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
	int16_t i = 0;
    uint8_t mode = getTestMode_f(test_mode);
	union{
		TEST_PORT port;
		uint16_t data;
	}un_port;
    uint16_t *p_u16 = (uint16_t *)&port_data;
	
	un_port.port = port_data;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	if(type_spe.port_num == PORT_NUM0)
	{
		return DATA_OUT_RANGE;
	}
	
	for(i = 0; i < type_spe.port_num; i++)
	{
		if(((*(p_u16 + i / 8)>>(2 * (i % 8))) & 3) > 2)
		{
			return DATA_OUT_RANGE;
		}
	}
	
	switch(mode)
	{
		case ACW:
			p->acw.port = un_port.port;
			break;
		case DCW:
			p->dcw.port = un_port.port;
			break;
		case IR:
			p->ir.port = un_port.port;
			break;
		default:
			return PAR_NOT_ALLOWED;
	}
    
	save_cur_step();
	test_flag.refresh_for_comm = 1;
    
	return NO_ERR;
}
int16_t get_port_for_comm(const uint8_t test_mode)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	union{
		TEST_PORT port;
		uint16_t data;
	}un_port;
		
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	if(type_spe.port_num == PORT_NUM0)
	{
		return DATA_OUT_RANGE;
	}
	
	un_port.port = cur_port;
	return un_port.data;
}
void clear_all_res_for_comm(void)
{
	sys_par.used_res_num = 0;
	sys_par.pass_res_num = 0;
	sys_par.cover_res_num = 0;
	
	save_sys_par();
	
	memset(&cur_result, 0, sizeof(cur_result));
}

/*已使用的结果信息查询*/
int16_t get_result_info_for_comm(uint8_t info)
{
    switch(info)
    {
        case RES_USED_NUM:
            return sys_par.used_res_num;
        case RES_FREE_NUM:
            return (get_res_total() - sys_par.used_res_num);
        case RES_TOTAL_NUM:
            return get_res_total();
        case RES_PASS_NUM:
            return sys_par.pass_res_num;
        case RES_FAIL_NUM:
            return (sys_par.used_res_num - sys_par.pass_res_num);
    }
    
    return 0;
}


int16_t set_test_port_for_comm(uint16_t* data)
{
    TEST_PORT t_test_port = *(TEST_PORT*)data;
    
	UN_STRUCT *p = NULL;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
    
    if(p->com.mode != getTestMode_f(t_test_port.mode))
    {
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
    }
    
    switch(p->com.mode)
    {
        case ACW:
            memcpy(&p->acw.port.ports ,&t_test_port.ports, sizeof (t_test_port.ports));
            break;
        case DCW:
            memcpy(&p->dcw.port.ports ,&t_test_port.ports, sizeof (t_test_port.ports));
            break;
        case IR:
            memcpy(&p->ir.port.ports ,&t_test_port.ports, sizeof (t_test_port.ports));
            break;
        case BBD:
            memcpy(&p->bbd.port.ports ,&t_test_port.ports, sizeof (t_test_port.ports));
            break;
        default:
            return DATA_OUT_RANGE;
    }
    
	save_cur_step();
	test_flag.refresh_for_comm = 1;/* 刷新屏幕 */
	return NO_ERR;
}

int16_t get_test_port_for_comm(uint16_t* data)
{
    TEST_PORT *p_port = NULL;
	UN_STRUCT *p = NULL;
	
    memset(data, 0, sizeof(TEST_PORT));
    
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
    
    switch(p->com.mode)
    {
        case ACW:
            p_port = &p->acw.port;
            break;
        case DCW:
            p_port = &p->dcw.port;
            break;
        case IR:
            p_port = &p->ir.port;
            break;
        case BBD:
            p_port = &p->bbd.port;
            break;
        default:
            return DATA_OUT_RANGE;
    }
    
    p_port->num = type_spe.port_num;
    memcpy(data, p_port, sizeof(TEST_PORT));
    
	return NO_ERR;
}

int16_t set_ir_auto_for_comm(const uint16_t arange_data)
{
	UN_STRUCT *p = NULL;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != IR)
	{
		return PAR_NOT_ALLOWED;
	}
	
	if(arange_data > 1)
	{
		return DATA_OUT_RANGE;
	}
	p->ir.auto_shift_gears = arange_data;
	
	save_cur_step();
	return NO_ERR;
}
int16_t get_ir_auto_for_comm(uint16_t *data)
{
	UN_STRUCT *p = NULL;
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != IR)
	{
		return PAR_NOT_ALLOWED;
	}
	
    *data = cur_auto;
	return NO_ERR;
}
int16_t set_timer_for_comm(const uint16_t time_data, const uint8_t test_mode, const uint8_t whitch)
{
	UN_STRUCT *p = NULL;
	uint16_t temp_h = 9999;
	uint16_t temp_l = 0;
	uint16_t temp_m = 3;
	uint16_t *p_time = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
		{
			switch(whitch)
			{
				case R_TIME:
					p_time = &p->acw.rise_time;
					break;
				case T_TIME:
					p_time = &p->acw.testing_time;
					break;
				case F_TIME:
					p_time = &p->acw.fall_time;
					break;
				case I_TIME:
					temp_m = 0;
					p_time = &p->acw.interval_time;
					break;
				default:
					return DATA_OUT_RANGE;
			}
			break;
		}
		case DCW:
		{
			switch(whitch)
			{
				case R_TIME:
					p_time = &p->dcw.rise_time;
					break;
				case T_TIME:
					p_time = &p->dcw.testing_time;
					break;
				case F_TIME:
					p_time = &p->dcw.fall_time;
					break;
				case D_TIME:
					p_time = &p->dcw.delay_time;
					break;
				case I_TIME:
					temp_m = 0;
					p_time = &p->dcw.interval_time;
					break;
				default:
					return DATA_OUT_RANGE;
			}
			break;
		}
		case IR:
		{
			switch(whitch)
			{
				case R_TIME:
					p_time = &p->ir.rise_time;
					break;
				case T_TIME:
					p_time = &p->ir.testing_time;
					break;
				case D_TIME:
					p_time = &p->ir.delay_time;
					break;
				case I_TIME:
					temp_m = 0;
					p_time = &p->ir.interval_time;
					break;
				default:
					return DATA_OUT_RANGE;
			}
			break;
		}
		case GR:
		{
			switch(whitch)
			{
				case T_TIME:
					p_time = &p->gr.testing_time;
					break;
				case I_TIME:
					temp_m = 0;
					p_time = &p->gr.interval_time;
					break;
				default:
					return DATA_OUT_RANGE;
			}
			break;
		}
		default:
			return PAR_NOT_ALLOWED;
	}
	
	if((time_data>temp_l && time_data<temp_m) || time_data>temp_h)
	{
		return DATA_OUT_RANGE;
	}
	
	*p_time = time_data; 
	save_cur_step();
	test_flag.refresh_for_comm = 1;/* 刷新屏幕 */
	return NO_ERR;
}
int16_t get_timer_for_comm(const uint8_t test_mode, const uint8_t whitch)
{
	UN_STRUCT *p = NULL;
    uint8_t mode = getTestMode_f(test_mode);
	
	if(g_cur_step == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	load_data();
	p = &g_cur_step->one_step;
	
	if(p->com.mode != mode)
	{
		return PAR_NOT_ALLOWED;
	}
	
	switch(mode)
	{
		case ACW:
		{
			switch(whitch)
			{
				case R_TIME:
					return p->acw.rise_time;
				case T_TIME:
					return p->acw.testing_time;
				case F_TIME:
					return p->acw.fall_time;
				case I_TIME:
					return p->acw.interval_time;
				default:
					return DATA_OUT_RANGE;
			}
		}
		case DCW:
		{
			switch(whitch)
			{
				case R_TIME:
					return p->dcw.rise_time;
				case T_TIME:
					return p->dcw.testing_time;
				case F_TIME:
					return p->dcw.fall_time;
				case D_TIME:
					return p->dcw.delay_time;
				case I_TIME:
					return p->dcw.interval_time;
				default:
					return DATA_OUT_RANGE;
			}
		}
		case IR:
		{
			switch(whitch)
			{
				case R_TIME:
					return p->ir.rise_time;
				case T_TIME:
					return p->ir.testing_time;
				case D_TIME:
					return p->ir.delay_time;
				case I_TIME:
					return p->ir.interval_time;
				default:
					return DATA_OUT_RANGE;
			}
		}
		case GR:
		{
			switch(whitch)
			{
				case T_TIME:
					return p->gr.testing_time;
				case I_TIME:
					return p->gr.interval_time;
				default:
					return DATA_OUT_RANGE;
			}
		}
		default:
			return PAR_NOT_ALLOWED;
	}
}
int16_t set_sys_par_for_comm(const uint16_t data, const uint8_t whitch)
{
	uint16_t temp_h = 1;
	uint16_t temp_l = 0;
	uint8_t *p_data = NULL;
	
	switch(whitch)
	{
		case SYS_CONTRAST:/* 液晶对比度 */
			temp_h = 9;
			temp_l = 1;
			p_data = &sys_par.contrast;
			break;
		case SYS_VOLUME:/* 系统音量 */
			temp_h = 9;
			temp_l = 0;
			p_data = NULL;
			break;
		case SYS_RES_ALLOWANCE_HINT:/* 余量提示 */
			temp_h = 9;
			temp_l = 0;
			p_data = &sys_par.allowance;
			break;
		case SYS_RES_SAVE:/* 结果保存 */
			p_data = &sys_par.is_save_res;
			break;
		case SYS_RES_COVER:/* 结果溢出覆盖 */
			p_data = &sys_par.is_overflow_cover;
			break;
		case SYS_GFI:/* GFI保护开关 */
			p_data = &sys_par.is_gfi_protect;
			break;
		case SYS_PHV:/* 预防高压 */
			p_data = NULL;
			break;
		case SYS_TEST_SIGNAL:/* 测试信号 */
			p_data = &sys_par.test_level;
			break;
		case SYS_SELF_CHECK:/* 自检允许 */
			p_data = &sys_par.is_self_check;
			break;
		case SYS_CHECK_HINT:/* 检验提示 */
			p_data = NULL;
			break;
		case SYS_TEST_PORT:/* 测试端口 */
			p_data = &sys_par.test_method;
			break;
		case SYS_LANGUAGE:/* 语言 */
			p_data = &sys_par.language;
			break;
		case SYS_FAIL_CONT:/* 失败继续 */
			p_data = &sys_par.is_falt_continue;
			break;
		case SYS_KEY_LOCK:/* 键盘锁 */
			p_data = &sys_par.keyboard_lock;
			break;
		case SYS_NUM_RULER:/* 编号规则 */
			temp_h = 2;
			temp_l = 0;
			p_data = &sys_par.num_rule;
			break;
	}
	if(p_data == NULL)
	{
		return NO_ERR;
	}
	if(data > temp_h || data < temp_l)
	{
		return DATA_OUT_RANGE;
	}
	*p_data = data;
    
	check_sys_par();
	save_sys_par();
	test_flag.refresh_for_comm = 1;
    
	return NO_ERR;
}

int16_t get_sys_par_for_comm(uint16_t *data, const uint8_t whitch)
{
	uint8_t *p_data = NULL;
	
	switch(whitch)
	{
		case SYS_CONTRAST:/* 液晶对比度 */
			p_data = &sys_par.contrast;
			break;
		case SYS_VOLUME:/* 系统音量 */
			p_data = NULL;
			break;
		case SYS_RES_ALLOWANCE_HINT:/* 余量提示 */
			p_data = &sys_par.allowance;
			break;
		case SYS_RES_SAVE:/* 结果保存 */
			p_data = &sys_par.is_save_res;
			break;
		case SYS_RES_COVER:/* 结果溢出覆盖 */
			p_data = &sys_par.is_overflow_cover;
			break;
		case SYS_GFI:/* GFI保护开关 */
			p_data = &sys_par.is_gfi_protect;
			break;
		case SYS_PHV:/* 预防高压 */
			p_data = NULL;
			break;
		case SYS_TEST_SIGNAL:/* 测试信号 */
			p_data = &sys_par.test_level;
			break;
		case SYS_SELF_CHECK:/* 自检允许 */
			p_data = &sys_par.is_self_check;
			break;
		case SYS_CHECK_HINT:/* 检验提示 */
			p_data = NULL;
			break;
		case SYS_TEST_PORT:/* 测试端口 */
			p_data = &sys_par.test_method;
			break;
		case SYS_LANGUAGE:/* 语言 */
			p_data = &sys_par.language;
			break;
		case SYS_FAIL_CONT:/* 失败继续 */
			p_data = &sys_par.is_falt_continue;
			break;
		case SYS_KEY_LOCK:/* 键盘锁 */
			p_data = &sys_par.keyboard_lock;
			break;
		case SYS_NUM_RULER:/* 编号规则 */
			p_data = &sys_par.num_rule;
			break;
	}
    
	if(p_data == NULL)
	{
		return NO_ERR;
	}
    
    *data = *p_data;
	return NO_ERR;
}
int16_t set_sys_time_for_comm(SYS_TIME * system_time)
{
	uint8_t time_data[6] = "";
	
	time_data[0] = system_time->year;	/* 取出年的低两位 */
	time_data[1] = system_time->month;
	time_data[2] = system_time->day;
	time_data[3] = system_time->hour;
	time_data[4] = system_time->minute;
	time_data[5] = system_time->second;
	
	if(EXE_NOT_ALLOWED == JudgeSaveTime(time_data))
	{
		return DATA_OUT_RANGE;
	}
	
	time_now.tm_year = time_data[0]+2000;
	time_now.tm_mon  = time_data[1];
	time_now.tm_mday = time_data[2];
	time_now.tm_hour = time_data[3];
	time_now.tm_min  = time_data[4];
	time_now.tm_sec  = time_data[5];
	
	Time_Adjust();
	return NO_ERR;
}


void get_sys_time_for_comm(SYS_TIME * system_time)
{
	u32 CurrenTime = 0;
	CurrenTime = Time_GetTime();
	time_now = Time_ConvToCalendar(CurrenTime);
	
    system_time->year = time_now.tm_year % 100;
    system_time->month = time_now.tm_mon + 1;
    system_time->day = time_now.tm_mday;
    system_time->hour = time_now.tm_hour;
    system_time->minute = time_now.tm_min;
    system_time->second = time_now.tm_sec;
}

int16_t set_dut_name_for_comm(const char* name)
{
	if(strlen(name) > NAME_LON && strlen(name))
	{
		return DATA_OUT_RANGE;
	}
	strcpy(sys_par.dut_name, name);
	save_sys_par();
	return NO_ERR;
}
int16_t get_dut_name_for_comm(char* name)
{
	if(g_cur_file == NULL)
	{
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	strcpy(name, sys_par.dut_name);
	return NO_ERR;
}

uint16_t get_cap_for_comm(uint16_t data)
{
    app_flag.into_get_cap = 1;
    
	return NO_ERR;
}
uint16_t get_offset_for_comm(uint16_t data)
{
    app_flag.into_get_offset = 1;
    
	return NO_ERR;
}
void get_test_fetch_for_comm(char* data)
{	
    char buf[50] = {0};
    
    memset(data, 0, 1);
    mysprintf((uint8_t*)buf, ",", 120, cur_step);
    strcat(data, buf);
    
    mysprintf((uint8_t*)buf, ",", 10, getTestMode(cur_mode));
    strcat(data, buf);
    
    switch(cur_mode)
    {
        case ACW:
        {
            strcat(data, (const char*)output_buf);
            strcat(data, ",");
            
            mysprintf((uint8_t*)buf, ",", 10, cur_gear_comm);
            strcat(data, buf);
            
            strcat(data, (const char*)loop_buf);
            strcat(data, ",");
            
            mysprintf((uint8_t*)buf, ",", 10, !!cur_real_cur);
            strcat(data, buf);
            
            if(0 != cur_real_cur)
            {
                mysprintf((uint8_t*)buf, ",", ac_gear[cur_gear].decs + 150, real_ave);
                strcat(data, buf);
            }
            else
            {
                strcat(data, "-----,");
            }
            break;
        }
        case DCW:
        {
            strcat(data, (const char*)output_buf);
            strcat(data, ",");
            
            mysprintf((uint8_t*)buf, ",", 10, cur_gear_comm);
            strcat(data, buf);
            
            strcat(data, (const char*)loop_buf);
            strcat(data, ",");
            break;
        }
        case IR:
        {
            uint8_t temp = cur_gear_comm;
            
            if(cur_gear_comm > 3)
            {
                temp = 5;
            }
            
            strcat(data, (const char*)output_buf);
            strcat(data, ",");
            
            mysprintf((uint8_t*)buf, ",", 10, temp);
            strcat(data, buf);
            
            strcat(data, (const char*)loop_buf);
            strcat(data, ",");
            break;
        }
        case GR:
        {
            strcat(data, (const char*)output_buf);
            strcat(data, ",");
            
            strcat(data, (const char*)loop_buf);
            strcat(data, ",");
            break;
        }
        case BBD:
        {
            strcat(data, (const char*)output_buf);//测试电压
            strcat(data, ",");
            
            strcat(data, (const char*)loop_buf);//测试电容
            strcat(data, ",");
            
            strcat(data, (const char*)real_buf);//基准电容
            strcat(data, ",");
            break;
        }
    }
    
    mysprintf((uint8_t*)buf, ",", 151, g_dis_time);
    strcat(data, buf);
    mysprintf((uint8_t*)buf, NULL, 120, cur_status);
    strcat(data, buf);
}



uint32_t get_plc_relay_status_for_comm(void)
{
	typedef	struct {
		uint8_t data1;
		uint8_t data2;
		uint8_t data3;
		uint8_t data4;
	}PLC_ST;
	union{
		uint32_t data;
		PLC_ST plc_st;
	}plc_un;
	
	memset(&plc_un, 0, sizeof plc_un);
	
	if((type_spe.interface_config & INTERFACE_PLC) && sys_par.plc_on_off)
	{
		plc_un.plc_st.data1 = sys_par.test_level;
		plc_un.plc_st.data2 = cur_status_plc;
		plc_un.plc_st.data3 = cur_plc_err_st;
	}
    
	return plc_un.data;
}
int16_t get_testing_status_for_comm(void)
{
	return cur_status;
}
int32_t check_res_data(RESULT_STR *res)
{
    if(res->step > MAX_STEPS)
    {
        return -1;
    }
    if(res->total > MAX_STEPS)
    {
        return -1;
    }
    if(-1 == check_this_mode(res->mode))
    {
        return -1;
    }
    
    return 0;
}

/*
 * 函数名：get_cur_result_fetch_for_comm
 * 描述  ：获取当前刚刚测试完的记忆组的测试结果
 * 输入  ：data 返回数据缓存 result_num 结果编号 范围1-n (n为当前记忆组的总步数)
 * 输出  ：无
 * 返回  ：命令执行状态
 */
int16_t get_cur_result_fetch_for_comm(uint16_t *data, uint16_t result_num)
{
	RESULT_STR temp_res;
    
    data[0] = 0;
    
	if(result_num < 1)
	{
		return PAR_NOT_ALLOWED;
	}
	
	/* 结果列表中没有那么多的结果了告诉上位机已经没有了 */
	if(result_num > get_cur_file_total() || result_num > app_flag.res_list_num_count)
	{
		strcpy((char*)data, "__RESULT_LIST_END__");
		return NO_ERR;
	}
    
    read_result_list_dis_one(&temp_res, result_num - 1);
    
    if(0 == check_res_data(&temp_res))
    {
        change_result_to_string_comm(&temp_res, (char*)data);
    }
    else
    {
        return EXE_TIME_OUT;
    }
    
	return NO_ERR;
}

int16_t get_result_fetch_for_comm(uint16_t *data, uint16_t result_num)
{
	RESULT_STR temp_res;
    
    data[0] = 0;
    
	if(result_num > get_res_total())
	{
		return PAR_NOT_ALLOWED;
	}
	
    if(result_num < 1)
    {
		return PAR_NOT_ALLOWED;
    }
    
    read_one_result(&temp_res, result_num - 1);
	
    if(0 == check_res_data(&temp_res))
    {
        change_result_to_string_comm(&temp_res, (char*)data);
    }
    else
    {
        return EXE_TIME_OUT;
    }
    
	return NO_ERR;
}

uint16_t get_output_freq(uint8_t mode)
{
    uint32_t fre_buf[]={
      500,500,600,1000,1500,2000,2500,3000,3500,4000,  
    };
    
	switch(mode)
	{
		case ACW:
		case GR:
            return fre_buf[cur_frequency_gear%10];
		case DCW:
			return fre_buf[9];
		case IR:
			return fre_buf[9];
	}
    
	return fre_buf[0];
}

void test_ports_f(TEST_PORT *p, uint8_t *buf)
{
    uint16_t ports_num = p->num;
    int32_t i = 0;
    int32_t j = 0;
    uint16_t *p_u16 = (uint16_t*)&p->ports;
    
    buf[0] = 0;
    
    if(type_spe.port_num == PORT_NUM0)
    {
        return;
    }
    
    for(i = 0, j = 0; i < ports_num; i++)
    {
        buf[j++] = "XLH"[(p_u16[i/8] >> ((i % 8) * 2)) & 3];
    }
    
    buf[j] = 0;
}
void get_active_step_msg_for_comm(char *data)
{
    uint8_t buf[50] = {0};

    memset(data, 0, 1);

    mysprintf((uint8_t*)buf, ",", 120, cur_step);
    strcat(data, (const char*)buf);
    
    mysprintf((uint8_t*)buf, ",", 10, getTestMode(cur_mode));
    strcat(data, (const char*)buf);
    
    switch(cur_mode)
    {
        case ACW:
        {
            mysprintf((uint8_t*)buf, ",", 153, cur_vol);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, cur_gear_comm);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", ac_gear[cur_gear].decs + 150, cur_high);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", ac_gear[cur_gear].decs + 150, cur_low);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", ac_gear[cur_gear].decs + 150, cur_real_cur);
            strcat(data, (const char*)buf);
            
            if(g_cur_file->arc_mode == ARC_GRADE_MODEL)
            {
                mysprintf((uint8_t*)buf, ",", 10, cur_arc_gear);
            }
            else
            {
                mysprintf((uint8_t*)buf, ",", 2 + 150, cur_arc_gear);
            }
            
            strcat(data, (const char*)buf);
            
            get_hz_for_comm_f((uint16_t*)buf);
            strcat((char*)buf, ",");
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, ris_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, tes_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, fal_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, int_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_pass);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_con);
            strcat(data, (const char*)buf);
            
            test_ports_f(&cur_port, (uint8_t*)buf);
            strcat(data, (const char*)buf);
            break;
        }
        case DCW:
        {
            mysprintf((uint8_t*)buf, ",", 153, cur_vol);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, cur_gear_comm);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", dc_gear[cur_gear].decs + 150, cur_high);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", dc_gear[cur_gear].decs + 150, cur_low);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", dc_gear[cur_gear].decs + 150, cur_charge_cur);
            strcat(data, (const char*)buf);
            
            if(g_cur_file->arc_mode == ARC_GRADE_MODEL)
            {
                mysprintf((uint8_t*)buf, ",", 10, cur_arc_gear);
            }
            else
            {
                mysprintf((uint8_t*)buf, ",", 2 + 150, cur_arc_gear);
            }
            
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, cur_delay_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, ris_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, tes_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, fal_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, int_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_pass);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_con);
            strcat(data, (const char*)buf);
            
            test_ports_f(&cur_port, (uint8_t*)buf);
            strcat(data, (const char*)buf);
            break;
        }
        case IR:
        {
            mysprintf((uint8_t*)buf, ",", 153, cur_vol);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, cur_auto);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 150, cur_high);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 150, cur_low);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, ris_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, tes_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, cur_delay_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, int_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_pass);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_con);
            strcat(data, (const char*)buf);
            
            test_ports_f(&cur_port, (uint8_t*)buf);
            strcat(data, (const char*)buf);
            break;
        }
        case GR:
        {
            mysprintf((uint8_t*)buf, ",", 152, cur_vol);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, cur_high);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, cur_low);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, tes_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 151, int_t);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_pass);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_con);
            strcat(data, (const char*)buf);
            
            get_hz_for_comm_f((uint16_t*)buf);
            strcat(data, (const char*)buf);
            break;
        }
        case BBD:
        {
            mysprintf((uint8_t*)buf, ",", 153, cur_vol);//输出电压
            strcat(data, (const char*)buf);
            
            strcat(data, (const char*)real_buf);//基准电容
            strcat(data, ",");
            
            mysprintf((uint8_t*)buf, ",", 130, cur_open_ratio);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 130, cur_short_ratio);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_pass);
            strcat(data, (const char*)buf);
            
            mysprintf((uint8_t*)buf, ",", 10, steps_con);
            strcat(data, (const char*)buf);
            
            test_ports_f(&cur_port, (uint8_t*)buf);
            strcat(data, (const char*)buf);
        }
    }
    
    if(data[strlen(data) - 1] == ',')
    {
        data[strlen(data) - 1] = 0;
    }
}


void get_active_file_msg_for_comm(FILE_MSG *p_file)
{
    p_file->file_num = g_cur_file->num;
    p_file->step_sum = g_cur_file->total;
    strcpy((char *)p_file->file_name, (const char*)g_cur_file->name);
	
    p_file->pass_keep_time = g_cur_file->hold_pass_time;
    p_file->pass_beep_time = g_cur_file->pass_buzzing_time;
	
#ifndef SINGLE_GND
	p_file->mode_type = getFileModeType(g_cur_file->work_mode);
	p_file->arc_type = getFileArcType(g_cur_file->arc_mode);
#endif
    
	test_flag.refresh_for_comm = 1;/* 刷新屏幕数据 */
}
/* 查询文件是否存在存在就返回其文件内容 */
void get_appoint_file_msg_for_comm(CATALOG_FILE *p_file, uint16_t file_num)
{
    const TEST_FILE *p = NULL;
    
    memset(p_file, 0, sizeof(FILE_MSG));
    
	if(0 == check_file_exist(file_num))
	{
		return;/* 文件不存在 */
	}
    
    p = &file_table[file_num];
    
    p_file->file_num = p->num;
    p_file->step_sum = p->total;
    strcpy((char *)p_file->file_name, (const char*)p->name);
	
    p_file->pass_keep_time = p->hold_pass_time;
    p_file->pass_beep_time = p->pass_buzzing_time;
	
#ifndef SINGLE_GND
	p_file->mode_type = getFileModeType(p->work_mode);
	p_file->arc_type = getFileArcType(p->arc_mode);
#endif
}

int16_t active_appoint_file_for_comm(uint16_t file_num)
{
	if(0 == check_file_exist(file_num))
	{
		return PAR_NOT_ALLOWED;/* 文件不存在 */
	}
	
	g_cur_file = pos_file(file_num);/* 重新定位当前文件 */
	sys_flag.last_group = g_cur_file->num;/* 更新最后使用的文件标志 */
	save_sys_flag();
	read_group_table(file_num);
	read_step_used_flag(file_num);
    
	/* 读出最近使用的记忆组 */
	if(read_list(g_cur_file->num))
	{
		sys_flag.mem_init = 0;
		save_sys_flag();
		return PAR_NOT_ALLOWED;/* 仪器未准备好 */
	}
	
	g_cur_step = list_99xx.head;
	test_flag.refresh_for_comm = 1;/* 刷新屏幕 */
	
	return NO_ERR;
}

int16_t active_appoint_step_for_comm(uint16_t step_num)
{
    if(0 != check_step_exist_for_comm(step_num))
    {
        return PAR_NOT_ALLOWED;
    }
    
    load_steps_to_list(step_num);
    g_cur_step = list_99xx.head;
    
	test_flag.refresh_for_comm = 1;/* 刷新屏幕数据 */
    
	return NO_ERR;
}
/*******************************************************************/

int16_t set_all_sys_par_for_comm(SYS_PAR * p_sys_par)
{
// 	sys_par = *p_sys_par;
//     sys_par = *(SYS_PAR *)usb_loading_buf;
	save_sys_par();
	return NO_ERR;
}

int16_t set_sys_flag_for_comm(SYS_FLAG * p_sys_flag)
{
	save_sys_flag();
	return NO_ERR;
}

int16_t set_step_used_flag_for_comm(uint16_t pack_num, uint8_t * data, uint16_t size)
{
    memcpy(step_used_flag + pack_num * USB_PACKAGE_SIZE, data, size);
    
	return NO_ERR;
}

int16_t set_group_table_for_comm(uint16_t pack_num, uint8_t * data, uint16_t size)
{
    memcpy(cur_group_table + pack_num * USB_PACKAGE_SIZE / 2, data, size);
	return NO_ERR;
}

int16_t set_group_steps_for_comm(uint16_t file_num, uint16_t note_num, uint8_t * data, uint16_t size)
{
    g_cur_file = &file_table[file_num];//pos_file(file_num);
    read_group_table(file_num);
    
    save_one_step((NODE_STEP*)data, note_num);
    
    if(file_num == MAX_FILES - 1)
    {
        g_cur_file = pos_file(0);/* 定位到默认文件 */
        read_group_table(0);
        read_step_used_flag(0);
    }
	return NO_ERR;
}

uint16_t enter_auto_cal_ui_for_comm(uint16_t *data)
{
    /* 本控不允许发送进入退出校准界面指令 */
    if(g_comm_status == LOCAL_COMM)
    {
        return EXE_NOT_ALLOWED;
    }
	
	enter_auto_cal_ui();
	
	return NO_ERR;
}
#include "auto_calibration.h"
uint16_t exit_auto_cal_ui_for_comm(uint16_t *data)
{
    /* 本控不允许发送进入退出校准界面指令 */
    if(g_comm_status == LOCAL_COMM)
    {
        return EXE_NOT_ALLOWED;
    }
	
	exit_auto_cal_ui();
	
	return NO_ERR;
}
uint16_t get_cal_status_for_comm(uint16_t *data)
{
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	get_auto_cal_status(data);
	
	return NO_ERR;
}

uint16_t get_cal_mode_for_comm(uint16_t *data)
{
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	strcpy((char*)data, (const char*)get_cur_point_mode());
	
	return NO_ERR;
}
uint16_t get_cal_range_for_comm(uint16_t *data)
{
	uint8_t buf[15];
	
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	strcpy((char*)data, (const char*)get_cur_point_range(buf));
	
	return NO_ERR;
}

uint16_t auto_cal_start_for_comm(uint16_t *data)
{
    /* 不在自动校准状态不允许发送启动和停止指令 */
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	auto_cal_start_output();
	
	return NO_ERR;
}

uint16_t auto_cal_stop_for_comm(uint16_t *data)
{
    /* 不在自动校准状态不允许发送启动和停止指令 */
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	auto_cal_stop_output();
	
	return NO_ERR;
}

uint16_t set_cal_measure_for_comm(uint16_t *data)
{
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	set_auto_cal_measure(*(uint64_t*)data);
	
	return NO_ERR;
}

uint16_t get_cal_points_for_comm(uint16_t *data)
{
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	get_auto_cal_points(data);
	
	return NO_ERR;
}

uint16_t load_cal_point_for_comm(uint16_t *data)
{
    if(app_flag.auto_cal_st == 0)
    {
        return EXE_NOT_ALLOWED;
    }
	
	if(load_auto_cal_point(data[0]) > 0)
	{
		return DATA_OUT_RANGE;
	}
	
	return NO_ERR;
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
