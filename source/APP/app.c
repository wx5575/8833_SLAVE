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
#define   APP_GLOBALS 

#include    "string.h"
#include    "stdio.h"
#include    "app.h"
#include	"BSP_config.h"
#include	"keyboard.h"
#include	"RTC_Config.h"
#include 	"DAC_VREF.h"
#include	"irq.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include 	"mb_server.h"

#include    "mb.h"
#include    "mb_m.h"
#include    "mb_cmd_define.h"
#include    "mb_master.h"
#include    "user_mb_app.h"
#include    "mbrtu.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_update.h"
#include    "cs99xx_it.h"
#include    "cs99xx_collect.h"
#include    "cs99xx_result.h"
#include    "cs99xx_plc.h"
#include    "auto_calibration.h"
#include    "dc_module.h"
#include 	"spi_flash.h"
#include    "module_manage.h"
#include    "send_test_data_fpga.h"


/***************************全局变量*************************/
uint8_t plain_code = 0; /* 密码明码开关 1为明码 */
uint8_t gunlock = 0;	/* 当打开键盘锁时，将该值置1，一回操作就不用再输入密码了 */

void main_task(void* p_arg);
void keyscan_task(void* p_arg);
void sample_task(void* p_arg);

void schedule_task(void *p_arg);
void modbus_polling_task(void *p_arg);

/*
 * 函数名：get_cal_pin
 * 描述  ：获取键盘状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：键盘状态
 */
uint8_t get_cal_pin(void)
{
    uint8_t res = 0;
    
    if(g_custom_sys_par.cal_medium == CAL_MEDIUM_FLASH)
    {
        res =  (CAL_PIN == SET);
    }
    else
    {
        res =  (CAL_PIN == RESET);
    }
    
    return res;
}
/*
 * 函数名：get_key_task_state
 * 描述  ：获取键盘状态
 * 输入  ：无
 * 输出  ：无
 * 返回  ：键盘状态
 */
uint8_t get_key_task_state(void)
{
	return key_status;
}
/*
 * 函数名：off_keyboard
 * 描述  ：关闭键盘
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void off_keyboard(void)
{
	INT8U err = 0;
	key_status = 1;
	OSTaskSuspend(Keyscan_TASK_PRIO);
	while(NULL != OSQAccept(KeyboardQSem, &err));
}

/*
 * 函数名：on_keyboard
 * 描述  ：打开键盘
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void on_keyboard(void)
{
	INT8U err = 0;
	key_status = 0;
	OSTaskResume(Keyscan_TASK_PRIO);
	while(NULL != OSQAccept(KeyboardQSem, &err));
}

/*
 * 函数名：clear_keyboard
 * 描述  ：清空按键缓冲
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void clear_keyboard(void)
{
	INT8U err = 0;
	while(NULL != OSQAccept(KeyboardQSem, &err));
}

/*
 * 函数名：off_sample_task
 * 描述  ：关闭采样任务
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void off_sample_task(void)
{
    set_sample_rate(0);/* 停止采样 */
    OSTaskSuspend(SAMPLE_TASK_PRIO);
}

void disable_sample_task(void)
{
    test_flag.sample_task_en = 0;
}
/*
 * 函数名：on_sample_task
 * 描述  ：打开采样任务
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void on_sample_task(void)
{
    test_flag.sample_task_en = 1;
}

void resume_sample_task(void)
{
	OSTaskResume(SAMPLE_TASK_PRIO);
    OSTimeDlyResume(SAMPLE_TASK_PRIO);
}

/*
 * 函数名：off_schedule_task
 * 描述  ：关闭调度任务
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void off_schedule_task(void)
{
    while(OSSemAccept(ScheduleSem));
	OSTaskSuspend(SCHEDULE_TASK_PRIO);
}

/*
 * 函数名：on_schedule_task
 * 描述  ：打开调度任务
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void on_schedule_task(void)
{
    while(OSSemAccept(ScheduleSem));
	OSTaskResume(SCHEDULE_TASK_PRIO);
}

/*
 * 函数名：mdbus_init
 * 描述  ：MODBUS初始化函数，并使能，配置串口
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void mdbus_init(void)
{
    p_prvvUARTRxISR = xMBRTUReceiveFSM;//串口中断处理函数
    p_prvvUARTTxReadyISR = xMBRTUTransmitFSM;
    
    /* 从机 */
	eMBInit(MB_RTU, 0x01, 2, 115200,  MB_PAR_EVEN);
	eMBEnable();
    /* 主机 */
    eMBMasterInit(MB_RTU, 5, 115200,  MB_PAR_EVEN);
	eMBMasterEnable();
}

/*
 * 函数名：soft_init
 * 描述  ：软件初始化
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void soft_init(void)
{
	init_file_table(); /* 初始化文件表 */
	init_99xx_list(); /* 将test_group初始化为一个仅有一个步的链表 */
	g_cur_file = &default_file; /* 暂时将当前文件指向默认的文件，既开机后的默认文件，后面要改为最后一次使用的文件 */
	init_ratio_all(); /* 初始化校准参数 */
    
    mdbus_init();/* mdbus 初始化 */
}
/*
 * 函数名：startup_task
 * 描述  ：起始任务
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void startup_task(void* p_arg)
{
void test_md5(void);
    test_md5();
    bsp_init();/* 硬件初始化 */
	soft_init();/* 软件初始化 */
    
	#if(OS_TASK_STAT_EN>0)
		OSStatInit();//使能ucos 的统计任务
	#endif
	/* 创建主任务 */
	while(OS_ERR_NONE != OSTaskCreateExt(main_task,
					(void*)0,
					&Main_Task_STK[MAIN_TASK_STK_SIZE-1],
					Main_TASK_PRIO,
					Main_TASK_ID,
					&Main_Task_STK[0],
					MAIN_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
	OSTaskDel(OS_PRIO_SELF);  //删除启动任务
}

/*
 * 函数名：schedule_task
 * 描述  ：测试调度任务
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void schedule_task(void *p_arg)
{
	INT8U err = 0;
	
	while(1)
	{
		/* 等待信号量 */
		OSSemPend(ScheduleSem, 0, &err);
        
		if(STOP)
		{
			continue;
		}
        
		if(test_irq_fun != NULL)
		{
            if(test_flag.gradation == STAGE_TEST)
            {
                if(ERR_NUM_OVER != ERR_NONE && g_dis_time + 1 == tes_t)
                {
                    close_test_timer();
                    g_dis_time = tes_t;
                }
            }
            
            test_irq_fun();
		}
        
        /* 测试灯闪烁 */
        if(test_flag.gradation != STAGE_INTER)
        {
            if(test_flag.test_led_flag)
            {
                LED_TEST = !LED_TEST;
            }
        }
        else
        {
            LED_TEST = LED_OFF;
        }
	}
}


/*
 * 函数名：sample_task
 * 描述  ：采样任务
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void sample_task(void* p_arg)
{
    off_sample_task();//关闭采样任务
    
    while(1)
    {
        if(test_flag.sample_task_en)
        {
            OSTimeDlyHMSM(0,0,0,app_flag.sample_cycle);//控制采样率
            adc_sample();
        }
        else
        {
            off_sample_task();//关闭采样任务
        }
    }
}

/*
 * 函数名：keyscan_task
 * 描述  ：按键扫描任务
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void keyscan_task(void* p_arg)
{
	off_keyboard();	/* 关闭键盘 */
	
	while(1)
	{
		report_key_value();/* 周期性扫描按键 */
		OSTimeDlyHMSM(0,0,0,5);
	}
}


/*
 * 函数名：modbus_polling_task
 * 描述  ：MODBUS监听任务
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void modbus_polling_task(void *p_arg)
{
    init_module_manage_env();
    
	while(1)
	{
        module_comm_task();
        send_slave_test_data_fpga();
        OSTimeDlyHMSM(0,0,0,20);
   	}
}
void modbus_master_polling_task(void *p_arg)
{
	while(1)
	{
        eMBMasterPoll();
        OSTimeDlyHMSM(0,0,0,10);
   	}
}
void clear_mdbus_send_queue(void)
{
    uint8_t err = 0;
    
    OSMutexPend(MdbusMutex, 0, &err);
    while(NULL != OSQAccept(MdbusQSem, &err));
    OSMutexPost(MdbusMutex);
    
    test_flag.mb_master_timeout = 0;
}
/*
 * 函数名：modbus_master_send_task
 * 描述  ：modbus主机发送任务
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void modbus_master_send_task(void* p_arg)
{
    #define RETRY_TIMES_MAX   2 /* 最大重发次数 */
    MB_PACK_T *mb_pack;
    uint16_t retry_times = 0;
    uint16_t count = 0;
    uint8_t err = 0;
    
	while(1)
	{
        test_flag.mb_break_send = MB_MASETER_SEND_END;
        OSMutexPend(MdbusMutex, 0, &err);
		mb_pack = OSQPend(MdbusQSem,10,&err);
        OSMutexPost(MdbusMutex);
        
        if(test_flag.mb_break_send == MB_MASETER_SEND_BREAK)/* 打断 */
        {
            clear_mdbus_send_queue();
            test_flag.mb_break_send = MB_MASETER_SEND_END;
            continue;
        }
        
        test_flag.mb_break_send = MB_MASETER_SENDING;
        
        if(mb_pack != NULL)
        {
			app_flag.dc_gr_task_send_idle = 0;
            OSTimeDlyHMSM(0,0,0,10);
            test_flag.mb_syn = 0;
            retry_times = 0;
            
            do
            {
                mb_send_cmd(mb_pack);
                /* modbus不需要等待回应因为发送的是广播指令 */
                if(mb_pack->type == MB_NO_WAIT_ACK)
                {
                    break;
                }
                
                for(count = 0; count < 80; count++)
                {
                    OSTimeDlyHMSM(0,0,0,10);
                    
                    if(test_flag.mb_break_send == MB_MASETER_SEND_BREAK)/* 打断 */
                    {
                        clear_mdbus_send_queue();
                        break;
                    }
                    
                    if(test_flag.mb_syn == 1)
                    {
                        break;
                    }
                }
            }while(++retry_times <= RETRY_TIMES_MAX && test_flag.mb_syn == 0);
            
            
            if(retry_times == RETRY_TIMES_MAX + 1 && test_flag.mb_syn == 0)
            {
                test_flag.mb_master_timeout = 1;
            }
        }
		else
		{
			app_flag.dc_gr_task_send_idle = 1;
		}
   	}
}

/*
 * 函数名：create_other_task
 * 描述  ：创建其他任务
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void create_other_task(void)
{
//    /* 创建键盘扫描任务 */
//	while(OS_ERR_NONE != OSTaskCreateExt(keyscan_task,
//					(void*)0,
//					&Keyscan_Task_STK[KEY_TASK_STK_SIZE-1],
//					Keyscan_TASK_PRIO,
//					Keyscan_TASK_ID,
//					&Keyscan_Task_STK[0],
//					KEY_TASK_STK_SIZE,
//					(void*)0,
//					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
//					));
    /* 测试调度任务 */
	while(OS_ERR_NONE != OSTaskCreateExt(schedule_task,
					(void*)0,
					&Startup_Task_STK[SCHEDULE_TASK_STK_SIZE-1],
					SCHEDULE_TASK_PRIO,
					SCHEDULE_TASK_ID,
					&Startup_Task_STK[0],
					SCHEDULE_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
    /* 采样任务 */
	while(OS_ERR_NONE != OSTaskCreateExt(sample_task,
					(void*)0,
					&Sample_Task_STK[SAMPLE_TASK_STK_SIZE-1],
					SAMPLE_TASK_PRIO,
					SAMPLE_TASK_ID,
					&Sample_Task_STK[0],
					SAMPLE_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
    /* MODBUS监听任务 */
 	while(OS_ERR_NONE != OSTaskCreateExt(modbus_polling_task,
 					(void*)0,
 					&Modbus_Send_STK[MODBUS_SEND_STK_SIZE-1],
 					MODBUS_SEND_TASK_PRIO,
 					MODBUS_SEND_TASK_ID,
 					&Modbus_Send_STK[0],
 					MODBUS_SEND_STK_SIZE,
 					(void*)0,
 					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
 					));
// 	while(OS_ERR_NONE != OSTaskCreateExt(modbus_master_polling_task,
// 					(void*)0,
// 					&Modbus_M_Send_STK[MODBUS_M_SEND_STK_SIZE-1],
// 					MODBUS_M_SEND_TASK_PRIO,
// 					MODBUS_M_SEND_TASK_ID,
// 					&Modbus_M_Send_STK[0],
// 					MODBUS_M_SEND_STK_SIZE,
// 					(void*)0,
// 					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
// 					));
//    /* MODBUS主机发送任务 */
// 	while(OS_ERR_NONE != OSTaskCreateExt(modbus_master_send_task,
// 					(void*)0,
// 					&Modbus_Master_Send_STK[MODBUS_MASTER_SEND_STK_SIZE-1],
// 					MODBUS_MASTER_SEND_TASK_PRIO,
// 					MODBUS_MASTER_SEND_TASK_ID,
// 					&Modbus_Master_Send_STK[0],
// 					MODBUS_MASTER_SEND_STK_SIZE,
// 					(void*)0,
// 					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
// 					));
}

/*
 * 函数名：exe_new_status
 * 描述  ：执行当前状态操作
 * 输入  ：exe_status 传入新的状态
 * 输出  ：exe_status 输出新的状态
 * 返回  ：无
 */
void exe_cur_status(EXE_STATUS_ENUM *exe_status)
{
    EXE_STATUS_ENUM  t_exe_status = EXE_NULL;
    
    if(NULL == exe_status)
    {
        return;
    }
    
    t_exe_status = *exe_status;
    *exe_status = EXE_NULL;
    
    switch(t_exe_status)
    {
        case RE_MAIN://重绘主界面
        {
            clear_();   /* 清屏 */
            gui_draw_main();/* 绘出主界面 */
            break;
        }
        case INTO_TEST://进入测试界面
        {
            off_rtc_int();/* 关闭RTC中断 */
            
            app_flag.into_test_ui = 1;/* 置位 */
            test_subtask();/* 进入 测试子任务 */
			plc_signal_cmd(PLC_SIG_EXIT);/* PLC状态更新为未就绪 */
            app_flag.into_test_ui = 0;/* 清除标志 */
            on_rtc_int();/* 打开RTC 时钟 */
            
            /* 判断是否要进入测试界面 */
            if(test_flag.into_set)
            {
                test_flag.into_set = 0;
                g_return = 0;
                g_exit = 0;
                *exe_status = INTO_SET;//进入设置界面
            }
            /* 判断是否再次进入测试程序 */
            if(test_flag.into_test)
            {
                test_flag.into_test = 0;
                g_return = 0;
                g_exit = 0;
                *exe_status = INTO_TEST;//进入测试界面
            }
            break;
        }
        case INTO_SET://进入设置界面
            set_subtask(0);/* 进入设置界面 0表示从主界面进入，如果是1表示从测试界面进入 区别请看 set_subtask 函数 */
            break;
        case INTO_HELP://进入帮助界面
            help_subtask();
            break;
        case INTO_MENU://进入菜单界面
            menu_subtask();
            cursor_pos(-5,0);//将光标移出屏幕 防止光标残留位置会对界面显示造成
            break;
        case INTO_AUTO_CAL:
            app_flag.calibration = 1;/* 进入校准模式 */
            auto_calibration();//进入校准程序
            app_flag.calibration = 0;/* 退出校准模式 */
            *exe_status = RE_MAIN;/* 重绘主界面 */
            break;
        case INTO_SCAN_MODULES:
            break;
        default:
            Time_Show(20,40);//刷新时间
            break;
    }
}

/*
 * 函数名：main_task
 * 描述  ：主任务，负责参数设置，测试等主要工作
 * 输入  ：void* p_arg 没有用
 * 输出  ：无 
 * 返回  ：无 
 */
void main_task(void* p_arg)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int16_t start_delay = 0;
    EXE_STATUS_ENUM exe_status = RE_MAIN;
    
    exit_off();/* 关闭中断 */
    read_custom_par();/* 读出定制参数 必须先读定制参数在读其他参数 */
    check_type();/* 检查硬件型号 */
	init_par_info();/* 检查完型号后就对界面参数信息初始化 */
    create_other_task();/* 创建其他任务 */
    
	
	/* 从存储器中读数据如果出错就不要进入测试界面而是进入主界面 */
	if(0 != read_paramenter())
	{
		TimeDisplay = 1;/* 更新时间显示标志位 */
        on_keyboard();	/* 打开键盘 */
	}
    /* 从存储器中读取数据没有出错 */
    else
    {
        if(DC_GR_EN == ENABLE)
        {
            get_dc_gr_cal_info();
        }
        
        cur_node = *g_cur_step;//加载当前步数据到全局变量cur_node中
        init_par_info_list(&cur_node);/* 模式参数链表初始化 */
        
        clear_();	/* 清屏 */
        Time_Show(20,40); /* 显示时间 */
        gui_draw_main();  /* 显示主界面 */
        
        while(1)
        {
            Time_Show(20,40);
            
            /* 开机时按着复位键就不会进入测试界面 */
            if(STOP_PIN == 0)
            {
                on_keyboard();/* 打开键盘 */
                break;
            }
            
            /* 开机延时等待直接进入测试界面 */
            if(++start_delay > 6000)
            {
                on_keyboard();/* 打开键盘 */
                exe_status = INTO_TEST;//进入测试界面
                break;
            }
        }
    }
    
    rtc_init(); /* RTC初始化 注意:之所以在这里才初始化rtc因为,将rtc放在bsp_init中时默认时间总是设置失败
                     经试验，确定为初始化需要一段时间延时，由于这段延时太长了，界面会卡顿，因此发在这里 */
    
	while(1)
	{
        /* 清除标志 */
		if(g_return || g_exit)
		{
			g_return = 0;
			g_exit = 0;
			exe_status = RE_MAIN;/* 重绘主界面 */
		}
        
        /* 检查进入设置界面的标志 */
        if(test_flag.into_set)
        {
            test_flag.into_set = 0;
            exe_status = INTO_SET;//进入设置界面
        }
        
        /* 检查进入测试界面的标志 */
        if(test_flag.into_test)
        {
            test_flag.into_test = 0;
            exe_status = INTO_TEST;//进入测试界面
        }
        
        /* 检查进入自动校准的标志 */
        if(test_flag.into_auto_cal_ui)
        {
            test_flag.into_auto_cal_ui = 0;
            exe_status = INTO_AUTO_CAL;
        }
        
        exe_cur_status(&exe_status);//执行新的界面状态
        
		pkey_value = OSQAccept(KeyboardQSem, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_F1:/* 进入设置界面 */
                exe_status = INTO_SET;
                break;
			case KEY_F2:/* 进入菜单界面 */
                exe_status = INTO_MENU;
				break;
			case KEY_F3:/* 进入测试界面 */
                exe_status = INTO_TEST;
				break;
			case KEY_F4:/* 帮助界面 */
                exe_status = INTO_HELP;
				break;
			case KEY_ENTER & KEY_0:/* 中英文切换 */
				sys_par.language = !sys_par.language;
				save_sys_par();
				exe_status = RE_MAIN;/* 重绘主界面 */
				break;
			case KEY_ENTER & KEY_1:/* 恢复系统默认密码 */
				strcpy(sys_par.pass_word, PASS_WORD);
				save_sys_par();
				prompt_recover_pw();
				exe_status = RE_MAIN;/* 重绘主界面 */
				break;
			case KEY_0 & KEY_1:/* 初始化eep 需要重启 */
				if(!get_cal_pin())
				{
					break;
				}
                
				sys_flag.mem_init = 0;
				save_sys_flag();/* 保存系统标志 */
				break;
			case KEY_F4 & KEY_8:/* 进入校准 */
				break;
			case KEY_F4 & KEY_9:/* 仪器型号选择 */
#ifndef BOOT_LOADER
				if(!get_cal_pin())
				{
					break;
				}
				select_type();
#endif
				break;
			case KEY_F4 & KEY_5:/* 检验日期更新 */
				sys_check_date();
				break;
			case KEY_0 & KEY_2:/* 重启系统 */
				if(!get_cal_pin())
				{
					break;
				}
				NVIC_SystemReset();
				while(1);
			case KEY_0 & KEY_3:/* 检查程序编译日期信息 */
				if(!get_cal_pin())
				{
					break;
				}
				dis_program_info();
                exe_status = RE_MAIN;/* 重绘主界面 */
                break;
			case KEY_0 & KEY_4:/* 查看系统异常信息日志 */
				if(!get_cal_pin())
				{
					break;
				}
				dis_exception_log();
                exe_status = RE_MAIN;/* 重绘主界面 */
                break;
			case KEY_0 & KEY_5:/* 自动校准 */
				if(!get_cal_pin())
				{
					break;
				}
                exe_status = INTO_AUTO_CAL;
                break;
			case KEY_0 & KEY_6:/* 模块扫描界面 */
				if(!get_cal_pin())
				{
					break;
				}
                exe_status = INTO_SCAN_MODULES;
                break;
		}
	}
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
