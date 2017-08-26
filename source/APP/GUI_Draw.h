/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：GUI_Draw.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
#ifndef __GUI_DRAW_H__
#define __GUI_DRAW_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"


#define GX	       ((int16_t)60)
#define GY	       ((int16_t)15)
#define OX 	       ((int16_t)4)
#define OY 	       ((int16_t)14)
#define CW12       ((int16_t)6) /* 12号字体宽度 英文字符 */
#define CW15       ((int16_t)9) /* 15号字体宽度 英文字符 */
#define CW16       ((int16_t)9) /* 16号字体宽度 英文字符 */
#define CH12       ((int16_t)12) /* 12号字体高度 */
#define CH15       ((int16_t)15) /* 16号字体高度 */
#define CH16       ((int16_t)16) /* 16号字体高度 */

#define PX	       ((int16_t)(GX+OX))
#define PY	       ((int16_t)(0))
#define SX	       ((int16_t)(PX+18))
#define SY	       ((int16_t)(0))
#define SOX	       ((int16_t)(58))
#define SOY	       ((int16_t)(3))


#define SYS_MENU_X      ((int16_t)15)
#define SYS_MENU_Y      ((int16_t)2)
#define SYS_MENU_OX1    ((int16_t)75)
#define SYS_MENU_OX2    ((int16_t)73)
#define SYS_MENU_OY     ((int16_t)16)

/* 状态显示坐标 定义 */
#define GUI_GEN_STATUS_CW   ((int16_t)9)
#define GUI_GEN_STATUS_CH   ((int16_t)16)
#define GUI_GEN_STATUS_X	((int16_t)6)
#define GUI_GEN_STATUS_X1	((int16_t)6)
#define GUI_GEN_STATUS_X2	((int16_t)86)
#define GUI_GEN_STATUS_Y	((int16_t)42)

#define GUI_SYN_STATUS_CW	((int16_t)6)
#define GUI_SYN_STATUS_CH	((int16_t)12)
#define GUI_SYN_STATUS_X	((int16_t)10)
#define GUI_SYN_STATUS_X1	((int16_t)10)
#define GUI_SYN_STATUS_X2	((int16_t)10+8*6)
#define GUI_SYN_STATUS_Y	((int16_t)46)

/* 标题栏坐标 定义 */
#if (MAX_STEPS < 100)
#define TITLE_STEP_X        ((int16_t)(4))
#define TITLE_STEP_COLON_X  ((int16_t)(27))//:
#define TITLE_STEP_NUM_X    ((int16_t)(39))
#define TITLE_STEP__X       ((int16_t)(51))
#define TITLE_STEP_TOTAL_X  ((int16_t)(57))
#else
#define TITLE_STEP_X        ((int16_t)(-100))//不显示
#define TITLE_STEP_COLON_X  ((int16_t)(-100))//:不显示
#define TITLE_STEP_NUM_X    ((int16_t)(15))
#define TITLE_STEP__X       ((int16_t)(39))
#define TITLE_STEP_TOTAL_X  ((int16_t)(45)) //4+35+18-2*6
#endif


#define TITLE_WORK_MODE_X   ((int16_t)(76))//4+35+37, 0
#define TITLE_MODE_X        ((int16_t)(90))//4+35+38+13
#define TITLE_FILE_X        ((int16_t)(112))// 4+35+38+15+22
#define TITLE_FILE_COLON_X  ((int16_t)(134))//:4+35+38+15+20+4*6
#define TITLE_FILE_NAME_X   ((int16_t)(143))// 4+35+38+15+22+6*5+1

#define TITLE_Y             ((int16_t)(0))


#define MODE1	   ((uint8_t)1)
#define MODE2	   ((uint8_t)2)
#define MODE3	   ((uint8_t)3)
#define MODE4	   ((uint8_t)4)
#define MODE5	   ((uint8_t)5)

/* 居中计算x坐标只适用于字宽6的字体 */
#define	BTP_STR(s, x1, x2) (((x1)+(x2))/2 - (strlen(s)*6/2))

/* 变宽字体cw 指定平均字体宽度 中文字体减半 */
#define	BTP_STR_(s, x1, x2, cw) (((x1)+(x2))/2 - (strlen(s)*(cw)/2))

/*********************** 坐标 **************************/
#define TEST_ORIGIN_X       ((int16_t)10)
#define TEST_ORIGIN_Y       ((int16_t)13)
#define TEST_UI_OFFSET      ((int16_t)13)

/********************* 位图声明 ************************/
/***************** 数据结构定义 ***********************/
enum{
	INT_T,			/* 整数类型 */
	FLOAT_T, 		/* 浮点数类型 */
	STRING_T,		/* 字符串类型 */
	OPTION_T,		/* 选项类型 可以有一个或以上的选项 */
	EITHER_OR, 		/* 二选一 */
    STRUCT_T,       /* 结构类型 */
};
typedef enum{
	UD_B,	  /* up down 空 back */
	UD_B_RE,  /* UD_B+刷屏 */
	UDSB,	  /* up down short_cut back */
	UDSB_RE,  /* UDSB+刷屏 */
	UDRB,	  /* up down round_shift back */
	UDRB_RE,  /* UDRB+刷屏 */
	UDR_,
	UDR_RE,
	___B,
	___B_RE,
	LRDB,    /* lift_shift right_shift del back */
	LRDB_RE,
    __RB,
    __RB_RE,
}SHORT_CUT_TYPE;

/* 参数数据信息数组下标 */
typedef enum{
	/********** COM 12 0 ************/
	GUI_COM_STEP = 0,       /* 测试步骤 */
	GUI_COM_MODE,           /* 测试模式 */
	GUI_COM_DELAY_T,        /* 延时时间 */
	GUI_COM_RISE_T,         /* 上升时间 */
	GUI_COM_STAB_T,         /* 稳压时间 */
	GUI_COM_TEST_T,         /* 测试时间 */
	GUI_COM_FALL_T,         /* 下降时间 */
	GUI_COM_INTER_T,        /* 间隔时间 */
	GUI_COM_STEPS_PASS,     /* 步间pass */
	GUI_COM_STEPS_CONT,     /* 步间连续 */
	GUI_COM_OUTPUT_FREQ,    /* 输出频率 */
	GUI_COM_PORT,           /* 多路端口 */
	GUI_COM_CUR_OFFSET,		/* 电流偏移 */
	GUI_COM_RES_OFFSET,		/* 电阻偏移 */
	
	/********** ACW 8  12 ************/
	GUI_ACW_OUTPUT_VOL,     /* 输出电压 */
	GUI_ACW_CUR_GEAR,       /* 电流档位 */
	GUI_ACW_CUR_H,          /* 电流上限 */
	GUI_ACW_CUR_L,          /* 电流下限 */
	GUI_ACW_REAL_CUR,       /* 真实电流 */
	GUI_ACW_ARC_SUR,        /* 电弧侦测 */
	
	/********** DCW 8 20 ************/
	GUI_DCW_OUTPUT_VOL,     /* 输出电压 */
	GUI_DCW_CUR_GEAR,       /* 电流档位 */
	GUI_DCW_CUR_H,          /* 电流上限 */
	GUI_DCW_CUR_L,          /* 电流下限 */
	GUI_DCW_CHARGE_CUR,     /* 充电电流 */
	GUI_DCW_ARC_SUR,        /* 电弧侦测 */
    GUI_DCW_OUTPUT_IMPEDANCE, /* 输出阻抗 */
	
	/********** IR 4 28 ************/
	GUI_IR_OUTPUT_VOL,      /* 输出电压 */
	GUI_IR_AUTO_GEAR,       /* 自动换挡 */
	GUI_IR_RES_H,           /* 电阻上限 */
	GUI_IR_RES_L,           /* 电阻下限 */
	
	/********** GR 7 32 ************/
	GUI_GR_VOL_GEAR,        /* 电压档位 */
	GUI_GR_OUTPUT_CUR,      /* 输出电流 */
	GUI_GR_RES_H,           /* 电阻上限 */
	GUI_GR_RES_L,           /* 电阻下限 */
	GUI_GR_VOL_H,           /* 电压上限 */
	GUI_GR_VOL_L,           /* 电压下限 */
	GUI_GR_TEST_METHOD,     /* 测试方法 连续测试 脉冲测试 */
	
	/********** BBD 3 39 ************/
	GUI_BBD_OUTPUT_VOL,     /* 输出电压 */
	GUI_BBD_OPEN_CHECK,     /* 开路检测 */
	GUI_BBD_SHORT_CHECK,    /* 短路检测 */
	GUI_BBD_CAP_H,			/* 电容上限 */
	GUI_BBD_CAP_L,			/* 电容下限 */
	
	/********** CC 6 42 ************/
	GUI_CC_OUTPUT_VOL,      /* 输出电压 */
	GUI_CC_CUR_GEAR,        /* 电流档位 */
	GUI_CC_CUR_H,           /* 电流上限 */
	GUI_CC_CUR_L,           /* 电流下限 */
	GUI_CC_REAL_CUR,        /* 真实电流 */
	GUI_CC_CUR_INTENSITY,   /* 电流强度 */
	
	/********** SYS 19 48 ************/
	GUI_SYS_CONTRAST,       /* 液晶分辨率 */
	GUI_SYS_REMAIN_HINT,    /* 余量提示 */
	GUI_SYS_RES_SAVE,       /* 结果保存 */
	GUI_SYS_VOER_COVER,     /* 溢出覆盖 */
	GUI_SYS_GFI_PROT,       /* GFI保护 */
	GUI_SYS_SELF_CHECK,     /* 自检允许 */
	GUI_SYS_LANGUAGE,       /* 系统语言 */
	GUI_SYS_FAIL_CONT,      /* 失败继续 */
	GUI_SYS_LIST_DIS,       /* 列表显示 */
	GUI_SYS_TEST_PORT,      /* 测试端口 接地模式 浮地模式 */
	GUI_SYS_TEST_SIGNAL,    /* 测试信号 */
	GUI_SYS_BUZZER_SW,      /* 蜂鸣开关 */
	GUI_SYS_CHECK_DATE,     /* 检验日期 */
	GUI_SYS_NO_RULE,        /* 编号规则 */
    GUI_SYS_FAIL_MODE,      /* 失败模式 */
    GUI_SYS_SILENT_SW,      /* 静音开关 */
    GUI_SYS_PLC_SIGNAL,     /* PLC信号 */
    GUI_SYS_OUTPUT_DELAY,   /* 输出延时 */
	GUI_SYS_IR_GEAR_HOLD,	/* 档位保持 */
	GUI_SYS_IR_SPEED,		/* IR测试速度 快 中 慢 通过滤波深度不同来实现 */
    
	/********** CUSTOM 9 67 ************/
    GUI_CUSTOM_AMP_SELECT,/* 功放选择老功放新功放 */
    GUI_CUSTOM_AMP_TYPE,    /* 定制功放类型 LINE PWM */
    GUI_CUSTOM_PAR_MEDIUM,/* 参数存储介质 */
    GUI_CUSTOM_RES_MEDIUM,/* 结果存储介质 */
    GUI_CUSTOM_CAL_MEDIUM,/* 校准存储介质 */
    GUI_CUSTOM_BUZZER_EN,/* 蜂鸣器硬件使能 */
    GUI_CUSTOM_MUTE_SW,/* 静音开关 */
    GUI_CUSTOM_LEADING_SW,/* 数据导入导出开关 */
    GUI_CUSTOM_IR_G_HOLD_SW,/* IR换挡保持开关 */
	GUI_CUSTOM_IR_SPEED_SW,/* IR测试速度开关 */
	GUI_CUSTOM_OFFSET_SW,/* 偏移设置开关 */
	
	GUI_MENU_END,/* 结束 */
}MENU_ENUM;
typedef enum{
    GUI_U8,         /* 8位 */
    GUI_U16,        /* 16位 */
    GUI_U32,        /* 32位 */
    GUI_STRING,     /* 字符串 */
    GUI_STRUCT,     /* 结构数据 */
    GUI_U8_ARRAY,   /* 数组 */
    GUI_U16_ARRAY,  /* 数组 */
    GUI_U32_ARRAY,  /* 数组 */
}DATE_LON_ENUM;
typedef struct Data_Info{
	void* pData;           /* 数据指针 */
	uint8_t x;             /* 数据的x坐标 */
	uint8_t y;             /* 数据的y坐标 */
	uint8_t type;          /* 0表示int,1表示float,2表示字符串 3 表示给出选择项 4 表示二选一 */
	uint8_t lon;           /* 显示数据长度 */
	uint8_t decs;          /* 小数点后几位 */
	uint8_t total;         /* 标记当前页显示的数据总数 */
	uint8_t cur;           /* 标记当前操作的是第几个数据 */
	uint8_t ch_bit;        /* 当前修改的位 从左到右 0-n ex:123.4 0->1 ,1->2 ,2->3 ,3->. ,4->4 */
	uint8_t page;	       /* 当前页码 */
	uint8_t bits;          /* 表明当前值是8位，16位，32位，0代表8位，1代表16位，2代表32位,3字符串, 4结构数据 DATE_LON_ENUM */
	uint8_t last_ch_bit;   /* 保存最近一次更改过的位 */
	uint32_t upper;        /* 参数上限值 */
	uint32_t lower;        /* 参数下限值 */
	
	const uint8_t *name[2];   /* 名称 */
	const uint8_t *unit[10];  /* 单位或选项变量的所有选项 */
	void (*fun) (void *p); /* 处理函数 */
    uint8_t no_dis_bits;/* 不显示的位数,从右边数不显示的位数 如lon = 5 xx.xx no_dis_bits = 1 结果 xx.x */
	MENU_ENUM num;         /* 编号 对应枚举 MENU_ENUM */
    SHORT_CUT_TYPE short_cut;     /* 编辑该变量需要的功能键类型 */
	const uint8_t *upper_str;
    const uint8_t *lower_str;
	uint8_t x_offset;      /* x坐标的偏移值 */
	uint8_t y_offset;      /* y坐标的偏移值 */
	uint8_t range_flag;    /* 参数范围是否需要更新 1表示不更新 0更新 */
	
    void *user_data;       /* 用户自定义使用 的指针 携带参数 */
	struct Data_Info *next;/* 下一个参数 */
	struct Data_Info *prev;/* 上一个参数 */
}DATA_INFO;


typedef struct {
	struct Data_Info *head;/* 链表头 */
	struct Data_Info *tail;/* 链表尾 */
	uint32_t size;         /* 节点个数 */
}PAR_INFO_LIST;

typedef struct{
    const char* name[2];       /* 名称 中文 英文 */
    void (*fun)(uint32_t opt); /* 处理函数 */
    uint32_t data;             /* 数据 */
    uint32_t size;             /* 用户数据个数 */
}MENU_INFO;

typedef struct{
    uint8_t dis_buf[15];/* 显示内容的缓冲 */
    uint8_t lon;/* 最大长度 */
}DIS_AREA_T_;
/* 显示区域结构定义 */
typedef struct{
    uint8_t uint[5];/* 单位信息 */
    uint8_t x;/* x坐标 */
    uint8_t y;/* y坐标 */
//    const GUI_FONT *font;/* 字体 */
    uint8_t lon2;/* 最大长度 */
}DIS_AREA_T;

/* 多选项设置参数的结构参数 */
/* 在设置ACW DCW电流档位时会用到 */
typedef struct{
    const uint8_t **opt_buf;   /* 位置信息 在运行的过程中使用 */
    uint8_t x;              /* x坐标 */
    uint8_t y;              /* y坐标 */
    uint8_t x_o[10];        /* x方向第n次偏移 */
}MORE_SEL_STR;


typedef struct More_Options_Sele{
    const uint8_t* name[2];
    uint8_t x;
    uint8_t y;
    uint8_t opt;
    struct More_Options_Sele *next;
    struct More_Options_Sele *prev;
    uint8_t cur;/* 当前行第几个 */
    uint8_t total;/* 当前行一共有几个 */
}MORE_OPTIONS_STR;

typedef struct more_options_list{
    struct More_Options_Sele *head;
    struct More_Options_Sele *tail;
    uint8_t size;
}MORE_OPTIONS_LIST;
/********************* 全局变量定义 *******************/
#ifdef MENU_GLOBAL
	#define EXTERN
#else
	#define EXTERN extern
#endif
/*********************************************/
EXTERN MORE_OPTIONS_LIST plc_signal_list;/* PLC设置链表 */
EXTERN MORE_OPTIONS_LIST cc_cur_gear_list;/* 电流强度设置项链表 */
EXTERN MORE_OPTIONS_LIST amp_type_list;/* 功放类型 */
EXTERN MORE_OPTIONS_LIST par_medium_list;/* 参数存储介质 eep 或 flash */
EXTERN MORE_OPTIONS_LIST res_medium_list;/* 结果存储介质 eep 或 flash */
EXTERN MORE_OPTIONS_LIST cal_medium_list;/* 校准存储介质 eep 或 flash */

EXTERN MORE_OPTIONS_LIST ir_filtering_deep_list;/* 快 中 慢 用在绝缘测试电阻滤波上 */


/* PLC信号 */
EXTERN MORE_OPTIONS_STR plc_signal_option_pool[];
typedef enum{
    EACH_STEP, /* 单步 */
    ALL_STEP, /* 总步 正常模式 */
    TEST_HOLD,/* TEST信号保持 */
}PLC_SIGNAL_ENUM;

/* 功放类型 */
EXTERN MORE_OPTIONS_STR amp_type_option_pool[];
typedef enum{
    AMP_8833,
    AMP_OLD,
}AMP_TYPE_ENUM;

EXTERN MORE_OPTIONS_STR par_medium_option_pool[];
typedef enum{
    PAR_MEDIUM_EEP,
    PAR_MEDIUM_FLASH,
}PAR_TYPE_ENUM;

EXTERN MORE_OPTIONS_STR res_medium_option_pool[];
typedef enum{
    RES_MEDIUM_EEP,
    RES_MEDIUM_FLASH,
}RES_TYPE_ENUM;

EXTERN MORE_OPTIONS_STR cal_medium_option_pool[];
typedef enum{
    CAL_MEDIUM_EEP,
    CAL_MEDIUM_FLASH,
}CAL_TYPE_ENUM;

/* CC电流输出强度 */
enum{
    CC_CUR_HIGH,/* 高 */
    CC_CUR_MID,/* 中 */
    CC_CUR_LOW,/* 低 */
};
EXTERN 	MORE_OPTIONS_STR 	cc_cur_gear_option_pool[];

/* IR滤波深度 直接影响的测试程序的快中慢 */
enum{
    IR_FILTERING_DEEP_FAST,/* 快 */
    IR_FILTERING_DEEP_MID,/* 中 */
    IR_FILTERING_DEEP_SLOW,/* 慢 */
};
EXTERN	 MORE_OPTIONS_STR 	ir_filtering_deep_option_pool[];

/* 多选菜单数组 */
EXTERN MORE_SEL_STR mor_sel_menu[];
enum{
    ACW_GEAR_SEL_MENU,      /* ACW电流档位 */
    DCW_GEAR_SEL_MENU,      /* DCW电流档位 */
    GR_VOL_GEAR_SEL_MENU,   /* GR电压档位 */
    MODE_SET_SEL_MENU,      /* 测试模式 */
    SYS_OPT_SET_MENU,       /* 系统界面多选项 */
};

/* 失败模式项  */
EXTERN const uint8_t* fail_mode_pool[];
enum{
    FAIL_MODE_STOP,/* stop键 start键 再次启动从第一步开始 */
    FAIL_MODE_HALT,/* stop键 start键 再次启动从下一步开始 */
    FAIL_MODE_CON,/* 根据配置如果连续打开就自动进入下一步 */
    FAIL_MODE_RESTART,/* start键 从第一步开始 */
    FAIL_MODE_NEXT,/* start键 从下一步开始 */
    FAIL_MODE_FPDFC,/* First pass done fail continue 第一步pass结束测试 失败就继续测试 
                    第一步测试合格就完成测试，第一步失败就继续测试下面的步骤 
                    注意：应用于多路扫描测试，通常第一步会对多路端口同时加上高压
                    当第一步测试合格了，就表明每个端口的测品都是合格的。否则就逐
                    路扫描每一个端口，找出不合格的测品
                    */
    FAIL_MODE_END,
};

/* 输出阻抗选项 */
typedef struct{
    const uint8_t *opts[4];/* 选择项 数组 */
    const uint8_t size;/* 选项个数 */
}OUTPUT_IMPEDAN_OPTS_T;

EXTERN const OUTPUT_IMPEDAN_OPTS_T output_impedan_opt[];
typedef enum{
    OUTPUT_IMPEDAN_OPT_1,
    OUTPUT_IMPEDAN_OPT_2,
}OUTPUT_IMPEDAN_OPTS;



EXTERN DATA_INFO menu_par[];
EXTERN DATA_INFO* pmenu_data;

EXTERN DATA_INFO* g_cur_info[6];/* 当前页的变量信息 */
EXTERN uint32_t g_cur_val[6];/* 当前页变量值 */
EXTERN PAR_INFO_LIST par_info_list;/* 参数信息链表 */

EXTERN uint32_t colon_x;/* 参数名称后面冒号的x坐标 用在步骤参数设置界面 和 系统参数设置界面 这两个地方的值不一样 */

/*************** 枚举与对应的数组定义 ******************/
/* GFI ARC HV 状态 在测试见面显示 当没有多路扫描时就 */
EXTERN const char* sys_sw_status_buf[][2];
enum{
	GFI_STATUS,
	ARC_STATUS,
	HV_STATUS,
};
/***************************************/
/* 测试模式项 */
EXTERN const uint8_t *mode_pool[][10];

enum{
    NUL     = 0,
    ACW     = 1,
    DCW     = 2,
    IR      = 3,
    GR      = 4,
    BBD     = 5,
    CC      = 6,
    MODE_END,
};


/* 测试仪数据项目 */
EXTERN const uint8_t *opt_pool[];
enum{
    NUL_OPT,
    VOL_OPT,
    CUR_OPT,
    RES_OPT,
    CAP_OPT,
};

/* 工作模式 */
EXTERN const char *work_mode_buf[];
/***************************************/
/* 测试状态项 */
EXTERN const char* status_str[][2];

enum{
	TEST_WAIT_INDEX,
	TEST_RISE_INDEX,
	TEST_TEST_INDEX,
	TEST_FALL_INDEX,
	TEST_CHANGE_INDEX,
	TEST_INTER_INDEX,
	TEST_PASS_INDEX,
	TEST_FAIL_INDEX,
	TEST_CHARGE_INDEX,
	TEST_STAB_INDEX,
	TEST_DISCHARGE_INDEX,
	TEST_DISCHARGE_OVER_INDEX,
    TEST_OUTPUT_DELAY,
};
/***************************************/
/* 异常信息 */
extern const char* except_buf[][3];
/********* 参数单位管理 *********/
/* 单位项 */
EXTERN const uint8_t *unit_pool[];
EXTERN const uint8_t *unit_pool_[];

/********************************/
/* 交流输出频率 */
enum{
    AC_50HZ = 1,
    AC_60HZ,
    AC_100HZ,
    AC_150HZ,
    AC_200HZ,
    AC_250HZ,
    AC_300HZ,
    AC_350HZ,
    AC_400HZ,
};

/********************************/
EXTERN DIS_AREA_T test_gui_area[6];
enum{
    TEST_GUI_AREA_1,
    TEST_GUI_AREA_2,
    TEST_GUI_AREA_3,
    TEST_GUI_AREA_4,
    TEST_GUI_AREA_5,
    TEST_GUI_AREA_6,
};
    
/********************************/
/* IR显示电阻时换档时使用的 */
EXTERN const char* res_infinite_str[];

/********************************/
/* 多路扫描端口设置 */
#define PORT_STATUS_STR     ((const char*)"XLH")

/**************************************/
/* 偏移结果显示项 */
EXTERN const char *offset_res_buf[4];
enum{
	OFFSET_PASS		= 2,
	OFFSET_FAIL		= 1,
	OFFSET_NONE		= 0,
};

/**************************************/
typedef enum{
    EXE_NULL,//空操作
    RE_MAIN,//重绘主界面
    INTO_TEST,//进入测试界面
    INTO_SET,//进入设置界面
    INTO_MENU,//进入菜单界面
    INTO_HELP,//进入帮助界面
    INTO_AUTO_CAL,//进入自动校准
    INTO_SCAN_MODULES,//进入扫描模块界面
}EXE_STATUS_ENUM;

/**************************************/

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t u_x;
    uint8_t o_y;
    uint8_t cw;
    uint8_t *str;
}ELEMENT_INFO;

// typedef struct{
//     ELEMENT_INFO info[4];//电压 电流 真实电流 时间
// }TEST_UI_ELE_SET;

EXTERN ELEMENT_INFO test_ui_ele_set[6];/* 测试界面下的元素信息集合 */

enum{
    TEST_ELE_1,/* 测试界面的元素1 ACW DCW IR的电压 GR的电流 */
    TEST_ELE_2,/* 测试界面的元素2 ACW DCW 的电流 IR GR 的电阻 */
    TEST_ELE_3,/* 测试界面的元素3 ACW 真实电流 或 电容 */
    TEST_ELE_4,/* 测试界面的元素4 测试时间 */
    TEST_ELE_5,/* 测试界面的元素5 测试状态等 */
    TEST_ELE_6,/* 测试界面的元素6 测试结果状态等 */
    
    OFFSET_ELE_1 = 0,/* 偏移界面的元素1 ACW DCW IR的电压 GR的电流 */
    OFFSET_ELE_2,/* 偏移界面的元素2 ACW DCW 的电流 IR GR 的电阻 */
    OFFSET_ELE_3,/* 偏移界面的元素3 ACW 真实电流 或 电容 */
    OFFSET_ELE_4,/* 偏移界面的元素4 测试时间 */
    OFFSET_ELE_5,/* 偏移界面的元素4 测试状态等 */
};

typedef enum{
    /* 设置界面 */
    UI_SET_PORTS_BASE_X = 10,/* x坐标基值 */
    UI_SET_PORTS_BASE_Y = 35,/* y坐标基值 */
    UI_SET_PORT_X_O = 12,/* x方向相邻通道偏移值 */
    UI_SET_PORT_Y_O = 12,/* y方向相邻通道偏移值 */
    /* 测试界面 */
    UI_TEST_PORTS_BASE_X = 90,/* x坐标基值 */
    UI_TEST_PORTS_BASE_Y = 41,/* y坐标基值 */
    UI_TEST_PORT_X_O = 9,/* x方向相邻通道偏移值 */
    UI_TEST_PORT_Y_O = 12,/* y方向相邻通道偏移值 */
    /* 偏移界面 */
    UI_OFFSET_PORTS_BASE_X = 5,/* x坐标基值 */
    UI_OFFSET_PORTS_BASE_Y = 0,/* y坐标基值 */
    UI_OFFSET_PORT_X_O = 12,/* x方向相邻通道偏移值 */
    UI_OFFSET_PORT_Y_O = 0,/* y方向相邻通道偏移值 */
}UI_PORTS_ENUM;/* 多路扫描通道的界面信息 */

#define TEST_ELE_1_X        10
#define TEST_ELE_1_Y        13
#define TEST_ELE_1_U_X(cw)  (TEST_ELE_1_X + 5 * cw + 1)

#define TEST_ELE_2_X        109
#define TEST_ELE_2_Y        13
#define TEST_ELE_2_U_X(cw)  (TEST_ELE_2_X + 5 * cw + 1)

#define TEST_ELE_3_X        109
#define TEST_ELE_3_Y        26
#define TEST_ELE_3_U_X(cw)  (TEST_ELE_3_X + 5 * cw + 1)

#define TEST_ELE_4_X        10
#define TEST_ELE_4_Y        26
#define TEST_ELE_4_U_X(cw)  (TEST_ELE_4_X + 5 * cw + 1)

// GUI_FontHZ_SimSun_16 
#define TEST_ELE_5_X        (GUI_GEN_STATUS_X1)
#define TEST_ELE_5_Y        (GUI_GEN_STATUS_Y)
#define TEST_ELE_5_U_X(cw)  (GUI_GEN_STATUS_X2)

#define TEST_ELE_6_X        (100)
#define TEST_ELE_6_Y        (42)
#define TEST_ELE_6_U_X(cw)  (87+12*8)
//////////////////////OFFSET///////////////////////////
#define OFFSET_ELE_1_X        152 //测试电压
#define OFFSET_ELE_1_Y        27
#define OFFSET_ELE_1_U_X(cw)  (OFFSET_ELE_1_X + 5 * cw)

#define OFFSET_ELE_2_X        30  //偏移电流
#define OFFSET_ELE_2_Y        15
#define OFFSET_ELE_2_U_X(cw)  (OFFSET_ELE_2_X + 5 * cw)

#define OFFSET_ELE_3_X        78   //偏移的真实电流
#define OFFSET_ELE_3_Y        15
#define OFFSET_ELE_3_U_X(cw)  (OFFSET_ELE_3_X + 5 * cw)

#define OFFSET_ELE_4_X        152   //偏移时间
#define OFFSET_ELE_4_Y        38
#define OFFSET_ELE_4_U_X(cw)  (OFFSET_ELE_4_X + 5 * cw)

#define OFFSET_ELE_5_X        (150)  //偏移状态
#define OFFSET_ELE_5_Y        51
#define OFFSET_ELE_5_U_X(cw)  (OFFSET_ELE_5_X + 8 * cw)
/********************* 函数声明 ***********************/
extern void draw_more_options_gui(MORE_OPTIONS_LIST *list, MORE_OPTIONS_STR *iteration);
extern void draw_short_cut_bm(uint8_t type);
extern void draw_short_cut_str(uint8_t *str1, uint8_t *str2, uint8_t *str3, uint8_t *str4);

extern void draw_menu_frame_line(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2);

extern uint8_t * mystrcat(uint8_t *buf, uint8_t *str1, uint8_t *str2);
extern void front_add_space_at_string(uint8_t *buf, uint8_t c);
extern uint32_t ten_power(u8 n);
extern char* div_str_pre_zero(char *str);
extern void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value);
extern void draw_colon(void);
extern void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit);
extern void load_this_page_info(DATA_INFO* data_info[]);
extern void load_this_page_value(void);
extern void cursor_pos(int16_t x, int16_t y);
extern void dis_cursor_pos(void);
extern void cursor_pos_n(int16_t x, int16_t y);/* 不带刷新屏幕 */
extern void dis_cursor(DATA_INFO * p);
extern void shift_cursor (DATA_INFO * p);
extern void shift_cursor_l (DATA_INFO * p);
extern void change_data(uint8_t base);
extern void my_sprintf(char *buf, DATA_INFO * p, uint32_t data);
extern void update_data(DATA_INFO * p);
extern void save_par_to_ram(DATA_INFO *data_info, uint32_t value);
extern void save_par_to_array(DATA_INFO *data_info, uint32_t value, uint8_t array_num);
extern uint32_t read_par_to_buf(DATA_INFO* data_info, uint32_t * value);
// void dis_test_time(uint16_t time, const GUI_FONT *font, uint16_t x, uint16_t y);

extern void clear_(void);
extern void clear_menu(void);
extern void MenuTitle(u8 which);
extern void syn_test_menu_title(void);
extern void set_syn_roads_cur_upper (void);
extern void test_menu_title(u8 which);
extern void draw_port_for_set(TEST_PORT *p);
extern void dis_port(void);
extern void gui_draw_menu(void);

extern void dis_set_port(const uint8_t val,const uint8_t n, const uint8_t ports);
extern void ports_cursor_pos(uint8_t n, uint8_t ports);

extern void gui_dis_float_at(const float v, const uint8_t decs, const uint8_t x, const uint8_t y);

extern void acw_gr_hz_sel_gui(uint8_t kinds, uint8_t n, const uint8_t** gear_buf);
extern void ac_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n);
extern void gr_vol_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n);
extern void dcw_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n);
extern void mode_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n);
extern void sys_option_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n);

extern void auto_cal_pass(void);
extern void gui_par_inval(void); 
extern void gui_g_mode_inval(void);
extern void ratio_broken(void);
extern void cal_warning(void);
extern void rate_of_advance(void);
extern void type_mismatch(void);
extern void gui_draw_main(void);

extern void cal_sw_on_warning(void);
extern void draw_wait_usb_connect_gui(void);
extern void draw_auto_calibration_gui(void);
extern void draw_no_connect_usb_gui(void);
extern void dcgr_commuincation_err(void);
extern void gui_draw_build_connect_to_board(void);
extern void gui_draw_build_connect_fail(void);
extern void gui_draw_dc_gr_module_comm_fail(void);
extern void gui_draw_dc_gr_module_being_in_comm(void);
extern void gui_draw_self_check(void);
extern void gui_draw_set_sys_cur_limit(void);

extern void gui_draw_test (void);
extern void gui_drow_test_syn (void);

extern void gui_draw_menu_sys(MENU_INFO* p_sys_menu, uint8_t lon);

extern void draw_custom_type_gui(void);
extern void gui_draw_help_1(void);
extern void gui_draw_help_2(void);
extern void gui_draw_help_3(void);

extern void gui_draw_file_management(void);
extern void gui_draw_communication_interface(MENU_INFO* pmenu_info);
extern void gui_draw_password_manage(void);
extern void gui_draw_time_manage(void);

extern void gui_draw_offset(void);
extern void gui_draw_bbd(void);
extern void gui_draw_test_result(void);
extern void gui_draw_keyboard_lock_manage(void);
extern void gui_draw_recover(void);

extern void dis_syn_cur_ui(const uint8_t n);

extern void dis_scan_modules_gui(void);

#endif //__GUI_Draw_H__
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
