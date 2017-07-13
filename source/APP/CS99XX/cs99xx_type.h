/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_type.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __TYPE_SELECT__
#define __TYPE_SELECT__
#include <stm32f10x.h>
#include "cs99xx_config.h"


/* 仪器型号与硬件保持一致 0x00表示该型号未启用 */
enum{
DEBUG_TYPE, /* 该型号是调试使用的正常生产不存在 */

CS9919AX,
CS9919BX,
CS9929AX,
CS9929BX,
CS9922BX,

CS9922CX,
CS9933X,
CS9912AX,
CS9912BX,
CS9913AX,

CS9913BX,
CS9950X,
CS9950CX,
CS9950DX,
CS9922DX,

CS9922EX,
CS9914AX,
CS9914BX,
CS9929CX,
CS9922FX,

CS9921BX,
CS9912Y,
CS9914Y,
CS9922CX_1,
CS9929EX,

CS9919G,
CS9922G,
CS9922G_1,
CS9933G,
CS9950CG,

CS9950CG_1,
CS9914G,
CS9919A,
CS9919B,
CS9929A,

CS9929B,
CS9922M,
CS9915AX,
CS9916AX,
CS9916BX,

CS9917AX,
CS9917BX,
CS9920A,
CS9920B,
CS9923 ,

CS9923A,
CS9933EG,
CS9922H_50,
CS9922H_25,
CS9914CX,

CS9914DX,
CS9914AT_10,/* 未使用的型号 */
CS9906B,
CS9929A_8,
CS9919A_8,

CS9917B,
CS9906B_K,
CS9906A,
CS9933ET,
CS9929A_4,
CS9917CX_20,
CS9924_200,
CS9914AM_H,
CS9917A_8,

CS9919B_4,
CS9929B_4,
CS9933G_4,
CS9923G,
CS9939X,
CS9933EG_1,
CS9914AM,
CS9914BM,
CS9917CX,
CS9917DX,
CS9929AX_T,
CS9929AX_F,

CS9922BX_DZ,
CS9933ET_1,
CS9922BX_1,
CS9923_5,
CS9922EXJ,
CS9914AX_1,
CS9922BXJ,
CS9912BX_T,
CS9933EG_2,
CS9923G_1,
CS9933X_100,
CS9924,
CS9922LB,
CS9917CX_C,
CS9912BX_L,/* 没有生产的临时备用与CS9912BX + BBD */
VTC9929,/* 定制机 */
TSK7630,/* 定制机 */
CS9922G_2,
CS2676CH,
CS9914D,
CS9929AX_8,
CS9919D,
CS9933G_1,
CS9933ET_2,
CS9933ET_3,



///////////////
CS9923ECG,
CS9923G_2,
CS9923G_3,

CS9923ECG_1,
CS9950ED,
CS9929EX_CC,
CS9914CX_C,
CS9927LB,
CS2676CH_1,
CS9933G_,
CS9923CG,
CS9929AX_CC,
CS9930A_T,

CS99XX_T_END,
};

/* 模式选择 */
#define __ACW		0X02
#define __DCW		0X04
#define __IR		0X08
#define __GR		0X10
#define __CC		0X20
#define __BBD		0X40

#define MODEL_EN		(type_spe.mode_en)



/* 硬件开关 */
#define BUZZER_EN	g_custom_sys_par.buzzer_en	/* 蜂鸣器使能 */
#define MUTE_EN     g_custom_sys_par.mute_sw   /* 系统静音模式开关 */


#define OFFSET_ON	0
#define OFFSET_OFF	1

/* 机型的端口数 */
typedef enum{
    PORT_NUM0	= 0,
    PORT_NUM4	= 4,
    PORT_NUM8	= 8,
    PORT_NUM14	= 14,
}PORT_NUM_TYPE;

typedef enum{
	HZ_TYPE_GRADE = 1,/* 等级模式 */
	HZ_TYPE_CONT  = 0,/* 连续模式 */
}FREQ_TYPE;/* 频率类型 */

/* 小数位数 */
enum{
	POINT_0 = 0,/* 0位小数 */
	POINT_1 = 1,
	POINT_2 = 2,
	POINT_3 = 3,
	POINT_4 = 4,
};
#define ACW_TRANSFORMER_GROUP_1     case ACW_500V: \
                                    case ACW_1kV: \
                                    case ACW_5kV
#define ACW_TRANSFORMER_GROUP_2     case ACW_10kV
#define ACW_TRANSFORMER_GROUP_3     case ACW_20kV
/* 电压输出量程 */
typedef enum{
	ACW_500V    = 0,/* 0.5kV */
	ACW_1kV		= 1,/* 1kV */
	ACW_5kV		= 2,/* 5kV */
	ACW_10kV	= 3,/* 10kV 以下 */
	ACW_20kV	= 4,/* 20KV 以下 */
	ACW_30kV	= 5,/* 20KV 以下 */
}ACW_VOL_RANGE_ENUM;
#define DCW_TRANSFORMER_GROUP_1     case DCW_2kV: \
                                    case DCW_6kV
#define DCW_TRANSFORMER_GROUP_2     case DCW_10kV
#define DCW_TRANSFORMER_GROUP_3     case DCW_20kV
typedef enum{
	DCW_500V,
	DCW_2kV,
	DCW_6kV,
	DCW_10kV,
	DCW_20kV,
}DCW_VOL_RANGE_ENUM;
typedef enum{
	IR_750V,
	IR_1kV,
	IR_1_5kV,
	IR_2_5kV,/* 2.5kV */
	IR_5kV,
	IR_10kV,
}IR_VOL_RANGE_ENUM;
/* 直流GR 压降测试仪有电压档 */
typedef enum{
	GR_NORMAL 		 = 0X0,
	GR_DC_20mV		 = 0x2,
	GR_DC_200mV		 = 0x4,
	GR_DC_2000mV	 = 0x8,
	GR_DC_12V		 = 0x10,
	GR_DC_12V_100mA  = 0x20,
	GR_DC_12V_1000mA = 0x40,
}GR_VOL_GEAR_T;
/* 交流电流档位 */
typedef enum{
	_AC_200uA	= 0X2,
	_AC_2mA		= 0X4,
	_AC_10mA	= 0X8,
	_AC_20mA	= 0X10,
	_AC_50mA	= 0x20,
	_AC_100mA	= 0X40,
	_AC_200mA	= 0X80,

/* 直流电流档位 */
	_DC_2uA		= 0X2,
	_DC_20uA	= 0x4,
	_DC_200uA	= 0X8,
	_DC_2mA		= 0x10,
	_DC_10mA	= 0X20,
	_DC_20mA	= 0X40,
	_DC_50mA	= 0X80,
	_DC_100mA	= 0X100,
}CUR_GEAR_T;
/* 输出频率 */
typedef enum{
	_AC_50HZ	= 0X2,
	_AC_60HZ	= 0X4,
	_AC_100HZ	= 0X8,
	_AC_150HZ	= 0X10,
	_AC_200HZ	= 0x20,
	_AC_250HZ	= 0X40,
	_AC_300HZ	= 0x80,
	_AC_350HZ	= 0X100,
	_AC_400HZ	= 0X200,
}FREQ_GEAR_T;
typedef enum{
	GR_DIS_VOL	= 0X01,
	GR_DIS_RES	= 0X02,
}GR_DIS_OPT;
/* 机型结构体 */
typedef struct{
    char *name;
    char *ver;
    uint16_t num;
    void (*config_fun)(void);
}TYPE_STR;

typedef struct{
    uint8_t amp_select;/* 功放类型 老功放 和 8833功放 */
    uint8_t par_medium;/* 参数存储介质 eep 或 flash */
    uint8_t res_medium;/* 结果存储介质 eep 或 flash */
    uint8_t cal_medium;/* 校准存储介质 eep 或 flash */
    uint8_t buzzer_en;/* 蜂鸣器硬开关 */
    uint8_t mute_sw;/* 系统静音模式开关 静音时测试报警不会一直响 */
    uint8_t amp_type;/* 功放类型 LINE PWM */
    uint8_t leading_sw;/* 数据导入导出到U盘开关 */
	uint8_t ir_gear_hold;/* IR换挡保持时间开关 */
	uint8_t ir_speed_sw;/* IR测试速度定制开关 打开后系统参数中有相应的设置项 快 中 慢 */
	uint8_t offset_set_en;/* 偏移手动设置使能 */
}CUSTOM_SYS_PAR;//定制系统参数

typedef struct{
    uint16_t type_num;/* 定制机编号 */
    uint16_t custom_en;//定制机使能标志
    uint8_t name[20];/* 定制机名称 */
    TYPE_STR    type;/* 定制机 */
    CUSTOM_SYS_PAR custom_sys_par;/* 菜单化定制参数 */
}CUSTOM_TYPE_INFO;//定制机型信息

typedef enum{
    __STOP     = 0x1,/* stop键 start键 再次启动从第一步开始 */
    __HALT     = 0x2,/* stop键 start键 再次启动从下一步开始 */
    __CON      = 0x4,/* 根据配置如果连续打开就自动进入下一步 */
    __RESTART  = 0x8,/* start键 从第一步开始 */
    __NEXT     = 0x10,/* start键 从下一步开始 */
    __FPDFC    = 0x20,/* First pass done fail con 第一步pass结束测试 失败就继续测试剩余的所有步 */
}_FAIL_MODE_;
typedef enum{
    GR_AMP_32A,
    GR_AMP_40A,
    GR_AMP_60A,
    GR_AMP_62A,
}GR_AMP_TYPE;
typedef enum{
    SINGLE_POINT,/* 单触点变压器 */
    DOUBLE_POINT,/* 双触点变压器 */
}TRANSFORMER_POINT;
/* 变压器输入输出比 */
typedef enum{
    TRANS_500V,/* 25V 1kV  40  */
    TRANS_750V,/* 25V 1kV  40  */
    TRANS_1kV,/* 25V 1kV  40  */
    TRANS_2kV,/* 25V 2kV  80  */
    TRANS_5kV,/* 25V 5kV  200 */
    TRANS_10kV,/* 25V 10kV 400 */
    TRANS_20kV,/* 25V 20kV 800 */
    TRANS_30kV,/* 25V 20kV 800 */
}TRANSFORMER_TYPE;

typedef enum{
    LINE_AMP,/* 线性功放 */
    PWM_AMP,/* PWM功放 */
}AMP_TYPE;/* 功放类型 */

typedef enum{
    SYS_SW_OFF,/* 系统开关关闭 */
    SYS_SW_ON,/* 系统开关打开 */
}SYS_SW_ENUM;

typedef enum{
    MC14094_CTRL,/* 4094控制多路高压板 */
    CPLD_CTRL,/* CPLD控制多路高压板 */
}PORTS_CTRL_TYPE;

enum{
    INTERFACE_PLC       = 1<<0,
    INTERFACE_COMM      = 1<<1,
    INTERFACE_USB_OUT   = 1<<2,
    INTERFACE_USB_IN    = 1<<3,
};
enum{
    VOL_SEGMENT_NO,  /* 电压不分段 */
    VOL_SEGMENT_YES, /* 电压分段 */
};
typedef enum{
    CUR_FORMAT,/* 以电流的形式显示真实电流 */
    CAP_FORMAT,/* 以电容的形式显示真实电流 */
}REAL_CUR_FORMAT;
/* 机型特性结构体 */
typedef struct{
/*************** COM ***********************/
	uint8_t mode_en;/* 模式使能标志 第1-5位分别表示ACW DCW IR GR BBD 的使能位 第0位没有用 */
	
	uint8_t offset_fun_en;/* 偏移功能使能 */
    TRANSFORMER_POINT transformer_point;/* 变压器触点 */
    TRANSFORMER_TYPE transformer_type;/* 变压器类型 */
    AMP_TYPE amp_type;/* 功放类型 */
	
    PORTS_CTRL_TYPE ports_ctrl_type;/* 多路控制硬件选择 */
	PORT_NUM_TYPE port_num;/* 测试端口数 0 4 8 */
    
	FREQ_TYPE hz_type;/* 频率的类型 分为等级和连续 默认是连续模式 */
	FREQ_TYPE gr_hz_type;/* 频率的类型 分为等级和连续 默认是连续模式 */
	uint16_t freq_h;/* 频率上限值 */
	uint16_t freq_l;/* 频率下限值 */
	uint8_t comm_check_en;/* 通信检查使能 */
	uint8_t interface_en;/* 菜单接口是否允许访问 */
    uint8_t interface_config;/* 接口功能配置 */
    uint8_t def_fail_mode;/* 定义支持的失败模式枚举 _FAIL_MODE_ */
    uint8_t vol_is_segment;/* 电压是否分段, 0不分段 1不分段 */ 
    uint16_t vol_segment_point;/* 电压段分界点 */
    uint8_t support_g_mode;/* 支持G模式 */
    uint16_t contact_segment_point;/* 双触点变压器切换变压器触头的电压点 */
    
    uint8_t silent_en;/* 静音模式开关 */
    uint8_t plc_signal_en;/* plc信号使能 系统菜单项 */
	uint8_t rise_time_min_value;//上升时间最小值
    
/*************** ACW ***********************/
	uint16_t acw_hz;/* 1-9 每个位表示一个频率 只有在等级模式下有用 */
	uint8_t acw_real;/* acw 是否有真实电流 */
    REAL_CUR_FORMAT real_cur_format;/* acw 真实电流的显示形式 */
	uint16_t acw_gear;/* acw 设计了3个档位 1 200uA, 2 2mA, 3 20mA 其中每一个位表示一个档位 如 第0位 未使用 第1位是200uA 第2位是2mA */
	uint16_t dcw_gear;/* dcw 设计了5个档位 1-5 2uA 20uA 200uA 2mA 10mA 设定方法同acw */
	ACW_VOL_RANGE_ENUM  acw_vol_range;/* acw电压档位 0低于1万伏 1万伏以上 */
	uint16_t acw_vol_h;/* acw 电压最大值 单位V */
	uint16_t acw_vol_l;/* acw 电压最小值 单位V */
	uint16_t acw_arc_h;/* acw 电弧侦测电流模式下的上限值 */
	uint8_t  acw_arc_gear;/* acw 电弧侦测记录电流的基准 */
    
/*************** CC ***********************/
    uint16_t cc_gear;
    uint16_t cc_vol_h;/* acw 电压最大值 单位V */
	uint16_t cc_vol_l;/* acw 电压最小值 单位V */
    
/*************** BBD ***********************/
    uint16_t bbd_vol_h;/* BBD电压上限值 */
    uint16_t bbd_vol_l;/* BBD电压下限值 */
/*************** DCW ***********************/
	DCW_VOL_RANGE_ENUM  dcw_vol_range;/* dcw电压档位 0低于1万伏 1 1万伏以上 */
	uint16_t dcw_vol_h;/* dcw 电压最大值 单位v */
	uint16_t dcw_vol_l;/* dcw 电压最小值 单位V */
	uint16_t dcw_arc_h;/* dcw 电弧侦测电流模式下的上限值 */
	uint8_t  dcw_arc_gear;/* dcw 电弧侦测记录最大档位 */
    uint8_t  dcw_big_cap_en;/* dcw 测大电容 */
    uint8_t  dcw_output_impedan_opt_index;/* dcw 大电容测试时设置输出阻抗的选项索引 */
	
/*************** IR ***********************/
	IR_VOL_RANGE_ENUM  ir_vol_range;/* ir电压档位 0低于1万伏 1 1万伏以上 */
	uint16_t ir_vol_h;/* ir电压的最大值 单位v */
	uint16_t ir_vol_l;/* ir电压的最小值 单位v */
	float 	 ir_res_h;/* ir电阻上限最大值 单位Mohm */
	float 	 ir_res_l;/* ir电阻上限最大值 单位Mohm */
	uint8_t ir_short_int_en;/* 有的机型在绝缘测试时要开短路中断 */
	uint8_t ir_absorptance_sw;/* ir吸收比开关 */
	
/*************** GR ***********************/
    GR_AMP_TYPE gr_amp_type;/* gr功放的类型 */
	uint16_t gr_cur_h;/* gr电流的最大值 单位0.01A*/
	uint16_t gr_cur_l;/* gr电流的最大值 单位0.01A*/
	uint16_t gr_res_h;/* gr电阻的最大值 单位mohm */
	uint16_t gr_res_l;/* gr电阻的最小值 单位mohm */
	uint8_t gr_hz;/* gr的输出频率 交流时需要 与acw一样 */
	uint8_t gr_vol_gear;/* 直流gr 压降测试仪使用 */
    uint16_t gr_max_cur_res_rang;/* 最大电流时能测的电阻最大值 */
    uint16_t one_dc_module_max_cur;/* 一个模块能输出的最大电流值，该参数仅仅是为了判断需要几个模块工作
									不会对具体测试参数构成影响 */
	
	uint8_t gr_dis_opt; /* gr需要显示的项目 有电压 和 电阻供组合选择 */
	uint8_t gr_cur_unit;/* gr电流的单位 A ，mA */
	uint8_t gr_cur_decs;/* gr电流的小数位数 */
	uint8_t gr_res_unit;/* gr电阻的单位 Ω ，mΩ */
	uint8_t gr_res_decs;/* gr电阻的小数位数 */
	uint8_t gr_vol_unit;/* gr电压单位 */
	uint8_t gr_vol_decs;/* gr电压的小数位数 */
	
	uint8_t gr_dc_en;/* 标记gr 是交流的还是直流的 */
	uint8_t gr_vol_drop_mode;/* gr直流压降模式 1打开 0关闭 */
	uint8_t gr_cur_divide_gear_en;/* gr直流压降模式 电流分档采样使能 */
    uint8_t single_gr;/* 单GR标记 */
	
/*************** OTHER ***********************/
	uint8_t g_mode_en;/* 有的机型有G模式 该标志由程序自动检查是否有ACW\DCW模式来决定是否使能 */
	uint8_t syn_roads_en;/* 多路同步使能 */
}TYPE_SPECIALITY;

/***************** 全局变量定义 ******************/
#ifdef   _CS99xx_TYPE
#define  _TYPE_EXT
#else
#define  _TYPE_EXT  extern
#endif

_TYPE_EXT const TYPE_STR        *g_cur_type;/* 指向当前机型结构的全局指针 */
_TYPE_EXT TYPE_SPECIALITY       type_spe;  /* 机器型号的特点 */
_TYPE_EXT CUSTOM_SYS_PAR        g_custom_sys_par;/* 定制系统参数，会存放到内部flash中 */
/*************** 函数声明 ***************/
extern uint8_t is_custom_type(void);
extern void set_custom_type_info(void);
extern void updata_sys_language_par(void);
extern void judge_amp_type(void);
extern void read_custom_par(void);
extern void save_custom_par(void);
extern void select_type(void);
extern int32_t check_type(void);
extern void init_machine(const uint8_t mode);
extern void init_default_type(void);
extern void judge_single_gr(void);
extern uint8_t get_max_cur_gear(uint8_t mode);
extern uint32_t define_hz_kinds(uint8_t mode, const uint8_t** gear_buf, uint8_t *flag);
extern uint32_t define_cur_kinds(uint8_t mode, const uint8_t *gear[], uint8_t* flag);
extern uint32_t define_fail_mode_kinds(const uint8_t *fail_mode_buf[], uint8_t *flag);
extern uint32_t define_vol_kinds(uint8_t mode, const uint8_t **gear, uint8_t *flag);
extern int32_t check_mode(void);
extern int32_t check_this_mode(uint8_t mode);
extern uint32_t define_interface_config(uint8_t *flag);
extern uint8_t * get_version_number(void);
extern uint8_t get_acw_max_gear(void);
extern uint8_t get_cc_max_gear(void);
extern uint8_t get_dcw_max_gear(void);
extern int32_t check_gr_mode(void);

#endif //__TYPE_SELECT__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
