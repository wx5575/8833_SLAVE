
#include "stm32f10x.h"
#include "mb_dc_module.h"
#include "module.h"
#include "string.h"
#include "dc_module.h"
#include "calibration.h"
#include "mb_cmd_define.h"
#include "cs99xx_struct.h"


/*
 * 函数名：m_dc_module_cal_data_dispose
 * 描述  ：直流模块校准系数通讯处理
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_cal_data_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    uint8_t n = 0;
    
    if(-1 == find_dc_gr_map_ratio_n(slove_num, &n))
    {
        return;
    }
    
    if(cmd == MB_READ_PAR)
    {
        memcpy(&ratio_dc_gr[0][n], data, sizeof (RATIO_DC_GR));
    }
    else
    {
//         memcpy(data, &ratio_dc_gr[0][n], sizeof (RATIO_DC_GR));
    }
}
/*
 * 函数名：m_dc_module_dac_dispose
 * 描述  ：设置直流模块输出DA通讯处理程序
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_dac_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
}
/*
 * 函数名：m_dc_module_start_dispose
 * 描述  ：启动直流模块指令的返回处理函数
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_start_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    dc_gr_master_info[slove_num].st = 1;
}
/*
 * 函数名：m_dc_module_stop_dispose
 * 描述  ：停止直流模块指令的返回处理函数
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_stop_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    dc_gr_master_info[slove_num].st = 0;
}
/*
 * 函数名：m_dc_module_test_fetch_dispose
 * 描述  ：询问直流模块测试数据的返回处理函数
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
#include "STM32_GPIO_Config.h"
#include "serve_test.h"
void m_dc_module_test_fetch_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    uint32_t i = 0;
    uint32_t c = sizeof(MODULE_DATA);
    uint8_t *p1 = (void*)&dc_gr_master_info[slove_num].data;
    uint8_t *p2 = (void*)data;
    
    for(i = 0; i < c; i++)
    {
        p1[i] = p2[i];
    }
    dc_gr_master_info[slove_num].usable = 1;
}
void m_dc_module_test_fetch_short_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    uint32_t i = 0;
    uint32_t c = sizeof(MODULE_DATA_SHORT);
    uint8_t *p1 = (void*)&dc_gr_master_info[slove_num].short_data;
    uint8_t *p2 = (void*)data;
	
    for(i = 0; i < c; i++)
    {
        p1[i] = p2[i];
    }
    dc_gr_master_info[slove_num].usable = 1;
}
/*
 * 函数名：m_dc_module_vol_gear_dispose
 * 描述  ：设置直流模块档位指令的返回处理函数
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_vol_gear_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
}
/*
 * 函数名：m_dc_module_test_method_dispose
 * 描述  ：设置直流模块测试方法指令的返回处理函数
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_test_method_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
}

void m_dc_module_set_data_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
//     memcpy(&dc_gr_master_info[slove_num].id, data, sizeof (DC_GR_PARAMETER));
//     dc_gr_master_info[slove_num].en = 1;
//     select_dc_gr_moduls(DC_GR_SEL_ALL,0);
}
/*
 * 函数名：m_dc_module_info_dispose
 * 描述  ：查询直流模块信息指令的返回处理函数
 * 输入  ：slove_num 从机地址
 *         cmd 读写命令
 *         data 数据
 * 输出  ：无
 * 返回  ：无
 */
void m_dc_module_info_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    memcpy(&dc_gr_master_info[slove_num].id, data, sizeof (MODULE_INFO));
    dc_gr_master_info[slove_num].en = 1;
    select_dc_gr_moduls(DC_GR_SEL_ALL,0);
}



/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

