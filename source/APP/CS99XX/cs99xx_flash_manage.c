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
#include    "cs99xx_flash_manage.h"
#include	"cs99xx_parameter.h"
#include    "cs99xx_mem_api.h"
#include    "STM32_GPIO_Config.h"
#include    "cs99xx_err_log.h"
#include    "spi_flash.h"
#include    <stdio.h>
#include    "app.h"
#include    "string.h"
#include	"calibration.h"

uint8_t writebuffer_spi_flash(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize, uint8_t chip)
{
    uint8_t err = 0;
    
    flash_cs_set_at45db161(chip);
    
    OSMutexPend(MemMutex, 0, &err);
    
    at45_buffer_write(_pBuf, _uiWriteAddr, _usWriteSize);
    
	OSMutexPost(MemMutex);
    
    return 0;
}

void readbuffer_spi_flash(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize, uint8_t chip)
{
    uint8_t err = 0;
    
    flash_cs_set_at45db161(chip);
    
    OSMutexPend(MemMutex, 0, &err);
    
    at45_buffer_read(_pBuf, _uiReadAddr, _uiSize);
    
	OSMutexPost(MemMutex);
}

void save_ratio_from_flash(uint8_t mode)
{
    switch(mode)
    {
    case ACW:
        writebuffer_spi_flash((uint8_t*)&ratio_acw, RATIO_ACW_BASE, sizeof ratio_acw, SPI_FLASH_CHIP3);
        break;
    case DCW:
        writebuffer_spi_flash((uint8_t*)&ratio_dcw, RATIO_DCW_BASE,  sizeof ratio_dcw, SPI_FLASH_CHIP3);
        break;
    case IR:
        writebuffer_spi_flash((uint8_t*)&ratio_ir, RATIO_IR_BASE,  sizeof ratio_ir, SPI_FLASH_CHIP3);
        break;
    case GR:
        writebuffer_spi_flash((uint8_t*)&ratio_gr, RATIO_GR_BASE,  sizeof ratio_gr, SPI_FLASH_CHIP3);
        break;
    case BBD:
        writebuffer_spi_flash((uint8_t*)&ratio_bbd, F_RATIO_BBD_BASE,  sizeof ratio_bbd, SPI_FLASH_CHIP3);
        break;
    }
}

int32_t read_ratio_from_flash(void)
{
    uint8_t flag = 0;
    
    readbuffer_spi_flash((uint8_t*)&ratio_acw, F_RATIO_ACW_BASE, sizeof ratio_acw, SPI_FLASH_CHIP3);
    readbuffer_spi_flash((uint8_t*)&ratio_dcw, F_RATIO_DCW_BASE, sizeof ratio_dcw, SPI_FLASH_CHIP3);
    readbuffer_spi_flash((uint8_t*)&ratio_ir , F_RATIO_IR_BASE,  sizeof ratio_ir, SPI_FLASH_CHIP3);
    readbuffer_spi_flash((uint8_t*)&ratio_gr , F_RATIO_GR_BASE,  sizeof ratio_gr, SPI_FLASH_CHIP3);
    readbuffer_spi_flash((uint8_t*)&ratio_bbd, F_RATIO_BBD_BASE,  sizeof ratio_bbd, SPI_FLASH_CHIP3);
	
    /* CRC 校验 */
    CRC_ResetDR();
    if(ratio_acw.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_acw, sizeof ratio_acw / 4 - 1))
    {
        init_ratio(ACW);
        save_ratio_from_flash(ACW);
        flag++;
    }
    
    CRC_ResetDR();
    if(ratio_dcw.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_dcw, sizeof ratio_dcw / 4 - 1))
    {
        init_ratio(DCW);
        save_ratio_from_flash(DCW);
        flag++;
    }
    
    CRC_ResetDR();
    if(ratio_ir.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_ir, sizeof ratio_ir / 4 - 1))
    {
        init_ratio(IR);
        save_ratio_from_flash(IR);
        flag++;
    }
    
    CRC_ResetDR();
    if(ratio_gr.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_gr, sizeof ratio_gr / 4 - 1))
    {
        init_ratio(GR);
        save_ratio_from_flash(GR);
        flag++;
    }
	
    CRC_ResetDR();
    if(ratio_bbd.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_bbd, sizeof ratio_bbd / 4 - 1))
    {
        init_ratio(BBD);
        save_ratio_from_flash(BBD);
        flag++;
    }
	
    if(flag)
    {
        return -1;
    }
    return 0;
}

void save_list_flash(void)
{
	NODE_STEP *pnode = NULL;
	int8_t i = 0;
    uint16_t offset_addr = TABLE_VALUE_NULL;
    uint16_t step = g_cur_step->one_step.com.step;
    
	/* 将当前记忆组中所有步都写入eep中 */
	for(i = 0, pnode=list_99xx.head; pnode; pnode=pnode->next, i++)
	{
        offset_addr = cur_group_table[i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            return;
        }
        
        save_one_step_flash(pnode, g_cur_file->num, step + i);
	}
}

void copy_cur_file_to_new_pos_flash(const int8_t n)
{
	NODE_STEP node;
	int8_t i = 0;
	char temp_buf[NAME_LON];
    uint32_t offset_addr = TABLE_VALUE_NULL;
    
	if(0 == check_file_exist(n))
	{
		return;
	}
	
	strcpy(temp_buf, file_table[n].name);/* 备份文件名 */
	/* 更新文件信息 */
	file_table[n] = *g_cur_file;
	strcpy(file_table[n].name, temp_buf);/* 恢复文件名 */
	file_table[n].num = n;
    
	save_file(n);
    save_group_table_flash(n);
    save_step_used_flag_flash(n);
	
    for(i = 0; i < g_cur_file->total; i++)
    {
        offset_addr = cur_group_table[i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            return;
        }
        
        read_one_step_flash(&node, g_cur_file->num, i + 1);
        
        save_one_step_flash(&node, n, i + 1);
    }
}

int32_t init_group_step_used_flag_flash(void)
{
    int32_t i = 0;
    
    memset(step_used_flag, 0, F_FILE_USED_STEP_FLAG_OFFSET);
    
    for(i = 0; i < MAX_FILES; i++)
    {
        writebuffer_spi_flash((uint8_t*)step_used_flag,
                  F_FILE_USED_STEP_FLAG_BASE + F_FILE_USED_STEP_FLAG_OFFSET * i,
                  F_FILE_USED_STEP_FLAG_OFFSET, SPI_FLASH_CHIP1);
    }
    
    return 0;
}

/* n就是文件号 */
void save_step_used_flag_flash(uint8_t n)
{
    if(n > MAX_FILES)
    {
        return;
    }
    
    writebuffer_spi_flash((uint8_t*)step_used_flag,
                F_FILE_USED_STEP_FLAG_BASE + F_FILE_USED_STEP_FLAG_OFFSET * n,
                F_FILE_USED_STEP_FLAG_OFFSET, SPI_FLASH_CHIP1);
}

/* n就是文件号 */
void read_step_used_flag_flash(const uint8_t n)
{
    if(n > MAX_FILES)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)step_used_flag,
                F_FILE_USED_STEP_FLAG_BASE + F_FILE_USED_STEP_FLAG_OFFSET * n,
                F_FILE_USED_STEP_FLAG_OFFSET, SPI_FLASH_CHIP1);
}

/* n就是文件号 */
void read_group_table_flash(const uint8_t n)
{
    if(n > MAX_FILES)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)cur_group_table,
                  F_FILE_STEP_TABLE_BASE
                + F_GROP_TABLE_SIZE * n,
                  F_GROP_TABLE_SIZE, SPI_FLASH_CHIP1);
}

int32_t init_file_step_table_flash(void)
{
    int32_t i = 0;
    
    memset(cur_group_table, 0xff, sizeof cur_group_table);
    clear_group_table();
    
    for(i = 0; i < MAX_FILES; i++)
    {
        save_group_table_flash(i);
    }
    
    return 0;
}

void save_as_cur_group_flash(const uint8_t file_num)
{
	NODE_STEP node;
	int8_t i = 0;
    TEST_FILE *pfile = pos_file(file_num);
    uint32_t offset_addr = 0;
	
	if(0 == check_file_exist(file_num))
	{
		return;
	}
    
    save_step_used_flag_flash(file_num);
    save_group_table_flash(file_num);
	
	/* 更新文件信息 */
	pfile->total = g_cur_file->total;
    
    save_file_flash(file_num);
	
    for(i = 0; i < g_cur_file->total; i++)
    {
        offset_addr = cur_group_table[i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            return;
        }
        
        read_one_step_flash(&node, g_cur_file->num, i + 1);
        save_one_step_flash(&node, file_num, i + 1);
    }
}

void save_init_group_flash(const uint8_t n, NODE_STEP *p_node)
{
    TEST_FILE *pfile = pos_file(n);
    TEST_FILE *p_file_bak = g_cur_file;
    
    if(pfile == NULL)
    {
        return;
    }
    
    g_cur_file = pfile;
    
    clear_step_used_flag();
    clear_group_table();
    
    step_used_flag[0] = 1;
    cur_group_table[0] = 0;
    g_cur_file->total = 1;
    
    save_file_flash(g_cur_file->num);
    save_step_used_flag_flash(g_cur_file->num);
    save_group_table_flash(g_cur_file->num);
	save_one_step_flash(p_node, g_cur_file->num, 1);
    
    g_cur_file = p_file_bak;
    read_step_used_flag_flash(g_cur_file->num);
    read_group_table_flash(g_cur_file->num);
}

int32_t save_file_flash(const int8_t n)
{
    if(n > MAX_FILES)
    {
        return -1;
    }
    
    writebuffer_spi_flash((uint8_t*)&file_table[n],
            F_FILE_BASE + F_FILE_OFFSET * n,
            F_FILE_OFFSET, SPI_FLASH_CHIP1);
	
	return 0;
}

void save_all_file_flash(void)
{
    int32_t i = 0;
    int32_t res;
    
    for(i = 0; i < MAX_FILES; i++)
    {
        res = save_file_flash(i);
        
        if(res == -1)
        {
            return;
        }
    }
}

void read_file_flash(const int8_t n)
{
    if(n > MAX_FILES - 1)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)&file_table[n],
            F_FILE_BASE + F_FILE_OFFSET * n,
            F_FILE_OFFSET, SPI_FLASH_CHIP1);
}

void read_all_file_flash(void)
{
    int32_t i = 0;
    
    for(i = 0; i < MAX_FILES; i++)
    {
        read_file_flash(i);
    }
}

int32_t save_sys_flag_flash(void)
{
    writebuffer_spi_flash((uint8_t*)&sys_flag,
            F_SYS_FLAG_BASE, F_SYS_FLAG_SIZE, SPI_FLASH_CHIP1);
	
	return 0;
}

void read_sys_flag_flash(void)
{
    readbuffer_spi_flash((uint8_t*)&sys_flag, F_SYS_FLAG_BASE, F_SYS_FLAG_SIZE, SPI_FLASH_CHIP1);
}

int32_t save_sys_par_flash(void)
{
	writebuffer_spi_flash((uint8_t*)&sys_par, F_SYS_PAR_BASE, F_SYS_PAR_SIZE, SPI_FLASH_CHIP1);
	
	return 0;
}

void save_group_table_flash(const uint8_t n)
{
    if(n > MAX_FILES - 1)
    {
        return;
    }
    
    writebuffer_spi_flash((uint8_t*)cur_group_table,
                F_FILE_STEP_TABLE_BASE + F_FILE_STEP_TABLE_OFFSET * n,
                F_FILE_STEP_TABLE_OFFSET, SPI_FLASH_CHIP1);
}

void read_sys_par_flash(void)
{
    readbuffer_spi_flash((uint8_t*)&sys_par, F_SYS_PAR_BASE, F_SYS_PAR_SIZE, SPI_FLASH_CHIP1);
}

void read_one_step_flash(NODE_STEP *node, const uint8_t file_num, const uint16_t step)
{
    uint16_t offset_addr = TABLE_VALUE_NULL;
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
    offset_addr = cur_group_table[step - 1];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)&node->one_step,
                  F_GROUP_BASE + F_GROUP_OFFSET * file_num + F_STEP_OFFSET * offset_addr,
                  F_UN_SIZE, SPI_FLASH_CHIP1);
}

void save_one_step_flash(NODE_STEP *node, const uint8_t file_num, const uint16_t step)
{
    uint16_t offset_addr = TABLE_VALUE_NULL;
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
    offset_addr = cur_group_table[step - 1];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        return;
    }
    
    writebuffer_spi_flash((uint8_t*)&node->one_step,
                  F_GROUP_BASE + F_GROUP_OFFSET * file_num + F_STEP_OFFSET * offset_addr,
                  F_UN_SIZE, SPI_FLASH_CHIP1);
}

uint8_t get_first_step_mode_flash(void)
{
    uint16_t offset_addr = TABLE_VALUE_NULL;
    NODE_STEP node;
    
    offset_addr = cur_group_table[0];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        return 0;
    }
    
    if(offset_addr != TABLE_VALUE_NULL && offset_addr < MAX_STEPS)
    {
        read_one_step_flash(&node, g_cur_file->num, 1);
        
        /* 计算出读出的步是第几步 并对错误进行修正 */
        if(node.one_step.com.step != 1)
        {
            node.one_step.com.step = 1;
            save_one_step_flash(&node, g_cur_file->num, 1);
        }
        
        return node.one_step.com.mode;
    }
    
    return 0;
}
uint8_t led_x = 0;
void load_steps_to_list_flash(const int16_t step)
{
    int32_t num = 0;
    uint16_t offset_addr = 0;
    int32_t i = 0;
    
    if(list_lon > 5)
    {
        return;
    }
    
    if(list_lon == 0)
    {
        list_lon = 2;
    }
    
    init_99xx_list();/* 初始化测试步链表 */
    
    if(g_cur_file->total == step)
    {
        num = 1;
    }
    else if(g_cur_file->total > step)
    {
        num = (g_cur_file->total - step + 1)>=list_lon? list_lon:((g_cur_file->total - step + 1)%list_lon);
    }
    else
    {
        return;
    }
    
	for(i = 0; i < num && i < list_lon; i++)
	{
		insert_node(i);//在当前步后面插入新步
	}
    
    for(i = 0; i < list_lon && i < num; i++)
    {
        offset_addr = cur_group_table[step - 1 + i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            return;
        }
        
        if(offset_addr != TABLE_VALUE_NULL && offset_addr < MAX_STEPS)
        {
            uint8_t retry_times = 0;
			
            do
            {
                read_one_step_flash(&test_group.test_steps[i], g_cur_file->num, step + i);
                
                if(retry_times > 0)
                {
                    led_x++;
                }
                
                if(++retry_times > 3)
                {
                    RECORD_EXCEP("memory err");
                    while(1)
                    {
                        if(STOP_PIN == 0)
                        {
                            return;
                        }
                    }
                }
            }while(0 > check_test_step_data(&test_group.test_steps[i], 1));
            
            /* 计算出读出的步是第几步 并对错误进行修正 */
            test_group.test_steps[i].one_step.com.step = step + i;
        }
    }
}

void save_one_result_flash(RESULT_STR *res, uint16_t n)
{
    if(n > F_RES_TOTAL)
    {
        return;
    }
    
    writebuffer_spi_flash((uint8_t*)res, F_RESULT_BASE + F_RESULT_SIZE * n,
            F_RESULT_SIZE, SPI_FLASH_CHIP2);
}

void save_one_result_list_dis_flash(RESULT_STR *res, uint16_t n)
{
    writebuffer_spi_flash((uint8_t*)res,
        F_RESULT_LIST_DIS_BASE + F_RESULT_SIZE * n,
            F_RESULT_SIZE, SPI_FLASH_CHIP2);
}

/*
 * 函数名：read_result_list_dis_one_page_flash
 * 描述  ：读当前界面页的列表显示结果数据，用于结果列表显示
 * 输入  ：res 数据缓冲 n 结果序号 0-max_res_total
 * 输出  ：无
 * 返回  ：无
 */
void read_one_result_flash(RESULT_STR *res, uint16_t n)
{
    if(n > F_RES_TOTAL)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)res, F_RESULT_BASE + F_RESULT_SIZE * n, F_RESULT_SIZE,
            SPI_FLASH_CHIP2);
}

/*
 * 函数名：read_result_list_dis_one_page_flash
 * 描述  ：读当前界面页的列表显示结果数据，用于结果列表显示
 * 输入  ：res 数据缓冲 page 页码 cur_page_num 当前读的结果数
 * 输出  ：无
 * 返回  ：无
 */
void read_result_list_dis_one_page_flash(RESULT_STR *res, uint16_t page, uint16_t cur_page_num)
{
    readbuffer_spi_flash((uint8_t*)res,
        F_RESULT_LIST_DIS_BASE + F_RESULT_SIZE * (4 * (page - 1)),
        F_RESULT_SIZE * cur_page_num, SPI_FLASH_CHIP2);
}
/*
 * 函数名：read_result_list_dis_one_flash
 * 描述  ：读一步的结果 用于结果列表显示
 * 输入  ：res 数据缓冲
 *         n 结果偏移地址 从0开始
 * 输出  ：无
 * 返回  ：无
 */
void read_result_list_dis_one_flash(RESULT_STR *res, uint16_t n)
{
    readbuffer_spi_flash((uint8_t*)res, F_RESULT_LIST_DIS_BASE + F_RESULT_SIZE * n,
                F_RESULT_SIZE, SPI_FLASH_CHIP2);
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
