/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_eep_manage.c
 * 摘  要  ：提供eep管理接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */


#include    <stdio.h>
#include	<string.h>
#include    "stm32f10x.h"
#include	"i2c_ee.h"
#include	"cs99xx_parameter.h"
#include    "cs99xx_mem_api.h"
#include	"calibration.h"
#include 	"serve_test.h"
#include    "cs99xx_err_log.h"
#include    "app.h"
#include    "cs99xx_type.h"
#include	"RTC_Config.h"



/*************************** 外部变量 **************************/

void eep_buf_write(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite, uint8_t id)
{
    uint8_t err = 0;
    
	EEPROM_ADDRESS = id;
    OSMutexPend(MemMutex, 0, &err);
	I2C_EE_BufferWrite(pBuffer, WriteAddr, NumByteToWrite);
	OSMutexPost(MemMutex);
}
void eep_buf_read(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead, uint8_t id)
{
    uint8_t err = 0;
    
	EEPROM_ADDRESS = id;
    
    OSMutexPend(MemMutex, 0, &err);
	I2C_EE_BufferRead(pBuffer, ReadAddr, NumByteToRead);
	OSMutexPost(MemMutex);
}
/*
 * 函数名：save_ratio_to_eep
 * 描述  ：将校准系数保存到eep
 * 输入  ：mode 测试模式
 * 输出  ：无
 * 返回  ：无
 */
void save_ratio_to_eep(const int8_t mode)
{
    CRC_ResetDR();/* 复位crc */
	switch(mode)
	{
		case ACW:
            ratio_acw.crc_val = CRC_CalcBlockCRC((uint32_t *)&ratio_acw, sizeof ratio_acw / 4 - 1);/* 计算 crc */
			eep_buf_write((u8 *)&ratio_acw, RATIO_ACW_BASE, sizeof ratio_acw, CHIP3);
			break;
		case DCW:
            ratio_dcw.crc_val = CRC_CalcBlockCRC((uint32_t *)&ratio_dcw, sizeof ratio_dcw / 4 - 1);
			eep_buf_write((u8 *)&ratio_dcw, RATIO_DCW_BASE, sizeof ratio_dcw, CHIP3);
			break;
		case IR:
            ratio_ir.crc_val = CRC_CalcBlockCRC((uint32_t *)&ratio_ir, sizeof ratio_ir / 4 - 1);
			eep_buf_write((u8 *)&ratio_ir, RATIO_IR_BASE, sizeof ratio_ir, CHIP3);
			break;
		case GR:
            ratio_gr.crc_val = CRC_CalcBlockCRC((uint32_t *)&ratio_gr, sizeof ratio_gr / 4 - 1);
			eep_buf_write((u8 *)&ratio_gr, RATIO_GR_BASE, sizeof ratio_gr, CHIP3);
			break;
		case BBD:
            ratio_bbd.crc_val = CRC_CalcBlockCRC((uint32_t *)&ratio_bbd, sizeof ratio_bbd / 4 - 1);
			eep_buf_write((u8 *)&ratio_bbd, RATIO_BBD_BASE, sizeof ratio_bbd, CHIP3); 
			break;
	}
}

/*
 * 函数名：read_ratio_to_eep
 * 描述  ：读出指定模式的校准系数
 * 输入  ：mode 测试模式
 * 输出  ：无
 * 返回  ：无
 */
void read_ratio_to_eep(const int8_t mode)
{
	switch(mode)
	{
		case ACW:
			eep_buf_read((u8 *)&ratio_acw, RATIO_ACW_BASE, sizeof ratio_acw, CHIP3);
			break;
		case DCW:
			eep_buf_read((u8 *)&ratio_dcw, RATIO_DCW_BASE, sizeof ratio_dcw, CHIP3);
			break;
		case IR:
			eep_buf_read((u8 *)&ratio_ir, RATIO_IR_BASE, sizeof ratio_ir, CHIP3);
			break;
		case GR:
			eep_buf_read((u8 *)&ratio_gr, RATIO_GR_BASE, sizeof ratio_gr, CHIP3);
			break;
		case BBD:
			eep_buf_read((u8 *)&ratio_bbd, RATIO_BBD_BASE, sizeof ratio_bbd, CHIP3);
			break;
	}
}
/*
 * 函数名：read_ratio_from_eep
 * 描述  ：当调用该函数时，从eep中读出所有的步到内存
 * 输入  ：m 文件编号
 * 输出  ：无
 * 返回  ：-1校准系数校验不通过 0 正常结束
 */
int32_t read_ratio_from_eep(void)
{
    uint8_t flag = 0;
    
	eep_buf_read((uint8_t*)&ratio_acw, RATIO_ACW_BASE, sizeof ratio_acw, CHIP3);
	eep_buf_read((uint8_t*)&ratio_dcw, RATIO_DCW_BASE, sizeof ratio_dcw, CHIP3);
	eep_buf_read((uint8_t*)&ratio_ir , RATIO_IR_BASE , sizeof ratio_ir , CHIP3);
	eep_buf_read((uint8_t*)&ratio_gr , RATIO_GR_BASE , sizeof ratio_gr , CHIP3);
	eep_buf_read((uint8_t*)&ratio_bbd, RATIO_BBD_BASE, sizeof ratio_bbd, CHIP3);
	
    /* CRC 校验 */
    CRC_ResetDR();
    if(ratio_acw.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_acw, sizeof ratio_acw / 4 - 1))
    {
        init_ratio(ACW);
        save_ratio_to_eep(ACW);
        RECORD_EXCEP("eep acw cal. crc err");
        flag++;
    }
    
    CRC_ResetDR();
    if(ratio_dcw.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_dcw, sizeof ratio_dcw / 4 - 1))
    {
        init_ratio(DCW);
        save_ratio_to_eep(DCW);
        RECORD_EXCEP("eep dcw cal. crc err");
        flag++;
    }
    
    CRC_ResetDR();
    if(ratio_ir.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_ir, sizeof ratio_ir / 4 - 1))
    {
        init_ratio(IR);
        save_ratio_to_eep(IR);
        RECORD_EXCEP("eep ir cal. crc err");
        flag++;
    }
    
    CRC_ResetDR();
    if(ratio_gr.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_gr, sizeof ratio_gr / 4 - 1))
    {
        init_ratio(GR);
        save_ratio_to_eep(GR);
        RECORD_EXCEP("eep gr cal. crc err");
        flag++;
    }
	
    CRC_ResetDR();
    if(ratio_bbd.crc_val != CRC_CalcBlockCRC((uint32_t *)&ratio_bbd, sizeof ratio_bbd / 4 - 1))
    {
        init_ratio(BBD);
        save_ratio_to_eep(BBD);
        RECORD_EXCEP("eep bbd cal. crc err");
        flag++;
    }
	
    if(flag)
    {
        return -1;
    }
    return 0;
}


/*
 * 函数名：save_group_table_eep
 * 描述  ：保存记忆组映射表到eep
 * 输入  ：file_num 记忆组编号
 * 输出  ：无
 * 返回  ：无
 */
void save_group_table_eep(const uint8_t file_num)
{
    eep_buf_write((uint8_t*)cur_group_table,
                FILE_STEP_TABLE_BASE + file_num * E_GROP_TABLE_SIZE, E_GROP_TABLE_SIZE, CHIP0);
}
/*
 * 函数名：read_one_res_from_eep
 * 描述  ：读出一步结果信息
 * 输入  ：res 数据缓冲
 *         n   指定读的是第几条
 * 输出  ：无
 * 返回  ：无
 */
void read_one_res_from_eep(RESULT_STR *res, const uint16_t n)
{
	if(n < 1)
    {
        return;
    }
    
	eep_buf_read((uint8_t*)res, RESULT_BASE_A2 + RESULT_OFFSET * (n-1),
				sizeof(RESULT_STR) * 1, CHIP2);
}


extern int32_t dcgr_communication_check(void);
static void count_eep_pos(uint8_t n, uint8_t *chip, uint16_t *add_base, uint8_t *pos);

/*
 * 函数名：read_group_table_eep
 * 描述  ：读出指定记忆组的步骤映射表
 * 输入  ：file_num 记忆组编号
 * 输出  ：无
 * 返回  ：无
 */
void read_group_table_eep(const uint8_t file_num)
{
    eep_buf_read((uint8_t*)cur_group_table,
                FILE_STEP_TABLE_BASE + file_num * E_GROP_TABLE_SIZE, E_GROP_TABLE_SIZE, CHIP0);
}

/*
 * 函数名：save_as_cur_group_eep
 * 描述  ：当前记忆组另存为到指定的记忆组
 * 输入  ：file_num 记忆组编号
 * 输出  ：无
 * 返回  ：无
 */
void save_as_cur_group_eep(const uint8_t file_num)
{
	NODE_STEP node;
	int8_t i = 0;
    TEST_FILE *pfile = pos_file(file_num);
    uint32_t offset_addr = 0;
	
	if(0 == check_file_exist(file_num))
	{
        RECORD_EXCEP("file no exist err");
		return;
	}
	
	/* 更新文件信息 */
    pfile->total = g_cur_file->total;
    save_group_table_eep(file_num);
    save_step_used_flag_eep(file_num);
    save_file_to_eep(file_num);
	
    /* 拷贝记忆组到指定文件位置 */
    for(i = 0; i < g_cur_file->total; i++)
    {
        offset_addr = cur_group_table[i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            RECORD_EXCEP("TABLE_VALUE_NULL err");
            return;
        }
        
        read_one_step_to_eep(&node, g_cur_file->num, i + 1);
        save_one_step_to_eep(&node, file_num, i + 1);
    }
}
/*
 * 函数名：init_file_step_table_eep
 * 描述  ：清空所有记忆组的位图表
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_group_step_used_flag_eep(void)
{
    int32_t i = 0;
    
    clear_step_used_flag();
    
    for(i = 0; i < MAX_FILES; i++)
    {
        eep_buf_write((uint8_t*)step_used_flag, FILE_USED_STEP_FLAG_BASE + FILE_USED_STEP_FLAG_OFFSET * i, sizeof step_used_flag, CHIP0);
    }
}

/*
 * 函数名：init_group_step_used_flag_eep
 * 描述  ：清空所有记忆组的映射表
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void init_file_step_table_eep(void)
{
    int32_t i = 0;
    
    clear_group_table();
    
    for(i = 0; i < MAX_FILES; i++)
    {
        eep_buf_write((uint8_t*)cur_group_table, FILE_STEP_TABLE_BASE + E_GROP_TABLE_OFFSET * i, sizeof cur_group_table, CHIP0);
    }
}

/*
 * 函数名：count_eep_pos
 * 描述  ：计算eep的存放信息
 * 输入  ：n 文件号
 * 输出  ：chip 那一片eep, add_base 所在片子的基址 pos 从基址开始的偏移位置
 * 返回  ：无
 */
static void count_eep_pos(uint8_t n, uint8_t *chip, uint16_t *add_base, uint8_t *pos)
{
	if(n < MAX_FILES_A0)
	{
		*chip = CHIP0;
		*add_base = MEM_GROUP_BASE;
		*pos = n;
	}
	else if(n < (MAX_FILES_A0 + MAX_FILES_A1))
	{
		*chip = CHIP1;
		*add_base = MEM_GROUP_BASE_A1;
		*pos = n - MAX_FILES_A0;
	}
	else if(n < (MAX_FILES_A0 + MAX_FILES_A1 + MAX_FILES_A2))
	{
		*chip = CHIP2;
		*add_base = MEM_GROUP_BASE_A2;
		*pos = n - MAX_FILES_A0 - MAX_FILES_A2;
	}
	else
	{
        RECORD_EXCEP("count group chip err");
		while(1);/* 出错 */
	}
}

/*
 * 函数名：save_sys_par_to_eep
 * 描述  ：当调用该函数时，将当前步写入到eep中
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void save_sys_par_to_eep(void)
{
	eep_buf_write((uint8_t*)&sys_par, SYS_PAR_EE_ADDR, sizeof sys_par, CHIP0);
}
/*
 * 函数名：read_sys_par_from_eep
 * 描述  ：读系统参数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void read_sys_par_from_eep(void)
{
	eep_buf_read((uint8_t*)&sys_par, SYS_PAR_EE_ADDR, sizeof sys_par, CHIP0);
}

/*
 * 函数名：save_list_to_eep
 * 描述  ：当调用该函数时，将当前记忆组保存到eep
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void save_list_to_eep(void)
{
	NODE_STEP *pnode = NULL;
	int8_t i = 0;
    uint16_t offset_addr = TABLE_VALUE_NULL;
    
	/* 将当前记忆组中所有步都写入eep中 */
	for(i = 0, pnode=list_99xx.head; pnode; pnode=pnode->next, i++)
	{
        offset_addr = cur_group_table[i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            RECORD_EXCEP("TABLE_VALUE_NULL err");
            return;
        }
        
        save_one_step_to_eep(pnode, g_cur_file->num, pnode->one_step.com.step);
	}
}

void copy_cur_file_to_new_pos_eep(const int8_t n)
{
	NODE_STEP node;
	char temp_buf[NAME_LON];
    uint16_t offset_addr = TABLE_VALUE_NULL;
    int32_t i = 0;
	
	if(0 == check_file_exist(n))
	{
        RECORD_EXCEP("file no exist err");
		return;
	}
	
	strcpy(temp_buf, file_table[n].name);/* 备份文件名 */
	/* 更新文件信息 */
	file_table[n] = *g_cur_file;
	strcpy(file_table[n].name, temp_buf);
	file_table[n].num = n;
    file_table[n].create_time = Time_GetTime();/* 保存当前RTC时间 */
    
	save_file_to_eep(n);
    save_group_table_eep(n);
    save_step_used_flag_eep(n);
	
    for(i = 0; i < g_cur_file->total; i++)
    {
        offset_addr = cur_group_table[i];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            return;
        }
        
        read_one_step_to_eep(&node, g_cur_file->num, i + 1);
        save_one_step_to_eep(&node, n, i + 1);
    }
}
void save_init_group_to_eep(const uint8_t n, NODE_STEP *p_node)
{
	int8_t step = 1;
	uint16_t add_base = 0;
	uint8_t chip = 0;
	uint8_t pos = 0;
	uint16_t offset_addr = TABLE_VALUE_NULL;
    TEST_FILE *pfile = pos_file(n);
    
    offset_addr = cur_group_table[step - 1];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        RECORD_EXCEP("TABLE_VALUE_NULL err");
        return;
    }
    
	count_eep_pos(n, &chip, &add_base, &pos);
	
	eep_buf_write((uint8_t*)&p_node->one_step,
						add_base + MEM_GROUP_OFFSET * pos + STEP_OFFSET * offset_addr, sizeof(UN_STRUCT), chip);
	
#if SYNCHRO_ROADS_EN>0
	eep_buf_write((uint8_t*)&p_node->syn_roads,
						add_base + MEM_GROUP_OFFSET * pos + STEP_OFFSET * offset_addr + UN_ONE_SIZE, SYN_ONE_SIZE, chip);
#endif
}

/*
 * 函数名：save_one_step_to_eep
 * 描述  ：当调用该函数时，保存一步到指定的文件中去,注意，是已有步骤编辑后调用的，不能用来新建步
 * 输入  ：node 数据缓冲
 *         file_num 记忆组编号
 *         step  步骤编号
 * 输出  ：无
 * 返回  ：无
 */
void save_one_step_to_eep(NODE_STEP	*node, uint8_t file_num, uint8_t step)
{
	uint8_t n = file_num;
	uint16_t add_base = 0;
	uint8_t chip = 0;
	uint8_t pos = 0;
	uint16_t offset_addr = TABLE_VALUE_NULL;
    
    if(node == NULL)
    {
        return;
    }
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
	count_eep_pos(n, &chip, &add_base, &pos);
    
    offset_addr = cur_group_table[step - 1];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        RECORD_EXCEP("TABLE_VALUE_NULL err");
        return;
    }
	node->one_step.com.step = step;
    
	eep_buf_write((uint8_t*)&node->one_step,
						add_base + MEM_GROUP_OFFSET * pos + STEP_OFFSET * offset_addr, sizeof(UN_STRUCT), chip);
	
#if SYNCHRO_ROADS_EN
	eep_buf_write((uint8_t*)&node->syn_roads,
						add_base + MEM_GROUP_OFFSET * pos + STEP_OFFSET * offset_addr + UN_ONE_SIZE, SYN_ONE_SIZE, chip);
#endif
}

/*
 * 函数名：read_one_step_to_eep
 * 描述  ：从eep中读出一步的数据到内存
 * 输入  ：node 数据缓冲
 *         file_num 记忆组编号
 *         step  步骤编号
 * 输出  ：无
 * 返回  ：无
 */
void read_one_step_to_eep(NODE_STEP	*node, uint8_t file_num, uint8_t step)
{
	uint8_t n = file_num;
	uint16_t add_base = 0;
	uint8_t chip = 0;
	uint8_t pos = 0;
	uint16_t offset_addr = TABLE_VALUE_NULL;
    
    if(node == NULL)
    {
        return;
    }
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
	count_eep_pos(n, &chip, &add_base, &pos);
    
    offset_addr = cur_group_table[step - 1];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        RECORD_EXCEP("TABLE_VALUE_NULL err");
        return;
    }
	
	eep_buf_read((uint8_t*)&node->one_step,
						add_base + MEM_GROUP_OFFSET * pos + STEP_OFFSET * offset_addr, sizeof(UN_STRUCT), chip);
	
#if SYNCHRO_ROADS_EN
	eep_buf_read((uint8_t*)&node->syn_roads,
						add_base + MEM_GROUP_OFFSET * pos + STEP_OFFSET * offset_addr + UN_ONE_SIZE, SYN_ONE_SIZE, chip);
#endif
}
/*
 * 函数名：get_first_step_mode_eep
 * 描述  ：获取第一步的测试模式
 * 输入  ：无
 * 输出  ：无
 * 返回  ：测试模式，NUL表示 出错
 */
uint8_t get_first_step_mode_eep(void)
{
    int32_t offset_addr = TABLE_VALUE_NULL;
    uint16_t step = 1;
    NODE_STEP node;
    
    offset_addr = cur_group_table[0];
    
    if(offset_addr == TABLE_VALUE_NULL)
    {
        RECORD_EXCEP("TABLE_VALUE_NULL err");
        return 0;
    }
    
    if(offset_addr != TABLE_VALUE_NULL && offset_addr < MAX_STEPS)
    {
        read_one_step_to_eep(&node, g_cur_file->num, step);
        
        /* 计算出读出的步是第几步 并对错误进行修正 */
        if(node.one_step.com.step != step)
        {
            node.one_step.com.step = step;
            save_one_step_to_eep(&node, g_cur_file->num, step);
        }
        
        return node.one_step.com.mode;
    }
    
    return NUL;
}
/*
 * 函数名：load_steps_to_list_eep
 * 描述  ：加载若干步到内存链表中
 * 输入  ：step 要加载的起始步
 * 输出  ：无
 * 返回  ：无
 */
void load_steps_to_list_eep(const uint16_t step)
{
    int32_t num = 0;
    uint16_t offset_addr = 0;
    int32_t i = 0;
    
    init_99xx_list();/* 初始化测试步链表 */
    
    if(step == 0)
    {
        return;
    }
    
    if(g_cur_file->total == step)
    {
        num = 1;
    }
    else if(g_cur_file->total > step)
    {
        num = (g_cur_file->total - step + 1) >= STEP_LIST_SIZE? 5 :((g_cur_file->total - step + 1)%STEP_LIST_SIZE);
    }
    else
    {
        RECORD_EXCEP("total > step err");
        return;
    }
    
	for(i = 0; i < num && i < 5; i++)
	{
		insert_node(i);//在当前步后面插入新步
	}
    
    /* 从eep中读出num个步骤 */
    for(i = 0; i < num && i < 5; i++)
    {
        offset_addr = cur_group_table[step + i - 1];
        
        if(offset_addr == TABLE_VALUE_NULL)
        {
            return;
        }
        
        if(offset_addr != TABLE_VALUE_NULL && offset_addr < MAX_STEPS)
        {
            read_one_step_to_eep(&test_group.test_steps[i], g_cur_file->num, step + i);
            
            /* 计算出读出的步是第几步 并对错误进行修正 */
            if(test_group.test_steps[i].one_step.com.step != step + i)
            {
                test_group.test_steps[i].one_step.com.step = step + i;
                
                save_one_step_to_eep(&test_group.test_steps[i], g_cur_file->num, step + i);
            }
        }
    }
}

/*
 * 函数名：save_file_to_eep
 * 描述  ：保存文件信息到eep
 * 输入  ：n 文件编号
 * 输出  ：无
 * 返回  ：无
 */
int32_t save_file_to_eep(const int8_t n)
{
	if(n > MAX_FILES-1)
	{
        RECORD_EXCEP("file_num > MAX_FILES err");
		return -1;
	}
	
	eep_buf_write((uint8_t*)&file_table[n], FILE_INFO_BASE + FILE_INFO_OFFSET * n,
					sizeof(TEST_FILE), CHIP0);
	return 0;
}

/*
 * 函数名：read_file_to_eep
 * 描述  ：从eep读出指定的文件信息
 * 输入  ：n 文件编号
 * 输出  ：无
 * 返回  ：无
 */
void read_file_to_eep(const int8_t n)
{
	if(n > MAX_FILES-1)
	{
        RECORD_EXCEP("file_num > MAX_FILES err");
		return;
	}
	
	eep_buf_read((uint8_t*)&file_table[n], FILE_INFO_BASE + FILE_INFO_OFFSET * n,
					sizeof(TEST_FILE), CHIP0);
}

/*
 * 函数名：save_all_file_to_eep
 * 描述  ：保存所有的文件到eep
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void save_all_file_to_eep(void)
{
    int32_t i = 0;
    
    for(i = 0; i < MAX_FILES; i++)
    {
        save_file_to_eep(i);
    }
}

/*
 * 函数名：read_all_file_to_eep
 * 描述  ：读出所有的文件到内存
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void read_all_file_to_eep(void)
{
	eep_buf_read((uint8_t*)&file_table[0], FILE_INFO_BASE + FILE_INFO_OFFSET * 0, FILE_INFO_OFFSET * MAX_FILES, CHIP0);/* 加载MAX_FILES个记忆组数据 */
}

/*
 * 函数名：save_sys_flag_to_eep
 * 描述  ：保存系统标志
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void save_sys_flag_to_eep(void)
{
	eep_buf_write((uint8_t*)&sys_flag, SYS_FLAG_EE_ADDR, sizeof(SYS_FLAG), CHIP0);
}

/*
 * 函数名：read_sys_flag_to_eep
 * 描述  ：读取系统标志
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void read_sys_flag_to_eep(void)
{
	eep_buf_read((uint8_t*)&sys_flag, SYS_FLAG_EE_ADDR, sizeof(SYS_FLAG), CHIP0);
}

/*
 * 函数名：save_step_used_flag_eep
 * 描述  ：保存指定记忆组的步骤位图
 * 输入  ：file_num 记忆组编号
 * 输出  ：无
 * 返回  ：无
 */
void save_step_used_flag_eep(const uint8_t file_num)
{
    if(file_num > MAX_FILES - 1)
    {
        return;
    }
    
    eep_buf_write((uint8_t*)&step_used_flag[0],
                FILE_USED_STEP_FLAG_BASE + file_num * FILE_USED_STEP_FLAG_OFFSET, sizeof step_used_flag, CHIP0);
}

/*
 * 函数名：read_step_used_flag_eep
 * 描述  ：读取指定记忆组的步骤位图
 * 输入  ：n 记忆组编号
 * 输出  ：无
 * 返回  ：无
 */
void read_step_used_flag_eep(uint8_t n)
{
    if(n > MAX_FILES)
    {
        return;
    }
    
    eep_buf_read((uint8_t*)&step_used_flag[0], 
                FILE_USED_STEP_FLAG_BASE + n * FILE_USED_STEP_FLAG_OFFSET, sizeof step_used_flag, CHIP0);
}
/*
 * 函数名：save_one_result_list_dis_eep
 * 描述  ：保存结果为结果列表显示
 * 输入  ：res 数据缓冲
 *         n 结果编号 n为偏移地址从0开始 
 * 输出  ：无
 * 返回  ：无
 */
void save_one_result_list_dis_eep(RESULT_STR *res, uint16_t n)
{    
    if(res == NULL)
    {
        return;
    }
    
    if(res->step == 0)
    {
        return;
    }
    
    eep_buf_write((uint8_t*)res, RESULT_LIST_DIS_BASE + RESULT_OFFSET * n, RESULT_OFFSET, CHIP2);
}
/*
 * 函数名：save_one_result_eep
 * 描述  ：保存结果到指定位置
 * 输入  ：res 数据缓冲
 *         n 结果编号 n为偏移地址从0开始 
 * 输出  ：无
 * 返回  ：无
 */
void save_one_result_eep(RESULT_STR *res, uint16_t n)
{
    eep_buf_write((uint8_t*)res, RESULT_BASE_A2 + RESULT_OFFSET * n, sizeof(RESULT_STR), CHIP2);
}

/*
 * 函数名：read_one_result_eep
 * 描述  ：读一步的结果
 * 输入  ：res 数据缓冲
 *         n 结果编号 n为偏移地址从0开始 
 * 输出  ：无
 * 返回  ：无
 */
void read_one_result_eep(RESULT_STR *res, uint16_t n)
{
    eep_buf_read((uint8_t*)res, RESULT_BASE_A2 + RESULT_OFFSET * n, sizeof(RESULT_STR), CHIP2);
}

/*
 * 函数名：read_result_list_dis_one_page_eep
 * 描述  ：读一页的结果 用于结果列表显示
 * 输入  ：res 数据缓冲
 *         page 页偏移 从0开始
 *         cur_page_num 当前页的个数
 * 输出  ：无
 * 返回  ：无
 */
void read_result_list_dis_one_page_eep(RESULT_STR *res, uint16_t page, uint16_t cur_page_num)
{
    eep_buf_read((uint8_t*)res, RESULT_LIST_DIS_BASE + RESULT_OFFSET * 4 * (page - 1), cur_page_num * RESULT_OFFSET, CHIP2);
}

/*
 * 函数名：read_result_list_dis_one_eep
 * 描述  ：读一步的结果 用于结果列表显示
 * 输入  ：res 数据缓冲
 *         n 结果偏移地址 从0开始
 * 输出  ：无
 * 返回  ：无
 */
void read_result_list_dis_one_eep(RESULT_STR *res, uint16_t n)
{
    eep_buf_read((uint8_t*)res, RESULT_LIST_DIS_BASE + RESULT_OFFSET * n, RESULT_OFFSET, CHIP2);
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
