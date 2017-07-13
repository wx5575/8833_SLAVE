/*
 * Copyright(c) 2015,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_err_log.c
 * 摘  要  ：错误日志相关的接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#define _99xxERR_LOG_GLOBALS
#include "stm32f10x.h"
#include "cs99xx_struct.h"
#include "on_chip_flash_api.h"
#include "stm32f10x_flash.h"
#include "RTC_Config.h"
#include "cs99xx_err_log.h"


/*
 * 函数名：updata_excep_info
 * 描述  ：更新异常信息
 * 输入  ：p_excep 指向要写入的异常信息
 * 输出  ：无
 * 返回  ：无
 */
void updata_excep_info(EXCEP_INFO_STR *p_excep)
{
    uint32_t fpage = (EXCEP_INFO_ADDR - 0X08000000) / F_PAGE_SIZE_I;
    uint32_t i = 0;
    uint32_t n = 1024;
    uint16_t *total_exce = on_chip_flash_page_buf;
    uint8_t *p1 = NULL;
    uint8_t *p2 = NULL;
    uint32_t temp = 0;
    EXCEP_INFO_STR *p_e = NULL;
    
    /* 读出所有异常信息 */
    for(i = 0; i < n; i++)
    {
        on_chip_flash_page_buf[i] = ((uint16_t*)EXCEP_INFO_ADDR)[i];
    }
    
    /* 异常信息计数 大于最大值就清零 */
    if(*total_exce >= MAX_EXCE_INFO_NUM)
    {
        *total_exce = 0;
    }
    
    p_e = ((EXCEP_INFO_STR *)&on_chip_flash_page_buf[1]) + *total_exce; /* 定位到新的位置 */
    
    p_excep->excep_time = Time_GetTime();/* 记录系统时间 */
    
    temp = sizeof(EXCEP_INFO_STR);
    p1 = (uint8_t *)p_e;
    p2 = (uint8_t *)p_excep;
    
    for(i = 0; i < temp; i++)
    {
        p1[i] = p2[i];/* 更新异常信息到缓存中 */
    }
    
    *total_exce = (*total_exce + 1) % MAX_EXCE_INFO_NUM;/* 总异常数递增 */
    
    erase_pages_flash(fpage * F_PAGE_SIZE_I + 0X08000000, 1);/* 擦除flash页 */
    
    write_one_page_bin(on_chip_flash_page_buf, EXCEP_INFO_ADDR);/* 写入flash */
}

/*
 * 函数名：clear_excep_info
 * 描述  ：清除所有的异常信息
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void clear_excep_info(void)
{
    uint32_t fpage = (EXCEP_INFO_ADDR - 0X08000000) / F_PAGE_SIZE_I;
    
    erase_pages_flash(fpage * F_PAGE_SIZE_I + 0X08000000, 1);
}

/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE****/
