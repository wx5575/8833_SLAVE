
#define DC_GR_GLOBALS
#include "stm32f10x.h"
#include "dc_module.h"
#include "dc_module_bridge.h"
#include "serve_test.h"
#include "mb_cmd_define.h"
#include "app.h"
#include "keyboard.h"
#include "string.h"
#include "cs99xx_collect.h"

static uint8_t cur_dc_module_addr;

void draw_select_cal_dc_gr_gui(void)
{
}

void dis_one_module_for_cal(MODULE_INFO *p, uint8_t n)
{
}

void dis_one_page_modules_for_cal(MODULE_INFO *p, uint8_t base, uint8_t num)
{
}

int32_t slelect_cal_dc_gr_module(uint8_t *num)
{
    return 0;
}

void clear_dc_gr_st(void)
{
    uint8_t i = 0;
    
    for(i = 0; i < DC_MODULE_NUM; i++)
    {
        dc_gr_master_info[i + 1].st = 0;
    }
}
/*
 * 函数名:get_dc_gr_module_id
 * 描述  :获取制定直流模块的id值
 * 输入  :index直流模块的索引值
 * 输出  :无 
 * 返回  :直流模块的id
 */
uint32_t get_dc_gr_module_id(uint8_t index)
{
    return  sys_par.dc_module_using_info.addr[index];
}
/*
 * 函数名:get_dc_module_base_addr
 * 描述  :获取直流模块id基址
 * 输入  :无
 * 输出  :无
 * 返回  :id基址 
 */
uint8_t *get_dc_module_base_addr(void)
{
    return sys_par.dc_module_using_info.addr;
}
/*
 * 函数名:get_dc_module_using_num
 * 描述  :获取系统可用的直流模块个数
 * 输入  :无
 * 输出  :无 
 * 返回  :可用的直流模块的个数 
 */
uint8_t get_dc_module_using_num(void)
{
    return sys_par.dc_module_using_info.count;
}
void get_dc_gr_module_inf(MODULE_INFO *pool, uint32_t *count)
{
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    int32_t i = 0;
    
    if(c > DC_MODULE_NUM_MAX + 1)
    {
        c = DC_MODULE_NUM_MAX + 1;
    }
    
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        pool[i] = dc_gr_master_info[addr].id;
    }
    
    *count = c;
}

int32_t get_dc_gr_ad(uint8_t slave_addr, uint16_t *cur_ad, uint16_t *vol_ad)
{
    if(dc_gr_master_info[slave_addr].sel == 1)
    {
        if(dc_gr_master_info[slave_addr].usable == 0)
        {
            return -1;
        }
        
        *cur_ad = dc_gr_master_info[slave_addr].data.cur_ad;
        *vol_ad = dc_gr_master_info[slave_addr].data.vol_ad;
        dc_gr_master_info[slave_addr].usable = 0;
        
        if(dc_gr_master_info[slave_addr].data.status == DC_MODULE_ST_OPENERR)
        {
            dc_gr_master_info[slave_addr].data.status = DC_MODULE_ST_STOPING;
            set_dc_gr_open_err();
        }
        else if(dc_gr_master_info[slave_addr].data.status == DC_MODULE_ST_AMPERR)
        {
            dc_gr_master_info[slave_addr].data.status = DC_MODULE_ST_STOPING;
            set_dc_gr_amp_err();
        }
        return 0;
    }
    
    return -1;
}

void clear_dc_gr_data(void)
{
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    int32_t i = 0;
    
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        
        if(dc_gr_master_info[addr].sel == 1)
        {
            dc_gr_master_info[addr].usable = 0;
        }
    }
}

int32_t get_dc_gr_short_data(float *cur, float *vol)
{
    uint16_t i = 0;
    float t_cur = 0;
    float t_vol = 0;
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        if(dc_gr_master_info[addr].sel == 1)
        {
            if(dc_gr_master_info[addr].usable == 0)
            {
                break;
            }
            else
            {
				if(dc_gr_master_info[addr].short_data.cur == 0xff5a)
				{
                    dc_gr_master_info[addr].data.status = DC_MODULE_ST_STOPING;
                    set_dc_gr_amp_err();
					return -1;
				}
				else if(dc_gr_master_info[addr].short_data.cur == 0xff5b)
				{
                    dc_gr_master_info[addr].data.status = DC_MODULE_ST_STOPING;
                    set_dc_gr_open_err();
					return -1;
				}
				else
				{
					t_cur += dc_gr_master_info[addr].short_data.cur;
					t_vol = dc_gr_master_info[addr].short_data.vol;
				}
            }
        }
    }
    
    *cur = t_cur / 100.0;
    *vol = t_vol / 100.0;
    
    /* 数据不可用 */
    if(i < c)
    {
        return -1;
    }
    
    /* 清除数据可用标志 */
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        dc_gr_master_info[addr].usable = 0;
    }
    
    /* 所有选中模块的数据都可以使用了 */
    return 0;
}
int32_t get_dc_gr_data(float *cur, float *vol)
{
    uint16_t i = 0;
    float t_cur = 0;
    float t_vol = 0;
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        if(dc_gr_master_info[addr].sel == 1)
        {
            if(dc_gr_master_info[addr].usable == 0)
            {
                break;
            }
            else
            {
                t_cur += dc_gr_master_info[addr].data.cur;
                t_vol = dc_gr_master_info[addr].data.vol;
                
                if(dc_gr_master_info[addr].data.status == DC_MODULE_ST_OPENERR)
                {
                    dc_gr_master_info[addr].data.status = DC_MODULE_ST_STOPING;
                    set_dc_gr_open_err();
                }
                else if(dc_gr_master_info[addr].data.status == DC_MODULE_ST_AMPERR)
                {
                    dc_gr_master_info[addr].data.status = DC_MODULE_ST_STOPING;
                    set_dc_gr_amp_err();
                }
            }
        }
    }
    
    *cur = t_cur;
    *vol = t_vol;
    
    /* 数据不可用 */
    if(i < c)
    {
        return -1;
    }
    
    /* 清除数据可用标志 */
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        dc_gr_master_info[addr].usable = 0;
    }
    
    /* 所有选中模块的数据都可以使用了 */
    return 0;
}

int32_t ask_dc_gr_start_ok(void)
{
    #define RE_TRY_COUNT    (1000 * c)   /* 根据模块个数来确定等待时间 */
    uint16_t count = 0;
    uint16_t i = 0;
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    
    
    if(c > DC_MODULE_NUM_MAX)
    {
        c = DC_MODULE_NUM_MAX;
    }
    
    for(i = 0; i < c; i++)
    {
        if(p[i] > DC_MODULE_NUM_MAX)
        {
            p[i] = i;
        }
    }
    
    do{
        OSTimeDlyHMSM(0, 0, 0, 10);
        
        for(i = 0; i < c; i++)
        {
            addr = p[i];
            if(dc_gr_master_info[addr].sel == 1)
            {
                if(dc_gr_master_info[addr].st == 0)
                {
                    break;
                }
            }
        }
    }while(count++ < RE_TRY_COUNT && (i < c));
    
    if(count >= RE_TRY_COUNT)
    {
        return -1;
    }
    
    return 0;
}

void check_dc_gr_moduls(void)
{
    send_dc_module_cmd(DC_MODULE_CMD_INFO, 0);
}

void get_dc_gr_cal_info(void)
{
    send_dc_module_cmd(DC_MODULE_CMD_GET_CAL_DATA, 0);
}

void set_dc_gr_cal_info(void)
{
    send_dc_module_cmd(DC_MODULE_CMD_SET_CAL_DATA, 0);
}

void set_dc_gr_vol_gear(uint8_t gear)
{
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    int32_t i = 0;
    
    
    if(c > DC_MODULE_NUM_MAX)
    {
        c = DC_MODULE_NUM_MAX;
    }
    
    for(i = 0; i < c; i++)
    {
        addr = p[i];
        
        if(dc_gr_master_info[addr].sel == 1)
        {
            dc_gr_master_info[addr].par.gear = gear;
        }
    }
}

void set_dc_gr_test_method(uint8_t method)
{
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint8_t addr = 0;
    int32_t i = 0;
    
    
    if(c > DC_MODULE_NUM_MAX)
    {
        c = DC_MODULE_NUM_MAX;
    }
    
    for(i = 0; i < c; i++)
    {
        addr = p[i];
		
		if(addr > DC_MODULE_NUM_MAX)
		{
			continue;
		}
        
        if(dc_gr_master_info[addr].sel == 1)
        {
            dc_gr_master_info[addr].par.method = method;
        }
    }
}

void set_dc_gr_output_da(uint8_t slave_addr, uint16_t value)
{
    dc_gr_master_info[slave_addr].par.da = value;
}

uint8_t get_cur_cal_dc_module_addr(void)
{
    return cur_dc_module_addr;
}
void set_cur_cal_dc_module_addr(uint8_t addr)
{
    cur_dc_module_addr = addr;
}

int32_t find_dc_gr_map_ratio_n(uint8_t slove_num, uint8_t *map_r_n)
{
    uint8_t *p = get_dc_module_base_addr();
    uint8_t c = get_dc_module_using_num();
    uint32_t i = 0;
    
    for(i = 0; i < c && i < 5; i++)
    {
        if(p[i] == slove_num)
        {
            *map_r_n = i;
            return 0;
        }
    }
    
    return -1;
}
void select_dc_gr_moduls(uint8_t type, uint8_t slave_addr)
{
    uint32_t i = 0;
    uint8_t addr = 0;
    
    for(i = 0; i < DC_MODULE_NUM_MAX; i++)
    {
        addr = i + 1;
        
        if(dc_gr_master_info[addr].en == 1)
        {
            if(type == DC_GR_SEL_ONE)
            {
                if(dc_gr_master_info[addr].id.id == slave_addr)
                {
                    dc_gr_master_info[addr].sel = 1;
                }
                else
                {
                    dc_gr_master_info[addr].sel = 0;
                }
            }
            else
            {
                dc_gr_master_info[addr].sel = 1;
            }
        }
    }
}

/*
 * 函数名:select_dc_gr_n_moduls
 * 描述  :选择直流接地模块n个
 * 输入  : n 选择模块的数目,选择前n个模块
 * 输出  :无
 * 返回  :无
 */
void select_dc_gr_n_moduls(uint8_t n)
{
    uint32_t i = 0;
    uint8_t addr = 0;
    uint8_t count = get_dc_module_using_num();
    uint8_t c = 0;
    
    for(i = 0; i < DC_MODULE_NUM_MAX; i++)
    {
        addr = i + 1;
        
        if(c < n && dc_gr_master_info[addr].en == 1)
        {
            dc_gr_master_info[addr].sel = 1;
            c++;
        }
        else
        {
            dc_gr_master_info[addr].sel = 0;
        }
    }
}

void send_dc_module_cmd(DC_MODULE_CMD_T cmd, uint16_t value)
{
    uint8_t slave_addr = 0;
    int32_t i = 0;
    uint8_t *p = get_dc_module_base_addr();
    uint8_t count = get_dc_module_using_num();
    uint8_t addr = 0;
    
    if(count > 16)
    {
        count = 16;
    }
    
    for(i = 0; i < count; i++)
    {
        if(p[i] > 15)
        {
            p[i] = i;
        }
    }
    
    switch(cmd)
    {
        case DC_MODULE_CMD_START:
            for(i = 0; i < count && i < 16; i++)
            {
                addr = p[i];
                if(dc_gr_master_info[addr].sel == 1)
                {
                    set_start_dc_module_bridge(addr);
                }
            }
            break;
        case DC_MODULE_CMD_STOP:
            set_stop_dc_module_bridge(0);/* 广播 */
            set_stop_dc_module_bridge(0);/* 广播 */
//             for(i = 0; i < count && i < 16; i++)
//             {
//                 addr = p[i];
//                 if(dc_gr_master_info[addr].sel == 1)
//                 {
//                     set_stop_dc_module_bridge(addr);
//                 }
//             }
            break;
        case DC_MODULE_CMD_FETCH:
            for(i = 0; i < count && i < 16; i++)
            {
                addr = p[i];
                if(dc_gr_master_info[addr].sel == 1)
                {
                    get_test_fetch_dc_module_bridge(addr);
                }
            }
            break;
        case DC_MODULE_CMD_FETCH_SHORT:
            for(i = 0; i < count && i < 16; i++)
            {
                addr = p[i];
                if(dc_gr_master_info[addr].sel == 1)
                {
                    get_test_fetch_short_dc_module_bridge(addr);
                }
            }
            break;
        case DC_MODULE_CMD_METHOD:
            for(i = 0; i < count && i < 16; i++)
            {
                addr = p[i];
                if(dc_gr_master_info[addr].sel == 1)
                {
                    set_test_method_dc_module_bridge(addr, value);
                }
            }
            break;
        case DC_MODULE_CMD_GEAR:
            set_vol_gear_dc_module_bridge(slave_addr, value);
            break;
        case DC_MODULE_CMD_DC:
            for(i = 0; i < count && i < 16; i++)
            {
                addr = p[i];
                if(dc_gr_master_info[addr].sel == 1)
                {
                    set_dac_value_dc_module_bridge(addr, value);
                }
            }
            break;
        case DC_MODULE_CMD_INFO:
            /* 此命令是开机检查模块时使用 */
            for(i = 0; i < count && i < 16; i++)
            {
                addr = p[i];
                get_info_dc_module_bridge(addr);
            }
            break;
        case DC_MODULE_CMD_SET_CAL_DATA:
            for(i = 0; i < count; i++)
            {
                addr = p[i];
                if(dc_gr_master_info[addr].sel == 1)
                {
                    set_cal_data_dc_module_bridge(addr);
                }
            }
            break;
        case DC_MODULE_CMD_GET_CAL_DATA:
            /* 此命令是开机使用 */
            for(i = 0; i < count; i++)
            {
                addr = p[i];
                get_cal_data_dc_module_bridge(addr);
            }
            break;
        case DC_MODULE_CMD_SCAN_MODULES:
            get_info_dc_module_bridge(value);
            break;
        case DC_MODULE_CMD_SET_DATA:
            for(i = 0; i < count; i++)
            {
                addr = p[i];
                
                if(dc_gr_master_info[addr].sel == 1)
                {
                    set_data_dc_module_bridge(addr);
                }
            }
            break;
    }
}

/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

