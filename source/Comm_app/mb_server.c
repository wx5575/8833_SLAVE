
#include    <string.h>
#include 	"serve_test.h"
#include    "app.h"
#include    "mb.h"
#include    "mb_m.h"
#include    "user_mb_app.h"
#include    "mbrtu.h"
#include    "mb_cmd_define.h"
#include    "mb_server.h"
#include    "mb_dc_module.h"



void user_read_write_mult_hold_reg_hook_m(uint8_t slove_num)
{
    uint16_t *frame = usMRegHoldBuf[slove_num];
    uint16_t mb_cmd_st = 0;
    uint16_t mb_cmd_2 = 0;
    uint16_t *data = NULL;
    
    mb_cmd_st = frame[0];
    mb_cmd_2 = frame[1];
    data = &frame[2];
    
    test_flag.mb_syn = 1;
    
//     LED_PASS++;
    
    
    switch(mb_cmd_st)
    {
        case M_DC_MODULE_CAL_DATA:
            m_dc_module_cal_data_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_DAC:
            m_dc_module_dac_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_START:
            m_dc_module_start_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_STOP:
            m_dc_module_stop_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_TEST_FETCH:
            m_dc_module_test_fetch_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_TEST_FETCH_SHORT:
            m_dc_module_test_fetch_short_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_VOL_GEAR:
            m_dc_module_vol_gear_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_TEST_METHOD:
            m_dc_module_test_method_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_INFO:
            m_dc_module_info_dispose(slove_num, mb_cmd_2, data);
            break;
        case M_DC_MODULE_SET_DATA:
            m_dc_module_set_data_dispose(slove_num, mb_cmd_2, data);
            break;
    }
}


void user_read_write_mult_hold_reg_hook_s(void)
{
    uint16_t mb_cmd_st;
    uint16_t mb_cmd_2;
    uint16_t *data = &usSRegHoldBuf[2];
    
    mb_cmd_st = *(uint16_t *)usSRegHoldBuf;
    mb_cmd_2 = usSRegHoldBuf[1];
    
    
    if(mb_cmd_st >= M_SYS_PAR_USB)
    {
        app_flag.leading_watch_dog = 0;
    }
    
    switch(mb_cmd_st)
    {
	case M_POWER_CHECKRES:/* 电源检测 */
        m_power_checkres_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COMM_ADDR:/*与上位机通信地址*/
        m_comm_addr_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COMM_CTRL_STATUS:/*控制状态*/
        m_comm_ctrl_status_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
	case M_CONTROL_TEST_BUTTON:/*设置测试状态*/
        m_control_test_button_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_DELETE_FILE:
        m_delete_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_QUERY_ACTIVE_FILE_NUM:/*查询当前文件的编号*/
        m_query_active_file_num_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_QUERY_ACTIVE_STEP_NUM:/*查询当前步骤编号*/
        m_query_active_step_num_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
	case M_CATALOG_FILE:/* 查询文件是否存在存在就返回其文件内容 */
        m_catalog_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_INSERT_STEP:
        m_insert_step_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_DELETE_STEP:
        m_delete_step_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_ACTIVE_STEP_FRONT:
        m_active_step_front_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_ACTIVE_STEP_BEHIND:
        m_active_step_behind_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_INTER_CHANGE_STEP:
        m_inter_change_step_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_UPDATA_STEP_MODE:
        m_updata_step_mode_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
	case M_SYSTEM_NRULE:
        m_system_nrule_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SYSTEM_TIME:
        m_system_time_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_PASS_WORD:
        m_pass_word_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_KEYKLOCK:
        m_keyklock_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_FAIL_CONTINUE:
        m_fail_continue_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SYS_LANGUAGE:
        m_sys_language_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_TEST_PORT:
        m_test_port_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESET_STOP:/* 复位暂停 */
        m_reset_stop_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_CHECK_HINT:/* 检验提示 */
        m_check_hint_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SELF_CHECK:
        m_self_check_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_TEST_SIGNAL:
        m_test_signal_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_PHV:
        m_phv_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_GFI:
        m_gfi_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_RECOVER:
        m_result_recover_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_SAVE:
        m_result_save_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RES_REMAIN_HINT:/* 余量提示 */
        m_res_remain_hint_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SYS_VOLUME:/* 系统音量 */
        m_sys_volume_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SYS_CONTRAST:/* 屏幕对比度 */
        m_sys_contrast_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_DUT_NAME:
        m_dut_name_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_FETCH:
        m_result_fetch_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_CLEAR:
        m_result_clear_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_FAIL:
        m_result_fail_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_PASS:
        m_result_pass_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_USED_NUM:
        m_result_used_num_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_REMAIN_NUM:
        m_result_remain_num_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_RESULT_TOTAL_NUM:
        m_result_total_num_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_TEST_PORTS:
        m_test_ports_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_IR_AUTO_SHIFT:
        m_ir_auto_shift_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
        
	case M_COM_STEPS_CONT:
        m_com_steps_cont_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_STEPS_PASS:
        m_com_steps_pass_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_DELAY_T:
        m_com_delay_t_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_CHANGE_T:
        m_com_change_t_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_INTER_T:
        m_com_inter_t_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_FALL_T:
        m_com_fall_t_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_TEST_T:
        m_com_test_t_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_RISE_T:
        m_com_rise_t_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_FREQUENCY:
        m_com_frequency_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_ARC:
        m_com_arc_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_CURRENT:
        m_com_current_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_CHARGE_CURRENT:
        m_com_charge_current_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_REAL_CURRENT:
        m_com_real_current_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_CUR_LOW:
        m_com_cur_low_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_CUR_HIGH:
        m_com_cur_high_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_OFFSET:
        m_com_cur_offset_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_CUR_GEAR:
        m_com_cur_gear_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_VOLTAGE:
        m_com_voltage_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COM_MODE:
        m_com_mode_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
	case M_BBD_OPEN_CHECK:
        m_bbd_open_check_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
        break;
	case M_BBD_SHORT_CHECK:
        m_bbd_short_check_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
        break;
    
	case M_SWAP_STEPS:
        m_swap_steps_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_MOVE_FORWARD:
        m_move_forward_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_MOVE_BACKWARD:
        m_move_backward_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_TEST_MODE:
        m_test_mode_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
    
	case M_TEST_FETCH_MSG:
        m_test_fetch_msg_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_GET_CAP:
        m_get_cap_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_GET_OFFSET:
        m_get_offset_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_PLC_STATUS:
        m_plc_status_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_TEST_STATUS:
        m_test_status_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_DEL_ALL_FILE:
        m_del_all_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_STEP_MSG:
        m_step_msg_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_FILE_MSG:
        m_file_msg_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
	case M_SET_ACTIVE_STEP:/* 源指令设置指定的测试步为当前步 */
        m_set_active_step_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SET_ACTIVE_FILE:
        m_set_active_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_SAVE_FILE:
        m_save_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_EDIT_FILE:
        m_edit_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_NEW_FILE:
        m_new_file_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_ID_MSG:
        m_id_msg_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;/* 测试仪标识查询 */
	case M_RST:
        m_rst_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;/* 测试仪复位指令 */
	case M_TESTER_INFO:
        m_tester_info_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COMM_BPS:
        m_comm_bps_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_COMM_SW_STATUS:
        m_comm_sw_status_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
	case M_PLC_SW_STATUS:
        m_plc_sw_status_dispose(mb_cmd_2, &usSRegHoldBuf[2]);
		break;
    
    
    
/********************************************************/
    case M_SYS_PAR_USB:    /* 系统参数文件 包含了1个记录 */
        app_flag.usb_loading_advance = 10;
        m_sys_par_usb_dispose(mb_cmd_2, data);
        break;
    case M_SYS_FLAG_USB:   /* 系统标记文件 包含了1个记录 */
        app_flag.usb_loading_advance = 20;
        m_sys_flag_usb_dispose(mb_cmd_2, data);
        break;
    case M_FILE_LIST_USB:  /* 文件表目录 包含31个子记录号 分别是 31个文件 */
        app_flag.usb_loading_advance = 30;
        m_file_list_usb_dispose(mb_cmd_2, data);
        break;
    
    case M_STEP_USED_FLAG_BASE:
        app_flag.usb_loading_advance = 35;
        m_step_used_flag_base_dispose(mb_cmd_2, data);
        break;
    case M_GROUP_TABLE_BASE:
        app_flag.usb_loading_advance = 55;
        m_group_table_base_dispose(mb_cmd_2, data);
        break;
    case M_MEM_GROP_BASE_USB:
        app_flag.usb_loading_advance = 85;
        m_mem_group_base_usb_dispose(mb_cmd_2, data);
        break;
    case M_DATA_LOADING_END:/* 数据传输结束 */
        m_data_loading_end_dispose(mb_cmd_2, data);
        break;
    
    
    case M_RESULT_USB:    /* 结果文件号包含n条记录 n为测试仪已存在的记录个数 */
        m_result_usb_dispose(mb_cmd_2, data);
        break;
    case M_RESULT_INFO:   /* 获取测试仪结果相关信息 */
        m_result_info_dispose(mb_cmd_2, data);
        break;
    
    case M_RESULT_ONE_USB: /* 实时获取结果信息使用 */
        m_result_one_usb_dispose(mb_cmd_2, data);
        break;
    
    case M_SYS_PAR_LEN_USB:
        m_sys_par_len_usb_dispose(mb_cmd_2, data);
        break;
    case M_FILE_STEPS_NUM_USB:
        m_file_steps_num_usb_dispose(mb_cmd_2, data);
        break;
    case M_GET_TESTER_TYPE_USB:
        m_get_tester_type_usb_dispose(mb_cmd_2, data);
        break;
    case M_USB_CONNECT_STATUS:
        m_usb_connect_status_dispose(mb_cmd_2, data);
        break;
    case M_CYLINDER_CTRL_STATUS://;
        m_cylinder_ctrl_status_dispose(mb_cmd_2, data);
        break;
    
    
    /*******************AUTO_CAL***********************/
    case M_AUTO_CAL:
        m_auto_cal_dispose(mb_cmd_2, data);
        break;
	
	case M_GET_INTERFACE_INFO:
        m_get_interface_info_dispose(mb_cmd_2, data);
		break;
	/*************************************************/
	//校准指令集
	case M_CAL_ENTER:
        m_cal_enter_dispose(mb_cmd_2, data);
		break;
	case M_CAL_EXIT:
        m_cal_exit_dispose(mb_cmd_2, data);
		break;
	case M_CAL_STATUS:
        m_cal_status_dispose(mb_cmd_2, data);
		break;
	case M_CAL_MODE:
        m_cal_mode_dispose(mb_cmd_2, data);
		break;
	case M_CAL_RANGE:
        m_cal_range_dispose(mb_cmd_2, data);
		break;
	case M_CAL_START:
        m_cal_start_dispose(mb_cmd_2, data);
		break;
	case M_CAL_STOP:
        m_cal_stop_dispose(mb_cmd_2, data);
		break;
	case M_CAL_MEASURE:
        m_cal_measure_dispose(mb_cmd_2, data);
		break;
	case M_CAL_POINTS:
        m_cal_points_dispose(mb_cmd_2, data);
		break;
	case M_CAL_LOAD:
        m_cal_load_dispose(mb_cmd_2, data);
		break;
	/*************************************************/
    }
}


/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE*******************/

