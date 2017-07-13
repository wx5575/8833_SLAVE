
#include	<string.h>
#include	"RTC_Config.h"
#include 	"app.h"
#include	"irq.h"
#include	"cs99xx_struct.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_exception.h"
#include	"cs99xx_type.h"
#include    "cs99xx_result.h"
#include	"keyboard.h"
#include    "mb_cmd_define.h"
#include    "cs99xx_for_comm.h"
#include    "serve_test.h"
#include    "auto_cal_comm_interface.h"



/* 电源检测 */
void m_power_checkres_dispose(uint16_t cmd, uint16_t *data)
{
    app_flag.power_check = *data;
    *data = NO_ERR;
} 
 /*与上位机通信地址*/
void m_comm_addr_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = sys_par.local_addr;
    }
    else
    {
        sys_par.local_addr = *data;
        *data = NO_ERR;
    }
}
/*控制状态*/
void m_comm_ctrl_status_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t status = *data;
    
    if(cmd == MB_READ_PAR)
    {
        *data = g_comm_status;
    }
    else
    {
        if(status == CTRL_REMOTE)
        {
            /*远程控制 关闭键盘 */
            off_keyboard();
            g_comm_status = REMOTE_COMM;
            test_flag.refresh_for_comm = 1;
            /* 进入测试界面 */
            if(app_flag.into_test_ui == 0)
            {
                static uint16_t key_val = KEY_EXIT;
                
                OSQPost(KeyboardQSem, &key_val);
                g_exit = 1;
                g_return = 1;
                test_flag.into_test = 1;
            }
        }
        else if(status == CTRL_LOCAL)
        {
            /*本机控制 打开键盘*/
            on_keyboard();
            g_comm_status = LOCAL_COMM;
            test_flag.refresh_for_comm = 1;
        }
        *data = NO_ERR;
    }
}
/* 控制测试仪启动停止 */
void m_control_test_button_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t status = *data;
    
    /* 本控不允许发送启动和停止指令 */
    if(g_comm_status == LOCAL_COMM)
    {
        *data = EXE_NOT_ALLOWED;
        return;
    }
    
    if(cmd == MB_READ_PAR)
    {
        
    }
    else
    {
        /* 启动键 */
        if(status == 1)
        {
            if(cur_status == ST_WAIT || cur_status == ST_PASS|| cur_status == ST_STOP)
            {
                static uint16_t g_key_val = 0;
                uint8_t temp = 0;
                
                if(app_flag.into_test_ui == 0)
                {
                    g_key_val = KEY_EXIT;
                    OSQPost(KeyboardQSem,&g_key_val);
                    
                    test_flag.into_test = 1;
                    
                    while(app_flag.into_test_ui == 0)
                    {
                        OSTimeDlyHMSM(0,0,0,10);
                        
                        if(++temp >= 5)
                        {
                            *data = EXE_NOT_ALLOWED;
                            return;
                        }
                    }
                }
                
                g_key_val = KEY_START;
                OSQPost(KeyboardQSem, &g_key_val);
                
                test_flag.start_comm = 1;
                cur_status = ST_TESTING;
                cur_cylinder_ctrl_status = CY_TESTING;
            }
        }
        /* 复位键 */
        else if(status == 2)
        {
            stop_irq();
        }
        
        *data = NO_ERR;
    }
}

/*删除文件，参数为0删除所有文件，不为0删除单个文件*/
void m_delete_file_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = *data;
    
    if(1 == check_file_exist(file_num))
    {
        *data = del_file_for_comm(file_num);
    }
    else
    {
        *data = PAR_NOT_ALLOWED;
    }
}       
/*读取当前文件编号*/
void m_query_active_file_num_dispose(uint16_t cmd, uint16_t *data)
{
    *data = g_cur_file->num;
}
/*读取当前步骤为编号*/
void m_query_active_step_num_dispose(uint16_t cmd, uint16_t *data)
{
    *data = g_cur_step->one_step.com.step;
}

/* 查询文件是否存在存在就返回其文件内容 */
void m_catalog_file_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = *data;
    
    get_appoint_file_msg_for_comm((CATALOG_FILE *)data, file_num);
//     if(1 == check_file_exist(file_num))
//     {
//         *data = NO_ERR;
//     }
//     else
//     {
//         *data = PAR_NOT_ALLOWED;
//     }
}
/*插入测试步骤*/
void m_insert_step_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t test_mode = *data;
    
    *data = inster_one_step_for_comm(test_mode);
}
/*删除当前活动步骤*/
void m_delete_step_dispose(uint16_t cmd, uint16_t *data)
{
    *data = del_cur_step_for_comm();
}
/*当前活动步骤前移一位*/
void m_active_step_front_dispose(uint16_t cmd, uint16_t *data)
{
    *data = advance_cur_step_for_comm();
}
/*当前活动步骤后移一位*/
void m_active_step_behind_dispose(uint16_t cmd, uint16_t *data)
{
    *data = retreat_cur_step_for_comm();
}
/*交换步骤信息*/
void m_inter_change_step_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t step = *data;
    
    *data = swap_two_step_for_comm(step);
}
/* 修改当前活动步的模式 */
void m_updata_step_mode_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t step = *data;
    
    set_mode_for_comm(step + 1);
}

void m_system_nrule_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t temp = *data;
    
    if(MB_READ_PAR == cmd)
    {
        *data = sys_par.num_rule; 
    }
    else
    {
        *data = set_sys_par_for_comm(temp, SYS_NUM_RULER);
    }
}
void m_system_time_dispose(uint16_t cmd, uint16_t *data)
{
    SYS_TIME time_t = *(SYS_TIME *)data;
    
    if(MB_READ_PAR == cmd)
    {
        get_sys_time_for_comm(&time_t);
        *(SYS_TIME *)data = time_t;
    }
    else
    {
        *data = set_sys_time_for_comm(&time_t);
    }
}
void m_pass_word_dispose(uint16_t cmd, uint16_t *data)
{
    PASS_WORD_T *p = (PASS_WORD_T*)data;
    
    if(MB_READ_PAR == cmd)
    {
        strcpy(p->old, sys_par.pass_word);
    }
    else
    {
        if(0 == strcmp(sys_par.pass_word, p->old))
        {
            strcpy(sys_par.pass_word, p->new);
            *data = NO_ERR;
            save_sys_par();
        }
        else
        {
            *data = PAR_NOT_ALLOWED;
        }
    }
}
void m_keyklock_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_KEY_LOCK);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_KEY_LOCK);
    }
}
void m_fail_continue_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_FAIL_CONT);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_FAIL_CONT);
    }
}
void m_sys_language_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_LANGUAGE);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_LANGUAGE);
    }
}
void m_test_port_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_TEST_PORT);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_TEST_PORT);
    }
}
/*复位暂停功能目前不支持*/
void m_reset_stop_dispose(uint16_t cmd, uint16_t *data)
{
	*data = NO_ERR;
}
/* 检验提示目前不支持 */
void m_check_hint_dispose(uint16_t cmd, uint16_t *data)
{
	*data = NO_ERR;
}
void m_self_check_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_SELF_CHECK);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_SELF_CHECK);
    }
}
void m_test_signal_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_TEST_SIGNAL);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_TEST_SIGNAL);
    }
}
/*高压保护目前不支持*/
void m_phv_dispose(uint16_t cmd, uint16_t *data)
{
}
void m_gfi_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_GFI);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_GFI);
    }
}
void m_result_recover_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_RES_COVER);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_RES_COVER);
    }
}
void m_result_save_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_RES_SAVE);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_RES_SAVE);
    }
}
/* 余量提示 */
void m_res_remain_hint_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_RES_ALLOWANCE_HINT);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_RES_ALLOWANCE_HINT);
    }
}
/* 系统音量 */
void m_sys_volume_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_VOLUME);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_VOLUME);
    }
}
/* 屏幕对比度 */
void m_sys_contrast_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_sys_par_for_comm(data, SYS_CONTRAST);
    }
    else
    {
        *data = set_sys_par_for_comm(*data, SYS_CONTRAST);
    }
}
void m_dut_name_dispose(uint16_t cmd, uint16_t *data)
{
    RESULT_DUT_T *p = (RESULT_DUT_T*)data;
    
    if(MB_READ_PAR == cmd)
    {
        get_dut_name_for_comm(p->name);
    }
    else
    {
        set_dut_name_for_comm(p->name);
        *data = NO_ERR;
    }
}
void m_result_fetch_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t num = *data;
    uint16_t flag = data[1];
    
    if(num < 1)
    {
        return;
    }
    /* 获取总的测试结果 */
    if(0 == flag)
    {
        data[0] = get_result_fetch_for_comm(data+1, num);
    }
    /* 获取最近测试的记忆组的测试结果 */
    else
    {
        data[0] = get_cur_result_fetch_for_comm(data+1, num);
    }
}
void m_result_clear_dispose(uint16_t cmd, uint16_t *data)
{
    clear_all_res_for_comm();
    *data = NO_ERR;
}

void m_result_fail_dispose(uint16_t cmd, uint16_t *data)
{
     *data = get_result_info_for_comm(RES_FAIL_NUM);
}
void m_result_pass_dispose(uint16_t cmd, uint16_t *data)
{
     *data = get_result_info_for_comm(RES_PASS_NUM);
}
void m_result_used_num_dispose(uint16_t cmd, uint16_t *data)
{
     *data = get_result_info_for_comm(RES_USED_NUM);
}
void m_result_remain_num_dispose(uint16_t cmd, uint16_t *data)
{
     *data = get_result_info_for_comm(RES_FREE_NUM);
}
void m_result_total_num_dispose(uint16_t cmd, uint16_t *data)
{
    *data = get_result_info_for_comm(RES_TOTAL_NUM);
}
void m_test_ports_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_test_port_for_comm(data);
    }
    else
    {
        *data = set_test_port_for_comm(data);
    }
}
void m_ir_auto_shift_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_ir_auto_for_comm(data);
    }
    else
    {
        *data = set_ir_auto_for_comm(*data);
    }
}
void m_com_steps_cont_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_steps_cont_for_comm(data[0]);
    }
    else
    {
        *data = set_steps_cont_for_comm(data[1], data[0]);
    }
}
void m_com_steps_pass_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_pass_signal_for_comm(data[0]);
    }
    else
    {
        *data = set_pass_signal_for_comm(data[1], data[0]);
    }
}
void m_com_delay_t_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_timer_for_comm(data[0], D_TIME);
    }
    else
    {
        *data = set_timer_for_comm(data[1], data[0], D_TIME);
    }
}
void m_com_change_t_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_timer_for_comm(data[0], C_TIME);
    }
    else
    {
        *data = set_timer_for_comm(data[1], data[0], C_TIME);
    }
}
void m_com_inter_t_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_timer_for_comm(data[0], I_TIME);
    }
    else
    {
        *data = set_timer_for_comm(data[1], data[0], I_TIME);
    }
}
void m_com_fall_t_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_timer_for_comm(data[0], F_TIME);
    }
    else
    {
        *data = set_timer_for_comm(data[1], data[0], F_TIME);
    }
}
void m_com_test_t_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_timer_for_comm(data[0], T_TIME);
    }
    else
    {
        *data = set_timer_for_comm(data[1], data[0], T_TIME);
    }
}
void m_com_rise_t_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_timer_for_comm(data[0], R_TIME);
    }
    else
    {
        *data = set_timer_for_comm(data[1], data[0], R_TIME);
    }
}
void m_com_frequency_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_hz_for_comm(data);
    }
    else
    {
        *data = set_hz_for_comm(data[1], data[0]);
    }
}
void m_com_arc_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        get_arc_for_comm(data);
    }
    else
    {
        *data = set_arc_for_comm(data[1], data[0]);
    }
}
void m_com_current_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_vol_for_comm(&data[1], data[0]);
    }
    else
    {
        *data = set_vol_for_comm(data[1], data[0]);
    }
}
void m_com_charge_current_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_charge_cur_for_comm(&data[0]);
    }
    else
    {
        *data = set_charge_cur_for_comm(data[1], data[0]);
    }
}
void m_com_real_current_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_real_cur_for_comm(&data[0]);
    }
    else
    {
        *data = set_real_cur_for_comm(data[0]);
    }
}
void m_com_cur_low_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_low_for_comm(&data[1], data[0]);
    }
    else
    {
        *data = set_low_for_comm(data[1], data[0]);
    }
}
void m_com_cur_offset_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_offset_value_for_comm(&data[1], data[0]);
    }
    else
    {
        *data = set_offset_value_for_comm(data[1], data[0]);
    }
}
void m_com_cur_high_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_high_for_comm(&data[1], data[0]);
    }
    else
    {
        *data = set_high_for_comm(data[1], data[0]);
    }
}
void m_com_cur_gear_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_gear_for_comm(data[0]);
    }
    else
    {
        *data = set_gear_for_comm(data[1], data[0]);
    }
}
void m_com_voltage_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_vol_for_comm(&data[1], data[0]);
    }
    else
    {
        *data = set_vol_for_comm(data[1], data[0]);
    }
}

void m_com_mode_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_mode_for_comm();
    }
    else
    {
        *data = set_mode_for_comm(data[0]);
    }
}

void m_bbd_open_check_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_bbd_open_check_for_comm(data);
    }
    else
    {
        *data = set_bbd_open_check_for_comm(data[0], data[1]);
    }
}
void m_bbd_short_check_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_bbd_short_check_for_comm(data);
    }
    else
    {
        *data = set_bbd_short_check_for_comm(data[0], data[1]);
    }
}

void m_swap_steps_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        *data = swap_two_step_for_comm(data[0]);
    }
}
void m_move_forward_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        *data = retreat_cur_step_for_comm();
    }
}
void m_move_backward_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        *data = advance_cur_step_for_comm();
    }
}
void m_test_mode_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        *data = get_mode_for_comm();
    }
    else
    {
        *data = set_mode_for_comm(data[0]);
    }
}
void m_test_fetch_msg_dispose(uint16_t cmd, uint16_t *data)
{
#if 1
    TEST_FETCH_T* p = (TEST_FETCH_T*)data;
    
    get_test_fetch_for_comm((char *)p->buf);
    p->plc_status = get_plc_relay_status_for_comm();
#else
    get_test_fetch_for_comm((char *)data);
#endif
    
}
void m_get_cap_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        *data = get_cap_for_comm(data[0]);
    }
}
void m_get_offset_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        *data = get_offset_for_comm(data[0]);
    }
}

void m_plc_status_dispose(uint16_t cmd, uint16_t *data)
{
    *(uint32_t*)data = get_plc_relay_status_for_comm();
}
void m_test_status_dispose(uint16_t cmd, uint16_t *data)
{
    *data = get_testing_status_for_comm();
}
void m_set_plc_st_dispose(uint16_t cmd, uint16_t *data)
{
}

void m_del_all_file_dispose(uint16_t cmd, uint16_t *data)
{
    *data = del_all_file_for_comm();
}
void m_step_msg_dispose(uint16_t cmd, uint16_t *data)
{
    get_active_step_msg_for_comm((char*)data);
}
void m_file_msg_dispose(uint16_t cmd, uint16_t *data)
{
    get_active_file_msg_for_comm((FILE_MSG *)data);
}
// void m_file_is_exist_dispose(uint16_t cmd, uint16_t *data)
// {
//     uint16_t file_num = *data;
//     
//     get_appoint_file_msg_for_comm((FILE_MSG *)data, file_num);
// }
/* 源指令设置指定的测试步为当前步 */
void m_set_active_step_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t step_num = *data;
    
    *data = active_appoint_step_for_comm(step_num);
}
void m_set_active_file_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = *data;
    
    *data = active_appoint_file_for_comm(file_num);
}
/* 当前文件另存为 带参 通过文件结构将文件名与文件编号传过来 */
void m_save_file_dispose(uint16_t cmd, uint16_t *data)
{
    *data = app_save_file_for_comm((FILE_MSG *)data);
}
void m_edit_file_dispose(uint16_t cmd, uint16_t *data)
{
    *data = app_edit_file_for_comm((FILE_MSG *)data);
}
void m_new_file_dispose(uint16_t cmd, uint16_t *data)
{
    *data = app_new_file_for_comm((FILE_MSG *)data);
}
void m_id_msg_dispose(uint16_t cmd, uint16_t *data)
{
    get_idn_msg_for_comm((IDN_MSG *)data);
}
void m_rst_dispose(uint16_t cmd, uint16_t *data)
{
    *data = tester_reset_for_comm();
}
// void m_comm_ctrl_st_dispose(uint16_t cmd, uint16_t *data)
// {
// }
void m_tester_info_dispose(uint16_t cmd, uint16_t *data)
{
    if(cmd == MB_READ_PAR)
    {
        *data = updata_comm_flag;
        updata_comm_flag = 0;
    }
}
void m_comm_bps_dispose(uint16_t cmd, uint16_t *data)
{
    if(cmd == MB_READ_PAR)
    {
        *data = sys_par.baud_rate_gear;
    }
}

void m_comm_sw_status_dispose(uint16_t cmd, uint16_t *data)
{
    if(cmd == MB_READ_PAR)
    {
        *data = sys_par.commu_on_off;
    }
}
void m_plc_sw_status_dispose(uint16_t cmd, uint16_t *data)
{
    if(cmd == MB_READ_PAR)
    {
        *data = sys_par.plc_on_off;
    }
}


//////////////////////////////////////////////////////////////////////

void m_sys_par_usb_dispose(uint16_t cmd, uint16_t *data)
{
    if(cmd == MB_READ_PAR)
    {
        memcpy(data, &sys_par, sizeof sys_par);
    }
    else
    {
        memcpy(&sys_par, data, sizeof sys_par);
        save_sys_par();
    }
}
void m_sys_flag_usb_dispose(uint16_t cmd, uint16_t *data)
{
    SYS_FLAG_T *p = (SYS_FLAG_T *)&sys_flag;
    
    if(cmd == MB_READ_PAR)
    {
        memcpy(data, p, sizeof(SYS_FLAG_T));
    }
    else
    {
        memcpy(p, data, sizeof(SYS_FLAG_T));
        save_sys_flag();
    }
}
void m_file_list_usb_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t file_num = *data;
    
    if(cmd == MB_READ_PAR)
    {
        memcpy(data, &file_table[file_num], sizeof(TEST_FILE));
    }
    else
    {
        memcpy(&file_table[file_num], data + 2, sizeof(TEST_FILE));
        save_file(file_num);
    }
}

void m_step_used_flag_base_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = data[0];
    uint8_t pack_num = data[1];
    uint8_t len = data[2];
    uint16_t *p = data + 3;
    
    if(cmd == MB_READ_PAR)
    {
        if(pack_num == 0)
        {
            read_step_used_flag(file_num);
        }
        
        memcpy(&data[3], (uint16_t *)(step_used_flag + USB_PACKAGE_SIZE * pack_num), len);
    }
    else
    {
        set_step_used_flag_for_comm(pack_num, (uint8_t *)p, len);
        
        if(len != USB_PACKAGE_SIZE)
        {
            save_step_used_flag(file_num);
        }
    }
}

void m_group_table_base_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = data[0];
    uint8_t pack_num = data[1];
    uint8_t len = data[2];
    uint16_t *p = data + 3;
    
    if(cmd == MB_READ_PAR)
    {
        if(pack_num == 0)
        {
            read_group_table(file_num);
        }
        
        memcpy(&data[3], (uint16_t *)(cur_group_table + USB_PACKAGE_SIZE / 2 * pack_num), len);
    }
    else
    {
        set_group_table_for_comm(pack_num, (uint8_t *)p, len);
        
        if(len != USB_PACKAGE_SIZE)
        {
            save_group_table(file_num);
        }
    }
}

void m_mem_group_base_usb_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = data[0];
    uint8_t step_num = data[1];
    uint8_t len = data[2];
    NODE_STEP node;
    uint16_t *p = data + 3;
    
    if(cmd == MB_READ_PAR)
    {
        if(step_num == 1)
        {
            read_step_used_flag(file_num);
            read_group_table(file_num);
            g_cur_file = &file_table[file_num];
        }
        
        read_one_step(&node, step_num);
        
        memcpy(&data[3], &node, len);
    }
    else
    {
        set_group_steps_for_comm(file_num, step_num, (uint8_t *)p, len);
    }
}

void m_data_loading_end_dispose(uint16_t cmd, uint16_t *data)
{
    app_flag.usb_loading_advance = 100;
}

void m_result_usb_dispose(uint16_t cmd, uint16_t *data)
{
    uint16_t res_num = *(data + 1);
    
    RESULT_STR t_res;
    RESULT_STRING_STR *res_str = (RESULT_STRING_STR *)data;
    
    app_flag.usb_con_st = *data;
    
    if(app_flag.usb_con_st != USB_CON_OK)
    {
        return;
    }
    
    if(res_num < 1)
    {
        return;
    }
    
    read_one_result(&t_res, res_num - 1);
    change_result_to_string((const RESULT_STR*)&t_res, res_str, res_num);
    
    app_flag.usb_loading_advance = res_num * 100 / sys_par.used_res_num;/* 计算进度 */
} 
void m_result_info_dispose(uint16_t cmd, uint16_t *data)
{
    RESULT_INFO_T *p = (RESULT_INFO_T *)data;
    
    p->used_res_num = sys_par.used_res_num;
    p->pass_res_num = sys_par.pass_res_num;
    p->res_str_size = sizeof(RESULT_STRING_STR);
}
void m_result_one_usb_dispose(uint16_t cmd, uint16_t *data)
{
}

void m_sys_par_len_usb_dispose(uint16_t cmd, uint16_t *data)
{
    SYS_DATA_LEN_T l_sys_data_len;
    
    l_sys_data_len.sys_par = sizeof(SYS_PAR);
    l_sys_data_len.sys_flag = sizeof(SYS_FLAG_T);
    l_sys_data_len.test_file = sizeof(TEST_FILE);
    l_sys_data_len.step = sizeof(NODE_STEP);
    l_sys_data_len.result = sizeof(RESULT_STRING_STR);
    
    l_sys_data_len.group_bitmap = get_step_used_flag_size();
    l_sys_data_len.group_map_table = get_group_table_size();
    
    memcpy(data, &l_sys_data_len, sizeof(l_sys_data_len));
}

void m_file_steps_num_usb_dispose(uint16_t cmd, uint16_t *data)
{
    uint8_t file_num = data[0];
    
    *data = file_table[file_num % MAX_FILES].total;
}

void m_get_tester_type_usb_dispose(uint16_t cmd, uint16_t *data)
{
    memset(data, 0, 1);
    strcat((char*)data, g_cur_type->name);
}

void m_usb_connect_status_dispose(uint16_t cmd, uint16_t *data)
{
    app_flag.usb_con_st = *data;
    *data = NO_ERR;
}

void m_cylinder_ctrl_status_dispose(uint16_t cmd, uint16_t *data)
{
    data[0] = cur_cylinder_ctrl_status;
}

void m_auto_cal_dispose(uint16_t cmd, uint16_t *data)
{
//     auto_cal_dispose_for_comm(data);
}

#include "module.h"
void m_get_interface_info_dispose(uint16_t cmd, uint16_t *data)
{
	memcpy(&interface_info, data, sizeof(interface_info));
}




/*************************************************/
//校准指令集
void m_cal_enter_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        data[0] = enter_auto_cal_ui_for_comm(data + 1);
    }
}

void m_cal_exit_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        data[0] = exit_auto_cal_ui_for_comm(data + 1);
    }
}

void m_cal_status_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_cal_status_for_comm(data + 1);
    }
    else
    {
    }
}

void m_cal_mode_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_cal_mode_for_comm(data + 1);
    }
    else
    {
    }
}

void m_cal_range_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_cal_range_for_comm(data + 1);
    }
    else
    {
    }
}

void m_cal_start_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        data[0] = auto_cal_start_for_comm(data + 1);
    }
}

void m_cal_stop_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        data[0] = auto_cal_stop_for_comm(data + 1);
    }
}

void m_cal_measure_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        data[0] = set_cal_measure_for_comm(data);
    }
}

void m_cal_points_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
        data[0] = get_cal_points_for_comm(data + 1);
    }
    else
    {
    }
}

void m_cal_load_dispose(uint16_t cmd, uint16_t *data)
{
    if(MB_READ_PAR == cmd)
    {
    }
    else
    {
        data[0] = load_cal_point_for_comm(data);
    }
}

/*************************************************/
