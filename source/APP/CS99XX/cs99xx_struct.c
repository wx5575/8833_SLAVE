/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：99xx_struct.c
 * 摘  要  ：定义主要的数据结构，供整个工程使用
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

/********************** 加载文件 ************************/
#define _99xxSTR_GLOBALS
#include "cs99xx_struct.h"
#include "string.h"
#include "cs99xx_parameter.h"
#include "cs99xx_type.h"
#include "serve_test.h"
#include "cs99xx_mem_api.h"

/****************** 内部函数声明 *************************/

/****************** 全局变量的定义及初始化 *****************/
/* 定义位图表用来查询步存储空间已经使用 */
static uint8_t  const  cs99xx_maptbl[256] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x00 to 0x0F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x10 to 0x1F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x20 to 0x2F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,       /* 0x30 to 0x3F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x40 to 0x4F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x50 to 0x5F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x60 to 0x6F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,       /* 0x70 to 0x7F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x80 to 0x8F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x90 to 0x9F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xA0 to 0xAF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,       /* 0xB0 to 0xBF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xC0 to 0xCF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0xD0 to 0xDF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xE0 to 0xEF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 0        /* 0xF0 to 0xFF */
};
TEST_FILE default_file = 
{
	0,/* 默认文件的编号为0 */
	"DEFAULT",/* name */
// 	"????????",/* 被测品名 */
	N_MODE,	/* 工作模式为N，正常模式 */
	1,	//文件中设置了多少步
	10,	/* pass蜂鸣器保持时间 1s */
	0,	/* pass信号保持时间 1s */
	ARC_CUR_MODEL,	/* 电弧侦测为电流模式 */
    0,
};


SYS_PAR sys_par =
{
	5,	/* 液晶对比度(0-9) */
	5,	/* 余量提示 (0-9) */
	1,	/* 是否保存结果，0不保存，1保存 */
	0,	/* 是否溢出覆盖 0不覆盖，1覆盖 */
	GFI_OFF,	/* 是否GFI保护 0关闭，1打开 */
	0,	/* 是否打开开机自检功能 0关闭，1打开 */
	1,	/* 语言切换，1中文，0英文 */
	0,	/* 失败继续是否失败继续，0不继续，1继续 */
    1,
	
	FLOAT_MODE,	/* 测试方法 0接地 1浮地 */
	0,	/* 测试电平 0电平 1触点 */
	
	"2014-12-12 12:12:12",	/* 检验时间 */
	DISABLE,	/* 是否打开列表显示功能 0，关闭，1打开 */
	0,	/* 编号规则 (0-2) */
	1,	/* 蜂鸣器开关打开 */
	
	ENABLE,	/* 通信开关 1打开 0关闭*/
	1,	/* 波特率：1表示9600 2表示14400 3表示19200 */
	1,	/*本机地址 (1-255) */
	
	ENABLE,	/* PLC开关 1打开，0关闭 */
	
	"888888",	/* 密码最多8位，最少1位 */
	
	0,	/* 一共使用的结果数 用于统计结果 */
	0,	/* 合格的结果数 用于统计结果 */
	
	0,	/* 键盘锁 */
	0,/* 被测品名 */
    DISABLE,/* 静音开关 */
	
	0,/* plc 信号 当失败继续打开后如果plc_signal=each_step 表示每步都会发Plc信号
                                   最后一步也是，如果 plc_signal=all_steps 表示最后一步不单独发plc信号
                                   而是做一个综合判断 */
	0,/* 输出延时 用户要求启动后等待一段时间在输出高压 */
	200,/* IR切换档位延时 */
};

/*********************** 函数的定义 **************************/

/*
 * 函数名：init_sys_flag
 * 描述  ：初始化系统标志
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_sys_flag(void)
{
//     memset(&sys_flag, 0, sizeof sys_flag);
	sys_flag.mem_init = 1;	/* 将标志位写1 */
	sys_flag.last_group = 0;
	sys_flag.boot_times = 1;
	sys_flag.amp_times = 0;
	sys_flag.key_start_times = 0;
	sys_flag.key_stop_times = 0;
	sys_flag.flag_flag = FLAG_FLAG;
}
/*
 * 函数名：init_sys_par
 * 描述  ：初始化系统参数
 * 输入  ：SYS_PAR *p 传入要初始化的系统变量指针
 * 输出  ：无
 */
void init_sys_par(SYS_PAR *p)
{
	p->contrast = 5;	        /* 液晶对比度(0-9) */
	p->allowance = 5;	        /* 余量提示 (0-9)*/
	p->is_save_res = 1;	        /* 是否保存结果，0不保存，1保存 */
	p->is_overflow_cover = 0;	/* 是否溢出覆盖 */
	
	p->is_gfi_protect = 0;	    /* 是否GFI保护 */
	p->is_self_check = 0;	    /* 是否打开开机自检功能 */
	p->language = CHINESE;	    /* 语言切换，1中文，0英文 */
	p->is_falt_continue = 0;	/* 失败继续是否失败继续，0不继续，1继续 */
	p->fail_mode = FAIL_MODE_STOP;	/* 失败继续是否失败继续，0不继续，1继续 */
	
	p->is_table_dis = 0;	    /* 是否打开列表显示功能 0，关闭，1打开 */
	p->test_method = 0;		    /* 测试方法 1接地 0浮地 */
	p->test_level = 1;			/* 测试电平 1电平 0触点 */
	p->buzzer_sw = ENABLE;      /* 蜂鸣器开关 */
    p->silent_sw = DISABLE;     /* 静音开关 */
	
    //该参数在恢复出厂设置时受到保护
    if(app_flag.recover_flag == 0)
    {
        strcpy (p->data_of_survey, "2015-12-12 12:12:12");	/* 检验时间 */
    }
    
	p->num_rule = 0;	        /* 编号规则 (0-2) */
	
	p->commu_on_off = ENABLE;	    /* 通信开关 1打开 0关闭*/
	p->baud_rate_gear = 1;	    /* 波特率：1表示9600 2表示14400 3表示19200 */
	p->local_addr = 1;	        /*本机地址 (1-255) */
	
	p->plc_on_off = 1;	        /* PLC开关 1打开，0关闭 */
	strcpy(p->pass_word, "888888");	/* 密码最多8位，最少1位 */
	
	p->used_res_num = 0;        /* 已经记录的结果总数 用于统计结果*/
	p->pass_res_num = 0;        /* 合格的数 用于统计结果 */
	p->cover_res_num = 0;       /* 覆盖计数器 */
	
	p->keyboard_lock = 0;	    /* 键盘锁 1打开 0关闭 */
	
	strcpy (p->dut_name, "????????");	/* 被测品名 */
    
	p->plc_signal = ALL_STEP;        /* PLC信号 */
	p->output_delay = 0;        /* 延时时间 */
	p->ir_gear_hold = 200;/* IR换挡延时时间 */
	p->ir_gear_hold = IR_FILTERING_DEEP_FAST;/* IR测试速度默认为快速 */
	
	{
		uint32_t i = 0;
		
		for(i = 0; i < 16; i++)
		{
			if(p->dc_module_using_info.addr[i] > 15
				|| p->dc_module_using_info.count > 15)
			{
				memset(&p->dc_module_using_info, 0, sizeof(p->dc_module_using_info));
				break;
			}
		}
	}
}

void init_file_table(void)
{
	uint8_t i = 0;
    
	memset(file_table, 0, sizeof(file_table[0])*MAX_FILES);
    
	for(i = 0; i < MAX_FILES; i++)
	{
		file_table[i].num = i;  /* 所有文件都有一个唯一的编号范围(0 - MAX_FILES) */
	}
}

/*
 * 函数名：init_one_file
 * 描述  ：初始化指定文件的内容
 * 输入  ：n文件编号
 * 输出  ：无
 * 返回  ：无
 */
void init_one_file(uint8_t n)
{
    if(n >= MAX_FILES)
    {
        return;
    }
    
	file_table[n].num = n; /* 文件编号，每个文件都有唯一的编号 0 默认文件 1-30用户文件 */
	file_table[n].work_mode = N_MODE; /* 1表示N 2表示G */
	file_table[n].hold_pass_time = 10; /* pass信号保持时间 */
	file_table[n].pass_buzzing_time = 10; /* pass蜂鸣器保持时间 */
	file_table[n].arc_mode = ARC_CUR_MODEL; /* 1电流模式2等级模式 */
	file_table[n].total = 0; /* 存放该文件中一共设置了几步 */
}

/*
 * 函数名：clear_one_file
 * 描述  ：清除指定文件的内容
 * 输入  ：n文件编号
 * 输出  ：无
 * 返回  ：无
 */
void clear_one_file(uint8_t n)
{
    if(n >= MAX_FILES)
    {
        return;
    }
    
    memset(&file_table[n], 0, sizeof file_table[0]);
    
	file_table[n].num = n; /* 文件编号，每个文件都有唯一的编号 0 默认文件 1-30用户文件 */
}

/*
 * 函数名：init_default_file
 * 描述  ：初始化默认文件
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_default_file(void)
{
	default_file.num = 0; /* 文件编号，每个文件都有唯一的编号 0 默认文件 1-30用户文件 */
    strcpy(default_file.name, "DEFAULT"); /* 文件名称 */
// 	strcpy(default_file.dut_name,"????????"); /* 被测品名称 */
// 	default_file.work_mode = N_MODE; /* 1表示N 2表示G */
// 	default_file.hold_pass_time = 0; /* pass信号保持时间 */
// 	default_file.pass_buzzing_time = 10; /* pass蜂鸣器保持时间 */
// 	default_file.arc_mode = ARC_CUR_MODEL; /* 1电流模式2等级模式 */
	default_file.total = 0; /* 存放该文件中一共设置了几步 */
	default_file.create_time = 0; /* 创建时间 */
}

/*
 * 函数名：del_file
 * 描述  ：删除指定文件
 * 输入  ：file_num要删除的文件编号
 * 输出  ：无
 * 返回  ：无
 */
void del_file(const uint8_t file_num)
{
    if(file_num >= MAX_FILES)
    {
        return;
    }
    
    clear_step_used_flag();/* 清空标记 */
    clear_group_table();/* 清空文件步映射表 */
    
    clear_one_file(file_num);/* 清除文件内容 */
    
    save_group_info(file_num);
    
    if(g_cur_file->num == file_num)
    {
        g_cur_file = &file_table[0];
        
		sys_flag.last_group = g_cur_file->num;/* 更新最后使用的文件标志 */
		save_sys_flag();
    }
    
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
}

/*
 * 函数名：del_all_file
 * 描述  ：删除所有的文件
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void del_all_file(void)
{
    int32_t i = 0;
    
    default_file = file_table[0];/* 备份 */
    init_file_table();
    
    clear_step_used_flag();/* 清空标记 */
    clear_group_table();/* 清空文件步映射表 */
    
    for(i = 1; i < MAX_FILES; i++)
    {
        if(file_table[i].name[0] != 0)
        {
            save_step_used_flag(i);
            save_group_table(i);
        }
    }
    
    /* 全部删除时就直接将当前文件指针指向默认文件 */
    file_table[0] = default_file;/* 恢复备份 */
    g_cur_file = &file_table[0];
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
    
    save_all_file();/* 保存所有文件 */
    
    sys_flag.last_group = 0;/* 将最近一次使用的文件设为默认 */
    save_sys_flag();
}

/*
 * 函数名：init_99xx_list
 * 描述  ：初始化测试步链表
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_99xx_list(void)
{
	int8_t i = 0;
	NODE_STEP	*first = NULL;
	NODE_STEP	*second = NULL;
    
	//将空的测试步池初始化为一个单向链表将表头给free_list_steps   file_table[MAX_FILES];
	memset(test_group.test_steps,0,sizeof(NODE_STEP)*STEP_LIST_SIZE);
	
	for(i = 0; i < STEP_LIST_SIZE - 1; i++)
	{
		first  = &test_group.test_steps[i];
		second = &test_group.test_steps[i + 1];
		first->next = second;
	}
    
	first  = &test_group.test_steps[STEP_LIST_SIZE - 1];//小心越界，之前写成MAX_STEPS会产生意想不到的后果，可能是灾难性的
	
	first->next = NULL;
	
	free_list_steps = &test_group.test_steps[0];	//将空步链表头指向test_steps[0]
	
	list_99xx.head = NULL;
	list_99xx.tail = NULL;
	list_99xx.size = 0;
}

/*
 * 函数名：init_acw_step
 * 描述  ：初始化ACW测试步
 * 输入  ：p指向指定步节点数据
 * 输出  ：无
 * 返回  ：无
 */
void init_acw_step(NODE_STEP * p)
{
	ACW_STRUCT 	l_acw;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	
	define_cur_kinds(ACW, l_gear_buf, l_flag);/* 电流种类 */
	
	memset(&l_acw, 0, sizeof l_acw);
	
	l_acw.step = p->one_step.com.step;
	l_acw.mode = ACW;
	l_acw.testing_voltage = ACW_VOL_L;
	l_acw.gear_i = l_flag[0];
	l_acw.upper_limit = 500;
	l_acw.lower_limit = 0;
	l_acw.ac_real_cur = 0;
	l_acw.arc_sur = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_acw.output_freq = 1;/* 50hz */
	}
	/* 连续模式 */
	else
	{
		l_acw.output_freq = 500;/* 50hz */
	}
	
	l_acw.rise_time = type_spe.rise_time_min_value;
	l_acw.testing_time = 30;
	l_acw.fall_time = 0;
	l_acw.interval_time = 0;
	l_acw.steps_pass = 1;
	l_acw.steps_cont = 0;
	
	l_acw.offset_cur = 0;
	l_acw.offset_real = 0;
	l_acw.offset_result = 0;
    
	if(g_cur_file->work_mode == G_MODE)
	{
		l_acw.steps_pass = 0;
		l_acw.steps_cont = 1;
	}
    
    l_acw.port.num = type_spe.port_num;
    l_acw.work_port.num = type_spe.work_port_num;
    
    if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_acw.work_port.ports[0].port1 = 2;
        l_acw.work_port.ports[0].port2 = 2;
        l_acw.work_port.ports[0].port3 = 2;
        l_acw.work_port.ports[0].port4 = 2;
    }
    else if(PORT_NUM8 == type_spe.work_port_num)
    {
        l_acw.work_port.ports[0].port1 = 2;
        l_acw.work_port.ports[0].port2 = 2;
        l_acw.work_port.ports[0].port3 = 2;
        l_acw.work_port.ports[0].port4 = 2;
        l_acw.work_port.ports[0].port5 = 2;
        l_acw.work_port.ports[0].port6 = 2;
        l_acw.work_port.ports[0].port7 = 2;
        l_acw.work_port.ports[0].port8 = 2;
    }
    
	p->one_step.acw = l_acw;
}

/*
 * 函数名：init_cc_step
 * 描述  ：初始化CC测试步
 * 输入  ：p指向指定步节点数据
 * 输出  ：无
 * 返回  ：无
 */
void init_cc_step(NODE_STEP * p)
{
	CC_STRUCT 	l_cc;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	
	define_cur_kinds(CC, l_gear_buf, l_flag);/* 电流种类 */
	
	memset(&l_cc, 0, sizeof l_cc);
	
	l_cc.step = p->one_step.com.step;
	l_cc.mode = CC;
	l_cc.testing_voltage = CC_VOL_L;
	l_cc.gear_i = l_flag[0];
	l_cc.upper_limit = 500;//ac_gear[get_cc_max_gear()].high_max;//;
	l_cc.lower_limit = 50;
	l_cc.ac_real_cur = 0;
	l_cc.cur_intensity = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_cc.output_freq = 1;/* 50hz */
	}
	/* 连续模式 */
	else
	{
		l_cc.output_freq = 500;/* 50hz */
	}
	
	l_cc.rise_time = 0;
	l_cc.testing_time = 1;
	l_cc.fall_time = 0;
	l_cc.interval_time = 0;
	l_cc.steps_pass = 1;
	l_cc.steps_cont = 0;
	
	l_cc.offset_cur = 0;
	l_cc.offset_real = 0;
	l_cc.offset_result = 0;
    
	if(g_cur_file->work_mode == G_MODE)
	{
		l_cc.steps_pass = 0;
		l_cc.steps_cont = 1;
	}
    
    l_cc.port.num = type_spe.port_num;
    l_cc.work_port.num = type_spe.work_port_num;
    
    if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_cc.work_port.ports[0].port1 = 2;
        l_cc.work_port.ports[0].port2 = 2;
        l_cc.work_port.ports[0].port3 = 2;
        l_cc.work_port.ports[0].port4 = 2;
    }
    else if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_cc.work_port.ports[0].port1 = 2;
        l_cc.work_port.ports[0].port2 = 2;
        l_cc.work_port.ports[0].port3 = 2;
        l_cc.work_port.ports[0].port4 = 2;
        l_cc.work_port.ports[0].port5 = 2;
        l_cc.work_port.ports[0].port6 = 2;
        l_cc.work_port.ports[0].port7 = 2;
        l_cc.work_port.ports[0].port8 = 2;
    }
    
	p->one_step.cc = l_cc;
}
/*
 * 函数名：init_dcw_step
 * 描述  ：初始化DCW测试步
 * 输入  ：p指向指定步节点数据
 * 输出  ：无
 * 返回  ：无
 */
void init_dcw_step(NODE_STEP * p)
{
	DCW_STRUCT 	l_dcw;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	
	l_kinds = define_cur_kinds(DCW, l_gear_buf, l_flag);/* 电流种类 */
	memset(&l_dcw, 0, sizeof l_dcw);
	
	l_dcw.step = p->one_step.com.step;
	l_dcw.mode = DCW;
	l_dcw.testing_voltage = DCW_VOL_L;
	l_dcw.gear_i = l_flag[l_kinds-1];
	l_dcw.upper_limit = 500;
	l_dcw.lower_limit = 0;
	l_dcw.charge_cur = 0;
	l_dcw.arc_sur = 0;
	l_dcw.delay_time = 0;
	l_dcw.rise_time = 0;
	l_dcw.stab_time = 0;
	l_dcw.testing_time = 30;
	l_dcw.fall_time = 0;
	l_dcw.interval_time = 0;
	l_dcw.steps_pass = 1;
	l_dcw.steps_cont = 0;
	
	l_dcw.offset_cur = 0;
	l_dcw.offset_result = 0;
	
	if(g_cur_file->work_mode == G_MODE)
	{
		l_dcw.steps_pass = 0;
		l_dcw.steps_cont = 1;
	}
    
    l_dcw.port.num = type_spe.port_num;
    l_dcw.work_port.num = type_spe.work_port_num;
    
    if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_dcw.work_port.ports[0].port1 = 2;
        l_dcw.work_port.ports[0].port2 = 2;
        l_dcw.work_port.ports[0].port3 = 2;
        l_dcw.work_port.ports[0].port4 = 2;
    }
    else if(PORT_NUM8 == type_spe.work_port_num)
    {
        l_dcw.work_port.ports[0].port1 = 2;
        l_dcw.work_port.ports[0].port2 = 2;
        l_dcw.work_port.ports[0].port3 = 2;
        l_dcw.work_port.ports[0].port4 = 2;
        l_dcw.work_port.ports[0].port5 = 2;
        l_dcw.work_port.ports[0].port6 = 2;
        l_dcw.work_port.ports[0].port7 = 2;
        l_dcw.work_port.ports[0].port8 = 2;
    }
    
	p->one_step.dcw = l_dcw;
}

/*
 * 函数名：init_ir_step
 * 描述  ：初始化IR测试步
 * 输入  ：p指向指定步节点数据
 * 输出  ：无
 * 返回  ：无
 */
void init_ir_step(NODE_STEP * p)
{
	IR_STRUCT 	l_ir;
	
	memset(&l_ir, 0, sizeof l_ir);
	
	l_ir.step = p->one_step.com.step;
	l_ir.mode = IR;
	l_ir.testing_voltage = IR_VOL_L;
	l_ir.auto_shift_gears = 1;
	l_ir.upper_limit = 0;
	l_ir.lower_limit = IR_RES_L;
	l_ir.rise_time = 0;
	l_ir.testing_time = 30;
	l_ir.delay_time = 0;
	l_ir.interval_time = 0;
	l_ir.steps_pass = 1;
	l_ir.steps_cont = 0;
	
    l_ir.port.num = type_spe.port_num;
    l_ir.work_port.num = type_spe.work_port_num;
    
    if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_ir.work_port.ports[0].port1 = 2;
        l_ir.work_port.ports[0].port2 = 2;
        l_ir.work_port.ports[0].port3 = 2;
        l_ir.work_port.ports[0].port4 = 2;
    }
    else if(PORT_NUM8 == type_spe.work_port_num)
    {
        l_ir.work_port.ports[0].port1 = 2;
        l_ir.work_port.ports[0].port2 = 2;
        l_ir.work_port.ports[0].port3 = 2;
        l_ir.work_port.ports[0].port4 = 2;
        l_ir.work_port.ports[0].port5 = 2;
        l_ir.work_port.ports[0].port6 = 2;
        l_ir.work_port.ports[0].port7 = 2;
        l_ir.work_port.ports[0].port8 = 2;
    }
    
	p->one_step.ir = l_ir;
}

/*
 * 函数名：init_gr_step
 * 描述  ：初始化GR测试步
 * 输入  ：p指向指定步节点数据
 * 输出  ：无
 * 返回  ：无
 */
void init_gr_step(NODE_STEP * p)
{
	GR_STRUCT 	l_gr;
	const uint8_t *l_gear_buf[VOL_KINDS_MAX];
	uint8_t l_flag[VOL_KINDS_MAX];
	
	memset(l_flag, 0, sizeof l_flag);
	memset(&l_gr, 0, sizeof l_gr);
	define_vol_kinds(GR, l_gear_buf, l_flag);/* 电流种类 */
	
	l_gr.step = p->one_step.com.step;
	l_gr.mode = GR;
	l_gr.voltage_gear = l_flag[0];
	l_gr.testing_cur = GR_CUR_L;
	l_gr.upper_limit = 1000;
	l_gr.lower_limit = 0;
	l_gr.testing_time = 30;
	l_gr.interval_time = 0;
	l_gr.steps_pass = 1;
	l_gr.steps_cont = 0;
    
	if(type_spe.gr_hz_type == HZ_TYPE_GRADE)
	{
		l_gr.output_freq = 1;
	}
	else
	{
		l_gr.output_freq = 500;
	}
    
	l_gr.test_method = DC_GR_CONT;
	
	l_gr.offset_res = 0;
	l_gr.offset_result = 0;
	
    l_gr.work_port.num = type_spe.work_port_num;
    
    if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_gr.work_port.ports[0].port1 = 2;
        l_gr.work_port.ports[0].port2 = 2;
        l_gr.work_port.ports[0].port3 = 2;
        l_gr.work_port.ports[0].port4 = 2;
    }
    else if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_gr.work_port.ports[0].port5 = 2;
        l_gr.work_port.ports[0].port6 = 2;
        l_gr.work_port.ports[0].port7 = 2;
        l_gr.work_port.ports[0].port8 = 2;
    }
    
	p->one_step.gr = l_gr;
}

/*
 * 函数名：init_bbd_step
 * 描述  ：初始化BBD测试步
 * 输入  ：p指向指定步节点数据
 * 输出  ：无
 * 返回  ：无
 */
void init_bbd_step(NODE_STEP * p)
{
	BBD_STRUCT 	l_bbd;
	
	memset(&l_bbd, 0, sizeof l_bbd);
	
	l_bbd.step = p->one_step.com.step;
	l_bbd.mode = BBD;
	l_bbd.testing_voltage = 100;
	l_bbd.open_ratio = 50;/* 表示10% 这样做是为了 限制百分比以1%递增 */
	l_bbd.short_ratio = 120;/* 表示120% */
	l_bbd.cap_value = 0;
	l_bbd.testing_time = 30;
	l_bbd.interval_time = 0;
	l_bbd.steps_pass = 1;
	l_bbd.steps_cont = 0;
	l_bbd.output_freq = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_bbd.output_freq = 9;/* 400hz */
	}
	/* 连续模式 */
	else
	{
		l_bbd.output_freq = 4000;/* 400hz */
	}
    
	l_bbd.gear = AC_20mA;
	l_bbd.get_cs_ok = 0;
	l_bbd.offset_cap = 0;/* 偏移电容 */
	l_bbd.offset_result = 0;/* 偏移电流测量结果 0默认 1失败 2合格 */
	
    l_bbd.port.num = type_spe.port_num;
    l_bbd.work_port.num = type_spe.work_port_num;
    
    if(PORT_NUM4 == type_spe.work_port_num)
    {
        l_bbd.work_port.ports[0].port1 = 2;
        l_bbd.work_port.ports[0].port2 = 2;
        l_bbd.work_port.ports[0].port3 = 2;
        l_bbd.work_port.ports[0].port4 = 2;
    }
    else if(PORT_NUM8 == type_spe.work_port_num)
    {
        l_bbd.work_port.ports[0].port1 = 2;
        l_bbd.work_port.ports[0].port2 = 2;
        l_bbd.work_port.ports[0].port3 = 2;
        l_bbd.work_port.ports[0].port4 = 2;
        l_bbd.work_port.ports[0].port5 = 2;
        l_bbd.work_port.ports[0].port6 = 2;
        l_bbd.work_port.ports[0].port7 = 2;
        l_bbd.work_port.ports[0].port8 = 2;
    }
    
	p->one_step.bbd = l_bbd;
}

/*
 * 函数名：insert_node
 * 描述  ：插入节点
 * 输入  ：step插入点
 * 输出  ：无
 * 返回  ：无
 */
void insert_node(uint8_t step)
{
    NODE_STEP nod;
	NODE_STEP	*node = NULL;
	NODE_STEP	*cur = NULL;
	
	cur = free_list_steps;	/* 获取空步 */
	nod.one_step.com.step = step+1;//插入到当前步的后面
	init_acw_step(&nod);
    
	if(cur)
	{
		free_list_steps = cur->next;
		cur->one_step = nod.one_step;
	}
	else
	{
		return;//错误处理
	}
	
    
    if(NULL != list_99xx.head)
    {
        for(node = list_99xx.head; NULL != node; node = node->next)
        {
            if(node->one_step.com.step == step)
            {
                break;
            }
        }
    }
    else
    {
        list_99xx.head = cur;
    }
	
	cur->prev = node;
    
    if(NULL != node)
    {
        cur->next = node->next;
    }
    else
    {
        cur->next = NULL;
    }
    
    if(NULL != node)
    {
        if(node->next)
        {
            node->next->prev = cur;
        }
        else
        {
            list_99xx.tail = cur;
        }
        
        node->next = cur;
    }
    else
    {
        list_99xx.tail = cur;
    }
    
	for(node=cur->next; node; node = node->next)
	{
		node->one_step.com.step++;//将新节点后的节点的步数全部加1
	}
	
	list_99xx.size++;//总步数加1
}

/*
 * 函数名：insert_node
 * 描述  ：插入测试步
 * 输入  ：pos 插入的位置 
 *         mode 测试模式
 * 输出  ：无
 * 返回  ：无
 */
void insert_step(uint8_t pos, uint8_t mode)
{
	NODE_STEP node;
    uint16_t l_index;
    int16_t i = 0;
    uint16_t free_step = TABLE_VALUE_NULL;
    
    if(pos > (MAX_STEPS-1))
    {
        return;
    }
    
    if(g_cur_file == NULL)
    {
        return;
    }
    
    if(g_cur_file->total == 0)
    {
        clear_step_used_flag();
    }
    
    for(i = 0; i < (MAX_STEPS/8+1); i++)
    {
        l_index = step_used_flag[i];
        
        if(l_index != 0xff)
        {
            free_step = cs99xx_maptbl[l_index];
            step_used_flag[i] |= (1<<free_step);/* 将标志置一 */
            free_step = free_step + i * 8;/* 获取空步 */
            break;
        }
    }
    
    if(free_step == TABLE_VALUE_NULL)
    {
        return;
    }
    
//     memcpy(&cur_group_table[pos + 1], &cur_group_table[pos], (g_cur_file->total - pos) * 2); //莫名其妙出问题
//     cur_group_table[pos] = free_step;
    
    for(i = g_cur_file->total; i > pos; i--)
    {
        cur_group_table[i] = cur_group_table[i - 1];
    }
    
    cur_group_table[pos] = free_step;
    
    g_cur_file->total++;
    
    node.one_step.com.step = pos + 1;
	node.one_step.com.mode = mode;
    
    init_mode(&node);/* 初始化刚刚插入的步 */
    save_one_step(&node, node.one_step.com.step);/* 保存 */
}

/*
 * 函数名：del_step
 * 描述  ：删除测试步
 * 输入  ：step 要删除的步骤号
 * 输出  ：无
 * 返回  ：无
 */
void del_step(uint8_t step)
{
    uint16_t l_index = TABLE_VALUE_NULL;
    uint16_t l_index1 = 0;
    uint16_t pos = step - 1;
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
    if(g_cur_file->total == 1)
    {
        return;
    }
    
    if(g_cur_step == NULL)
    {
        return;
    }
    
    l_index = cur_group_table[pos];
    l_index1 = l_index / 8;
    
    if(l_index == TABLE_VALUE_NULL)
    {
        return;
    }
    
    step_used_flag[l_index1] &= ~(1<<(l_index%8));/* 将标志清零 */
    
    memcpy(&cur_group_table[pos], &cur_group_table[pos + 1], (g_cur_file->total - step) * 2);
    cur_group_table[g_cur_file->total - 1] = TABLE_VALUE_NULL;
    
    g_cur_file->total--;
}
/*
 * 函数名：del_all_steps
 * 描述  ：删除所有测试步
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void del_all_steps(void)
{
    int32_t i = 0;
    int32_t num = g_cur_file->total;
    
    for(i = 0; i < num; i++)
    {
        del_step(i + 1);
    }
}

/*
 * 函数名：swap_step
 * 描述  ：交换两个测试步数据
 * 输入  ：one 第一个步号
 *         two 第二个步号
 * 输出  ：无
 * 返回  ：无
 */
void swap_step(const uint16_t one, const uint16_t two)
{
    uint16_t temp = 0;
    uint16_t l_addr1 = TABLE_VALUE_NULL;
    uint16_t l_addr2 = TABLE_VALUE_NULL;
    
    if(one == two)
    {
        return;
    }
    
    if(one > g_cur_file->total || one == 0)
    {
        return;
    }
    
    if(two > g_cur_file->total || two == 0)
    {
        return;
    }
    
    l_addr1 = cur_group_table[one - 1];
    l_addr2 = cur_group_table[two - 1];
    
    if(l_addr1 == TABLE_VALUE_NULL || l_addr2 == TABLE_VALUE_NULL)
    {
        return;
    }
    
    /* 交换数据 */
    temp = l_addr1;
    l_addr1 = l_addr2;
    l_addr2 = temp;
    
    /* 把交换后的映射数据保存到映射表中 */
    cur_group_table[one - 1] = l_addr1;
    cur_group_table[two - 1] = l_addr2;
    
    save_group_table(g_cur_file->num);/* 把更改过的映射表写入到存储器 */
}

/*
 * 函数名：check_step_exist_for_comm
 * 描述  ：检查测试步是否存在
 * 输入  ：pos 要检查的步骤号
 * 输出  ：0 存在
 *         -1 存储信息出错
 *         -2 步骤号非法
 * 返回  ：无
 */
int32_t check_step_exist_for_comm(uint16_t pos)
{
    uint16_t l_addr = TABLE_VALUE_NULL;
    
    if(pos > MAX_STEPS)
    {
        return -2;
    }
    
    l_addr = cur_group_table[pos - 1];
    
    if(l_addr == TABLE_VALUE_NULL)
    {
        return -1;
    }
    
    return 0;
}
/*
 * 函数名：position_step
 * 描述  ：定位指定的测试步
 * 输入  ：pos 要定位的步骤号
 * 输出  ：无
 * 返回  ：定位到的步节点数据结构指针 如果为NULL则传入的参数pos非法
 */
NODE_STEP *position_step(uint16_t pos)
{
    uint16_t l_addr = TABLE_VALUE_NULL;
    
    if(pos > MAX_STEPS)
    {
        return NULL;
    }
    
    l_addr = cur_group_table[pos - 1];
    
    if(l_addr != TABLE_VALUE_NULL)
    {
        load_steps_to_list(pos);
    }
    
	return list_99xx.head;
}

/*
 * 函数名：get_group_total_step
 * 描述  ：获取指定记忆组的总步数
 * 输入  ：note_num 记忆组编号
 * 输出  ：无
 * 返回  ：步骤数
 */
uint32_t get_group_total_step(uint16_t note_num)
{
    return file_table[note_num % MAX_FILES].total;
}

uint16_t get_cur_file_total(void)
{
    return g_cur_file->total;
}

uint8_t check_file_exist(const uint8_t n)
{
    if(n >= MAX_FILES)
    {
        return 0;
    }
    
	return (file_table[n].name[0] != 0);
}

TEST_FILE *pos_file(const uint8_t n)
{
	if(check_file_exist(n) == 0)
	{
		return NULL;
	}
    
	return &file_table[n];
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
