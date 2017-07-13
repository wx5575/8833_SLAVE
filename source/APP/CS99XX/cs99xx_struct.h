/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：99xx_struct.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __99xx_STURCT_H__
#define __99xx_STURCT_H__

#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/******************* 加载文件 *********************/
#include "stm32f10x.h"
#include "cs99xx_config.h"

/******************* 宏定义***********************/
#define FLAG_FLAG			((uint32_t)0x5A5AA5A5)  /* eep正常标记 ，开机时会检查 */
#define MAX_STEPS           (99)          /* 单个文件容纳最大步数 */
#define TEST_PORTS_MAX      (8 * 2) /* 测试端口的最大数目 必须是8的倍数 */
#define STEP_LIST_SIZE      ((uint16_t)5) /* 存放最近连续步的个数 */

#define MAX_FILES			((uint16_t)31) /* 最大文件数目包括一个默认的文件 */
#define NAME_LON			((uint16_t)14) /* 文件名的长度 */

#define PER_PAGE_RES_NUM 	((uint16_t)4) /* 屏幕每页能显示的结果数 */
#define PW_LON				((uint16_t)8) /* 长度为 8个字符定义密码长度PW_LON */


/****************数据结构定义********************/
enum{
	CHINESE = 1,            /* 汉语 */
	ENGLISH = 0,            /* 英语 */
	
	G_MODE = 0,             /* 工作模式 G模式 */
	N_MODE = 1,             /* 工作模式 N模式 */
	
	ARC_CUR_MODEL 	= 0,    /* 电流模式 */
	ARC_GRADE_MODEL = 1,    /* 等级模式 */
	
	FLOAT_MODE	= 0,        /* 浮地 模式 */
	GND_MODE	= 1,        /* 接地 模式 */
	
	GFI_OFF		= 0,        /* GFI关闭 */
	GFI_ON		= 1,        /* GFI打开 */
	
	PLC_OFF		= 0,        /* GFI关闭 */
	PLC_ON		= 1,        /* GFI打开 */
    
	DC_GR_PULSE = 0,        /* 直流GR脉冲测试 */
	DC_GR_CONT	= 1,        /* 直流GR连续测试 */
	
	GR_V_PULSE_MODEL = 0,   /* 脉冲测试模式 */
	GR_V_CONT_MODEL  = 1,   /* 连续测试模式 */
};
enum{
    AUTO_SHIFT_OFF,         /* IR自动换挡关闭 */
    AUTO_SHIFT_ON,          /* IR自动换挡打开 */
};
typedef enum {
	NO = 0, 
	YES = !NO ,
} Single_Switch;

/* 定义8个测试端口数据 */
typedef struct {
	uint16_t port1:2;
	uint16_t port2:2;
	uint16_t port3:2;
	uint16_t port4:2;
	uint16_t port5:2;
	uint16_t port6:2;
	uint16_t port7:2;
	uint16_t port8:2;
}_TEST_PORT;

typedef struct{
    _TEST_PORT ports[TEST_PORTS_MAX / 8];
    uint8_t num;
    uint8_t mode;/* 通信时使用 */
}TEST_PORT;

/* ACW参数 */
typedef struct {
	uint16_t 	step;                       /* 测试步 */
	uint8_t 	mode;                       /* 模式 */
	uint16_t 	testing_voltage;            /* 测试电压 */
	uint8_t 	gear_i;                     /* 电流档位  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				/* 电流上限 */
	uint16_t	lower_limit;				/* 电流下限 */
	uint16_t	ac_real_cur;                /* 真实电流上限值 */
	uint16_t	arc_sur;					/* 电弧侦测 可能是等级模式即整数 可能为电流模式即为浮点数 这个有电弧侦测模式决定 */
	uint16_t	output_freq;				/* 输出频率选择 可以连续也可以是等级 */
	uint16_t	rise_time;                  /* 上升时间 */
	uint16_t	testing_time;               /* 测试时间 */
	uint16_t	fall_time;                  /* 下降时间 */
	uint16_t	interval_time;              /* 间隔时间 */
	uint8_t 	steps_pass;					/* 步间pass */
	uint8_t 	steps_cont;					/* 步间连续 */
	TEST_PORT	port;                       /* 端口 */
	
	uint16_t	offset_cur;                 /* 偏移电流 */
	uint16_t 	offset_real;                /* 偏移真实电流 */
	uint8_t 	offset_result;              /* 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}ACW_STRUCT;

/* CC参数 */
typedef struct {
	uint16_t 	step;                       /* 测试步 */
	uint8_t 	mode;                       /* 模式 */
	uint16_t 	testing_voltage;            /* 测试电压 */
	uint8_t 	gear_i;                     /* 电流档位  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				/* 电流上限 */
	uint16_t	lower_limit;				/* 电流下限 */
	uint16_t	ac_real_cur;                /* 真实电流上限值 */
	uint16_t	cur_intensity;              /* 电流强度 高 中 低 */
	uint16_t	output_freq;      			/* 输出频率选择 可以连续也可以是等级 */
	uint16_t	rise_time;                  /* 上升时间 */
	uint16_t	testing_time;               /* 测试时间 */
	uint16_t	fall_time;                  /* 下降时间 */
	uint16_t	interval_time;              /* 间隔时间 */
	uint8_t 	steps_pass;         		/* 步间pass */
	uint8_t 	steps_cont; 				/* 步间连续 */
	TEST_PORT	port;                       /* 端口 */
	
	uint16_t	offset_cur;                 /* 偏移电流 */
	uint16_t 	offset_real;                /* 偏移真实电流 */
	uint8_t 	offset_result;              /* 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}CC_STRUCT;

/* DCW 的参数 */
typedef struct {
	uint16_t     step;                      /* 测试步 */
	uint8_t     mode;                       /* 测试步 */
	uint16_t 	testing_voltage;            /* 测试电压 */
	uint8_t 	gear_i;                     /* 电流档位   2uA ,  20uA , 200uA ,  2mA,   10mA */
	uint16_t	upper_limit;				/* 电流上限 */
	uint16_t	lower_limit;				/* 电流下限 */
	uint16_t	charge_cur;         		/* 充电电流 */
	uint16_t	arc_sur;   					/* 电弧侦测 */
    uint8_t     output_impedance;           /* 输出阻抗 */
	uint16_t	delay_time;                 /* 延时时间 延时时间到了才判断 */
	uint16_t	rise_time;                  /* 上升时间 */
	uint16_t	stab_time;                  /* 稳压时间 */
	uint16_t	testing_time;               /* 测试时间 */
	uint16_t	fall_time;                  /* 下降时间 */
	uint16_t	interval_time;              /* 间隔时间 */
	uint8_t 	steps_pass;         		/* 步间pass */
	uint8_t		steps_cont; 				/* 步间连续 */
	TEST_PORT	port;                       /* 端口 */
	
	uint16_t	offset_cur;                 /* 偏移电流 */
	uint8_t 	offset_result;              /* 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}DCW_STRUCT;

/* IR 参数 */
typedef struct {
	uint16_t 	step;                       /* 测试步 */
	uint8_t 	mode;                       /* 模式 */
	uint16_t 	testing_voltage;			/* 输出电压 */
	uint8_t 	auto_shift_gears;           /* 自动换挡 */
	uint32_t 	upper_limit;     			/* 电阻上限 */
	uint32_t 	lower_limit;	    		/* 电阻下限 */
	uint16_t	rise_time;	                /* 上升时间 */
	uint16_t	testing_time;	            /* 测试时间 */
	uint16_t	delay_time;	                /* 延时时间 */
	uint16_t	interval_time;	            /* 间隔时间 */
	uint8_t 	steps_pass;	        		/* 步间pass */
	uint8_t 	steps_cont;					/* 步间连续 */
	TEST_PORT	port;	                    /* 端口 */
}IR_STRUCT;

/* GR参数 */
typedef struct {
	uint16_t 	step;
	uint8_t 	mode;
	uint8_t		voltage_gear;               /* 电压档位 1 20mv 2 200mv 3 2000mv */
	uint16_t	testing_cur;                /* 输出电流 */
	uint16_t 	upper_limit;				/* 电阻上限 */
	uint16_t 	lower_limit;				/* 电阻下限 */
	uint16_t	testing_time;               /* 测试时间 */
	uint16_t	interval_time;              /* 间隔时间 */
	uint8_t 	steps_pass;					/* 步间pass */
	uint8_t 	steps_cont; 				/* 步间连续 */
	uint16_t	output_freq;				/* 输出频率 */
	uint16_t	test_method;                /* 当脉冲测试模式使能了以后 0表示脉冲测试 1表示连续测试 */
	
	uint16_t	offset_res;                 /* 偏移电阻 */
	uint8_t	    offset_result;              /* 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}GR_STRUCT;

/* BBD参数 */
typedef struct {
	uint16_t 	step;                       /* 设置步 */
	uint8_t 	mode;                       /* 模式 */
	uint16_t 	testing_voltage;            /* 输出电压 */
	uint16_t	open_ratio;                 /* 开路比例 0，10%-100% */
	uint16_t	short_ratio;                /* 短路比例 100% - 500% */
	uint16_t 	upper_limit;				/* 电容上限 */
	uint16_t 	lower_limit;				/* 电容下限 */
	uint16_t	output_freq;      			/* 输出频率 */
	uint16_t	testing_time;               /* 测试时间 */
	uint16_t	interval_time;	            /* 间隔时间 */
	uint8_t 	steps_pass;	        		/* 步间pass */
	uint8_t 	steps_cont;					/* 步间连续 */
	float		cap_value;                  /* 电容值 */
	uint8_t 	gear;                       /* 保存获取电容值时的电流档位 默认 20mA */
	TEST_PORT	port;	                    /* 端口 */
	
	uint8_t 	get_cs_ok;                  /* 标记获取电容已经OK */
    
	float   	offset_cap;                 /* 偏移电阻 */
	uint8_t	    offset_result;              /* 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}BBD_STRUCT;

/* 公共数据 */
typedef struct {
	uint16_t step;                           /* 设置步 */
	uint8_t mode;                            /* 模式 */
}COMMON;

/* 将每一步都定义为一个联合体实现统一操作 */
typedef union{
	COMMON      com;
	ACW_STRUCT 	acw;
	DCW_STRUCT 	dcw;
	IR_STRUCT 	ir;
	GR_STRUCT 	gr;
	BBD_STRUCT  bbd;
    CC_STRUCT   cc;
}UN_STRUCT,*PUN_STRUCT,**PPUN_STRUCT;

/* 多路同时测试用到的上下限 */


/* 单步的节点 */
typedef struct Node_Step{
	UN_STRUCT 	one_step;                   /* 步结构 */
	struct Node_Step *prev;                 /* 前指针 */
	struct Node_Step *next;                 /* 后指针 */

}NODE_STEP;

/* 测试步骤链表定义 */
typedef struct List_Step{
	NODE_STEP	*head;                      /* 头 */
	NODE_STEP	*tail;                      /* 尾 */
	uint8_t 	size;                       /* 大小 */
}LIST_99xx;

/* 记忆组定义 */
typedef struct TestGroup{
	NODE_STEP test_steps[STEP_LIST_SIZE];
}TESTGROUP;

/* 文件头的定义 */
typedef struct Test_File{
	uint8_t num;                            /* 文件编号，每个文件都有唯一的编号 0 默认文件 1-30用户文件 */
	char name[NAME_LON+1];	                /* 文件名称 */
	uint8_t work_mode;	                    /* 1表示N 2表示G */
	uint16_t hold_pass_time;                /* pass信号保持时间 */
	uint16_t pass_buzzing_time;             /* pass蜂鸣器保持时间 */
	uint8_t arc_mode;	                    /* 1电流模式2等级模式 */
	uint16_t total;	                        /* 存放该文件中一共设置了几步 */
	uint32_t create_time;                   /* 创建时间 */
}TEST_FILE;

typedef struct{
    uint8_t addr[16];/* 地址缓存 */
    uint8_t count;/* 模块个数计数 */
}DC_MODULE_USING_INFO;

/* 系统参数定义 */
typedef struct Sys_Parameter{
	uint8_t contrast;			/* 液晶对比度(0-9) */
	uint8_t allowance;			/* 余量提示 (0-9)*/
	uint8_t is_save_res;		/* 是否保存结果，0不保存，1保存 */
	uint8_t is_overflow_cover;	/* 是否溢出覆盖 */
	
	uint8_t is_gfi_protect;		/* 是否GFI保护 */
	uint8_t is_self_check;		/* 是否打开开机自检功能 */
	uint8_t language;			/* 语言切换，1中文，0英文 */
	uint8_t is_falt_continue;	/* 失败继续是否失败继续，0不继续，1继续 */
    uint8_t fail_mode;          /* 失败模式 */
	uint8_t test_method;		/* 测试方法 1接地 0浮地 */
	uint8_t test_level;			/* 测试电平 1电平 0触点 */
	
	char 	data_of_survey[21];	/* 检验时间 */
	uint8_t is_table_dis;		/* 是否打开列表显示功能 0，关闭，1打开 */
	uint8_t num_rule;			/* 编号规则 (0-2) */
	uint8_t buzzer_sw;			/* 蜂鸣器开关 */
	
	uint8_t commu_on_off;		/* 通信开关 1打开 0关闭*/
	uint8_t baud_rate_gear;		/* 波特率：1表示9600 2表示14400 3表示19200 */
	uint8_t local_addr;			/*本机地址 (1-255) */
	
	uint8_t plc_on_off;			/* PLC开关 1打开，0关闭 */
	char pass_word[PW_LON+1];	/* 密码最多8位，最少1位 */
	
	uint16_t used_res_num;		/* 已经记录的结果总数 用于统计结果*/
	uint16_t pass_res_num;		/* 合格的数 用于统计结果 */
	uint16_t cover_res_num;		/* 当used_res_num记满n条后就如果允许覆盖就启动该计数变量计数 当其记满4500条后清零 */
	
	uint8_t keyboard_lock;		/* 键盘锁 */
	
    char dut_name[NAME_LON+1];	/* 被测件名 */
	uint8_t silent_sw;			/* 静音开关 */
    uint8_t plc_signal;         /* plc 信号 当失败继续打开后如果plc_signal=each_step 表示每步都会发Plc信号
                                   最后一步也是，如果 plc_signal=all_steps 表示最后一步不单独发plc信号
                                   而是做一个综合判断 */
    uint16_t output_delay;		/* 输出延时 用户要求启动后等待一段时间在输出高压 */
    uint16_t ir_gear_hold;		/* IR切换档位延时 */
	uint8_t ir_speed_sw;		/* IR测试速度 快 中 慢 通过设置滤波深度实现 */
    DC_MODULE_USING_INFO    dc_module_using_info;/* 正在使用的DC模块地址信息 */
}SYS_PAR;


typedef struct {
    uint16_t vol;		/* 测试电压 */
    uint8_t gear;		/* 电流档位 */
    uint8_t real_gear;  /* 真实的电流档位 为了区别所有档位特加入的 wangxin 2015.08.10 */
    uint16_t cur;		/* 测试电流 */ 
    uint16_t real_cur;	/* 真实电流 */
}ACW_RES;

typedef struct {
    uint16_t vol;		/* 测试电压 */
    uint8_t gear;		/* 电流档位 */
    uint8_t real_gear;  /* 真实的电流档位 为了区别所有档位特加入的 wangxin 2015.08.10 */
    uint16_t cur;		/* 测试电流 */
    uint16_t real_cur;	/* 真实电流 */
}CC_RES;

typedef struct {
    uint16_t vol;		/* 测试电压 */
    uint8_t gear;		/* 电流档位 */
    uint8_t real_gear;  /* 真实的电流档位 为了区别所有档位特加入的 wangxin 2015.08.10 */
    uint16_t cur;		/* 测试电流 */
    uint16_t real_cur;	/* 真实电流 */
}DCW_RES;

typedef struct {
    uint16_t vol;		/* 测试电压 */
    uint8_t gear;		/* 电阻档位 */
    uint32_t res;		/* 绝缘电阻 */
    uint16_t out_range;	/* 电阻超程 */
	float absorptance;/* 吸收比 */
}IR_RES;

typedef struct {
    uint16_t cur;		/* 测试电流 */
    uint16_t vol;		/* 测试电压 */
    uint8_t gear;		/* 电压档位 */
    uint16_t res;		/* 接地电阻 */
    uint16_t real_cur;	/* 真实电流 */
}GR_RES;

typedef struct {
    uint16_t vol;		/* 测试电压 */
    uint16_t open_r;	/* 开路检测 */
    uint16_t short_r;   /* 短路检测 */
    uint32_t cap;		/* 电容单位pF */
    uint32_t base_cap;  /* 基准电容单位 pF */
    
}BBD_RES;

typedef enum{
	RES_FAIL,
	RES_PASS,
}RES_STR;

typedef struct{
    uint8_t num[5];
    uint8_t step[11];
    uint8_t dut_name[16];
    uint8_t file_name[16];
    uint8_t work_mode[2];
    uint8_t test_mode[4];
    uint8_t test_out[9];//输出
    uint8_t test_loop[9];//回路采集
    uint8_t real_cur[9];//真实电流
    uint8_t range[9];//电流档位
    uint8_t result[2];
    uint8_t test_time[9];//测试时间
    uint8_t time[21];
}RESULT_STRING_STR;

typedef struct {
    char dut_name[NAME_LON+1];			/* 被测件名 */
    uint8_t file_name[NAME_LON+1];		/* 测试文件名称 */
    uint16_t step;						/* 本次测试的步骤编号 */
    uint16_t total;						/* 总测试步骤数 */
    uint8_t work_mode;					/* 工作模式 */
    uint8_t mode;						/* 测试模式 */
    
    union {
        ACW_RES acw;
        DCW_RES dcw;
        IR_RES ir;
        GR_RES gr;
		BBD_RES bbd;
		CC_RES cc;
    }un;
    
    uint16_t test_time;					/* 测试时间 */
    RES_STR result;						/* 测试结果 */
	uint8_t err_num;					/* 错误编号 */
	uint32_t time;						/* 保存测试日期时间 */
	

}RESULT_STR;



/* 系统常驻标志 */
typedef struct {
	uint8_t mem_init;		            /* 存储器初始化标志 */
	uint8_t last_group;		            /* 最近一次使用的记忆组 */
	uint32_t boot_times;	            /* 开机次数 */
	uint32_t amp_times;		            /* 功放继电器吸合次数统计 */
	uint32_t key_start_times;           /* 启动键按下的次数 */
	uint32_t key_stop_times;            /* 复位键按下的次数 */
	uint32_t flag_flag;		            /* 开机后检查该标记如果与预定值不同就出问题了 */
	uint32_t res_count;					/* 结果计数 */
	uint32_t last_res_date;				/* 最近一条结果的保存时间 该时间用来判断是否要清零 res_count */
}SYS_FLAG;

typedef struct {
	uint8_t mem_init;		            /* 存储器初始化标志 */
	uint8_t last_group;		            /* 最近一次使用的记忆组 */
	uint32_t boot_times;	            /* 开机次数 */
	uint32_t amp_times;		            /* 功放继电器吸合次数统计 */
	uint32_t key_start_times;           /* 启动键按下的次数 */
	uint32_t key_stop_times;            /* 复位键按下的次数 */
	uint32_t flag_flag;		            /* 开机后检查该标记如果与预定值不同就出问题了 */
}SYS_FLAG_T;
    

/* 系统数据参数的长度 */
typedef struct{
    uint8_t sys_par;
    uint8_t sys_flag;
    uint8_t test_file;
    uint16_t group_bitmap;
    uint32_t group_map_table;
    uint8_t step;
    uint8_t result;
}SYS_DATA_LEN_T;

typedef struct{
    uint16_t used_res_num;		/* 已经记录的结果总数 用于统计结果*/
    uint16_t pass_res_num;		/* 合格的数 用于统计结果 */
    uint16_t res_str_size;      /* 存放结果信息结构体大小 */
}RESULT_INFO_T;
/******************* 全局变量声明 *********************/
#ifdef   _99xxSTR_GLOBALS
#define  _99xxSTR_EXT
#else
#define  _99xxSTR_EXT  extern
#endif


_99xxSTR_EXT SYS_FLAG	sys_flag;       /* 定义全局标志给系统运行时使用 */
_99xxSTR_EXT SYS_PAR 	sys_par;	    /* 存放系统参数 */

_99xxSTR_EXT TESTGROUP test_group;      /* 此结构表示一个完整的记忆组 */

_99xxSTR_EXT NODE_STEP *free_list_steps;
_99xxSTR_EXT LIST_99xx list_99xx;
_99xxSTR_EXT uint8_t list_lon;/* 链表最大长度 在测试 时为2 在偏移测试时为5 */

_99xxSTR_EXT LIST_99xx cs99xx_list;
_99xxSTR_EXT NODE_STEP *free_list_head;

_99xxSTR_EXT LIST_99xx cs99xx_cache_list;/* 缓存链表 */

_99xxSTR_EXT NODE_STEP *g_cur_step;     /* 指向当前步的指针 */
_99xxSTR_EXT NODE_STEP cur_node;        /* 全局节点用于设置参数时保存临时参数 */

_99xxSTR_EXT TEST_FILE file_table[MAX_FILES];/* 相当于一本书的目录 */
_99xxSTR_EXT TEST_FILE default_file;
_99xxSTR_EXT TEST_FILE *g_cur_file;     /* 指向当前文件 */

_99xxSTR_EXT NODE_STEP steps_buf[5];/* 存放连续的最近的步骤 */


_99xxSTR_EXT RESULT_STR cur_result;     /* 当前步测试结果信息就放在这里 */
_99xxSTR_EXT RESULT_STR *g_cur_res;     /* 指向当前的结果信息 */
_99xxSTR_EXT RESULT_STR result_buf[PER_PAGE_RES_NUM];   /* 每一页能显示4个测试步结果 */


_99xxSTR_EXT RESULT_STR result_list_buf[4];     /* 存放所有测试步的测试结果列表显示信息 */






/******************* 函数声明 *********************/
extern void init_sys_par(SYS_PAR *p);	        /* 初始化系统参数 */
extern void init_file_table(void);	            /* 初始化文件表 */
extern void init_one_file(uint8_t n);
extern void clear_one_file(uint8_t n);
extern void init_default_file(void);
extern void del_file(const uint8_t file_num);
extern void del_all_file(void);

extern void init_99xx_list(void);	            /* 初始化测试步链表 */

extern void clear_step_used_flag(void);
extern void insert_step(uint8_t pos, uint8_t mode);
extern void insert_node(uint8_t step);
extern void insert_step(uint8_t pos, uint8_t mode);
extern void del_step(uint8_t step);
extern void del_all_steps(void);
extern void swap_step(const uint16_t one, const uint16_t two);
extern void swap_node(PUN_STRUCT one,PUN_STRUCT two);
extern int32_t check_step_exist_for_comm(uint16_t pos);
extern NODE_STEP *position_step(uint16_t pos);

extern void init_cc_step(NODE_STEP * p);
extern void init_acw_step(NODE_STEP * p);
extern void init_dcw_step(NODE_STEP * p);
extern void init_ir_step(NODE_STEP * p);
extern void init_gr_step(NODE_STEP * p);
extern void init_bbd_step(NODE_STEP * p);
extern uint16_t get_cur_file_total(void);
/**************************************************/

#if defined(__cplusplus)
    }
#endif

#endif //__99xx_STURCT_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
