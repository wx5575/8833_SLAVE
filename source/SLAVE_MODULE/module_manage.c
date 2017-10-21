/**
  ******************************************************************************
  * @file    module_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块管理
  ******************************************************************************
  */
#define MODULE_GLOBALS
#include "module_manage.h"
#include "string.h"
#include "stdio.h"
#include "crc.h"
#include "USART3.H"
#include "Timer5_Config.h"
#include "auto_cal_comm.h"
#include "auto_calibration.h"
#include "serve_test.h"
#include "app.h"
#include "SysTemConfig.h"
#include "keyboard.h"
#include "cs99xx_mem_api.h"

uint8_t g_module_num;


enum{
    GET_MODULE_INF = 1,///<获取模块信息用来联机用
    SET_MODULE_NUM = 2,///<设置模块的编号，对于多路同步测试仪要告诉每个模块在系统中的编号,用这个编号来判断 \
                    是否参与测试工作
    FORMAT_DATA    = 3,///<格式化数据
    SET_CONFIG_PARAMETER    = 4,///<设置配置参数
    
    START_SLAVE_TEST        = 10,///<启动从机测试
    STOP_SLVAE_TEST         = 11,///<停止从机测试
    GET_SLAVE_TEST_DATA     = 12,///<获取测试数据
    TEST_OVER_SIGN_H        = 13,///<控制从机测试完成信息输出高电平
    TEST_OVER_SIGN_L        = 14,///<控制从机测试完成信号输出低电平
    GET_SLAVE_TEST_TIME     = 15,///<获取从机测试时间
    
    SLAVE_NEW_FILE          = 30,///<新建文件
    SLAVE_EDIT_FILE         = 31,///<设置文件
    SLAVE_READ_FILE         = 32,///<读取文件	
    SLAVE_DEL_FILE          = 33,///<删除文件
    SLAVE_SAVE_FILE         = 34,///<保存文件
    SLAVE_LOAD_FILE         = 35,///<加载文件作为当前文件
    SLAVE_CLEAR_ALL_FILES   = 36,///<清空所有文件
    
    SLAVE_INSERT_STEP       = 40,///<插入步骤	
    SLAVE_LOAD_STEP         = 41,///<加载步骤作为当前步骤
    SLAVE_EDIT_STEP         = 42,///<设置步骤	
    SLAVE_READ_STEP         = 43,///<读取步骤
    SLAVE_DEL_STEP          = 44,///<删除步骤	
    SLAVE_PRE_STEP          = 45,///<前移步骤	
    SLAVE_BACK_STEP         = 46,///<后移步骤
    SLAVE_SWAP_STEP         = 47,///<交换步骤
    
    SLAVE_READ_RES          = 50,///<读取一条结果
    SLAVE_CLEAR_RES         = 51,///<清空结果
    SLAVE_READ_LAST         = 52,///<读取最近一次测试的结果
    
    SLAVE_ENTER_CAL_ST      = 60,///<从机进入校准状态
    SLAVE_EXIT_CAL_ST       = 61,///<退出校准状态
    SLAVE_CAL_ST            = 62,///<校准状态查询
    SLAVE_CAL_MODE          = 63,///<校准模式查询
    SLAVE_CAL_POINT_RANGE   = 64,///<当前校准点量程查询
    SLAVE_CAL_START         = 65,///<启动校准输出
    SLAVE_CAL_STOP          = 66,///<停止校准输出
    SLAVE_CAL_MEASURE_VALUE = 67,///<设置校准测量值
    GET_MODULE_CAL_POINTS   = 68,///<获取模块的校准总个数
    LOAD_CAL_POINT          = 69,///<加载校准点作为当前校准点
    QUERY_CAL_POINT_INF     = 70,///<查询校准点信息
};

MODULE_INF module=
{
    "8833-BOARD",///< 模块名称
    "V1.0.0",///< 模块版本
    1///< 模块ID
};

/**
  * @brief  获取模块信息
  * @param  [in] data 数据
  * @param  [out] ack_data 应答数据
  * @param  [out] len 应答数据的长度
  * @retval 无
  */
void get_module_inf(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    memcpy(ack_data, &module, sizeof(module));
    *len = sizeof(module);
}
/**
  * @brief  设置模块编号，使用这个编号模块可以从步骤参数的work_port中判断出是否参与当前的测试工作
  * @param  [in] data 数据
  * @param  [out] ack_data 应答数据
  * @param  [out] len 应答数据的长度
  * @retval 无
  */
void set_module_num(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    memcpy(&g_module_num, data, sizeof(g_module_num));
    *len = 0;
}
/**
  * @brief  设置模块编号，使用这个编号模块可以从步骤参数的work_port中判断出是否参与当前的测试工作
  * @param  [in] data 数据
  * @param  [out] ack_data 应答数据
  * @param  [out] len 应答数据的长度
  * @retval 无
  */
void format_data(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    init_par();
}
void set_config_parameter(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint8_t lenx = sizeof(type_spe);
    
    memcpy(&type_spe, data, sizeof(type_spe));
    init_other_speciality();
}


void get_module_cal_points(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint8_t points = auto_cal.cal_total_points;
    
    memcpy(ack_data, &points, sizeof(points));
    *len = sizeof(points);
}


void slave_enter_cal_st(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    enter_auto_cal_ui();
}
void slave_exit_cal_st(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    exit_auto_cal_ui();
}


void slave_cal_st(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    get_auto_cal_status((void*)ack_data);
    *len = strlen((const char*)ack_data);
}

void slave_cal_mode(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    strcpy((char*)ack_data, (const char*)get_cur_point_mode());
    *len = strlen((const char*)ack_data);
}
void slave_cal_point_range(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
	sprintf((char*)ack_data, "%d", cal_order[g_cur_cal_opt_num].range);
    *len = strlen((const char*)ack_data);
}


void slave_cal_start(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    auto_cal_start_output();
}


void slave_cal_stop(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    auto_cal_stop_output();
}

void load_cal_point(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint8_t point = 0;
    
    memcpy(&point, data, sizeof(point));
    load_auto_cal_point(point);
}

void slave_cal_measure_value(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint64_t measure;
    
    memcpy(&measure, data, sizeof(measure));
    set_auto_cal_measure(measure);
}

void start_slave_test(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    static uint16_t g_key_val = KEY_START;
    OSQPost(KeyboardQSem, &g_key_val);
    
    test_flag.start_comm = 1;
    cur_status = ST_TESTING;
    TEST_0VER = TEST_OVER_N;//清除测试结束
}
void test_over_sign_h(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    TEST_0VER = TEST_OVER_Y;
//    TEST_ALARM = TEST_ALARM_Y;
}
void test_over_sign_l(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    TEST_0VER = TEST_OVER_N;
//    TEST_ALARM = TEST_ALARM_N;
}
void get_slave_test_time(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    memcpy(ack_data, (void*)&g_dis_time, sizeof(g_dis_time));
    *len = sizeof(g_dis_time);
}

//void get_slave_test_data(uint8_t *data, uint8_t *ack_data, uint32_t *len)
//{
//    COMM_TEST_DATA *test_data = (void*)ack_data;
//    
//    memcpy(&test_data->vol, output_buf, sizeof(test_data->vol));
//    memcpy(&test_data->cur, loop_buf, sizeof(test_data->cur));
//    memcpy(&test_data->time, time_buf, sizeof(test_data->time));
//    test_data->status = cur_status;
//    get_dis_par_unit(cur_mode);
//    test_data->vol_unit = cur_vol_unit;
//    test_data->cur_unit = cur_cur_unit;
//    test_data->real_unit = cur_real_unit;
//    test_data->work_st = cur_work_st;
//    memcpy(&test_data->mode, mode_pool[0][cur_mode], sizeof(test_data->mode));
//    sprintf((char*)&test_data->step, "%d/%d", cur_step, g_cur_file->total);
//    
//    if(cur_work_st == 1)
//    {
//        test_data->usable = 1;
//    }
//    else
//    {
//        test_data->usable = 0;
//    }
//    
//    *len = sizeof(COMM_TEST_DATA);
//}

void get_slave_test_data(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    UN_COMM_TEST_DATA *test_data = (void*)ack_data;
    
    test_data->time = g_dis_time;
    test_data->status = cur_status;
    test_data->flag = cur_work_st;
    
    switch(cur_mode)
    {
        case ACW:
            test_data->un.acw.vol = vol_ave;
            test_data->un.acw.cur = cur_ave;
            test_data->un.acw.real = real_ave;
            test_data->un.acw.range = cur_gear;
            break;
        case DCW:
            test_data->un.dcw.vol = vol_ave;
            test_data->un.dcw.cur = cur_ave;
            test_data->un.dcw.range = cur_gear;
            break;
        case IR:
            test_data->un.ir.vol = vol_ave;
            test_data->un.ir.res = res_ave;
            test_data->un.ir.range = cur_gear;
            break;
        case GR:
            test_data->un.gr.cur = cur_ave;
            test_data->un.gr.res = res_ave;
            break;
    }
    
    *len = sizeof(UN_COMM_TEST_DATA);
}
void slave_new_file(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    TEST_FILE *f = (void *)data;
	TEST_FILE *p = &file_table[f->num];
	TEST_FILE *back = g_cur_file;
    
    if(f->num > MAX_FILES)
    {
        return;
    }
    
    memcpy(p, f, sizeof(TEST_FILE));
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
}
void slave_edit_file(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    TEST_FILE *f = (void *)data;
	TEST_FILE *p = &file_table[f->num];
	TEST_FILE *back = g_cur_file;
    
    if(f->num != p->num)
    {
        return;
    }
    
    if(p->total < 1)
    {
        return;
    }
    
    memcpy(p, f, sizeof(TEST_FILE));
    save_file(f->num);
}

void slave_read_file(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    
}
void slave_del_file(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
	FILE_NUM file_num;
    
    memcpy(&file_num, data, sizeof(FILE_NUM));
    
	if(file_num == 0 || file_num > MAX_FILES)
	{
		return;
	}
	
    del_file(file_num);
    
    /* 读出最近使用的记忆组 */
    if(0 != read_list(g_cur_file->num))
    {
        return;/* 仪器未准备好 load_data*/
    }
    
    g_cur_step = list_99xx.head;
}

void slave_save_file(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    TEST_FILE *f = (void *)data;
	TEST_FILE *p = NULL;//
	TEST_FILE *back = g_cur_file;
    
	FILE_NUM file_num;
    
    file_num = f->num;
    p = &file_table[file_num];
    
    memcpy(p, f, sizeof(TEST_FILE));
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    copy_cur_file_to_new_pos(file_num);
}

void slave_load_file(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
	FILE_NUM file_num;
    
    memcpy(&file_num, data, sizeof(file_num));
    
	if(0 == check_file_exist(file_num))
	{
		return;/* 文件不存在 */
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
		return;/* 仪器未准备好 */
	}
	
	g_cur_step = list_99xx.head;
}
void slave_clear_all_files(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    del_all_file();
    
	/* 读出最近使用的记忆组 */
	if(read_list(g_cur_file->num))
	{
		sys_flag.mem_init = 0;
		save_sys_flag();
		return;/* 仪器未准备好 */
	}
	
	g_cur_step = list_99xx.head;
}
void slave_insert_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int32_t i = 0;
    uint8_t m = 0;
    uint8_t mode;
    uint16_t step = 0;
    
    step = g_cur_step->one_step.com.step;
	
    memcpy(&mode, data, sizeof(mode));
    
    m = get_first_step_mode();/* 备份第一步的模式 */
    
    insert_step(step, mode);/* 刚插入的新步的模式应该是型号序列的第一个型号 */
    
    load_steps_to_list(step + 1);
    
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
}

void slave_load_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint16_t step_num = 0;
    
    memcpy(&step_num, data, sizeof(step_num));
    
    if(0 != check_step_exist_for_comm(step_num))
    {
        return;
    }
    
    if(FAIL)
    {
        return;
    }
    
    load_steps_to_list(step_num);
    g_cur_step = list_99xx.head;
    TEST_0VER = TEST_OVER_N;//测试结束
}


void slave_edit_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    NODE_STEP *step = (void*)data;
    
    if(g_cur_step->one_step.com.step != step->one_step.com.step)
    {
        return;
    }
    
    if(g_cur_file->work_mode == G_MODE)
    {
        if(step->one_step.com.mode != g_cur_step->one_step.com.mode)
        {
            clear_step_used_flag();
            clear_group_table();
            g_cur_file->total = 0;
            insert_step(0, step->one_step.com.mode);//插入一步
            save_group_info(g_cur_file->num);//保存新建文件的记忆组信息
            load_steps_to_list(1);
            g_cur_step = list_99xx.head;
        }
    }
    
    memcpy(g_cur_step, step, sizeof(NODE_STEP));
    save_cur_step();
    load_data();
}
void slave_read_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
}

void slave_del_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint16_t step_num;
    
    memcpy(&step_num, data, sizeof(step_num));
    
	if(g_cur_file->total == 1 || step_num != g_cur_step->one_step.com.step)
	{
		return;
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
}

void slave_pre_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
}
void slave_back_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
}


void slave_swap_step(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    uint16_t step_num;
    
    memcpy(&step_num, data, sizeof(step_num));
    
    
    
	if(check_step_exist_for_comm(step_num) != 0)
	{
		return;
	}
	
    swap_step(g_cur_step->one_step.com.step, step_num);
    load_steps_to_list(step_num);
    g_cur_step = list_99xx.head;
    
	load_data();
    save_group_info(g_cur_file->num);
	save_list();/* 将改变后的结果保存的eep中 */
}











void query_cal_point_inf(uint8_t *data, uint8_t *ack_data, uint32_t *len)
{
    CAL_POINT_INF inf;
    uint8_t *t_mode = NULL;
    uint8_t *t_point = NULL;
    uint8_t index = 0;
    
    memcpy(&index, data, sizeof(index));
    
    inf.index = index;
    t_mode = get_cal_point_mode(index);
    t_point = get_cal_point_name(index);
    memcpy(inf.mode, t_mode, sizeof(inf.mode));
    memcpy(inf.point, t_point, sizeof(inf.point));
    inf.decs = cal_order[index].decs;
    inf.unit = cal_order[index].unit;
    inf.lon = cal_order[index].lon;
    inf.mul_power = cal_order[index].mul_power;
    
    memcpy(ack_data, &inf, sizeof(inf));
    *len = sizeof(inf);
}

/**
  * @brief  向发送数据添加CRC校验
  * @param  [in] data 数据
  * @param  [in] len 数据帧长度
  * @retval 无
  */
void add_crc_to_send_data(uint8_t *data, uint32_t len)
{
    uint16_t crc_val;
    uint16_t *p_u16;
    
    crc_val = get_crc16(data, len);
    p_u16 = (uint16_t *)&data[len];
    *p_u16 = crc_val;
}
void send_frame_data(COM_STRUCT *com, uint8_t *ack_frame, uint32_t frame_len)
{
    frame_len += FRAME_HEAD_SIZE;
    add_crc_to_send_data((uint8_t*)ack_frame, frame_len);
    frame_len += CRC_LEN;
    com->send_fun(com, (uint8_t*)ack_frame, frame_len);
    com->status = MODULE_COMM_SEND;//状态机走入发送状态
}

CS_BOOL com_cmd_dispose(FRAME_T *frame, FRAME_T *ack_frame, uint32_t *frame_len)
{
    CS_BOOL res = CS_TRUE;
    
    switch(frame->fun_code)
    {
        case GET_MODULE_INF:
            get_module_inf(frame->data, ack_frame->data, frame_len);
            break;
        case SET_MODULE_NUM:
            set_module_num(frame->data, ack_frame->data, frame_len);
            SLAVE_LED_F11++;
            break;
        case FORMAT_DATA:
            format_data(frame->data, ack_frame->data, frame_len);
            break;
        case SET_CONFIG_PARAMETER:
            set_config_parameter(frame->data, ack_frame->data, frame_len);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

CS_BOOL source_cmd_dispose(FRAME_T *frame, FRAME_T *ack_frame, uint32_t *frame_len)
{
    CS_BOOL res = CS_TRUE;
    
    switch(frame->fun_code)
    {
        case START_SLAVE_TEST:
            start_slave_test(frame->data, ack_frame->data, frame_len);
            break;
        case STOP_SLVAE_TEST:
            break;
        case GET_SLAVE_TEST_DATA:
            get_slave_test_data(frame->data, ack_frame->data, frame_len);
            break;
        case TEST_OVER_SIGN_H:
            test_over_sign_h(frame->data, ack_frame->data, frame_len);
            break;
        case TEST_OVER_SIGN_L:
            test_over_sign_l(frame->data, ack_frame->data, frame_len);
            break;
        case GET_SLAVE_TEST_TIME:
            get_slave_test_time(frame->data, ack_frame->data, frame_len);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}
CS_BOOL file_cmd_dispose(FRAME_T *frame, FRAME_T *ack_frame, uint32_t *frame_len)
{
    CS_BOOL res = CS_TRUE;
    
    switch(frame->fun_code)
    {
        case SLAVE_NEW_FILE:
            slave_new_file(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_EDIT_FILE:
            slave_edit_file(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_READ_FILE:
            slave_read_file(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_DEL_FILE:
            slave_del_file(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_SAVE_FILE:
            slave_save_file(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_LOAD_FILE:
            slave_load_file(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CLEAR_ALL_FILES:
            slave_clear_all_files(frame->data, ack_frame->data, frame_len);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

CS_BOOL step_cmd_dispose(FRAME_T *frame, FRAME_T *ack_frame, uint32_t *frame_len)
{
    CS_BOOL res = CS_TRUE;
    
    switch(frame->fun_code)
    {
        case SLAVE_INSERT_STEP:
            slave_insert_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_LOAD_STEP:
            slave_load_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_EDIT_STEP:
            slave_edit_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_READ_STEP:
            slave_read_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_DEL_STEP:
            slave_del_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_PRE_STEP:
            slave_pre_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_BACK_STEP:
            slave_back_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_SWAP_STEP:
            slave_swap_step(frame->data, ack_frame->data, frame_len);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

CS_BOOL res_cmd_dispose(FRAME_T *frame, FRAME_T *ack_frame, uint32_t *frame_len)
{
    CS_BOOL res = CS_TRUE;
    
    switch(frame->fun_code)
    {
        case SLAVE_READ_RES:
            slave_insert_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CLEAR_RES:
            slave_insert_step(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_READ_LAST:
            slave_insert_step(frame->data, ack_frame->data, frame_len);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}
CS_BOOL cal_cmd_dispose(FRAME_T *frame, FRAME_T *ack_frame, uint32_t *frame_len)
{
    CS_BOOL res = CS_TRUE;
    
    switch(frame->fun_code)
    {
        case GET_MODULE_CAL_POINTS:
            get_module_cal_points(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_ENTER_CAL_ST:
            slave_enter_cal_st(frame->data, ack_frame->data, frame_len);
            break;
        case QUERY_CAL_POINT_INF:
            query_cal_point_inf(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_EXIT_CAL_ST:
            slave_exit_cal_st(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CAL_ST:
            slave_cal_st(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CAL_MODE:
            slave_cal_mode(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CAL_POINT_RANGE:
            slave_cal_point_range(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CAL_START:
            slave_cal_start(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CAL_STOP:
            slave_cal_stop(frame->data, ack_frame->data, frame_len);
            break;
        case LOAD_CAL_POINT:
            load_cal_point(frame->data, ack_frame->data, frame_len);
            break;
        case SLAVE_CAL_MEASURE_VALUE:
            slave_cal_measure_value(frame->data, ack_frame->data, frame_len);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}
/**
  * @brief  串口1的接收处理函数
  * @param  [in] com_num 串口号
  * @param  [in] frame 数据帧
  * @param  [in] len 数据帧长度
  * @retval 无
  */
void com_receive_dispose(COM_STRUCT *com, uint8_t *data, uint32_t len)
{
    uint16_t *p_crc;
    uint16_t crc_val;
    FRAME_T *frame = (void*)data;
    FRAME_T *ack_frame = NULL;
    uint32_t frame_len = 0;
    CS_BOOL res = CS_FALSE;
    
    if(len <= CRC_LEN)
    {
        return;
    }
    
    /* CRC校验 */
    p_crc = (uint16_t *)&data[len - CRC_LEN];
    crc_val = get_crc16(data, len - CRC_LEN);
    
    ack_frame = (void*)com->frame_buf;
    ack_frame->addr = frame->addr;
    ack_frame->fun_code = frame->fun_code;
    
    /* 校验失败放弃解析 */
    if(*p_crc != crc_val || crc_val == 0)
    {
//        ack_frame->st = COMM_ST_CRC_ERROR;
//        send_frame_data(com, (uint8_t*)ack_frame, frame_len);
        return;
    }
    
    /* 检查通信地址 1.为本地地址相同 2.主播地址0 否则就不是本机的数据包 */
    if(frame->addr != module.id && frame->addr != 0)
    {
        return;
    }
    
    /* 准备应答帧数据 */
    ack_frame->st = COMM_ST_NO_ERROR;
    
    /* 公共指令处理 */
    res = com_cmd_dispose(frame, ack_frame, &frame_len);
    
    /* 源指令处理 */
    if(res == CS_FALSE)
    {
        res = source_cmd_dispose(frame, ack_frame, &frame_len);
    }
    
    /* 文件指令处理 */
    if(res == CS_FALSE)
    {
        res = file_cmd_dispose(frame, ack_frame, &frame_len);
    }
    
    /* 步骤指令处理 */
    if(res == CS_FALSE)
    {
        res = step_cmd_dispose(frame, ack_frame, &frame_len);
    }
    
    /* 结果指令处理 */
    if(res == CS_FALSE)
    {
        res = res_cmd_dispose(frame, ack_frame, &frame_len);
    }
    
    /* 校准指令处理 */
    if(res == CS_FALSE)
    {
        res = cal_cmd_dispose(frame, ack_frame, &frame_len);
    }
    
    /* 未定义指令处理 */
    if(res == CS_FALSE)
    {
        ack_frame->st = COMM_ST_UNDEFINED;
    }
    
    /* 发送应答 */
    if(frame->addr != 0)
    {
        ack_frame->data_len = frame_len - FRAME_HEAD_SIZE;
        send_frame_data(com, (uint8_t*)ack_frame, frame_len);
    }
}

/**
  * @brief  初始化模块管理的环境
  * @param  无
  * @retval 无
  */
void init_module_manage_env(void)
{
    register_tim5_server_fun(usart3_judge_timout);//注册串口接收完成超时定时器
}
/**
  * @brief  初始化模块管理的环境
  * @param  无
  * @retval 无
  */
void module_comm_task(void)
{
    com_comm_status_machine(&com1);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
