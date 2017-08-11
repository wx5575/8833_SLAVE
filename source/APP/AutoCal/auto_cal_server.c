
#define AUTO_CAL_SERVER_GLOBALS
#include "stm32f10x.h"
#include "calibration.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "cs99xx_mem_api.h"
#include "DAC_VREF.h"
#include "app.h"
#include "auto_calibration.h"
#include "auto_cal_server.h"
#include "string.h"
#include "auto_cal_comm.h"


	
uint8_t *cal_mode_pool[][2]={
	{"0","ACV"},//交流电压
	{"1","ACI"},//交流电流
	{"2","DCV"},//直流电压
	{"3","DCI"},//直流电流
	{"4","IRR"},//电阻
	{"5","GRI"},//接地电流
};

void get_res_cal_point_info(uint8_t mode, CAL_BASE_INFO* point, uint8_t n)
{
    uint8_t meal = get_ir_res_meal();
    
    if(mode == IR)
    {
        point->name = (void*)ir_cur_cal.point_menu[meal][n];
        point->ad = 0;
        point->da = 0;
        point->dis = ir_cur_cal.point_da[meal][n];
        point->measure = 0;
        point->gear = ir_cur_cal.gear[meal][n];
    }
}
/*
 * 函数名：get_acw_vol_cal_point_info
 * 描述  ：获取ACW电压校准点的信息
 * 输入  ：n 第几个校准点
 * 输出  ：point 带回校准点信息
 * 返回  ：无
 */
void get_vol_cal_point_info(uint8_t mode, CAL_BASE_INFO* point, uint8_t n)
{
    uint8_t i = n;
    uint8_t gear = 0;
    uint8_t meal = 0;
    const VOL_CAL (*vol_cal)[2] = NULL;
    const VOL_CAL *cur_cal = NULL;
    
    if(mode == ACW)
    {
        vol_cal = acw_vol_cal;
    }
    else if(mode == DCW)
    {
        vol_cal = dcw_vol_cal;
    }
    else if(mode == IR)
    {
        vol_cal = ir_vol_cal;
    }
    else if(mode == GR)
    {
        cur_cal = gr_cur_cal;
        gear = get_gr_gear();
        meal = select_gr_cal_menu();
    }
    else
    {
        return;
    }
    
    if(mode == GR)
    {
        point->name = (void*)cur_cal[gear].point_menu[meal][i][0];
        point->ad = 0;
        point->da = cur_cal[gear].point_da[meal][i][0];
        point->dis = cur_cal[gear].point_da[meal][i][1];
        point->measure = 0;
    }
    else
    {
        point->name = (void*)vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][i][0];
        point->ad = 0;
        point->da = vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
        point->dis = vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][1];
        point->measure = 0;
    }
}

void get_cur_cal_point_info(uint8_t mode, AUTO_CAL_CUR* info, uint8_t gear)
{
    GEAR_STR *gear_pool = NULL;
    CAL_BASE_INFO *base = &info->base;
    
    if(mode == ACW)
    {
        gear_pool = ac_gear;
    }
    else if(mode == DCW)
    {
        gear_pool = dc_gear;
    }
    else
    {
        return;
    }
    
    base->name = (void*)gear_pool[gear].name;
    base->ad = 0;
    base->da = info->output * info->da_k + info->da_b;
    base->measure = 0;
}

void clear_ui_ele(void)
{
    memset(g_auto_ele_info->ele_str, 0 , sizeof(g_auto_ele_info->ele_str));
//    draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
}

void init_ui_ele_info(ELEMENT_INFO *ele, uint8_t buf[12][10])
{
    uint8_t i = 0;
    
    /* 序号 */
    ele[i].x   = 0;
    ele[i].y   = 0;
    ele[i].u_x = ele[i].x + 6 * 3;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    /* 模式 */
    i++;
    ele[i].x   = 50;
    ele[i].y   = 0;
    ele[i].u_x = ele[i].x + 6 * 3;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 项目 */
    i++;
    ele[i].x   = 110;
    ele[i].y   = 0;
    ele[i].u_x = ele[i].x + 6 * 4;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* DA值 */
    i++;
    ele[i].x   = 30;
    ele[i].y   = 15;
    ele[i].u_x = ele[i].x + 6 * 4;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* AD值 电压 */
    i++;
    ele[i].x   = 30;
    ele[i].y   = 27;
    ele[i].u_x = ele[i].x + 6 * 4;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* AD值 电流 */
    i++;
    ele[i].x   = 72;
    ele[i].y   = 27;
    ele[i].u_x = ele[i].x + 6 * 4;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 档位 */
    i++;
    ele[i].x   = 30;
    ele[i].y   = 39;
    ele[i].u_x = ele[i].x + 6 * 7;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 校准点 */
    i++;
    ele[i].x   = 42;
    ele[i].y   = 51;
    ele[i].u_x = ele[i].x + 6 * 7;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 电压 */
    i++;
    ele[i].x   = 180;
    ele[i].y   = 15;
    ele[i].u_x = ele[i].x + 6 * 8;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 电流 */
    i++;
    ele[i].x   = 180;
    ele[i].y   = 27;
    ele[i].u_x = ele[i].x + 6 * 8;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 电阻 */
    i++;
    ele[i].x   = 180;
    ele[i].y   = 39;
    ele[i].u_x = ele[i].x + 6 * 8;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
    
    /* 测量 */
    i++;
    ele[i].x   = 180;
    ele[i].y   = 51;
    ele[i].u_x = ele[i].x + 6 * 8;
    ele[i].o_y = CH12;
    ele[i].cw  = CW12;
    ele[i].str = buf[i];
}


//void draw_auto_cal_ui_ele(const GUI_FONT * pNewFont, ELEMENT_INFO* p)
//{
//    int32_t i = 0;
//    
//    GUI_SetFont(pNewFont);
//    
//    for(i = 0; i < UI_ELE_END; i++)
//    {
//        GUI_ClearRect(p[i].x,  p[i].y, p[i].u_x - 1, p[i].y + p[i].o_y - 1);
//        GUI_DispStringAt((const char*)(const char*)p[i].str, p[i].x, p[i].y);
//    }
//    
//    LCD_REFRESH();/* 刷新屏幕 */
//}

void transform_auto_cal_num_to_string(uint8_t *buf, uint16_t num)
{
    mysprintf((uint8_t*)buf, NULL, 0 + 130, num);
}
void transform_auto_cal_da_ad_to_string(uint8_t *buf, uint16_t value)
{
    mysprintf((uint8_t*)buf, NULL, 0 + 140, value);
}
void transform_auto_cal_gear_to_string(uint8_t *buf, uint16_t value)
{
    mysprintf((uint8_t*)buf, NULL, 0 + 10, value);
}

void transform_auto_cal_vol_to_string(uint8_t *buf, uint16_t value)
{
    mysprintf((uint8_t*)buf, unit_pool[VOL_U_kV], 3 + 60, value);
}

void transform_auto_cal_cur_to_string(uint8_t *buf, uint16_t value, uint8_t mode, uint8_t gear)
{
    const uint8_t *unit;
    uint8_t decs = 0;
    
    if(mode == ACW)
    {
        unit = unit_pool[ac_gear[gear].unit];
        decs = ac_gear[gear].decs;
    }
    else if(mode == DCW)
    {
        unit = unit_pool[dc_gear[gear].unit];
        decs = dc_gear[gear].decs;
    }
    else
    {
        return;
    }
    
    mysprintf((uint8_t*)buf, unit, decs + 50, value);
}


void transform_auto_cal_res_to_string(uint8_t *buf, uint64_t value, uint8_t mode, uint8_t gear)
{
//     const uint8_t *unit;
//     uint8_t decs = 0;
    
    if(mode == IR)
    {
//         unit = unit_pool[ir_gear[gear].unit];
//         decs = ir_gear[gear].decs;
    }
    else
    {
        return;
    }
    
    mysprintf((uint8_t*)buf, unit_pool[RES_U_MOHM], 0 + 70, value);
}

void dis_cal_point_base_info(void)
{
    uint8_t mode = cal_order[g_cur_cal_opt_num].mode;
    uint8_t type = cal_order[g_cur_cal_opt_num].type;
    uint8_t opt = 0;
    
    switch(type)
    {
        case AUTOCAL_AC_V:
        case AUTOCAL_DC_V:
            opt = VOL_OPT;
            break;
        case AUTOCAL_AC_A:
        case AUTOCAL_DC_A:
        case AUTOCAL_GR_A:
            opt = CUR_OPT;
            break;
        case AUTOCAL_RES:
            opt = RES_OPT;
            break;
        default:
//         case AUTOCAL_PASS:
//             opt = NUL_OPT;
            break;
    }
    
    transform_auto_cal_num_to_string(g_auto_ele_info->ele_str[UI_ELE_NUM], g_cur_cal_opt_num);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_MODE], (void*)mode_pool[0][mode]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_OPT], (void*)opt_pool[opt]);
//    draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
}

void dis_cal_ele(uint16_t ad_v, uint16_t ad_i, uint16_t vol)
{
    transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_AD_VOL], ad_v);
    transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_AD_CUR], ad_i);
    if(vol)
        transform_auto_cal_vol_to_string(g_auto_ele_info->ele_str[UI_ELE_VOL], vol);
//    draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
}

void dis_gr_measure(float vol)
{
    mysprintf((uint8_t*)g_auto_ele_info->ele_str[UI_ELE_MEASURE],
                unit_pool[VOL_U_V], 3 + 50, vol);
}

/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/
