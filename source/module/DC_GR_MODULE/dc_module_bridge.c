
#include "stm32f10x.h"
#include "mb_master.h"
#include "dc_module_bridge.h"
#include "mb_cmd_define.h"
#include "module.h"
#include "dc_module.h"
#include "calibration.h"



void set_cal_data_dc_module_bridge(uint8_t slave_addr)
{
    uint8_t n = 0;
    
    if(-1 == find_dc_gr_map_ratio_n(slave_addr, &n))
    {
        return;
    }
    
    mb_set_slave_info(slave_addr, M_DC_MODULE_CAL_DATA,
                    (uint16_t*)&ratio_dc_gr[0][n], sizeof(RATIO_DC_GR) / 2);
}

void get_cal_data_dc_module_bridge(uint8_t slave_addr)
{
    mb_query_slave_info(slave_addr, M_DC_MODULE_CAL_DATA, NULL, 0, sizeof(RATIO_DC_GR) / 2);
}

void set_dc_gr_module_output_da(uint8_t slave_addr, uint16_t vol)
{
    uint8_t n = 0;
    
    if(-1 == find_dc_gr_map_ratio_n(slave_addr, &n))
    {
        return;
    }
    
    dc_gr_master_info[slave_addr].par.da = vol * ratio_dc_gr[0][n].dac_k + ratio_dc_gr[0][n].dac_b;
}
void set_dac_value_dc_module_bridge(uint8_t slave_addr, uint16_t da_value)
{
    mb_set_slave_info(slave_addr, M_DC_MODULE_DAC,
            &dc_gr_master_info[slave_addr].data.da, 1);
}

void set_data_dc_module_bridge(uint8_t slave_addr)
{
    mb_set_slave_info(slave_addr, M_DC_MODULE_SET_DATA,
            (uint16_t*)&dc_gr_master_info[slave_addr].par, sizeof(DC_GR_PARAMETER) / 2);
}

void set_vol_gear_dc_module_bridge(uint8_t slave_addr, uint16_t gear)
{
    mb_set_slave_info(slave_addr, M_DC_MODULE_VOL_GEAR, &gear, 1);
}

/*
 * 函数名：set_start_dc_module_bridge
 * 描述  ：启动直流模块 特别的说明 它同时将模块的参数也传了过去，避免有时候参数没有及时更新造成错误
 * 输入  ：slave_addr 从机地址
 * 输出  ：无
 * 返回  ：无
 */
void set_start_dc_module_bridge(uint8_t slave_addr)
{
    mb_set_slave_info(slave_addr, M_DC_MODULE_START,
        (uint16_t*)&dc_gr_master_info[slave_addr].par, sizeof(DC_GR_PARAMETER) / 2);
}

void set_stop_dc_module_bridge(uint8_t slave_addr)
{
    mb_set_slave_info(slave_addr, M_DC_MODULE_STOP, NULL, 0);
}

void get_test_fetch_dc_module_bridge(uint8_t slave_addr)
{
    mb_query_slave_info(slave_addr, M_DC_MODULE_TEST_FETCH, NULL, 0, sizeof(MODULE_DATA) / 2);
}
void get_test_fetch_short_dc_module_bridge(uint8_t slave_addr)
{
    mb_query_slave_info(slave_addr, M_DC_MODULE_TEST_FETCH_SHORT, NULL, 0, sizeof(MODULE_DATA_SHORT) / 2);
}

void get_info_dc_module_bridge(uint8_t slave_addr)
{
    mb_query_slave_info(slave_addr, M_DC_MODULE_INFO, NULL, 0, sizeof(MODULE_INFO) / 2);
}


void set_test_method_dc_module_bridge(uint8_t slave_addr, uint16_t gear)
{
    mb_set_slave_info(slave_addr, M_DC_MODULE_TEST_METHOD, &gear, 1);
}



/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

