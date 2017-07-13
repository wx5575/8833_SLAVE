/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：99xx_listS.c
 * 摘  要  ：定义主要的数据结构，供整个工程使用
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#define _CS99xxLIST_GLOBALS
#include "cs99xx_list.h"
#include "string.h"



void init_list(void)
{
	int8_t i;
	NODE_STEP	*first;
	NODE_STEP	*second;
    
	//将空的测试步池初始化为一个单向链表将表头给free_list_steps   file_table[MAX_FILES];
	memset(steps_buf,0,sizeof steps_buf);
	
	for(i = 0; i < 5 - 1; i++)
	{
		first  = &steps_buf[i];
		second = &steps_buf[i+1];
		first->next = second;
	}
    
	first  = &steps_buf[5-1];
    
	first->next = (NODE_STEP*)0;
	
	free_list_head = &steps_buf[0];	//将空步链表头指向test_steps[0]
	
	cs99xx_list.head = NULL;
	cs99xx_list.tail = NULL;
	cs99xx_list.size = 0;
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
