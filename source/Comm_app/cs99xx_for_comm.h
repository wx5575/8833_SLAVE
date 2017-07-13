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
 
#ifndef __CS99XX_FOR_COMM_H__
#define __CS99XX_FOR_COMM_H__

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


/******************** 函数定义 ************************/

extern uint8_t get_cur_gear_par_form(uint8_t mode);
extern uint8_t get_vol_gear_par_form(uint8_t mode);
extern uint16_t check_step_exist(const uint16_t n);
extern void init_file_group(const uint8_t n);
extern int16_t app_save_file_for_comm(FILE_MSG * file);
extern int16_t app_edit_file_for_comm(FILE_MSG * file);
extern int16_t app_new_file_for_comm(FILE_MSG * file);
extern int16_t del_file_for_comm(const uint8_t file_num);
extern int16_t del_all_file_for_comm(void);
extern int16_t get_idn_msg_for_comm(IDN_MSG * idn_msg);
extern int16_t tester_reset_for_comm(void);
extern int16_t inster_one_step_for_comm(const uint8_t test_mode);
extern int16_t del_cur_step_for_comm(void);
extern int16_t advance_cur_step_for_comm(void);
extern int16_t retreat_cur_step_for_comm(void);
extern int16_t swap_two_step_for_comm(const uint16_t step);
int16_t set_bbd_open_check_for_comm(const uint16_t test_mode, uint16_t open_ratio);
int16_t get_bbd_open_check_for_comm(uint16_t *data);
int16_t set_bbd_short_check_for_comm(const uint16_t test_mode, uint16_t short_ratio);
int16_t get_bbd_short_check_for_comm(uint16_t *data);
extern int16_t set_mode_for_comm(const uint16_t test_mode);
extern int16_t get_mode_for_comm(void);
extern int16_t set_vol_for_comm(const uint16_t voltage, const uint8_t test_mode);
extern int16_t get_vol_for_comm(uint16_t *data, const uint8_t test_mode);
extern int16_t set_gear_for_comm(const uint16_t range, const uint8_t test_mode);
extern int16_t get_gear_for_comm(const uint8_t test_mode);
extern int16_t set_high_for_comm(uint16_t high_data, const uint8_t test_mode);
extern int32_t get_high_for_comm(uint16_t *data, uint8_t test_mode);
extern int16_t set_low_for_comm(uint16_t low_data, const uint8_t test_mode);
extern int16_t get_offset_value_for_comm(uint16_t *offset_data, const uint8_t test_mode);
extern int16_t set_offset_value_for_comm(uint16_t offset_data, const uint8_t test_mode);
extern int32_t get_low_for_comm(uint16_t *data, uint8_t test_mode);
extern int16_t set_real_cur_for_comm(uint16_t real_cur);
extern int16_t get_real_cur_for_comm(uint16_t *data);
extern int16_t set_charge_cur_for_comm(uint16_t charge_cur, const uint8_t test_mode);
extern int16_t get_charge_cur_for_comm(uint16_t *data);
extern int16_t set_arc_for_comm(const uint16_t current, const uint8_t test_mode);
extern int16_t get_arc_for_comm(uint16_t *data);
extern int16_t set_hz_for_comm(const uint16_t frequency, const uint8_t test_mode);
extern int16_t get_hz_for_comm(uint16_t *data);
extern int16_t get_hz_for_comm_f(uint16_t *data);
extern int16_t set_pass_signal_for_comm(const uint16_t Psignal, const uint8_t test_mode);
extern int16_t get_pass_signal_for_comm(const uint8_t test_mode);
extern int16_t set_steps_cont_for_comm(const uint16_t cnext, const uint8_t test_mode);
extern int16_t get_steps_cont_for_comm(const uint8_t test_mode);
extern int16_t set_port_for_comm(const TEST_PORT port_data, const uint8_t test_mode);
extern int16_t get_port_for_comm(const uint8_t test_mode);
extern void clear_all_res_for_comm(void);
extern int16_t get_result_info_for_comm(uint8_t info);
extern int16_t set_test_port_for_comm(uint16_t* data);
extern int16_t get_test_port_for_comm(uint16_t* data);
extern int16_t set_ir_auto_for_comm(const uint16_t arange_data);
extern int16_t get_ir_auto_for_comm(uint16_t *data);
extern int16_t set_timer_for_comm(const uint16_t time_data, const uint8_t test_mode, const uint8_t whitch);
extern int16_t get_timer_for_comm(const uint8_t test_mode, const uint8_t whitch);
extern int16_t set_sys_par_for_comm(const uint16_t data, const uint8_t whitch);
extern int16_t get_sys_par_for_comm(uint16_t *data, const uint8_t whitch);
extern int16_t set_sys_time_for_comm(SYS_TIME * system_time);
extern void get_sys_time_for_comm(SYS_TIME * system_time);
extern int16_t set_dut_name_for_comm(const char* name);
extern int16_t get_dut_name_for_comm(char* name);
extern uint16_t get_cap_for_comm(uint16_t data);
extern uint16_t get_offset_for_comm(uint16_t data);
extern void get_test_fetch_for_comm(char* data);
extern uint16_t get_plc_relay_status_for_comm(void);
extern int16_t get_testing_status_for_comm(void);
extern int16_t get_result_fetch_for_comm(uint16_t *data, uint16_t result_num);
extern int16_t get_cur_result_fetch_for_comm(uint16_t *data, uint16_t result_num);
extern uint16_t get_output_freq(uint8_t mode);
extern void test_ports_f(TEST_PORT *p, uint8_t *buf);
extern void get_active_step_msg_for_comm(char *data);
extern void get_active_file_msg_for_comm(FILE_MSG *p_file);
extern void get_appoint_file_msg_for_comm(CATALOG_FILE *p_file, uint16_t file_num);
extern int16_t active_appoint_file_for_comm(uint16_t file_num);
extern int16_t active_appoint_step_for_comm(uint16_t step_num);
extern int16_t set_all_sys_par_for_comm(SYS_PAR * p_sys_par);
extern int16_t set_sys_flag_for_comm(SYS_FLAG * p_sys_flag);
extern int16_t set_step_used_flag_for_comm(uint16_t pack_num, uint8_t * data, uint16_t size);
extern int16_t set_group_table_for_comm(uint16_t pack_num, uint8_t * data, uint16_t size);
extern int16_t set_group_steps_for_comm(uint16_t file_num, uint16_t note_num, uint8_t * data, uint16_t size);


extern uint16_t enter_auto_cal_ui_for_comm(uint16_t *data);
extern uint16_t exit_auto_cal_ui_for_comm(uint16_t *data);
extern uint16_t get_cal_status_for_comm(uint16_t *data);
extern uint16_t get_cal_mode_for_comm(uint16_t *data);
extern uint16_t get_cal_range_for_comm(uint16_t *data);
extern uint16_t auto_cal_start_for_comm(uint16_t *data);
extern uint16_t auto_cal_stop_for_comm(uint16_t *data);
extern uint16_t set_cal_measure_for_comm(uint16_t *data);
extern uint16_t get_cal_points_for_comm(uint16_t *data);
extern uint16_t load_cal_point_for_comm(uint16_t *data);

#endif
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
