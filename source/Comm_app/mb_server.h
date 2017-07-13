
#ifndef __MB_SERVER_H__
#define __MB_SERVER_H__


#include    "mb.h"
#include    "mb_m.h"
#include    "user_mb_app.h"

#define USB_PACKAGE_SIZE      100 /* U盘数据导入导出包的大小 */


enum{
	
    R_TIME, /* 上升时间 */
    T_TIME, /* 测试时间 */
    F_TIME, /* 下降时间 */
    D_TIME, /* 延时时间 */
    I_TIME, /* 间隔时间 */
    C_TIME = I_TIME,/* 设置缓变时间 同间隔时间共用变量 */
	
/* 系统参数 */
	SYS_CONTRAST = 0,/* 液晶对比度 */
	SYS_VOLUME, /* 系统音量 */
	SYS_RES_ALLOWANCE_HINT,/* 余量提示 */
	SYS_RES_SAVE,/* 结果保存 */
	SYS_RES_COVER,/* 结果溢出覆盖 */
	SYS_GFI,/* GFI保护开关 */
	SYS_PHV,/* 预防高压 */
	SYS_TEST_SIGNAL,/* 测试信号 */
	SYS_SELF_CHECK,/* 自检允许 */
	SYS_CHECK_HINT,/* 检验提示 */
	SYS_TEST_PORT,/* 测试端口 */
	SYS_LANGUAGE,/* 语言 */
	SYS_FAIL_CONT,/* 失败继续 */
	SYS_KEY_LOCK,/* 键盘锁 */
	SYS_NUM_RULER,/* 编号规则 */
    
    
    RES_USED_NUM=0,
    RES_FREE_NUM,
    RES_TOTAL_NUM,
    RES_PASS_NUM,
    RES_FAIL_NUM,
};



extern USHORT   usSRegHoldBuf[S_REG_HOLDING_NREGS];
extern USHORT   usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];

extern void user_read_write_mult_hold_reg_hook_m(uint8_t slove_num);
extern void user_read_write_mult_hold_reg_hook_s(void);

extern void m_power_checkres_dispose(uint16_t cmd, uint16_t *data); /* 电源检测 */

extern void m_comm_addr_dispose(uint16_t cmd, uint16_t *data);       /*与上位机通信地址*/ 
extern void m_comm_ctrl_status_dispose(uint16_t cmd, uint16_t *data);       /*控制状态*/

extern void m_control_test_button_dispose(uint16_t cmd, uint16_t *data);       /*设置测试状态*/
extern void m_delete_file_dispose(uint16_t cmd, uint16_t *data);       /*删除文件，参数为0删除所有文件，不为0删除单个文件*/
extern void m_query_active_file_num_dispose(uint16_t cmd, uint16_t *data);       /*读取制定文件为活动文件*/  
extern void m_query_active_step_num_dispose(uint16_t cmd, uint16_t *data);       /*读取当前步骤为活动步骤*/
extern void m_catalog_file_dispose(uint16_t cmd, uint16_t *data);       /*查询文件是否存在*/ 
extern void m_insert_step_dispose(uint16_t cmd, uint16_t *data);       /*插入测试步骤*/    
extern void m_delete_step_dispose(uint16_t cmd, uint16_t *data);       /*删除当前活动步骤*/
extern void m_active_step_front_dispose(uint16_t cmd, uint16_t *data);       /*当前活动步骤前移一位*/  
extern void m_active_step_behind_dispose(uint16_t cmd, uint16_t *data);       /*当前活动步骤后移一位*/
extern void m_inter_change_step_dispose(uint16_t cmd, uint16_t *data);       /*交换步骤信息*/
extern void m_updata_step_mode_dispose(uint16_t cmd, uint16_t *data);       /*修改当前活动文件的模式*/

extern void m_system_nrule_dispose(uint16_t cmd, uint16_t *data);
extern void m_system_time_dispose(uint16_t cmd, uint16_t *data);
extern void m_pass_word_dispose(uint16_t cmd, uint16_t *data);
extern void m_keyklock_dispose(uint16_t cmd, uint16_t *data);
extern void m_fail_continue_dispose(uint16_t cmd, uint16_t *data);
extern void m_sys_language_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_port_dispose(uint16_t cmd, uint16_t *data);
extern void m_reset_stop_dispose(uint16_t cmd, uint16_t *data);/* 复位暂停 */
extern void m_check_hint_dispose(uint16_t cmd, uint16_t *data); /* 检验提示 */
extern void m_self_check_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_signal_dispose(uint16_t cmd, uint16_t *data);
extern void m_phv_dispose(uint16_t cmd, uint16_t *data);
extern void m_gfi_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_recover_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_save_dispose(uint16_t cmd, uint16_t *data);
extern void m_res_remain_hint_dispose(uint16_t cmd, uint16_t *data); /* 余量提示 */
extern void m_sys_volume_dispose(uint16_t cmd, uint16_t *data);/* 系统音量 */
extern void m_sys_contrast_dispose(uint16_t cmd, uint16_t *data);/* 屏幕对比度 */
extern void m_dut_name_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_fetch_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_clear_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_fail_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_pass_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_used_num_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_remain_num_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_total_num_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_ports_dispose(uint16_t cmd, uint16_t *data);
extern void m_ir_auto_shift_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_steps_cont_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_steps_pass_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_delay_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_change_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_inter_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_fall_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_test_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_rise_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_frequency_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_arc_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_current_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_charge_current_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_real_current_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_offset_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_low_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_high_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_gear_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_voltage_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_mode_dispose(uint16_t cmd, uint16_t *data);
extern void m_bbd_open_check_dispose(uint16_t cmd, uint16_t *data);
extern void m_bbd_short_check_dispose(uint16_t cmd, uint16_t *data);
extern void m_swap_steps_dispose(uint16_t cmd, uint16_t *data);
extern void m_move_forward_dispose(uint16_t cmd, uint16_t *data);
extern void m_move_backward_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_mode_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_fetch_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_cap_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_offset_dispose(uint16_t cmd, uint16_t *data);
extern void m_plc_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_del_all_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_step_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_file_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_set_active_step_dispose(uint16_t cmd, uint16_t *data);
extern void m_set_active_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_save_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_edit_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_new_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_id_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_rst_dispose(uint16_t cmd, uint16_t *data);
extern void m_tester_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_comm_bps_dispose(uint16_t cmd, uint16_t *data);
extern void m_comm_sw_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_plc_sw_status_dispose(uint16_t cmd, uint16_t *data);









extern void m_sys_par_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_sys_flag_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_file_list_usb_dispose(uint16_t cmd, uint16_t *data);
// extern void m_map_table_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_step_used_flag_base_dispose(uint16_t cmd, uint16_t *data);
// extern void m_step_used_flag_end_dispose(uint16_t cmd, uint16_t *data);
extern void m_group_table_base_dispose(uint16_t cmd, uint16_t *data);
// extern void m_group_table_end_dispose(uint16_t cmd, uint16_t *data);
// extern void m_group_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_mem_group_base_usb_dispose(uint16_t cmd, uint16_t *data);
// extern void m_mem_grop_end_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_data_loading_end_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_one_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_sys_par_len_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_file_steps_num_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_tester_type_usb_dispose(uint16_t cmd, uint16_t *data);


extern void m_usb_connect_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_cylinder_ctrl_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_auto_cal_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_interface_info_dispose(uint16_t cmd, uint16_t *data);



extern void m_cal_enter_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_exit_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_mode_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_range_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_start_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_stop_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_measure_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_points_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_load_dispose(uint16_t cmd, uint16_t *data);

#endif //__MB_SERVER_H__
