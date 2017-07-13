
#ifndef __DC_MODULE_H__
#define __DC_MODULE_H__

#include "module.h"

#define DC_MODULE_NUM       2
#define DC_MODULE_NUM_MAX   15

typedef struct{
    uint16_t da;        /* 输出DA */
    uint16_t cur_ad;    /* 电流AD */
    uint16_t vol_ad;    /* 电压AD */
    uint8_t status;     /* 测试状态 */
    float cur;          /* 电流值 */
    float vol;          /* 电压值 */
}MODULE_DATA;

typedef struct{
    uint16_t cur;/* 电流值 */
    uint16_t vol;/* 电压值 */
}MODULE_DATA_SHORT;

typedef struct{
    uint16_t da;        /* 输出DA */
    uint8_t gear;       /* 电压档位 */
    uint8_t method;     /* 测试方法 */
}DC_GR_PARAMETER;

typedef struct{
    MODULE_INFO id;     /* 模块ID信息 */
    MODULE_DATA data;   /* 模块数据信息 */
    MODULE_DATA_SHORT short_data;   /* 模块数据信息 短数据*/
    DC_GR_PARAMETER par;/* 模块设置参数 */
    uint8_t st;         /* 状态 */
    uint8_t en;         /* 使能 */
    uint8_t sel;        /* 选择标志用于单独工作模式和协调工作模式 */
    uint8_t usable;     /* 数据可用标志 */
}DC_GR_MASTER_INFO;


typedef enum {
    DC_MODULE_CMD_START,        /* 启动命令 */
    DC_MODULE_CMD_STOP,         /* 停止命令 */
    DC_MODULE_CMD_FETCH,        /* 查询数据命令 */
    DC_MODULE_CMD_METHOD,       /* 设置测试方法 */
    DC_MODULE_CMD_GEAR,         /* 设置档位 */
    DC_MODULE_CMD_DC,           /* 设置DA输出值 */
    DC_MODULE_CMD_INFO,         /* 获取模块信息 */
    DC_MODULE_CMD_SET_CAL_DATA, /* 设置校准数据 */
    DC_MODULE_CMD_GET_CAL_DATA, /* 获取校准数据 */
    DC_MODULE_CMD_SCAN_MODULES, /* 扫描模块 */
    DC_MODULE_CMD_SET_DATA,     /* 设置参数 */
    DC_MODULE_CMD_FETCH_SHORT,  /* 查询数据命令短指令 */
}DC_MODULE_CMD_T;

typedef enum{
    DC_MODULE_ST_STOPING,//停止测试
    DC_MODULE_ST_TESTING,//正在测试
    DC_MODULE_ST_OPENERR,//开路报警
    DC_MODULE_ST_AMPERR,//功放报警
}DC_MODULE_ST;

enum{
    DC_GR_SEL_ONE,/* 选择一个模块 */
    DC_GR_SEL_ALL,/* 选择所有的模块 */
};

#ifdef   DC_GR_GLOBALS
#define  DC_GR_EXT
#else
#define  DC_GR_EXT  extern
#endif

DC_GR_EXT   DC_GR_MASTER_INFO       dc_gr_master_info[DC_MODULE_NUM_MAX + 1];

extern uint8_t get_dc_module_using_num(void);
extern int32_t get_dc_gr_ad(uint8_t slave_addr, uint16_t *cur_da, uint16_t *vol_da);
extern uint8_t get_cur_cal_dc_module_addr(void);
extern void set_cur_cal_dc_module_addr(uint8_t addr);
extern void set_dc_gr_output_da(uint8_t slove_num, uint16_t da_value);
extern uint32_t get_dc_gr_module_id(uint8_t slave_num);
extern int32_t slelect_cal_dc_gr_module(uint8_t *n);
extern int32_t find_dc_gr_map_ratio_n(uint8_t slove_num, uint8_t *map_r_n);
extern void get_dc_gr_module_inf(MODULE_INFO *pool, uint32_t *count);
extern void select_dc_gr_moduls(uint8_t type, uint8_t slave_addr);
extern void select_dc_gr_n_moduls(uint8_t n);
extern void send_dc_module_cmd(DC_MODULE_CMD_T cmd, uint16_t value);
extern int32_t ask_dc_gr_start_ok(void);
extern void clear_dc_gr_st(void);
extern void check_dc_gr_moduls(void);
extern void get_dc_gr_cal_info(void);
extern void set_dc_gr_cal_info(void);
extern int32_t get_dc_gr_data(float *cur, float *vol);
extern void clear_dc_gr_data(void);
extern void set_dc_gr_vol_gear(uint8_t gear);
extern void set_dc_gr_test_method(uint8_t method);
#endif //__DC_MODULE_H__


/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

