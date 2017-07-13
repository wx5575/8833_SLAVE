/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：app.c
 * 摘  要  ：用户的应用程序入口，初始化软硬件，建立所需的任务
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __CS99XX_FLASH_MANAGE_H__
#define __CS99XX_FLASH_MANAGE_H__


#include "stm32f10x.h"
#include "cs99xx_struct.h"

#pragma once
/********************** Flash分配 **************************/
// #define F_PAGE_SIZE             (1024 * 4)	/* Flash 一页的大小 */

/**************************************/
#define 	F_RATIO_ACW_BASE				((uint16_t)0x0)         /* 存放ACW校准系数 */
#define		F_RATIO_DCW_BASE				((uint16_t)1*1024)      /* 存放DCW校准系数 */
#define		F_RATIO_GR_BASE					((uint16_t)3*1024)      /* 存放GR校准系数 */

#define		F_RATIO_IR_BASE					((uint16_t)4*1024)      /* 存放IR校准系数 3KB */
#define		F_RATIO_BBD_BASE				((uint16_t)8*1024)      /* 存放BBD校准系数 */
/*******************************/

#define F_GROUP_BASE		    (1024 * 1024)	/* 记忆组基址 1M */
#define F_GROUP_OFFSET          (F_UN_SIZE * MAX_STEPS)	/* 16K */ 


#define F_UN_SIZE       ((uint16_t)sizeof(UN_STRUCT))

#define F_STEP_OFFSET           (F_UN_SIZE)	/* 大概48个字节 */

#define	F_FILE_BASE             (F_GROUP_BASE - F_FILE_TOTAL_OFFSET)	/* 文件信息存储基址 空间64k 大概存1400个文件 */
#define F_FILE_OFFSET		    (sizeof(TEST_FILE))	/* 大概44个字节 */
// #define F_PER_P_FILES		    (F_PAGE_SIZE / F_FILE_OFFSET)	/* 每4k能放的文件数目 大概 93个文件 */
#define F_FILE_TOTAL_OFFSET     (F_FILE_OFFSET * MAX_FILES) /*  */


#define F_FILE_USED_STEP_FLAG_OFFSET        ((sizeof step_used_flag))

#define F_FILE_USED_FLAG_TOTAL_OFFSET       (MAX_FILES * F_FILE_USED_STEP_FLAG_OFFSET)
#define F_FILE_USED_STEP_FLAG_BASE          (F_FILE_STEP_TABLE_BASE - F_FILE_USED_FLAG_TOTAL_OFFSET)


#define F_GROP_TABLE_SIZE                   F_FILE_STEP_TABLE_OFFSET
#define F_FILE_STEP_TABLE_OFFSET            (2 * MAX_STEPS) /* 每一步占两个字节 */

#define F_FILE_TABLE_TOTAL_OFFSET           (MAX_FILES * F_FILE_STEP_TABLE_OFFSET) /* 所有表占的总页数 */
#define F_FILE_STEP_TABLE_BASE              (F_FILE_BASE - 0 - F_FILE_TABLE_TOTAL_OFFSET)

#define F_SYS_FLAG_BASE         (1*1024)	/* 存放标志 信息 */
#define F_SYS_FLAG_SIZE         (sizeof(SYS_FLAG))

#define F_SYS_PAR_BASE          (2*1024) /* 存放系统参数 占37个字节(可能会有变更)  */
#define F_SYS_PAR_SIZE          (sizeof(SYS_PAR))


// #define F_PAGE_OFFSET(n, per_page_num)          (F_PAGE_SIZE * ((n) / (per_page_num)))  /* 计算页偏移 */
// #define F_PAGE_INNER_OFFSET(n, per_page_num, size)    ((size) * ((n) % (per_page_num))) /* 计算页内偏移 */


/* CHIP2 放测试结果 */
/*结果列表显示使用 */
#define F_RESULT_LIST_DIS_BASE          (0)/* 存储测试结果基址0M */
#define F_RESULT_LIST_DIS_TOTAL         (F_RESULT_SIZE * MAX_STEPS)

/* 系统结果使用 */
#define F_RESULT_BASE					(F_RESULT_LIST_DIS_BASE + F_RESULT_LIST_DIS_TOTAL)
#define F_RESULT_OFFSET					((uint16_t)sizeof(RESULT_STR))   /* 结果偏移值 */
#define F_RESULT_SIZE                   (F_RESULT_OFFSET)

#define	F_RESULT_TOTAL_SIZE				(F_RES_TOTAL * F_PAGE_SIZE)

#define	F_RES_TOTAL		                ((uint16_t)8000)
#define	F_RES_TOTAL_DZ	                ((uint16_t)20000)

/***********************************************************/
extern void save_ratio_from_flash(uint8_t mode);
extern int32_t read_ratio_from_flash(void);
extern void save_step_flash(void);
extern void save_list_flash(void);
extern void copy_cur_file_to_new_pos_flash(const int8_t n);
extern int32_t save_file_flash(const int8_t n);
extern void save_all_file_flash(void);
extern void read_file_flash(const int8_t n);
extern void read_all_file_flash(void);
extern int32_t save_sys_flag_flash(void);
extern void read_sys_flag_flash(void);
extern int32_t save_sys_par_flash(void);
extern void read_sys_par_flash(void);
extern void save_init_group_flash(const uint8_t n, NODE_STEP *p_node);
extern void save_as_cur_group_flash(const uint8_t file_num);
extern int32_t init_group_step_used_flag_flash(void);
extern int32_t init_file_step_table_flash(void);
extern void save_step_used_flag_flash(uint8_t n);
extern void read_step_used_flag_flash(uint8_t n);
extern void save_group_table_flash(const uint8_t n);
extern void read_group_table_flash(const uint8_t n);
extern uint8_t get_first_step_mode_flash(void);
extern void load_steps_to_list_flash(const int16_t step);
extern void save_one_step_flash(NODE_STEP *node, const uint8_t file_num, const uint16_t step);
extern void read_one_step_flash(NODE_STEP *node, const uint8_t file_num, const uint16_t step);
extern void save_one_result_list_dis_flash(RESULT_STR *res, uint16_t n);
extern void save_one_result_flash(RESULT_STR *res, const uint16_t n);
extern void read_one_result_flash(RESULT_STR *res, const uint16_t n);
extern void read_result_list_dis_one_page_flash(RESULT_STR *res, uint16_t page, uint16_t cur_page_num);
extern void read_result_list_dis_one_flash(RESULT_STR *res, uint16_t n);

#endif //__CS99XX_FLASH_MANAGE_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
