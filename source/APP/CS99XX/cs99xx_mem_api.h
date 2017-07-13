/*
 * Copyright(c) 2015,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_mem_api.h
 * 摘  要  ：为外部存储管理提供接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CS99XX_MEM_API_H__
#define __CS99XX_MEM_API_H__


#include 	"cs99xx_eep_manage.h"
#include    "cs99xx_flash_manage.h"
#include    "on_chip_flash_api.h"

/******************* 全局变量声明 *********************/
#ifdef   _99xxMEM_GLOBALS
#define  _99xxMEM_EXT
#else
#define  _99xxMEM_EXT  extern
#endif

#define     TABLE_VALUE_NULL                ((uint16_t)0xffff)

_99xxMEM_EXT uint8_t step_used_flag[MAX_STEPS/8 + 1 + 1];/* 记录flash中自然步空间使用情况 + 2为了确保通信时数据不丢失 因为2字节对齐的 */
_99xxMEM_EXT uint16_t cur_group_table[MAX_STEPS];/* 当前记忆组步映射表 */

/***********************************************************/

extern uint32_t get_step_used_flag_size(void);
extern uint32_t get_group_table_size(void);

extern void save_cur_step(void);
extern void save_list(void);

extern void save_init_group(const uint8_t n, NODE_STEP *p_node);
extern void copy_cur_file_to_new_pos(const int8_t n);

extern int32_t read_list(const int8_t m);
extern int32_t read_ratio(void);

extern void save_group_info(const uint8_t file_num);

extern void init_par(void);
extern int32_t read_paramenter(void);

extern void save_one_result_list_dis(RESULT_STR *res, uint16_t n);
extern void save_one_result(RESULT_STR *res, uint16_t n);
extern void read_one_result(RESULT_STR *res, uint16_t n);
extern void read_n_results(RESULT_STR *res, uint32_t num, uint16_t n);
extern void read_result_list_dis_one_page(RESULT_STR *res, uint16_t page, uint16_t cur_page_num);
extern void read_result_list_dis_one(RESULT_STR *res, uint16_t n);

extern int32_t save_file(const uint8_t n);
extern void save_all_file(void);

extern void read_file(const uint8_t n);
extern void read_all_file(void);

extern int32_t save_sys_flag(void);
extern void read_sys_flag(void);

extern int32_t save_sys_par(void);
extern void read_sys_par(void);

extern void save_ratio(const int8_t mode);

extern void init_group_step_used_flag(void);
extern void init_file_step_table(void);

extern uint8_t get_first_step_mode(void);
extern void load_steps_to_list(const int16_t step);

extern void clear_step_used_flag(void);
extern void clear_group_table(void);

extern void save_step_used_flag(const uint8_t n);
extern void save_group_table(const uint8_t n);

extern void read_step_used_flag(const uint8_t n);
extern void read_group_table(const uint8_t n);

extern void save_one_step(NODE_STEP *node, const uint16_t step);
extern void read_one_step(NODE_STEP *node, const uint16_t step);

extern uint16_t get_cur_upper(uint8_t mode, uint8_t gear);
extern int32_t check_acw_par(UN_STRUCT *p);
extern int32_t check_dcw_par(UN_STRUCT *p);
extern int32_t check_ir_par(UN_STRUCT *p);
extern int32_t check_gr_par(UN_STRUCT *p);
extern int32_t check_bbd_par(UN_STRUCT *p);
extern int32_t check_sys_par(void);
extern int32_t check_test_step_data(NODE_STEP *p, uint16_t size);
extern int32_t check_save_step_data(NODE_STEP *p);

#endif //__CS99XX_MEM_API_H__
/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE*******************/
