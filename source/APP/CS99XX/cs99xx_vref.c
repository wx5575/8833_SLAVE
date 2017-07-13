/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_vref.c
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include	<string.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "cs99xx_vref.h"
#include 	"DAC_VREF.h"

uint16_t dcw_5kv_short_vref(uint16_t cur_h)
{
	int16_t l_short_vref = 0;	/* l大头的是局部变量 */
    uint32_t short_1mA = SHORT_V_DC_1MA;
    uint32_t short_2mA = SHORT_V_DC_2MA;
    uint8_t t_gear = 0;
    float t_cur = 0.0;
    
    t_gear = get_dcw_max_gear();
    
    t_cur = 1.0 * cur_h / pow(10, dc_gear[cur_gear].decs);/* 将电流上限值转换为浮点数便于比较 */
    
    if(cur_gear <= DC_200uA)
    {
        t_cur /= 1000.0;
    }
    
    /* 短路基准 */
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        if(test_flag.gradation == STAGE_CHARGE)
        {
            l_short_vref = SHORT_V_DC_20MA;
            return l_short_vref;
        }
        else
        {
            short_1mA = SHORT_V_DC_5MA;
            short_2mA = SHORT_V_DC_5MA;
        }
    }
    
    switch(t_gear)
    {
        case DC_2uA:
        case DC_20uA:
        case DC_200uA:
            l_short_vref = short_1mA;
            break;
        case DC_2mA:
        {
            if(t_cur <= 1.000)
            {
                l_short_vref = short_1mA;
            }
            else
            {
                l_short_vref = short_2mA;
            }
            break;
        }
        case DC_10mA:
        {
            if(t_cur <= 1.00)
            {
                l_short_vref = short_1mA;
            }
            else if(t_cur <= 2.00)
            {
                l_short_vref = short_2mA;
            }
            else if(t_cur <= 5.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 8.00)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_10MA;
            }
            break;
        }
        case DC_20mA:
        {
            if(t_cur <= 1.00)
            {
                l_short_vref = short_1mA;
            }
            else if(t_cur <= 2.00)
            {
                l_short_vref = short_2mA;
            }
            else if(t_cur <= 5.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 8.00)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else if(t_cur <= 10.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 15.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            break;
        }
        case DC_50mA:
        {
            if(t_cur <= 10.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 20.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 25.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 28.00)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else if(t_cur <= 35.00)
            {
                l_short_vref = SHORT_V_DC_8MA;//SHORT_V_DC_10MA;
            }
            else if(t_cur <= 45.00)
            {
                l_short_vref = SHORT_V_DC_8MA;//SHORT_V_DC_10MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_8MA;//SHORT_V_DC_10MA;
            }
            break;
        }
        case DC_100mA:
        {
            if(t_cur <= 10.0)
            {
                l_short_vref = short_1mA;
            }
            else if(t_cur <= 20.0)
            {
                l_short_vref = short_2mA;
            }
            else if(t_cur <= 50.0)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 80.0)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_10MA;
            }
            break;
        }
    }
    
    return l_short_vref;
}
uint16_t dcw_10kv_short_vref(uint16_t cur_h)
{
	int16_t l_short_vref = 0;	/* l大头的是局部变量 */
    uint32_t short_1mA = SHORT_V_DC_1MA;
    uint32_t short_2mA = SHORT_V_DC_2MA;
    uint8_t t_gear = 0;
    float t_cur = 0.0;
    
    t_gear = get_dcw_max_gear();
    t_cur = 1.0 * cur_h / pow(10, dc_gear[cur_gear].decs);/* 将电流上限值转换为浮点数便于比较 */
    
    if(cur_gear <= DC_200uA)
    {
        t_cur /= 1000.0;
    }
    
    /* 短路基准 */
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        if(test_flag.gradation == STAGE_CHARGE)
        {
            l_short_vref = SHORT_V_DC_20MA;
            return l_short_vref;
        }
        else
        {
            short_1mA = SHORT_V_DC_5MA;
            short_2mA = SHORT_V_DC_5MA;
        }
    }
    
    switch(t_gear)
    {
        case DC_2uA:
        case DC_20uA:
        case DC_200uA:
        case DC_2mA:
        {
            l_short_vref = SHORT_V_DC_5MA;
            break;
        }
        case DC_10mA:
        {
            if(t_cur <= 5.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            break;
        }
        case DC_20mA:
        {
            if(t_cur <= 1.00)
            {
                l_short_vref = short_1mA;
            }
            else if(t_cur <= 2.00)
            {
                l_short_vref = short_2mA;
            }
            else if(t_cur <= 5.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 8.00)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else if(t_cur <= 10.00)
            {
                l_short_vref = SHORT_V_DC_10MA;
            }
            else if(t_cur <= 15.00)
            {
                l_short_vref = SHORT_V_DC_15MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_20MA;
            }
            break;
        }
        case DC_50mA:
        {
            if(t_cur <= 10.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 20.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 25.00)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 28.00)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else if(t_cur <= 35.00)
            {
                l_short_vref = SHORT_V_DC_10MA;
            }
            else if(t_cur <= 45.00)
            {
                l_short_vref = SHORT_V_DC_15MA;
            }
            else
            {
                l_short_vref = SHORT_V_DC_20MA;
            }
            break;
        }
        case DC_100mA:
        {
            if(t_cur <= 10.0)
            {
                l_short_vref = short_1mA;
            }
            else if(t_cur <= 20.0)
            {
                l_short_vref = short_2mA;
            }
            else if(t_cur <= 50.0)
            {
                l_short_vref = SHORT_V_DC_5MA;
            }
            else if(t_cur <= 80.0)
            {
                l_short_vref = SHORT_V_DC_8MA;
            }
            else if(t_cur <= 100.0)
            {
                l_short_vref = SHORT_V_DC_10MA;
            }
            break;
        }
    }
    
    return l_short_vref;
}
/*
 * 函数名：test_vref
 * 描述  ：测试时短路基准与电弧侦测基准确定
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_vref(uint16_t cur_h)
{
	int16_t l_short_vref = 0;	/* l大头的是局部变量 */
	float arc_base = 0;
    static uint16_t set_arc_vref(uint16_t base);
    float temp_buf[][10] = {
        {0, ARC_AC_DATUM_1, ARC_AC_DATUM_2, ARC_AC_DATUM_3, ARC_AC_DATUM_4, ARC_AC_DATUM_5, ARC_AC_DATUM_6, ARC_AC_DATUM_7, ARC_AC_DATUM_8, ARC_AC_DATUM_9},
        {0, ARC_DC_DATUM_1, ARC_DC_DATUM_2, ARC_DC_DATUM_3, ARC_DC_DATUM_4, ARC_DC_DATUM_5, ARC_DC_DATUM_6, ARC_DC_DATUM_7, ARC_DC_DATUM_8, ARC_DC_DATUM_9}
    };
    uint8_t temp = 0;
    uint8_t t_gear = 0;
    float t_cur = 0.0;
    
	switch(cur_mode)
	{
		case ACW:/* 短路基准 */
        {
            t_gear = get_acw_max_gear();
            t_cur = 1.0 * cur_h / pow(10, ac_gear[cur_gear].decs);/* 将电流上限值转换为浮点数便于比较 */
            
            if(cur_gear <= AC_200uA)
            {
                t_cur /= 1000.0;
            }
            
			switch(t_gear)
			{
				case AC_200uA://200uA
                {
					l_short_vref = SHORT_V_AC_1MA;
					break;
                }
				case AC_2mA://2mA
				{
					if(t_cur <= 1.0)
					{
						l_short_vref = SHORT_V_AC_1MA;
					}
					else
					{
						l_short_vref = SHORT_V_AC_2MA;
					}
					break;
				}
				case AC_10mA:/* 10mA */
				{
					if(t_cur <= 1.0)
					{
						l_short_vref = SHORT_V_AC_1MA;
					}
					else if(t_cur <= 2.0)
					{
						l_short_vref = SHORT_V_AC_2MA;
					}
					else if(t_cur <= 5.0)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 8.0)
					{
						l_short_vref = SHORT_V_AC_8MA;
					}
					else
					{
						l_short_vref = SHORT_V_AC_10MA;
					}
					break;
				}
				case AC_20mA://20mA
				{
					if(t_cur <= 1.0)
					{
						l_short_vref = SHORT_V_AC_1MA;
					}
					else if(t_cur <= 2.0)
					{
						l_short_vref = SHORT_V_AC_2MA;
					}
					else if(t_cur <= 5.00)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 8.00)
					{
						l_short_vref = SHORT_V_AC_8MA;
					}
					else if(t_cur <= 10.00)
					{
						l_short_vref = SHORT_V_AC_10MA;
					}
					else if(t_cur <= 15.00)
					{
						l_short_vref = SHORT_V_AC_15MA;
					}
					else
					{
						l_short_vref = SHORT_V_AC_20MA;
					}
					break;
				}
				case AC_50mA:/* 50mA */
				{
					if(t_cur <= 10.00)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 20.00)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 25.00)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 28.00)
					{
						l_short_vref = SHORT_V_AC_8MA;
					}
					else if(t_cur <= 35.00)
					{
						l_short_vref = SHORT_V_AC_10MA;
					}
					else if(t_cur <= 45.00)
					{
						l_short_vref = SHORT_V_AC_15MA;
					}
					else
					{
						l_short_vref = SHORT_V_AC_20MA;
					}
					break;
				}
				case AC_100mA:/* 100mA */
				{
					if(t_cur <= 10.0)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 20.0)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 50.0)
					{
						l_short_vref = SHORT_V_AC_10MA;
					}
					else if(t_cur <= 70.0)
					{
						l_short_vref = SHORT_V_AC_15MA;//SHORT_V_AC_8MA;
					}
					else if(t_cur <= 80.0)
					{
						l_short_vref = SHORT_V_AC_15MA;//SHORT_V_AC_10MA;
					}
					else if(t_cur <= 90.0)
					{
						l_short_vref = SHORT_V_AC_15MA;//SHORT_V_AC_15MA;
					}
					else
					{
						l_short_vref = SHORT_V_AC_15MA;
					}
					break;
				}
				case AC_200mA:/* 200mA */
				{
					if(t_cur <= 10.0)
					{
						l_short_vref = SHORT_V_AC_1MA;
					}
					else if(t_cur <= 20.0)
					{
						l_short_vref = SHORT_V_AC_2MA;
					}
					else if(t_cur <= 50.0)
					{
						l_short_vref = SHORT_V_AC_5MA;
					}
					else if(t_cur <= 80.0)
					{
						l_short_vref = SHORT_V_AC_8MA;
					}
					else if(t_cur <= 100.0)
					{
						l_short_vref = SHORT_V_AC_10MA;//SHORT_V_AC_10MA;
					}
					else if(t_cur <= 150.0)
					{
						l_short_vref = SHORT_V_AC_15MA;//SHORT_V_AC_15MA;
					}
					else
					{
						l_short_vref = SHORT_V_AC_15MA;
					}
					break;
				}
			}
			break;
        }
		case DCW:
        {
            /* 大电容测试 在充电阶段短路基准值为最大值 */
            if(type_spe.dcw_big_cap_en == ENABLE && test_flag.gradation == STAGE_CHARGE)
            {
                l_short_vref = SHORT_V_DC_20MA;
            }
            /* 正常测试 */
            else
            {
                if(type_spe.dcw_vol_range == DCW_10kV
                || type_spe.dcw_vol_range == DCW_20kV)
                {
                    l_short_vref = dcw_10kv_short_vref(cur_h);
                }
                else
                {
                    l_short_vref = dcw_5kv_short_vref(cur_h);
                }
            }
			break;
        }
		case IR:
			/* 短路基准 */
			l_short_vref = SHORT_V_DC_10MA;//SHORT_V_DC_2MA /* 2014.10.21 ir太容易进入短路报警 */
			break;
		case GR:
			/* 无短路基准 */
			break;
		case BBD:
			/* 短路基准 */
			l_short_vref = SHORT_V_AC_2MA;
			break;
	}
    
	/*********** 电弧基准 ***************/
	/* 电流模式 */
    if(cur_mode == ACW)
    {
        temp = 0;
    }
    else
    {
        temp = 1;
    }
    
	if(g_cur_file->arc_mode == ARC_CUR_MODEL)
	{
        arc_base = temp_buf[temp][set_arc_vref(2000)];
	}
	/* 等级模式 */
	else
	{
        arc_base = temp_buf[temp][cur_arc_gear%10];
	}
	
	/* 短路基准 */
	DAC_Vol[1] = l_short_vref;
	
	/* ARC基准 */
	/* ARC开启 2017.5.24 */
    if(cur_arc_gear != 0)
    {
        DAC_Vol[2] = cur_arc_k * arc_base;
    }
    /* ARC关闭 2017.5.24 */
    else
    {
        DAC_Vol[2] = ARC_VREF_RESET;
    }
}

static uint16_t set_arc_vref(uint16_t base)
{
	switch(base)
	{
		case 1000:
		{
			if(cur_arc_gear < 280)
			{
				return 9;
			}
			else if(cur_arc_gear < 550)
			{
				return 8;
			}
			else if(cur_arc_gear < 770)
			{
				return 7;
			}
			else if(cur_arc_gear < 1000)
			{
				return 6;
			}
			else if(cur_arc_gear < 1200)
			{
				return 5;
			}
			else if(cur_arc_gear < 1400)
			{
				return 4;
			}
			else if(cur_arc_gear < 1600)
			{
				return 3;
			}
			else if(cur_arc_gear < 1800)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
		case 2000:
		{
			if(cur_arc_gear <= 280)
			{
				return 9;
			}
			else if(cur_arc_gear <= 550)
			{
				return 8;
			}
			else if(cur_arc_gear <= 770)
			{
				return 7;
			}
			else if(cur_arc_gear <= 1000)
			{
				return 6;
			}
			else if(cur_arc_gear <= 1200)
			{
				return 5;
			}
			else if(cur_arc_gear <= 1400)
			{
				return 4;
			}
			else if(cur_arc_gear <= 1600)
			{
				return 3;
			}
			else if(cur_arc_gear <= 1800)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
		case 5000:
		{
			if(cur_arc_gear < 280)
			{
				return 9;
			}
			else if(cur_arc_gear < 550)
			{
				return 8;
			}
			else if(cur_arc_gear < 770)
			{
				return 7;
			}
			else if(cur_arc_gear < 1000)
			{
				return 6;
			}
			else if(cur_arc_gear < 1200)
			{
				return 5;
			}
			else if(cur_arc_gear < 1400)
			{
				return 4;
			}
			else if(cur_arc_gear < 1600)
			{
				return 3;
			}
			else if(cur_arc_gear < 1800)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
	}
	return 1;
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
