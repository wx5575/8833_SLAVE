/*
 * Copyright(c) 2015,南京长盛仪器
 * All rights reserved
 * 文件名称：on_chip_flash_api.c
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#include "stm32f10x.h"
#include "cs99xx_struct.h"
#include "on_chip_flash_api.h"
#include "stm32f10x_flash.h"
#include "RTC_Config.h"


uint16_t on_chip_flash_page_buf[F_PAGE_SIZE_I/2];

/*
 * 函数名：erase_pages_flash
 * 描述  ：擦除内部flash指定的页
 * 输入  ：addr 要擦除的页地址
 *         pages  要擦除的页数
 * 输出  ：无
 * 返回  ：无
 */
void erase_pages_flash(uint32_t addr, uint32_t pages)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    uint32_t i = 0;
    
    FLASH_Unlock();               //解锁FLASH后才能向FLASH中写数据
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
    
    for (i = 0; (i < pages) && (FLASHStatus == FLASH_COMPLETE); i++)
    {
        while(FLASH_GetFlagStatus(FLASH_FLAG_BSY));
        FLASHStatus = FLASH_ErasePage(addr + (F_PAGE_SIZE_I * i));
    }
    
    FLASH_Lock();
}

/*
 * 函数名：write_u16_flash
 * 描述  ：以半字的方式向片内flash写入若干个数据
 * 输入  ：addr 要写入的地址
 *         buf  数据缓存
 *         size 数据个数
 * 输出  ：无
 * 返回  ：无
 */
void write_u16_flash(uint32_t addr, uint16_t *buf, uint16_t size)
{
    int32_t i = 0;
    
    FLASH_Unlock();
    
    for(i = 0; i < size; i++)
    {
        FLASH_ProgramHalfWord(addr + i * 2, buf[i]);
    }
    
    FLASH_Lock();
}

/*
 * 函数名：updata_sys_language_par
 * 描述  ：向片内flash更新语言信息，给bootloader程序使用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void updata_sys_language_par(void)
{
    uint16_t langu_t = *(uint16_t*)(SYS_LANGUAGE_ADDR);
    uint32_t fpage = (SYS_LANGUAGE_ADDR - 0X08000000) / F_PAGE_SIZE_I;
    
    if(langu_t == sys_par.language)
    {
        return;
    }
    
    langu_t = sys_par.language;
    
    erase_pages_flash(fpage * F_PAGE_SIZE_I + 0X08000000, 1);
    
    write_u16_flash(SYS_LANGUAGE_ADDR, &langu_t, 1);
}


/*
 * 函数名：write_one_page_bin
 * 描述  ：向片内flash写入1页数据
 * 输入  ：buf 数据缓存 addr 要写入flash的地址
 * 输出  ：无
 * 返回  ：无
 */
void write_one_page_bin(uint16_t *buf,uint32_t addr)
{
    int32_t i = 0;
    uint32_t temp = F_PAGE_SIZE_I / 2;
    
    FLASH_Unlock();
    
    for(i = 0; i < temp; i++)
    {
        FLASH_ProgramHalfWord(addr + i * 2, buf[i]);
    }
    
    FLASH_Lock();
}

/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE****/

