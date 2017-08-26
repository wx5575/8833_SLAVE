/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_type.c
 * 摘  要  ：管理仪器的型号，直接定义了仪器应该有的属性
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#define _CS99xx_TYPE

static void init_speciality(void);
#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include    "cs99xx_struct.h"
#include	"cs99xx_type.h" 
#include    "cs99xx_type_config.h"
#include	"calibration.h"

#define __TEST			1
#define __TEST_TYPE		CS9912BX//CS9929AX_CC //CS9927LB//TSK7630//CS9933ET_1//CS9922BX//CS9922BX_DZ//CS9917A_8//CS9950CX //CS9917CX_20//

#define VERSION_NUMBER__    "2.20.73"	/* 版本号 */



#define CUSTOM_NAME_        "9923ECG" //定制机名称
#define CUSTOM_TYPE_        CS9923ECG  //定制机型号
#define CONFIG_TYPE_FUN_    config_CS9923ECG //定制机配置函数

#ifdef	RH8400AS_DZ
#define CUSTOM_NAME        "RH8400AS" //定制机名称
#define CUSTOM_TYPE        CS9933X  //定制机型号
#define CONFIG_TYPE_FUN    config_RH8400AS //定制机配置函数
#endif

#ifdef LICHUN_DZ
#define CUSTOM_NAME         "CS9906B_K" //定制机名称
#define CUSTOM_TYPE         CS9906B_K     //定制机型号
#define CONFIG_TYPE_FUN     config_cs9906b_k //定制机配置函数
#endif

#ifdef VTC9929_DZ   /* ACW DCW IR 8路 */
#define CUSTOM_NAME         "VTC9929" //定制机名称
#define CUSTOM_TYPE         VTC9929     //定制机型号
#define CONFIG_TYPE_FUN     config_VTC9929 //定制机配置函数
#endif

#ifdef TSK7630_DZ   /* ACW DCW IR 8路 */
#define CUSTOM_NAME         "TSK7630" //定制机名称
#define CUSTOM_TYPE         TSK7630     //定制机型号
#define CONFIG_TYPE_FUN     config_TSK7630 //定制机配置函数
#endif

#ifndef CUSTOM_NAME
#define CUSTOM_NAME         CUSTOM_NAME_    //定制机名称
#endif
#ifndef CUSTOM_TYPE
#define CUSTOM_TYPE         CUSTOM_TYPE_     //定制机型号
#endif
#ifndef CONFIG_TYPE_FUN
#define CONFIG_TYPE_FUN     CONFIG_TYPE_FUN_ //定制机配置函数
#endif

#define TYPE_SEL_HARD   0//1//     /* 通过硬件选择 0软件 1硬件 */

uint16_t get_arc_h(uint8_t mode);

static void init_speciality(void);


/* 如果是定制机则在版本号后面加-DZ */
#if CUSTOM_TYPE_EN
#define VERSION_EX      "-DZ"
#else
#define VERSION_EX
#endif

#define   VERSION_NUMBER  (VERSION_NUMBER__ VERSION_EX)
#if CUSTOM_TYPE_EN
const static TYPE_STR custom_type=
{
	/* 型号名称       版本	         标号(ID)        配置函数    */
    CUSTOM_NAME, VERSION_NUMBER, CUSTOM_TYPE, CONFIG_TYPE_FUN
};

#endif

uint8_t * get_version_number(void)
{
	return VERSION_NUMBER;
}
/*******************************************************************************/
/*******************************************************************************/
/* 版本信息定义 增删改查随心所欲 */
const static TYPE_STR type_pool[] __attribute__((at(TYPE_POOL_ADDR))) = {
	/* 型号名称       版本	         标号(ID)        配置函数    */
	{"DEBUG_TYPE" ,	VERSION_NUMBER, DEBUG_TYPE  , config_debug_type     },/* 调试使用 */
    
	{"CS9906A"	  ,	VERSION_NUMBER, CS9906A     , config_CS9906A        },/* 特点 直流GR 压降测试仪 直接显示压降 */
	{"CS9906B"	  ,	VERSION_NUMBER, CS9906B     , config_CS9906B        },/* 特点 直流GR 压降测试仪 直接显示压降 */
	{"CS9906B-K"  ,	VERSION_NUMBER, CS9906B_K   , config_CS9906B_K      },/* 特点 直流GR 压降测试仪 直接显示压降 */
    
	{"CS9912AX"   ,	VERSION_NUMBER, CS9912AX    , config_CS9912AX       },
	{"CS9912BX"   ,	VERSION_NUMBER, CS9912BX    , config_CS9912BX       },/* ACW DCW */
	{"CS9912BX-L" ,	VERSION_NUMBER, CS9912BX_L  , config_CS9912BX_L     },/* ACW DCW */
	{"CS9912BX-T" ,	VERSION_NUMBER, CS9912BX_T  , config_CS9912BX_T     },/* DCW 2kV 双触点变压器 */
	{"CS9912Y"    ,	VERSION_NUMBER, CS9912Y     , config_CS9912Y        },/* ACW DCW */
    
	{"CS9913AX"   ,	VERSION_NUMBER, CS9913AX    , config_CS9913AX       },/* ACW */
	{"CS9913BX"   ,	VERSION_NUMBER, CS9913BX    , config_CS9913BX       },/* ACW DCW */
    
	{"CS9914AM"	  ,	VERSION_NUMBER, CS9914AM    , config_CS9914AM       },/* ACW */
	{"CS9914AX-200", VERSION_NUMBER, CS9914AM   , config_CS9914AM       },/* ACW */
	{"CS9914AM-H" , VERSION_NUMBER, CS9914AM_H  , config_CS9914AM_H     },/* 单ACW  200mA 双触点变压器 */
	{"CS9914AX"   ,	VERSION_NUMBER, CS9914AX    , config_CS9914AX       },/* ACW */
	{"CS9914AX-1" ,	VERSION_NUMBER, CS9914AX_1  , config_CS9914AX_1     },/* ACW */
    
	{"CS9914BM"	  ,	VERSION_NUMBER, CS9914BM    , config_CS9914BM       },/* ACW DCW */
	{"CS9914BX"   ,	VERSION_NUMBER, CS9914BX    , config_CS9914BX       },/* ACW DCW */
	{"CS9914CX"   ,	VERSION_NUMBER, CS9914CX    , config_CS9914CX       },/* DCW */
	{"CS9914D"	  ,	VERSION_NUMBER, CS9914D     , config_CS9914D        },/* ACW */
	{"CS9914CX-C" ,	VERSION_NUMBER, CS9914CX_C  , config_CS9914CX_C     },/* DCW */
	{"CS9914DX"	  ,	VERSION_NUMBER, CS9914DX    , config_CS9914DX       },/* ACW */
	{"CS9914G"    ,	VERSION_NUMBER, CS9914G     , config_CS9914G        },/* ACW DCW */
	{"CS9914Y"    ,	VERSION_NUMBER, CS9914Y     , config_CS9914Y        },/* ACW DCW */
	
	{"CS9915AX"   ,	VERSION_NUMBER, CS9915AX    , config_CS9915AX       },/* ACW PWM */
    
	{"CS9916AX"   ,	VERSION_NUMBER, CS9916AX    , config_CS9916AX       },/* ACW */
	{"CS9916BX"   ,	VERSION_NUMBER, CS9916BX    , config_CS9916BX       },/* DCW */
	
	{"CS9917A-8"  ,	VERSION_NUMBER, CS9917A_8   , config_CS9917A_8      },/* ACW DCW 8路 */
	{"CS9919D"	  ,	VERSION_NUMBER, CS9919D     , config_CS9919D        },/* ACW DCW 8路 */
	{"CS9917AX"   ,	VERSION_NUMBER, CS9917AX    , config_CS9917AX       },/* ACW DCW 4路 */
	{"CS9917B"	  ,	VERSION_NUMBER, CS9917B     , config_CS9917B        },
	{"CS9917BX"   ,	VERSION_NUMBER, CS9917BX    , config_CS9917BX       },/* DCW 10kv 大电容测试 */
	{"CS9917CX"	  ,	VERSION_NUMBER, CS9917CX    , config_CS9917CX       },
	{"CS9917CX-20",	VERSION_NUMBER, CS9917CX_20 , config_CS9917CX_20    },/* DCW 测大电容漏电流 */
	{"CS9917CX-C" ,	VERSION_NUMBER, CS9917CX_C  , config_CS9917CX_C     },/* DCW 10kv 大电容测试 */
	{"CS9917DX"	  ,	VERSION_NUMBER, CS9917DX    , config_CS9917DX       },/* ACW DCW 10kV */
    
	{"CS9919A"    ,	VERSION_NUMBER, CS9919A     , config_CS9919A        },/* ACW DCW 8路 */
	{"CS9919A-8"  ,	VERSION_NUMBER, CS9919A_8   , config_CS9919A_8      },/* ACW DCW IR 8路 */
	{"CS9919AX"   ,	VERSION_NUMBER, CS9919AX    , config_CS9919AX       },/* ACW DCW 8路 */
	{"CS9919B"    ,	VERSION_NUMBER, CS9919B     , config_CS9919B        },/* ACW DCW 4路 */
    {"9919B-4"    ,	VERSION_NUMBER, CS9919B_4   , config_CS9919B_4      },
	{"CS9919BX"   ,	VERSION_NUMBER, CS9919BX    , config_CS9919BX       },/* ACW DCW 4路 */
	{"CS9919G"    ,	VERSION_NUMBER, CS9919G     , config_CS9919G        },/* ACW DCW 8路 */
    
	{"CS9920A"    ,	VERSION_NUMBER, CS9920A     , config_CS9920A        },/* ACW 20kV */
	{"CS9920B"    ,	VERSION_NUMBER, CS9920B     , config_CS9920B        },/* DCW 20kV */
	{"CS9930A-T"  ,	VERSION_NUMBER, CS9930A_T   , config_CS9930A_T      },/* ACW 20kV */
    
	{"CS9921BX"   ,	VERSION_NUMBER, CS9921BX    , config_CS9921BX       },/* ACW DCW GR 32A */
    
	{"CS9922BX"   ,	VERSION_NUMBER, CS9922BX    , config_CS9922BX       },/* ACW DCW IR 10G */
	{"CS9922BXJ"  ,	VERSION_NUMBER, CS9922BXJ   , config_CS9922BXJ      },/* ACW DCW IR 10G */
	{"CS9922BX-1" ,	VERSION_NUMBER, CS9922BX_1  , config_CS9922BX_1     },/* ACW DCW IR 1.5kV 10G */
// 	{"CS9922BX-DZ",	VERSION_NUMBER, CS9922BX_DZ , config_CS9922BX_DZ    },/* 绝缘电阻的上下限加一个小数点 */
    
	{"CS9922LB"   ,	VERSION_NUMBER, CS9922LB    , config_CS9922LB       },/* ACW DCW IR 锂电池安规测试仪 */
	{"CS9922CX"   ,	VERSION_NUMBER, CS9922CX    , config_CS9922CX       },/* ACW IR 10G */
	{"CS9922DX"   ,	VERSION_NUMBER, CS9922DX    , config_CS9922DX       },/* ACW DCW IR 10G */
	{"CS9922EX"   ,	VERSION_NUMBER, CS9922EX    , config_CS9922EX       },/* 特点 ACW DCW IR 10G */
	{"CS9922EXJ"  ,	VERSION_NUMBER, CS9922EXJ   , config_CS9922EXJ      },/* 特点 ACW DCW IR 10G */
	{"CS9922FX"   ,	VERSION_NUMBER, CS9922FX    , config_CS9922FX       },/* ACW DCW IR 50G */
	{"CS9922G"    ,	VERSION_NUMBER, CS9922G     , config_CS9922G        },/* ACW DCW IR 50G */
	{"CS9922G-1"  ,	VERSION_NUMBER, CS9922G_1   , config_CS9922G_1      },/* ACW DCW IR 50G */
	{"CS9922G-2"  ,	VERSION_NUMBER, CS9922G_2   , config_CS9922G_2      },/* 特点 ir 2.5kV 50G */
	{"CS2676CH"   ,	VERSION_NUMBER, CS2676CH    , config_CS2676CH       },/* 特点 ir 2.5kV */
	{"CS2676CH-1" ,	VERSION_NUMBER, CS2676CH_1  , config_CS2676CH_1     },/* 特点 ir 5kV */
	{"CS9927LB"   ,	VERSION_NUMBER, CS9927LB    , config_CS9927LB       },/* 特点 BBD 50-500V */
	
	{"CS9922H-50" , VERSION_NUMBER, CS9922H_50  , config_CS9922H_50     },/* 特点 ir 5kV */
	{"CS9922M"    ,	VERSION_NUMBER, CS9922M     , config_CS9922M        },/* ACW DCW IR 10G */
    
	{"CS9923"     ,	VERSION_NUMBER, CS9923      , config_CS9923         },/* ACW DCW 10KV IR 1kV 50G */
	{"CS9923CG"   ,	VERSION_NUMBER, CS9923CG    , config_CS9923CG       },/* ACW DCW 10KV IR 1kV 50G */
	{"CS9923G-1"  ,	VERSION_NUMBER, CS9923G_1   , config_CS9923G_1      },/* 特点 10kV变压器 ir 1.5kV 50G */
	{"CS9923G-2"  ,	VERSION_NUMBER, CS9923G_2   , config_CS9923G_2      },/* 特点 10kV变压器 ir 2.5kV 50G */
	{"CS9923G-3"  ,	VERSION_NUMBER, CS9923G_3   , config_CS9923G_3      },/* 特点 10kV变压器 ir   5kV 50G */
	{"CS9923-5"   ,	VERSION_NUMBER, CS9923_5    , config_CS9923_5       },/* 10kV ACW DCW IR 1kV 50G */
	{"CS9923G"	  ,	VERSION_NUMBER, CS9923G     , config_CS9923G        },/* 10kV DCW IR 1kV 50G */
	{"CS9923ECG"  ,	VERSION_NUMBER, CS9923ECG   , config_CS9923ECG      },/* 10kV ACW DCW IR 2.5kV 50G GR 60A 105mOhM*/
	{"CS9923ECG-1",	VERSION_NUMBER, CS9923ECG_1 , config_CS9923ECG_1    },/* 10kV ACW DCW IR 5kV 50G GR 60A 105mOhM*/
    
	{"CS9924"     ,	VERSION_NUMBER, CS9924      , config_CS9924         },/* ACW 测电池用 双触点变压器 */
	{"CS9924-200" ,	VERSION_NUMBER, CS9924_200  , config_CS9924_200     },/* ACW 测电池用 双触点变压器 */
	
	{"CS9929A"    ,	VERSION_NUMBER, CS9929A     , config_CS9929A        },
	{"CS9929A-4"  ,	VERSION_NUMBER, CS9929A_4   , config_CS9929A_4      },/* 特点 ACW DCW IR 4路 */
	{"CS9929A-8"  ,	VERSION_NUMBER, CS9929A_8   , config_CS9929A_8      },/* ACW DCW IR 8路 */
	{"CS9929AX-8" ,	VERSION_NUMBER, CS9929AX_8  , config_CS9929AX_8     },/* ACW DCW IR 8路 */
    
	{"CS9929AX"   ,	VERSION_NUMBER, CS9929AX    , config_CS9929AX       },/* ACW DCW IR */
	{"CS9929AX-CC",	VERSION_NUMBER, CS9929AX_CC , config_CS9929AX_CC    },/* ACW DCW IR */
	{"CS9929AX-T" ,	VERSION_NUMBER, CS9929AX_T  , config_CS9929AX_T     },/* ACW DCW IR CC */
	{"CS9929B"    ,	VERSION_NUMBER, CS9929B     , config_CS9929B        },/* 特点 ACW DCW IR 4路 */
	{"CS9929B-4"  ,	VERSION_NUMBER, CS9929B_4   , config_CS9929B_4      },/* ACW DCW IR 5G */
	{"CS9929BX"   ,	VERSION_NUMBER, CS9929BX    , config_CS9929BX       },/* ACW DCW IR 10G */
	{"CS9929CX"   ,	VERSION_NUMBER, CS9929CX    , config_CS9929CX       },/* ACW DCW IR 50G */
	{"CS9929EX"   ,	VERSION_NUMBER, CS9929EX    , config_CS9929EX       },/* ACW DCW IR 50G */
	{"CS9929EX-CC",	VERSION_NUMBER, CS9929EX_CC , config_CS9929EX_CC    },/* ACW DCW IR 50G */
	
	{"CS9933EG"   , VERSION_NUMBER, CS9933EG    , config_CS9933EG       },/* 特点 直流GR IR 1kV 50G */
	{"CS9933EG-1" ,	VERSION_NUMBER, CS9933EG_1  , config_CS9933EG_1     },/* 特点 IR 2.5kV 50G */
	{"CS9933EG-2" ,	VERSION_NUMBER, CS9933EG_2  , config_CS9933EG_2     },/* 特点 IR 5kV 50G */
	{"CS9933ET"	  ,	VERSION_NUMBER, CS9933ET    , config_CS9933ET       },/* 特点 IR 1kV 直流GR */
	{"CS9933ET-1" , VERSION_NUMBER, CS9933ET_1  , config_CS9933ET_1     },/* 特点 IR 1.5kV 直流GR */
	{"CS9933ET-2" , VERSION_NUMBER, CS9933ET_2  , config_CS9933ET_2     },/* 特点 IR 2.5V 直流GR */
	{"CS9933ET-3" , VERSION_NUMBER, CS9933ET_3  , config_CS9933ET_3     },/* 特点 IR 5kV 直流GR */
	{"CS9933G"    ,	VERSION_NUMBER, CS9933G     , config_CS9933G        },/* ACW DCW IR 50G GR 32A */
	{"CS9933G-4"  ,	VERSION_NUMBER, CS9933G_4   , config_CS9933G_4      },/* ACW DCW IR 50G GR 40A */
	{"CS9933X"    ,	VERSION_NUMBER, CS9933X     , config_CS9933X        },/* ACW DCW IR GR */
	{"CS9933X-100",	VERSION_NUMBER, CS9933X_100 , config_CS9933X_100    },/* ACW DCW IR 10G GR 32A */
    
	{"CS9939X"	  ,	VERSION_NUMBER, CS9939X     , config_CS9939X        },/* ACW DCW IR 10G ACGR 32A */
    
	{"CS9950CG"   ,	VERSION_NUMBER, CS9950CG    , config_CS9950CG       },/* 单GR AC_32A */
	{"CS9950CG-1" , VERSION_NUMBER, CS9950CG_1  , config_CS9950CG_1     },/* 单GR AC_40A */
	{"CS9950CX"   ,	VERSION_NUMBER, CS9950CX    , config_CS9950CX       },/* 单GR AC_40A */
	{"CS9950DX"   ,	VERSION_NUMBER, CS9950DX    , config_CS9950DX       },/* 单GR AC_60A */
	{"CS9950ED"   ,	VERSION_NUMBER, CS9950ED    , config_CS9950ED       },/* 单GR DC_100A */
	{"CS9933G+"   ,	VERSION_NUMBER, CS9933G_    , config_CS9933G_       },
	{"CS9950X"    ,	VERSION_NUMBER, CS9950X     , config_CS9950X        },
	
	{0},/* 结尾用空初始化 */
};

/**************************** 函数定义 *************************/
/*
 * 函数名：read_type_pin
 * 描述  ：读硬件的型号引脚
 * 输入  ：无
 * 输出  ：无
 * 返回  ：型号值
 */
uint16_t read_type_pin(void)
{
#if TYPE_SEL_HARD>0&&__TEST==0
	uint16_t data1 = 0;
	uint16_t data2 = 0;
// 	uint16_t data3 = 0;
	
	data1 = GPIO_ReadInputData(GPIOF);/* PIN 6 - 11 */
	data2 = GPIO_ReadInputData(GPIOC);/* PIN 7 8 */
// 	data3 = GPIO_ReadInputData(GPIOA);
	
	data1 >>= 6;
	data1 &= 0x03f;
	data2 >>= 1;
	data2 &= 0xc0;
// 	data3 &= 0x0100;
	data1 |= data2;
#endif
    
#if __TEST>0
	return __TEST_TYPE;
#else
    //通过软件选择
#if TYPE_SEL_HARD==0
    return *(uint16_t*)TYPE_NUM_ADDR;
#else
    //由硬件选择型号
	return data1;
    #endif
#endif
}

/*
 * 函数名：check_gr_mode
 * 描述  ：检查当前机型是否支持GR
 * 输入  ：无
 * 输出  ：无
 * 返回  ：0 不支持GR 非0 支持GR
 */
int32_t check_gr_mode(void)
{
	return ( MODEL_EN & __GR);
}
/*
 * 函数名：get_first_mode
 * 描述  ：找到当前机型下的首个模式 优先顺序：ACW DCW IR GR BBD
 * 输入  ：无
 * 输出  ：无
 * 返回  ：首个模式
 */
uint8_t get_first_mode(void)
{
	uint8_t mode = 0;
	
	if(MODEL_EN & __ACW)
	{
		mode = ACW;
	}
	else if(MODEL_EN & __DCW)
	{
		mode = DCW;
	}
	else if(MODEL_EN & __IR)
	{
		mode = IR;
	}
	else if( MODEL_EN & __GR)
	{
		mode = GR;
	}
	else if(MODEL_EN & __BBD)
	{
		mode = BBD;
	}
	else if(MODEL_EN & __CC)
	{
		mode = CC;
	}
	else
	{
		while(1); /* 出错 */
	}
	return mode;
}
/*
 * 函数名：judge_support_g_mode
 * 描述  ：判断是否支持G模式
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void judge_support_g_mode(void)
{
    type_spe.support_g_mode = 0;
    
	if(MODEL_EN & __ACW)
	{
        type_spe.support_g_mode = 1;
	}
	else if(MODEL_EN & __DCW)
	{
        type_spe.support_g_mode = 1;
	}
}
/*
 * 函数名：init_default_type
 * 描述  ：初始化默认机型
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_default_type(void)
{
	g_cur_type = &type_pool[CS9919AX];
	init_speciality();
    
    if(g_cur_type->config_fun)
    {
        g_cur_type->config_fun();
    }
}
/*
 * 函数名：init_speciality
 * 描述  ：初始化机型的特性参数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void init_speciality(void)
{
	switch(type_spe.acw_vol_range)
	{
        case ACW_500V:
			type_spe.acw_vol_h = 500;	/* 0.500kV */
            break;
        case ACW_1kV:
			type_spe.acw_vol_h = 1000;	/* 1.000kV */
            break;
		case ACW_5kV:
			type_spe.acw_vol_h = 5000;	/* 5.000kV */
			break;
		case ACW_10kV:
            type_spe.transformer_type = TRANS_10kV;/* 变压器类型 */
			type_spe.acw_vol_h = 9999;	/* 9.999kV */
			type_spe.acw_vol_l = 500;	/* 500V */
			break;
		case ACW_20kV:
            type_spe.transformer_type = TRANS_20kV;/* 变压器类型 */
			type_spe.acw_vol_h = 2000;	/* 20.00kV */
			break;
		case ACW_30kV:
            type_spe.transformer_type = TRANS_30kV;/* 变压器类型 */
			type_spe.acw_vol_h = 3000;	/* 20.00kV */
			break;
	}
	
	switch(type_spe.dcw_vol_range)
	{
        case DCW_500V:
			type_spe.dcw_vol_h = 500;	/* 0.500kV */
            break;
		case DCW_2kV:
			type_spe.dcw_vol_h = 2000;	/* 2.000kV */
			break;
		case DCW_6kV:
			type_spe.dcw_vol_h = 6000;	/* 6.000kV */
			break;
		case DCW_10kV:
            type_spe.transformer_type = TRANS_10kV;/* 变压器类型 */
			type_spe.dcw_vol_h = 9999;	/* 9.999kV */
			type_spe.dcw_vol_l = 500;	/* 500V */
			break;
		case DCW_20kV:
            type_spe.transformer_type = TRANS_20kV;/* 变压器类型 */
			type_spe.dcw_vol_h = 2000;	/* 20.00kV */
			break;
	}
	
	switch(type_spe.ir_vol_range)
	{
		case IR_1kV:
			type_spe.ir_vol_h = 1000;	/* 1.000kV */
			break;
		case IR_1_5kV:
			type_spe.ir_vol_h = 1500;	/* 1.500kV */
			break;
		case IR_2_5kV:
			type_spe.ir_vol_h = 2500;	/* 2.500kV */
			break;
		case IR_5kV:
			type_spe.ir_vol_h = 5000;	/* 5.000kV */
			break;
		case IR_10kV:
			type_spe.ir_vol_h = 9999;	/* 10.00kV */
			break;
	}
	
	if(type_spe.mode_en & __ACW)
	{
		type_spe.acw_arc_h = get_arc_h(ACW);
    }
    
    if(type_spe.mode_en & __DCW)
	{
		type_spe.dcw_arc_h = get_arc_h(DCW);
	}
	
	if(type_spe.hz_type == HZ_TYPE_CONT)
	{
		type_spe.freq_h = 4000;/* 连续模式下频率上限是400.0hz */
		type_spe.freq_l = 400;/* 连续模式下频率下限是040.0hz */
	}
	else
	{
		type_spe.freq_h = 9;
		type_spe.freq_l = 1;
	}
}

/*
 * 函数名：get_arc_h
 * 描述  ：获取电弧侦测上限值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
uint16_t get_arc_h(uint8_t mode)
{
	uint16_t arc_upper = 0;
    
	switch(mode)
	{
		case ACW:
			arc_upper = 2000;
			menu_par[GUI_ACW_ARC_SUR].decs = 2;
			break;
		case DCW:
			arc_upper = 2000;
			menu_par[GUI_DCW_ARC_SUR].decs = 2;
			break;
	}
    
    return arc_upper;
}

void judge_single_gr(void)
{
	if(type_spe.mode_en == __GR)
    {
        type_spe.single_gr = 1;
    }
}
void judge_amp_type(void)
{
    if(g_custom_sys_par.amp_type == PWM_AMP)
    {
        type_spe.amp_type = PWM_AMP;
    }
    else
    {
        type_spe.amp_type = LINE_AMP;
    }
}

void judge_u_leading_data_sw(void)
{
    if(g_custom_sys_par.leading_sw == SYS_SW_ON)
    {
        type_spe.interface_config = type_spe.interface_config | INTERFACE_USB_OUT | INTERFACE_USB_IN/**/;
    }
    else
    {
        type_spe.interface_config = type_spe.interface_config /*| INTERFACE_USB_OUT | INTERFACE_USB_IN*/;
    }
}
/*
 * 函数名：init_other_speciality
 * 描述  ：初始化其他特性
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_other_speciality(void)
{
	if(type_spe.mode_en & __ACW)
	{
        if(1 == is_acw_segment())
        {
            type_spe.vol_is_segment = VOL_SEGMENT_NO;
        }
        else
        {
            type_spe.vol_is_segment = VOL_SEGMENT_YES;
        }
    }
    else if(type_spe.mode_en & __DCW)
	{
        if(1 == is_dcw_segment())
        {
            type_spe.vol_is_segment = VOL_SEGMENT_NO;
        }
        else
        {
            type_spe.vol_is_segment = VOL_SEGMENT_YES;
        }
    }
    else if(type_spe.mode_en & __IR)
	{
        if(1 == is_ir_segment())
        {
            type_spe.vol_is_segment = VOL_SEGMENT_NO;
        }
        else
        {
            type_spe.vol_is_segment = VOL_SEGMENT_YES;
        }
    }
    else
    {
        type_spe.vol_is_segment = VOL_SEGMENT_NO;
    }
    
//    judge_amp_type();
//    judge_u_leading_data_sw();
//    
//    judge_support_g_mode();/* G模式支持判断 */
//    judge_single_gr();/* 单gr判断 */
}


char* get_custom_type_name(void)
{
    return (char*)((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->name;
}

void init_custom_type(void)
{
	static TYPE_STR type;
	
    if(is_custom_type() == 1)
    {
        type = (((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->type);
		type.ver = (char*)get_version_number();
		
		g_cur_type = &type;
    }
}
/*
 * 函数名：check_type
 * 描述  ：检查仪器的型号
 * 输入  ：无
 * 输出  ：无
 * 返回  ：-1 未找到匹配的机型 0 机型成功匹配
 */
int32_t check_type(void)
{
#if CUSTOM_TYPE_EN==0
	uint16_t type_num = 0;
	uint16_t size = sizeof(type_pool)/sizeof(type_pool[0]);
	int32_t i = 0;
#endif
	
    g_cur_type = NULL;
    
#if CUSTOM_TYPE_EN
    g_cur_type = &custom_type;
#else
    type_num = read_type_pin();
    
    if(size >= 2 && type_num >= CS99XX_T_END)
    {
        type_num = CS9919AX;
    }
    
    for(i = 0; i < size; i++)
	{
		if(type_pool[i].num == type_num)
		{
			g_cur_type = &type_pool[i];
            break;
        }
    }
#endif
    
	config_all_init();
    
    if(g_cur_type == NULL)
    {
        return -1;
    }
    
    if(g_cur_type->config_fun)
    {
        g_cur_type->config_fun();
    }
    else
    {
        config_default();
    }
    
    init_other_speciality();//调用config后的其余初始化
    init_custom_type();//仅仅改变名称的定制机初始化
    
    return 0;
}


/*
 * 函数名：draw_page_type
 * 描述  ：画出一页的机型表
 * 输入  ：s 
 * 输出  ：无
 * 返回  ：-1 未找到匹配的机型 0 机型成功匹配
 */
void draw_page_type(const int8_t s, const int8_t num)
{
}

int32_t judge_valid_type(const uint16_t type_num, const int8_t n)
{
	int32_t res = 0;
	
	if(type_pool[n].num == type_num)
	{
		res = 0;
	}
	else
	{
		res = -1;
	}
	
	return res;
}

void prompt_recover_type(const int8_t s)
{
}

void select_type(void)
{
}

uint32_t define_vol_kinds(uint8_t mode, const uint8_t **gear, uint8_t *flag)
{
	uint32_t kinds = 0;
	
	switch(mode)
	{
		case GR://GR_DC_20mV | GR_DC_200mV | GR_DC_2000mV;
		{
			if(type_spe.gr_vol_gear & GR_DC_20mV)
			{
				flag[kinds] = 1;
				gear[kinds++] = "20mV";
			}
            
			if(type_spe.gr_vol_gear & GR_DC_200mV)
			{
				flag[kinds] = 2;
				gear[kinds++] = "200mV";
			}
            
			if(type_spe.gr_vol_gear & GR_DC_2000mV)
			{
				flag[kinds] = 3;
				gear[kinds++] = "2000mV";
			}
			
			if(type_spe.gr_vol_gear & GR_DC_12V)
			{
				flag[kinds] = 4;
				gear[kinds++] = "12V";
			}
			
			if(type_spe.gr_vol_gear & GR_DC_12V_100mA)
			{
				flag[kinds] = 5;
				gear[kinds++] = "12V_100mA";
			}
			
			if(type_spe.gr_vol_gear & GR_DC_12V_1000mA)
			{
				flag[kinds] = 6;
				gear[kinds++] = "12V_1000mA";
			}
			
			break;
		}
	}
	return kinds;
}

uint32_t define_fail_mode_kinds(const uint8_t *fail_mode_buf[], uint8_t *flag)
{
	uint32_t kinds = 0;
    uint8_t temp = type_spe.def_fail_mode;
    
    if(temp & __STOP)
    {
        flag[kinds] = FAIL_MODE_STOP;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_STOP];
    }
    if(temp & __HALT)
    {
        flag[kinds] = FAIL_MODE_HALT;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_HALT];
    }
    if(temp & __CON)
    {
        flag[kinds] = FAIL_MODE_CON;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_CON];
    }
    if(temp & __RESTART)
    {
        flag[kinds] = FAIL_MODE_RESTART;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_RESTART];
    }
    if(temp & __NEXT)
    {
        flag[kinds] = FAIL_MODE_NEXT;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_NEXT];
    }
    if(temp & __FPDFC)
    {
        flag[kinds] = FAIL_MODE_FPDFC;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_FPDFC];
    }
    
    return kinds;
}

uint32_t define_cur_kinds(uint8_t mode, const uint8_t *gear[], uint8_t *flag)
{
	uint32_t kinds = 0;
    uint8_t temp_gear = 0;
	
    if(mode == ACW)
    {
        temp_gear = type_spe.acw_gear;
    }
    else if(mode == CC)
    {
        temp_gear = type_spe.cc_gear;
    }
    
	switch(mode)
	{
		case ACW:
        case CC:
		{
			if(temp_gear & _AC_200uA)
			{
				flag[kinds] = AC_200uA;
				gear[kinds++] = ac_gear[AC_200uA].name;
			}
			if(temp_gear & _AC_2mA)
			{
				flag[kinds] = AC_2mA;
				gear[kinds++] = ac_gear[AC_2mA].name;
			}
			if(temp_gear & _AC_10mA)
			{
				flag[kinds] = AC_10mA;
				gear[kinds++] = ac_gear[AC_10mA].name;
			}
			if(temp_gear & _AC_20mA)
			{
				flag[kinds] = AC_20mA;
				gear[kinds++] = ac_gear[AC_20mA].name;
			}
			if(temp_gear & _AC_50mA)
			{
				flag[kinds] = AC_50mA;
				gear[kinds++] = ac_gear[AC_50mA].name;
			}
			if(temp_gear & _AC_100mA)
			{
				flag[kinds] = AC_100mA;
				gear[kinds++] = ac_gear[AC_100mA].name;
			}
			if(temp_gear & _AC_200mA)
			{
				flag[kinds] = AC_200mA;
				gear[kinds++] = ac_gear[AC_200mA].name;
			}
			break;
		}
		case DCW:
		{
			if(type_spe.dcw_gear & _DC_2uA)
			{
				flag[kinds] = DC_2uA;
				gear[kinds++] = dc_gear[DC_2uA].name;
			}
			if(type_spe.dcw_gear & _DC_20uA)
			{
				flag[kinds] = DC_20uA;
				gear[kinds++] = dc_gear[DC_20uA].name;
			}
			if(type_spe.dcw_gear & _DC_200uA)
			{
				flag[kinds] = DC_200uA;
				gear[kinds++] = dc_gear[DC_200uA].name;
			}
			if(type_spe.dcw_gear & _DC_2mA)
			{
				flag[kinds] = DC_2mA;
				gear[kinds++] = dc_gear[DC_2mA].name;
			}
			if(type_spe.dcw_gear & _DC_10mA)
			{
				flag[kinds] = DC_10mA;
				gear[kinds++] = dc_gear[DC_10mA].name;
			}
			if(type_spe.dcw_gear & _DC_20mA)
			{
				flag[kinds] = DC_20mA;
				gear[kinds++] = dc_gear[DC_20mA].name;
			}
			if(type_spe.dcw_gear & _DC_50mA)
			{
				flag[kinds] = DC_50mA;
				gear[kinds++] = dc_gear[DC_50mA].name;
			}
			if(type_spe.dcw_gear & _DC_100mA)
			{
				flag[kinds] = DC_100mA;
				gear[kinds++] = dc_gear[DC_100mA].name;
			}
			break;
		}
	}
	return kinds;
}

uint8_t get_max_cur_gear(uint8_t mode)
{
	switch(mode)
	{
		case ACW:
			if(type_spe.acw_gear & _AC_200mA)
			{
				return AC_200mA;
			}
			else if(type_spe.acw_gear & _AC_100mA)
			{
				return AC_100mA;
			}
			else if(type_spe.acw_gear & _AC_50mA)
			{
				return AC_50mA;
			}
			else if(type_spe.acw_gear & _AC_20mA)
			{
				return AC_20mA;
			}
			else if(type_spe.acw_gear & _AC_10mA)
			{
				return AC_10mA;
			}
			else if(type_spe.acw_gear & _AC_2mA)
			{
				return AC_2mA;
			}
			else if(type_spe.acw_gear & _AC_200uA)
			{
				return AC_200uA;
			}
			else
			{
				return 0;
			}
		case DCW:
			if(type_spe.dcw_gear & _DC_100mA)
			{
				return DC_100mA;
			}
			else if(type_spe.dcw_gear & _DC_50mA)
			{
				return DC_50mA;
			}
			else if(type_spe.dcw_gear & _DC_20mA)
			{
				return DC_20mA;
			}
			else if(type_spe.dcw_gear & _DC_10mA)
			{
				return DC_10mA;
			}
			else if(type_spe.dcw_gear & _DC_2mA)
			{
				return DC_2mA;
			}
			else if(type_spe.dcw_gear & _DC_200uA)
			{
				return DC_200uA;
			}
			else if(type_spe.dcw_gear & _DC_20uA)
			{
				return DC_20uA;
			}
			else if(type_spe.dcw_gear & _DC_2uA)
			{
				return DC_2uA;
			}
			else
			{
				return 0;
			}
	}
	return 0;
}
uint32_t define_hz_kinds(uint8_t mode, const uint8_t** gear_buf, uint8_t *flag)
{
	uint8_t kinds = 0;
	uint32_t hz_en = 0;
	switch(mode)
	{
		case ACW:
			hz_en = type_spe.acw_hz;
			break;
		case GR:
			hz_en = type_spe.gr_hz;
			break;
		default:
			return 0;
	}
	
	if(hz_en & _AC_50HZ)
	{
		gear_buf[kinds] = "1 --- 50Hz";
		flag[kinds++] = 1;
	}
	if(hz_en & _AC_60HZ)
	{
		gear_buf[kinds] = "2 --- 60Hz";
		flag[kinds++] = 2;
	}
	if(hz_en & _AC_100HZ)
	{
		gear_buf[kinds] = "3 -- 100Hz";
		flag[kinds++] = 3;
	}
	if(hz_en & _AC_150HZ)
	{
		gear_buf[kinds] = "4 -- 150Hz";
		flag[kinds++] = 4;
	}
	if(hz_en & _AC_200HZ)
	{
		gear_buf[kinds] = "5 -- 200Hz";
		flag[kinds++] = 5;
	}
	if(hz_en & _AC_250HZ)
	{
		gear_buf[kinds] = "6 -- 250Hz";
		flag[kinds++] = 6;
	}
	if(hz_en & _AC_300HZ)
	{
		gear_buf[kinds] = "7 -- 300Hz";
		flag[kinds++] = 7;
	}
	if(hz_en & _AC_350HZ)
	{
		gear_buf[kinds] = "8 -- 350Hz";
		flag[kinds++] = 8;
	}
	if(hz_en & _AC_400HZ)
	{
		gear_buf[kinds] = "9 -- 400Hz";
		flag[kinds++] = 9;
	}
	return kinds;
}

int32_t check_mode(void)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int16_t i = 0;
    
    if(NULL == g_cur_step)
    {
        return -1;
    }
	
	define_modes(mode_buf, flag, &kinds);/* 获取当前机型的模式信息 */
	
    for(i = 1; i <= kinds; i++)
    {
        if(g_cur_step->one_step.com.mode == flag[i])
        {
            return 0;
        }
    }
    
    return -1;
}


int32_t check_this_mode(uint8_t mode)
{
    if(mode < ACW || mode >= MODE_END)
    {
        return -1;
    }
    
    return 0;
}

int32_t check_test_mode(NODE_STEP * p)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int16_t i = 0;
    
    if(NULL == p)
    {
        return -1;
    }
	
	define_modes(mode_buf, flag, &kinds);/* 获取当前机型的模式信息 */
	
    for(i = 1; i <= kinds; i++)
    {
        if(p->one_step.com.mode == flag[i])
        {
            return 0;
        }
    }
    
    return -1;
}
uint32_t define_interface_config(uint8_t *flag)
{
    uint32_t kinds = 0;
    
//     if(type_spe.interface_config & INTERFACE_PLC)
//     {
// 		flag[kinds++] = 0;
//     }
    if(type_spe.interface_config & INTERFACE_COMM)
    {
		flag[kinds++] = 1;
    }
    if(type_spe.interface_config & INTERFACE_USB_OUT)
    {
		flag[kinds++] = 2;
    }
    if(type_spe.interface_config & INTERFACE_USB_IN)
    {
		flag[kinds++] = 3;
    }
    
    return kinds;
}
uint8_t get_acw_max_gear(void)
{
    if(type_spe.acw_gear & _AC_200mA)
    {
        return AC_200mA;
    }
    if(type_spe.acw_gear & _AC_100mA)
    {
        return AC_100mA;
    }
    if(type_spe.acw_gear & _AC_50mA)
    {
        return AC_50mA;
    }
    if(type_spe.acw_gear & _AC_20mA)
    {
        return AC_20mA;
    }
    if(type_spe.acw_gear & _AC_10mA)
    {
        return AC_10mA;
    }
    if(type_spe.acw_gear & _AC_2mA)
    {
        return AC_2mA;
    }
    if(type_spe.acw_gear & _AC_200uA)
    {
        return AC_200uA;
    }
    
    return AC_200mA;
}
uint8_t get_cc_max_gear(void)
{
    if(type_spe.cc_gear & _AC_200mA)
    {
        return AC_200mA;
    }
    if(type_spe.cc_gear & _AC_100mA)
    {
        return AC_100mA;
    }
    if(type_spe.cc_gear & _AC_50mA)
    {
        return AC_50mA;
    }
    if(type_spe.cc_gear & _AC_20mA)
    {
        return AC_20mA;
    }
    if(type_spe.cc_gear & _AC_10mA)
    {
        return AC_10mA;
    }
    if(type_spe.cc_gear & _AC_2mA)
    {
        return AC_2mA;
    }
    if(type_spe.cc_gear & _AC_200uA)
    {
        return AC_200uA;
    }
    
    return AC_200mA;
}

uint8_t get_dcw_max_gear(void)
{
    if(type_spe.dcw_gear & _DC_100mA)
    {
        return DC_100mA;
    }
    if(type_spe.dcw_gear & _DC_50mA)
    {
        return DC_50mA;
    }
    if(type_spe.dcw_gear & _DC_20mA)
    {
        return DC_20mA;
    }
    if(type_spe.dcw_gear & _DC_10mA)
    {
        return DC_10mA;
    }
    if(type_spe.dcw_gear & _DC_2mA)
    {
        return DC_2mA;
    }
    if(type_spe.dcw_gear & _DC_200uA)
    {
        return DC_200uA;
    }
    if(type_spe.dcw_gear & _DC_20uA)
    {
        return DC_20uA;
    }
    if(type_spe.dcw_gear & _DC_2uA)
    {
        return DC_2uA;
    }
    
    return DC_100mA;
}

void save_custom_type_info(uint8_t *buf, uint8_t size)
{
    erase_pages_flash(TYPE_NUM_ADDR, 1);
    write_u16_flash(TYPE_NUM_ADDR, (void*)buf, size);
}

static void check_custom_sys_par(CUSTOM_SYS_PAR *info)
{
	if(info->amp_select != AMP_OLD && info->amp_select != AMP_8833)    /* 功放类型 老功放 和 8833功放 */
	{
		info->amp_select = AMP_8833;
	}
	if(info->par_medium != PAR_MEDIUM_EEP && info->par_medium != PAR_MEDIUM_FLASH)
	{
		info->par_medium = PAR_MEDIUM_EEP;/* 参数存储介质 eep 或 flash */
	}
	if(info->res_medium != PAR_MEDIUM_EEP && info->res_medium != PAR_MEDIUM_FLASH)
	{
		info->res_medium = PAR_MEDIUM_EEP;/* 结果存储介质 eep 或 flash */
	}
	if(info->cal_medium != PAR_MEDIUM_EEP && info->cal_medium != PAR_MEDIUM_FLASH)
	{
		info->cal_medium = PAR_MEDIUM_EEP;/* 校准存储介质 eep 或 flash */
	}
	if(info->buzzer_en != 1 && info->buzzer_en != 0)
	{
		info->buzzer_en = 1;/* 蜂鸣器硬开关 */
	}
	if(info->mute_sw != 1 && info->mute_sw != 0)
	{
		info->mute_sw = 0;/* 系统静音模式开关 静音时测试报警不会一直响 */
	}
	if(info->amp_type != 1 && info->amp_type != 0)
	{
		info->amp_type = 0;/* 功放类型 LINE PWM */
	}
	if(info->leading_sw != 1 && info->leading_sw != 0)
	{
		info->leading_sw = 0;/* 数据导入导出到U盘开关 */
	}
	if(info->ir_gear_hold != 1 && info->ir_gear_hold != 0)
	{
		info->ir_gear_hold = 0;/* IR换挡保持时间开关 */
	}
	if(info->ir_speed_sw != 1 && info->ir_speed_sw != 0)
	{
		info->ir_speed_sw = 0;
	}
	if(info->offset_set_en != 1 && info->offset_set_en != 0)
	{
		info->offset_set_en = 0;//手动偏移开关
	}
}
void read_custom_par(void)
{
    CUSTOM_TYPE_INFO custom_type_info;
    uint8_t *p_addr = (void*)TYPE_NUM_ADDR;
    uint8_t count = sizeof(CUSTOM_TYPE_INFO);
    uint8_t *p_str = (void*)&custom_type_info;
    int32_t i = 0;
    
    /* 读出定制数据到内存 */
    for(i = 0; i < count; i++)
    {
        p_str[i] = p_addr[i];
    }
    
    g_custom_sys_par = custom_type_info.custom_sys_par;
	check_custom_sys_par(&g_custom_sys_par);
}

void save_custom_par(void)
{
    CUSTOM_TYPE_INFO custom_type_info;
    uint8_t *p_addr = (void*)TYPE_NUM_ADDR;
    uint8_t count = sizeof(CUSTOM_TYPE_INFO);
    uint8_t *p_str = (void*)&custom_type_info;
    int32_t i = 0;
    
    /* 读出定制数据到内存 */
    for(i = 0; i < count; i++)
    {
        p_str[i] = p_addr[i];
    }
    
    custom_type_info.custom_sys_par = g_custom_sys_par;
    
    save_custom_type_info((uint8_t *)&custom_type_info, count);
}

uint8_t is_custom_type(void)
{
    return ((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->custom_en;
}
uint16_t get_cur_type_num(void)
{
    return ((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->type_num;
}

void set_custom_type_info(void)
{
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
