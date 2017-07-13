/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：GUI_Draw.c
 * 摘  要  ：包含服务界面的主要函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
 /********************* 包含头文件 ***********************/
#define	 MENU_GLOBAL


#include	<stdio.h>
#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "GUI_Draw.h"
#include    "math.h"
#include    "mb_cmd_define.h"



/************************* 全局变量初始化 ******************/
const uint8_t *unit_pool[]=
{
	"","mV","V","kV","~A","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","~F",
};
const uint8_t *unit_pool_[]=
{
	"","mV","V","kV","uA","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","uF",
};
const uint8_t *mode_pool[][10]=
{
	{"", "ACW", "DCW", "IR ", "GR ", "BBD", "CC "},
	{"", "ACW", "DCW", "IR ", "DV ", "BBD", "CC "},
};

const uint8_t *opt_pool[]=
{
    "","电压", "电流", "电阻", "电容",
};

const char *work_mode_buf[]=
{
	"G", "N",
};//G_MODE

const char *offset_res_buf[4]=
{
	" ", "F", "P", "-"
};

const char* status_str[][2] = 
{
	{"等待测试", "TestWait"},
	{"电压上升", "  Rise  "},
	{"正在测试", "Testing "},
	{"电压下降", "  Fall  "},
	{"电压缓变", "Change  "},
	{"间隔等待", "Interval"},
	{"测试合格", "  PASS  "},
	{"测试失败", "TestFail"},
	{"正在充电", " Charge "},
	{"稳压时间", "Sta.Time"},
	{"正在放电", "Discharg"},
	{"放电结束", "Disch.OK"},
	{"输出延时", "Out.Dly"},
};

const char* except_buf[][3]=
{
	{"","",""},
	{"上限报警", "High Fail", "High"   },
	{"下限报警", "Low Fail" , "Low"    },
	{"真实报警", "Real Fail", "Real"   },
	{"充电报警", "Char Fail", "Charge" },
	{"开路报警", "Open Fail", "Open"   },
	{"短路报警", "ShortFail", "Short"  },
	{"ARC报警" , "ARC Fail" , "ARC"    },
	{"GFI报警" , "GFI Fail" , "GFI"    },
	{"功放报警", "AMP Fail" , "AMP"    },
	{"档位报警", "GEAR Fail", "GEAR"   },
	{"电压异常", "VOL. Fail", "VOL."   },
	{"未定义"  , "Undefine ", "Undef." },
};
const char* sys_sw_status_buf[][2] =
{
	{"GFI:DISABLE"	,"GFI:ENABLE "},
	{"ARC:DISABLE" 	,"ARC:ENABLE "},
	{" HV:ZERO"		," HV:HIGH   "},
};
/* IR显示电阻时换档时使用的 */
const char* res_infinite_str[]=
{
    "-----","-.---","--.--","---.-","-.---","--.--","---.-"
};

MORE_OPTIONS_STR cc_cur_gear_option_pool[]=
{
    {{"高", "High"},PX,0,CC_CUR_HIGH,NULL,NULL},
    {{"中", "Mid."},PX,0,CC_CUR_MID ,NULL,NULL},
    {{"低", "Low "},PX,0,CC_CUR_LOW ,NULL,NULL},
};

MORE_OPTIONS_STR ir_filtering_deep_option_pool[]=
{
    {{"快", "Fast"},PX,0,IR_FILTERING_DEEP_FAST,NULL,NULL},
    {{"中", "Mid."},PX,0,IR_FILTERING_DEEP_MID ,NULL,NULL},
    {{"慢", "slow "},PX,0,IR_FILTERING_DEEP_SLOW ,NULL,NULL},
};

MORE_SEL_STR mor_sel_menu[]=
{
    {0, PX/*x*/,0/*y*/,{0, 50, 50 + 40, 0, 50, 50 + 40, 0}},/* ACW电流档位 */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* DCW电流档位 */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* GR电流档位 */
    {0, PX/*x*/,0/*y*/,{0, 28, 28 + 28, 28 + 28 + 23, (28 + 28 + 23 + 23), 0}},/* 模式设置 */
    {0, SX/*x*/,0/*y*/,{0, SOX, 0, SOX, 0, SOX, 0, SOX, 0}},/* 系统失败模式设置 */
};

const uint8_t* fail_mode_pool[]=
{
    "STOP"      ,
    "HALT"      ,
    "CON."      ,
    "RESTART"   ,
    "NEXT"      ,
    "FPDFC"     ,
};

MORE_OPTIONS_STR plc_signal_option_pool[]=
{
    {{"单步", "EachStep"},SX,0,EACH_STEP,NULL,NULL},
    {{"总步", "AllSteps"},SX,0,ALL_STEP ,NULL,NULL},
    {{"保持", "Hold"    },SX,0,TEST_HOLD,NULL,NULL},
};


MORE_OPTIONS_STR amp_type_option_pool[]=
{
    {{"8833", "8833"} ,SX,0,AMP_8833,NULL,NULL},
    {{"Old", "Old"  } ,SX,0,AMP_OLD ,NULL,NULL},
};
MORE_OPTIONS_STR par_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, PAR_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, PAR_MEDIUM_FLASH ,NULL,NULL},
};
MORE_OPTIONS_STR res_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, RES_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, RES_MEDIUM_FLASH ,NULL,NULL},
};
MORE_OPTIONS_STR cal_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, CAL_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, CAL_MEDIUM_FLASH ,NULL,NULL},
};


const OUTPUT_IMPEDAN_OPTS_T output_impedan_opt[]=
{
    {
        {
            "0 - 0.00kΩ",
            "1 - 22.4kΩ",
            "2 - 7.60kΩ",
            "3 - 4.00kΩ",
        },
        4,
    },
    {
        {
            "0 - 4.40kΩ",
            "1 - 2.80kΩ",
            "2 - 1.10kΩ",
        },
        3,
    },
};

DATA_INFO menu_par[]=
{
	/************* COM ************/
	{// 1   测试步骤
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,4/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"测试步骤","Step"},{"","",""},set_step,0/*no_dis_bits*/,GUI_COM_STEP,UDSB,"99  ","01  ",
	},
	
	{// 2   测试模式
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		1,0,{"测试模式","Mode"},{"","ACW","DCW","IR","GR","BBD"},set_mode,0/*no_dis_bits*/,GUI_COM_MODE,UDRB,
	},
	
	{// 3   延时时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,3,{"延时时间","Delay"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_DELAY_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 4   上升时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"上升时间","Rise"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_RISE_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
    
	{// 5   稳压时间
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"稳压时间","Stabili."},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_STAB_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 6   测试时间
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"测试时间","Test"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_TEST_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 7	下降时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"下降时间","Fall"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_FALL_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 8	间隔时间 \ 缓变时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,0,{"间隔时间","Interval"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_INTER_T,UDRB,"999.9s","000.0s",0,0,1/*rang_flag*/,
	},
	
	{// 9 步间PASS
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"步间PASS","StepPass"},{"是","否","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_PASS,UDRB,"","",35,3,
	},
	
	{// 10 步间连续
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"步间连续","StepSer."},{"是","否","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_CONT,UDRB,"","",35,3,
	},
	
	{// 11 输出频率
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		0,10,{"输出频率","Freq."},{"","Hz",""},set_frequency,0/*no_dis_bits*/,GUI_COM_OUTPUT_FREQ,UDRB,"   9  ","   1  ",0,0,1/*rang_flag*/,
	},
	
	{// 12	测试端口
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,STRUCT_T/*type*/,0/*lon*/,0/*decs*/,8/*total*/,1/*cur*/,0/*ch_bit*/,6/*page*/,4/*bits*/,0,
		0,10,{"测试端口","Port"},{"",""},set_all_ports,0/*no_dis_bits*/,GUI_COM_PORT,UDRB,"","",40,3
	},
    
	{// 13 电流偏移
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,0,{"电流偏移","Offset"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_COM_CUR_OFFSET,UDRB,
	},
	
	{// 14 电阻偏移
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,0,{"电阻偏移","Offset"},{"mΩ",""},set_float_par,0/*no_dis_bits*/, GUI_COM_RES_OFFSET,UDRB,
	},
	
	/************* ACW ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_ACW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 电流档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电流档位","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_ACW_CUR_GEAR,UDRB,
	},
	
	{// 3 电流上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流上限","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_H,UDRB,
	},
	
	{// 4 电流下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流下限","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_L,UDRB,
	},
	
	{// 5 真实电流
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"真实电流","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_REAL_CUR,UDRB,
	},
	
	{// 6 电弧侦测
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"电弧侦测","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_ARC_SUR,UDRB,
	},
    
	/************* DCW ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_DCW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 电流档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电流档位","Range"},{"",""},set_dcw_cur_gear,0/*no_dis_bits*/, GUI_DCW_CUR_GEAR,UDRB,
	},
	
	{// 3 电流上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流上限","High"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_H,UDRB,
	},
	
	{// 4 电流下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流下限","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_L,UDRB,
	},
	
	{// 6 充电电流
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"充电电流","Charge_I"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CHARGE_CUR,UDRB,
	},
    
	{// 7 电弧侦测
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"电弧侦测","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_ARC_SUR,UDRB,
	},
    
	{// 8 输出阻抗
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		3,0,{"输出阻抗","Impedan"},{"","0 - 0.00kΩ","1 - 22.4kΩ","2 - 7.60kΩ","3 - 4.00kΩ"},set_float_par,0/*no_dis_bits*/, GUI_DCW_OUTPUT_IMPEDANCE,UDRB,"","",0,0,2/*rang_flag*/,
	},
    
	/************* IR ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_IR_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 自动换档
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"自动换档","Auto.Lev"},{"是","否","YES","NO"},set_either_or_par,0/*no_dis_bits*/, GUI_IR_AUTO_GEAR,UDRB,"","",35,3,
	},
	
	{// 3 电阻上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,0,{"电阻上限","High"},{"MΩ","MΩ"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_H,UDRB,"09999MΩ","00000MΩ",
	},
	
	{// 4 电阻下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,1,{"电阻下限","Low"},{"MΩ","MΩ"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_L,UDRB,"09999MΩ","00000MΩ",0,0,0/*rang_flag*/,
	},
    
	/************* GR ************/
	{// 1 电压档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电压档位","Gear"},{"","","20mV","20mV","200mV","200mV","2000mV""2000mV","12V","12V"},set_gr_vol_gear,0/*no_dis_bits*/,GUI_GR_VOL_GEAR,UDRB,
	},
	
	{// 2 输出电流
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"输出电流","Cur."},{"A","mA",""},set_gr_output_cur,1/*no_dis_bits*/,GUI_GR_OUTPUT_CUR,UDRB,
	},
	
	{// 3 电阻上限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电阻上限","High"},{"mΩ","Ω",""},set_gr_res_upper,0/*no_dis_bits*/,GUI_GR_RES_H,UDRB,"510.0mΩ","001.0mΩ",
	},
	
	{// 4 电阻下限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电阻下限","Low"},{"mΩ","Ω",""},set_gr_res_lower,0/*no_dis_bits*/,GUI_GR_RES_L,UDRB,
	},
	
	{// 5 电压上限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电压上限","High"},{"mV","V",""},set_gr_vol_upper,0/*no_dis_bits*/,GUI_GR_VOL_H,UDRB,"00.00V","00.00V",
	},
	
	{// 6 电压下限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电压下限","Low"},{"mV","v",""},set_gr_vol_lower,0/*no_dis_bits*/,GUI_GR_VOL_L,UDRB,"00.00V","00.00V",
	},
	
	{// 7 测试模式
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		1,0,{"测试方法","Method"},{"连续","脉冲","Cont.","Pulse"},set_gr_test_method,0/*no_dis_bits*/,GUI_GR_TEST_METHOD,UDRB,"","",40,3
	},
    
	/************* BBD************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_BBD_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 开路检测
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		100,10,{"开路检测","Open"},{"%","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_OPEN_CHECK,UDRB,"  100%","  10%",0,0,1/*rang_flag*/,
	},
	
	{// 3 短路检测
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"短路检测","Short"},{"%","v",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_SHORT_CHECK,UDRB," 500%","0,100%",0,0,1/*rang_flag*/,
	},
	
	{// 4 电容上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"电容上限","High"},{"nF","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_H,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	{// 5 电容下限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"电容下限","Low"},{"nF","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_L,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	/************* CC ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_CC_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 电流档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电流档位","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_CC_CUR_GEAR,UDRB,
	},
	
	{// 3 电流上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流上限","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_H,UDRB,
	},
	
	{// 4 电流下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流下限","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_L,UDRB,
	},
	
	{// 5 真实电流
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"真实电流","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_REAL_CUR,UDRB,
	},
	
	{// 6 电流强度
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"电流强度","CurInten."},{"中","Mid"},set_option_par,0/*no_dis_bits*/, GUI_CC_CUR_INTENSITY,UDRB,
	},
    
	/************* SYS ************/
	{//	1 液晶对比度
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,1,{"液晶对比度","Contrast"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CONTRAST,UDRB,"(1-9)",""
	},
	
	{//	2 余量提示
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,0,{"余量提示","RemainHint"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_REMAIN_HINT,UDRB,"(0-9)",""
	},
	
	{//	3 结果保存
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"结果保存","ResultSave"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_RES_SAVE,UDRB,"","",SOX,SOY,
	},
	
	{//	4 溢出覆盖
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"溢出覆盖","OverCover"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_VOER_COVER,UDRB,"","",SOX,SOY,
	},
	
	{//	5 GFI保护
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"GFI 保护","GFI Prot."},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_GFI_PROT,UDRB,"","",SOX,SOY,
	},
	
	{//	6 自检允许
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"自检允许","SelfCheck"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SELF_CHECK,UDRB,"","",SOX,SOY,
	},
	
	{//	7 语言选择
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"语言选择","Language"},{"中文","英文","Chinese","English",},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LANGUAGE,UDRB,"","",SOX,SOY,
	},
	
	{//	8 失败继续
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"失败继续","FailCont."},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_CONT,UDRB,"","",SOX,SOY,
	},
	
	{//	9 列表显示
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"列表显示","ListDisplay"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LIST_DIS,UDRB,"","",SOX,SOY,
	},
	
	{//	10 测试端口
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"测试端口","TestPort"},{"接地","浮地","GND","Float"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_PORT,UDRB,"","",SOX,SOY,
	},
    
	{//	11 测试信号
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"测试信号","TestSignal"},{"电平","触点","Level","Point"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_SIGNAL,UDRB,"","",SOX,SOY,
	},
	
	{//	12 蜂鸣开关
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"蜂鸣开关","Buzzer SW."},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_BUZZER_SW,UDRB,"","",SOX,SOY,
	},
	
	{//	13  检验日期
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,STRING_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,3/*bits*/,0,
		0,10,{"检验日期","CheckDate"},{"2015-1-1 12:12:12",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CHECK_DATE,UDRB,"","",SOX,SOY,
	},
	
	{//	14  编号规则
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		2,0,{"编号规则","NO.Rule"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_NO_RULE,UDRB,"(0-2)","",SOX,SOY,
	},
	
	{//	15 失败模式
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"失败模式","FailMode"},{"STOP","STOP","Yes","No"},set_option_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_MODE,UDRB,"","",SOX,SOY,
	},
    
	{//	16 静音开关
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"静音开关","Silent SW."},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SILENT_SW,UDRB,"","",SOX,SOY,
	},
    
	{//	17 PLC 信号
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"PLC 信号","PLC Signal"},{"单步","总步","EachStep","AllSteps"},set_plc_option_sys_par,0/*no_dis_bits*/, GUI_SYS_PLC_SIGNAL,UDRB,"","",SOX,SOY,
	},
    
	{//	18 输出延时
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,0,{"输出延时","Out.Delay"},{"s (0.0-999.9s)","s (0.0-999.9s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_OUTPUT_DELAY,UDRB,"(0.0-999.9s)","",SOX,SOY,
	},
    
	{//	19 档位保持
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,200,{"IR档位保持","IRGearHold"},{"s (0.2-9.999s)","s (0.2-9.999s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_IR_GEAR_HOLD,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	{//	20 IR 测试速度
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		9999,200,{"IR测试速度","IR SPEED"},{"",""},set_option_par,0/*no_dis_bits*/, GUI_SYS_IR_SPEED,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	/************* CUSTOM(定制参数) ************/
	{//	1 功放选择
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"功放选择","8833AMP"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_SELECT,UDRB,"","",SOX,SOY,
	},
	{//	2 功放类型
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"功放类型","AMP_TYPE"},{"PWM","LINE","PWM","LINE"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_TYPE,UDRB,"","",SOX,SOY,
	},
	{//	3 参数存储介质
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"参数存储","PAR_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_PAR_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	4 结果存储介质
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"结果存储","RES_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_RES_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	5 校准存储介质
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"校准存储","CAL_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_CAL_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	6  蜂鸣器使能
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"蜂鸣器使能","BUZZER-EN"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_BUZZER_EN,UDRB,"","",SOX,SOY,
	},
	{//	7  静音开关
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"静音开关","MUTE-SW"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_MUTE_SW,UDRB,"","",SOX,SOY,
	},
	{//	8  导入导出
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"导入导出","LeadingSW"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_LEADING_SW,UDRB,"","",SOX,SOY,
	},
	{//	9  换挡延时
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR换挡保持","IR_DELAY"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_G_HOLD_SW,UDRB,"","",SOX,SOY,
	},
	{//	10  IR测试速度
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR测试速度","IR_SPEED"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_SPEED_SW,UDRB,"","",SOX,SOY,
	},
	
	{//	11  偏移设置
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"偏移设置","OFFSET_SW."},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_OFFSET_SW,UDRB,"","",SOX,SOY,
	},
};


void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info);
/*************************** 函数定义 *************************/
uint8_t * mystrcat(uint8_t *buf, uint8_t *str1, uint8_t *str2)
{
    uint8_t t_buf1[20] = {0};
    uint8_t t_buf2[20] = {0};
    
    strcpy((char*)t_buf1, (const char*)str1);
    strcpy((char*)t_buf2, (const char*)str2);
    buf[0] = 0;
    
    strcat((char*)buf, (const char*)t_buf1);
    strcat((char*)buf, (const char*)t_buf2);
    
    return buf;
}

void front_add_space_at_string(uint8_t *buf, uint8_t c)
{
}

void init_test_gui_area_info(void)
{
}
/*
 * 函数名：draw_colon
 * 描述  ：在设置页面下方显示冒号
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void draw_colon(void)
{
}

/*
 * 函数名：save_par_to_array
 * 描述  ：将参数值更新到一个数组中
 * 输入  ：data_info 菜单项数据结构
 *         value 有更新的数据
 *         array_num 数组成员的个数
 * 输出  ：无
 * 返回  ：无
 */
void save_par_to_array(DATA_INFO *data_info, uint32_t value, uint8_t array_num)
{
}
/*
 * 函数名：save_par_to_ram
 * 描述  ：将参数值更新的内存
 * 输入  ：data_info 菜单项数据结构
 *         value 有更新的数据
 * 输出  ：无
 * 返回  ：无
 */
void save_par_to_ram(DATA_INFO *data_info, uint32_t value)
{
}

/*
 * 函数名：draw_one_par_info
 * 描述  ：显示一个菜单项的值
 * 输入  ：p 菜单项数据结构
 *         pdata 显示的数据
 *         unit  单位
 * 输出  ：无
 * 返回  ：无
 */
void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit)
{
}

/*
 * 函数名：read_par_to_buf
 * 描述  ：读出指定菜单项的数据
 * 输入  ：data_info
 * 输出  ：value 菜单项数据 如果为NULL就不赋值
 * 返回  ：菜单项数据
 */
uint32_t read_par_to_buf(DATA_INFO* data_info, uint32_t *value)
{
    return 0;
}

/*
 * 函数名：load_this_page_info
 * 描述  ：加载当前设置页面的数据项信息
 * 输入  ：无
 * 输出  ：data_info
 * 返回  ：无
 */
void load_this_page_info(DATA_INFO* data_info[])
{
}
/*
 * 函数名：load_this_page_value
 * 描述  ：把加载到内存数组的信息显示出来
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void load_this_page_value(void)
{
}

/*
 *去除前导零
 */
char* div_str_pre_zero(char *str)
{
    uint8_t len = strlen(str);
    uint32_t i = 0;
    char *p = str;
    
    for(i = 0; i < len - 1; i++)
    {
        if(p[0] == '0' && p[1] != '.')
        {
            p++;
        }
        else
        {
            break;
        }
    }
    
    return p;
}


/*
 * 函数名：ten_power
 * 描述  ：10的幂
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
uint32_t ten_power(u8 n)
{
	u8 i;
	uint32_t temp = 1;
	for(i = 0; i < n; i++)
	{
		temp *= 10;
	}
	return temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//函数：mysprintf                                                                                /
//参数type :153 表示数据是浮点数 1带前导零0不带前导零 最大长度为5包括小数点，3位小数 最大长度不能超过5，小数位不能超过3 /
//    unit:为浮点数后追加的单位字符串                                                            /
//    value:要转换为浮点型的整数数据                                                             /
//////////////////////////////////////////////////////////////////////////////////////////////////
void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value)
{
    uint8_t decs_n = (type / 1) % 10;/* 小数位 */
    uint8_t lon    = (type / 10) % 10;/* 长度 */
    uint8_t app_0  = (type / 100) % 10;/* 前导零标志 */
    uint8_t int_n = 0;
    uint8_t temp = 0;
    uint8_t temp_l = 0;
    int32_t i = 0;
    int32_t j = 0;
    
    if(buf == NULL)
    {
        return;
    }
    
    if(lon > 7 || decs_n > 3)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    
    if(lon == 4 && decs_n > 2)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    if(lon == 3 && decs_n > 1)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    if(lon == 2 && decs_n > 0)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    
    int_n =  lon - decs_n - (decs_n > 0);
    
    for(i = 0, j = 0; i < int_n; i++)
    {
        if(decs_n > 0)
        {
            temp = (value / ten_power(lon - 2 - i)) % 10;
        }
        else
        {
            temp = (value / ten_power(lon - 1 - i)) % 10;
        }
        
        /* 去除前导零 */
        if(app_0 == 0)
        {
            /* 最后一个整数位 */
            if(int_n - i == 1)
            {
                buf[j++] = temp + '0';
            }
            /* 不是最后一个整数位 */
            else
            {
                if(temp || j)
                {
                    buf[j++] = temp + '0';
                }
            }
        }
        /* 不去除前导零 */
        else
        {
            buf[j++] = temp + '0';
        }
    }
    
    if(decs_n)
    {
        buf[j++] = '.';
    }
    
    temp_l = j;
    
    /* 小数部分 */
    for(i = 0; i < decs_n; i++)
    {
        temp = (value / ten_power(decs_n - i - 1)) % 10;
        buf[i + temp_l] = temp + '0';
    }
    
    buf[i + temp_l] = 0;
    
    /* 追加后缀信息 */
    if(unit != NULL)
    {
        strcat((char*)buf, (const char*)unit);
    }
}
/*
 * 函数名：cursor_pos
 * 描述  ：显示屏幕光标 会清除掉上一次显示的光标
 * 输入  ：光标的坐标 将其坐标定义为有符号类型为了可以传入负值坐标，就是将光标移出屏幕
 * 输出  ：无
 * 返回  ：无
 */
void cursor_pos(int16_t x, int16_t y)
{
}
void dis_cursor_pos(void)
{
}
void cursor_pos_n(int16_t x, int16_t y)
{
}
/*
 * 函数名：shift_cursor_l
 * 描述  ：更新坐标的位置 并显示出来
 * 输入  ：p 指向当前设置参数的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void dis_cursor(DATA_INFO * p)
{
}
/*
 * 函数名：shift_cursor
 * 描述  ：光标循环右移
 * 输入  ：p 指向当前设置参数的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void shift_cursor (DATA_INFO * p)
{
}

/*
 * 函数名：shift_cursor_l
 * 描述  ：光标循环左移
 * 输入  ：p 指向当前设置参数的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void shift_cursor_l (DATA_INFO * p)
{
}

/*
 * 函数名：change_data
 * 描述  ：更新数据
 * 输入  ：base 正在操作的位的最新值0-9
 * 输出  ：无
 * 返回  ：无
 */
void change_data(uint8_t base)
{
}
/*
 * 函数名：my_sprintf
 * 描述  ：更新数据到显示器
 * 输入  ：p 指向当前操作数据的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void my_sprintf(char *buf, DATA_INFO * p, uint32_t data)
{
	int32_t i = 0;
	int32_t j = 0;
    uint8_t t = 0;
    
    uint8_t temp1 = p->lon;
    uint8_t temp2 = p->decs;
    
    if(p->decs != 0)
    {
        temp1 = p->lon - p->decs - 1;
    }
    
    //整数部分
    for (i = 0; i < temp1; i++)
    {
        t = (data / ten_power(temp1 + temp2 - i - 1)) % 10;
        buf[i] = '0' + t;
    }
    //小数点
    if(temp2)
    {
        buf[i++] = '.';
    }
    //小数部分
    for(j = 0; j < temp2; j++)
    {
        t = (data / ten_power(temp2 - j - 1)) % 10;
        buf[i++] = '0' + t;
    }
    //单位
    strcat(buf, (const char*)p->unit[0]);
}
/*
 * 函数名：update_data
 * 描述  ：更新数据到显示器
 * 输入  ：p 指向当前操作数据的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void update_data(DATA_INFO * p)
{
}
/*
 * 函数名：clear_
 * 描述  ：只清屏不显示结果 防止屏幕闪烁
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void clear_(void)
{
}

/*
 * 函数名：clear_menu
 * 描述  ：清除屏幕右侧的菜单栏 只清屏不显示结果 防止屏幕闪烁
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void clear_menu(void)
{
}
void clear_menu_n(uint8_t n)
{
}

///////////////////////////////////////////////////////////////////


/*
 * 函数名：gui_par_inval
 * 描述  ：说明参数设置无效
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_par_inval(void)
{
}

/*
 * 函数名：gui_par_inval
 * 描述  ：在g模式时有的参数有特殊要求在特定情况下不能设置的
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_g_mode_inval(void)
{
}
/*
 * 函数名：ratio_broken
 * 描述  ：校准系数丢失，请送修
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void ratio_broken(void)
{
}
/*
 * 函数名：cal_warning
 * 描述  ：校准开关打开后提示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cal_warning(void)
{
}
/*
 * 函数名：auto_cal_pass
 * 描述  ：自动校准通过
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void auto_cal_pass(void)
{
}
/*
 * 函数名：rate_of_advance
 * 描述  ：完成进度
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void rate_of_advance(void)
{
}
/*
 * 函数名：type_mismatch
 * 描述  ：机型不匹配是提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void type_mismatch(void)
{
}
/*
 * 函数名：dis_set_port
 * 描述  ：设置界面显示端口状态
 * 输入  ：val 状态值 n 位置 ports 端口数目 4 8
 * 输出  ：无
 * 返回  ：无
 */
void dis_set_port(const uint8_t val,const uint8_t n, const uint8_t ports)
{
}
/*
 * 函数名：ports_cursor_pos
 * 描述  ：设置界面设置端口时光标的显示位置
 * 输入  ：n 位置 ports 端口数目 4 8
 * 输出  ：无
 * 返回  ：无
 */
void ports_cursor_pos(uint8_t n, uint8_t ports)
{
}
void gui_dis_float_at(const float v, const uint8_t decs, const uint8_t x, const uint8_t y)
{
}
/*
 * 函数名：dcw_cur_sel_gui
 * 描述  ：dcw 电流档位选择用的界面服务程序
 * 输入  ：kinds 电流档位的种类 n 第几个电流档
 * 输出  ：无
 * 返回  ：无
 */
void dcw_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
}

/*
 * 函数名：acw_gr_hz_sel_gui
 * 描述  ：频率设置时的 显示服务程序
 * 输入  ：kinds 频率的种类 n 第几个档 gear_buf 档位的名称缓冲
 * 输出  ：无
 * 返回  ：无
 */
void acw_gr_hz_sel_gui(uint8_t kinds, uint8_t n, const uint8_t **gear_buf)
{
}
/*
 * 函数名：acw_cur_sel_gui
 * 描述  ：acw 电流选择界面服务程序
 * 输入  ：kinds 电流档位的种类 n 第几个电流档
 * 输出  ：无
 * 返回  ：无
 */
void ac_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
}
/*
 * 函数名：sys_option_sel_gui
 * 描述  ：acw 电流选择界面服务程序
 * 输入  ：kinds 电流档位的种类 n 第几个电流档
 * 输出  ：无
 * 返回  ：无
 */
void sys_option_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
}

void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info)
{
}

void draw_bitmap_circle(uint8_t x, uint8_t y, uint8_t st)
{
}
/*
 * 函数名：draw_more_options_gui
 * 描述  ：显示多选项，以及当前选中项
 * 输入  ：list 多选项的链表
 *         iteration 当前的选项
 * 输出  ：无
 * 返回  ：无
 */
void draw_more_options_gui(MORE_OPTIONS_LIST *list, MORE_OPTIONS_STR *iteration)
{
}

/*
 * 函数名：gr_vol_sel_gui
 * 描述  ：GR 直流压降模式下 选择电压档位
 * 输入  ：kinds 电流档位的种类 n 第几个电档
 * 输出  ：无
 * 返回  ：无
 */
void gr_vol_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
}

/*
 * 函数名：mode_sel_gui
 * 描述  ：选择模式界面显示服务程序
 * 输入  ：kinds 模式的种类 n 位置
 * 输出  ：无
 * 返回  ：无
 */
void mode_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
}
/*
 * 函数名：dcgr_commuincation_err
 * 描述  ：直流gr开机通信失败提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dcgr_commuincation_err(void)
{
}
/*
 * 函数名：cal_sw_on_warning
 * 描述  ：校准开关打开后就报警提示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cal_sw_on_warning(void)
{
}

void draw_wait_usb_connect_gui(void)
{
}
void draw_auto_calibration_gui(void)
{
}
void draw_no_connect_usb_gui(void)
{
}
/*
 * 函数名：GUI_Draw_build_connect_to_board
 * 描述  ：开机建立通信的提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_build_connect_to_board(void)
{
}
void my_refresh_lcd24064(void)
{
}
/*
 * 函数名：GUI_Draw_build_connect_fail
 * 描述  ：通信连接失败提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_build_connect_fail(void)
{
}
/*
 * 函数名：gui_draw_dc_gr_module_comm_fail
 * 描述  ：直流模块通信异常
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_dc_gr_module_comm_fail(void)
{
}

void gui_draw_dc_gr_module_being_in_comm(void)
{
}


/*
 * 函数名：GUI_Draw_self_check
 * 描述  ：开机自检
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_self_check(void)
{
}

/*
 * 函数名：GUI_Draw_main
 * 描述  ：画出主界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_main(void)
{
}

/*
 * 函数名：MenuTitle
 * 描述  ：更新标题栏的信息 设置界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
void MenuTitle(u8 which)
{
}
/*
 * 函数名：syn_test_menu_title
 * 描述  ：更新标题栏的信息 测试界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
void syn_test_menu_title(void)
{
}

/*
 * 函数名：draw_port_for_set
 * 描述  ：画出端口数据信息 设置界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
void draw_port_for_set(TEST_PORT *p)
{
}

/*
 * 函数名：dis_port
 * 描述  ：更新端口状态信息 测试界面使用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_port(void)
{
}

/*
 * 函数名：gui_draw_set_sys_cur_limit
 * 描述  ：设置支路电流时说明
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_set_sys_cur_limit(void)
{
}
/*
 * 函数名：draw_short_cut_bm
 * 描述  ：右边快捷键栏 图标显示函数
 * 输入  ：type 根据不同的类型决定显示那些图标
 * 输出  ：无
 * 返回  ：无
 */
void draw_short_cut_bm(uint8_t type)
{
}

/*
 * 函数名：draw_short_cut_str
 * 描述  ：右边快捷键栏 文字显示
 * 输入  ：str1 str2 str3 str4 分别代表4个快捷项
 * 输出  ：无
 * 返回  ：无
 */
void draw_short_cut_str(uint8_t *str1, uint8_t *str2, uint8_t *str3, uint8_t *str4)
{
}

/*
 * 函数名：draw_menu_frame_line
 * 描述  ：画出菜单框架线 并附带画一条直线 坐标通过参数出入
 * 输入  ：x1 x2 y 指定这条直线的坐标 如果 x1>x2即不再画这条线
 * 输出  ：无
 * 返回  ：无
 */
void draw_menu_frame_line(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2)
{
}
/*
 * 函数名：gui_draw_menu_sys
 * 描述  ：菜单设置界面下菜单项
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_menu_sys(MENU_INFO* p_sys_menu, uint8_t lon)
{
}

/*
 * 函数名：gui_draw_menu
 * 描述  ：显示一个最基本的界面框架 cs99xx_type.c用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_menu(void)
{
}

void draw_custom_type_gui(void)
{
}
/*
 * 函数名：gui_draw_help_1
 * 描述  ：帮助菜单1
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_help_1(void)
{
}

/*
 * 函数名：gui_draw_help_2
 * 描述  ：帮助菜单1
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_help_2(void)
{
}

/*
 * 函数名：gui_draw_help_3
 * 描述  ：帮助菜单1
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_help_3 (void)
{
}


/*
 * 函数名：gui_draw_file_management
 * 描述  ：文件管理界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_file_management(void)
{
}

/*
 * 函数名：gui_draw_communication_interface
 * 描述  ：通信界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_communication_interface(MENU_INFO* pmenu_info)
{
}

/*
 * 函数名：gui_draw_communication_plc
 * 描述  ：PLC通信开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_communication_plc(void)
{
}
/*
 * 函数名：gui_draw_communication_agreement
 * 描述  ：通信开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_communication_agreement(void)
{
}

/*
 * 函数名：gui_draw_communication_agreement
 * 描述  ：通信开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_password_manage(void)
{
}

/*
 * 函数名：gui_draw_time_manage
 * 描述  ：系统时间设置界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_time_manage(void)
{
}

/*
 * 函数名：gui_draw_offset
 * 描述  ：偏移测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_offset(void)
{
}
/*
 * 函数名：gui_draw_bbd
 * 描述  ：BBD测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_bbd(void)
{
}

/*
 * 函数名：gui_draw_test_result
 * 描述  ：测试结果显示界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_test_result(void)
{
}

/*
 * 函数名：gui_draw_keyboard_lock_manage
 * 描述  ：键盘锁显示界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_keyboard_lock_manage(void)
{
}

/*
 * 函数名：gui_draw_recover
 * 描述  ：恢复默认值显示界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_recover(void)
{
}

/*
 * 函数名：gui_draw_test
 * 描述  ：测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_test (void)
{
}

/*
 * 函数名：gui_drow_test_syn
 * 描述  ：多路同步测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_drow_test_syn (void)
{
}



/*
 * 函数名：prompt_recover_pw
 * 描述  ：当用户按下ENTER+1组合键时提示用户系统密码已经还原为默认值 该函数被help_subtask,main_task调用
 * 输入  ：uint8_t s 区分在不同状态下给用户的一些提示信息
 * 输出  ：无
 * 调用函数：GUI_SetFont，GUI_DispStringAt，GUI_DrawLine，GUI_ClearRect，
 *			OSQPend，GUI_DrawBitmap
 * 被调函数：test_subtask
 */
void prompt_recover_pw(void)
{
}

/*
 * 函数名：dis_program_info
 * 描述  ：显示烧录文件的时间信息
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_program_info(void)
{
}

void dis_interface_info(void)
{
}

/*
 * 函数名：dis_scan_modules_gui
 * 描述  ：显示扫描模块界面信息
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_scan_modules_gui(void)
{
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
