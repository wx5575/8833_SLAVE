/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：m45pe16_spi_flash.c
 * 摘  要  ：主程序
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include "stm32f10x.h"
#include "STM32_GPIO_Config.h"
#include "m45pe16_spi_flash.h"
#include "string.h"
#include "cs99xx_err_log.h"

static uint8_t      chipselect = 1;
static SFLASH_INFO  m45pe16_info;
static void m45_wait_for_write_end(void);

/**************************** 移植改动硬件 *********************************/
#define         M45_FLASH_RCC_APB2Periph_GPIO   RCC_APB2Periph_GPIOF
/* WP */
#define         M45_FLASH_WP_GPIO       GPIOF
#define         M45_FLASH_WP_PIN        GPIO_Pin_5
/* MOSI */
#define         M45_FLASH_MOSI_GPIO     GPIOF
#define         M45_FLASH_MOSI_PIN      GPIO_Pin_9
/* MISO */
#define         M45_FLASH_MISO_GPIO     GPIOF
#define         M45_FLASH_MISO_PIN      GPIO_Pin_4
/* CLK */
#define         M45_FLASH_CLK_GPIO      GPIOF
#define         M45_FLASH_CLK_PIN       GPIO_Pin_8

/* CS1 */
#define         M45_FLASH_CS1_GPIO      GPIOF
#define         M45_FLASH_CS1_PIN       GPIO_Pin_3
/* CS2 */
#define         M45_FLASH_CS2_GPIO      GPIOF
#define         M45_FLASH_CS2_PIN       GPIO_Pin_6
/* CS3 */
#define         M45_FLASH_CS3_GPIO      GPIOF
#define         M45_FLASH_CS3_PIN       GPIO_Pin_7

/*************************************************************************/


#define M45_FLASH_WP_EN()		GPIO_ResetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)
#define M45_FLASH_WP_DIS()		GPIO_SetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)

#define M45_FLASH_CS1_EN()      GPIO_ResetBits(M45_FLASH_CS1_GPIO,   M45_FLASH_CS1_PIN)
#define M45_FLASH_CS2_EN()      GPIO_ResetBits(M45_FLASH_CS2_GPIO,   M45_FLASH_CS2_PIN)
#define M45_FLASH_CS3_EN()      GPIO_ResetBits(M45_FLASH_CS3_GPIO,   M45_FLASH_CS3_PIN)
#define M45_FLASH_CS1_DIS()		GPIO_SetBits(M45_FLASH_CS1_GPIO, M45_FLASH_CS1_PIN)
#define M45_FLASH_CS2_DIS()		GPIO_SetBits(M45_FLASH_CS2_GPIO, M45_FLASH_CS2_PIN)
#define M45_FLASH_CS3_DIS()		GPIO_SetBits(M45_FLASH_CS3_GPIO, M45_FLASH_CS3_PIN)

#define M45_FLASH_DI_HIGH()		GPIO_SetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)
#define M45_FLASH_DI_LOW()		GPIO_ResetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)

#define M45_FLASH_CLK_HIGH()	GPIO_SetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)
#define M45_FLASH_CLK_LOW()		GPIO_ResetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)

#define M45_FLASH_DI()			GPIO_ReadInputDataBit(M45_FLASH_MISO_GPIO, M45_FLASH_MISO_PIN)

#define M45_CMD_WREN            0x06    /* 写使能命令 */
#define M45_CMD_RDID            0x9F    /* 读器件ID命令 */
#define M45_CMD_DISWR           0x04    /* 禁止写, 退出AAI状态 */
#define M45_CMD_RDSR            0x05    /* 读状态寄存器命令 */
#define M45_CMD_READ            0x03  	/* 读数据区命令 */
#define M45_CMD_FAST_READ       0x0B  	/* 读数据区命令 */
#define M45_CMD_PAGE_WRITE      0x0A  	/* 页写命令 */
#define M45_CMD_PAGE_PROGRAM    0x02  	/* 页写命令 */
#define M45_CMD_PAGE_ERASE      0xDB  	/* 页擦除命令 */
#define M45_CMD_SECTOR_ERASE    0xD8  	/* 块擦除命令 */
#define M45_CMD_DUMMY_BYTE      0xA5    /* 哑命令，可以为任意值，用于读操作 */

#define M45_WIP_FLAG            0x01		/* 状态寄存器中的正在编程标志（WIP) */


/*
 * 函数名：flash_cs_set_m45pe16
 * 描述  ：FLASH片选设置
 * 输入  ：n 1、2、3
 * 输出  ：无
 * 返回  ：无
 */
static void flash_cs_set_m45pe16(char n)
{
	switch(n)
	{
		case 1:
			chipselect = 1;
			break;
		case 2:
			chipselect = 2;
			break;
		case 3:
			chipselect = 3;
			break;
	}
}

/*
 * 函数名：flash_cs_en_m45pe16
 * 描述  ：FLASH片选使能
 * 输入  ：n 1、2、3
 * 输出  ：无
 * 返回  ：无
 */
static void flash_cs_en_m45pe16(void)
{
    M45_FLASH_CS1_DIS();
    M45_FLASH_CS2_DIS();
    M45_FLASH_CS3_DIS();
    
	switch(chipselect)
	{
		case 1:
            M45_FLASH_CS1_EN();
			break;
		case 2:
            M45_FLASH_CS2_EN();
			break;
		case 3:
            M45_FLASH_CS3_EN();
			break;
	}
}

/*
 * 函数名：flash_cs_dis_m45pe16
 * 描述  ：FLASH片选使能
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void flash_cs_dis_m45pe16(void)
{
	M45_FLASH_CS1_DIS();
	M45_FLASH_CS2_DIS();
	M45_FLASH_CS3_DIS();
}

/*
 * 函数名：m45_flash_lock
 * 描述  ：FLASH写保护上锁
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void m45_flash_lock(void)
{
	M45_FLASH_WP_EN();
}

/*
 * 函数名：m45_flash_unlock
 * 描述  ：FLASH写保护解锁
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void m45_flash_unlock(void)
{
	M45_FLASH_WP_DIS();
}
/*
 * 函数名：m45_gpio_init
 * 描述  ：初始化管脚
 * 输入  ：空
 * 输出  ：空
 */
static void m45_gpio_init(void)
{
	/*************************    定义初始化结构变量    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
    
	
	/*************************    开启外设时钟    *************************/
	RCC_APB2PeriphClockCmd(M45_FLASH_RCC_APB2Periph_GPIO, ENABLE);
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*********************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 //上拉输入
	
    /* SDO MDI */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_MISO_PIN;
	GPIO_Init(M45_FLASH_MISO_GPIO, &GPIO_InitStructure);
    
    /*********************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
    
	/* SDI MDO */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_MOSI_PIN;
	GPIO_Init(M45_FLASH_MOSI_GPIO, &GPIO_InitStructure);
    
    /* WP */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_WP_PIN;
	GPIO_Init(M45_FLASH_WP_GPIO, &GPIO_InitStructure);
    
    /* CLK */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CLK_PIN;
	GPIO_Init(M45_FLASH_CLK_GPIO, &GPIO_InitStructure);
    
    /* CS1 */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CS1_PIN;
	GPIO_Init(M45_FLASH_CS1_GPIO, &GPIO_InitStructure);
    
    /* CS2 */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CS2_PIN;
	GPIO_Init(M45_FLASH_CS2_GPIO, &GPIO_InitStructure);
    
    /* CS3 */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CS3_PIN;
	GPIO_Init(M45_FLASH_CS3_GPIO, &GPIO_InitStructure);
    
	flash_cs_dis_m45pe16();
}

/*
 * 函数名：m45_send_byte
 * 描述  ：data 发送的数据
 * 输入  ：空
 * 输出  ：空
 * 返回  ：收到的数据
 */
static void m45_send_byte(unsigned char data)
{
    uint8_t   i;
    
    for (i = 8; i != 0; i-- )
    {
        M45_FLASH_CLK_LOW();
        
        if (data & 0x80)
        {
            M45_FLASH_DI_HIGH();
        }
        else
        {
            M45_FLASH_DI_LOW();
        }
        
        M45_FLASH_CLK_HIGH();
        data <<= 1;
    }
    
    M45_FLASH_CLK_LOW();
}
//static int m45_send_byte(unsigned char data)
//{
//    #define  DELAY_SF()   {int j; for(j = 0; j < 200; j++);}
//	int result = 0;
//	int i = 0;
//	int pos = 0x8;
//	
////    M45_FLASH_CLK_HIGH();
//        M45_FLASH_CLK_LOW();
//        DELAY_SF();

//	for(i = 0; i < 8; i++)
//	{
//		if(data & pos)
//        {
//            M45_FLASH_DI_HIGH();
//        }
//		else
//        {
//            M45_FLASH_DI_LOW();
//        }
//        
//        
//		M45_FLASH_CLK_HIGH();
//        DELAY_SF();
//        
//		pos >>= 1;
//		result <<= 1;
//        
//		M45_FLASH_CLK_LOW();
//        DELAY_SF();
//        
//		result |= M45_FLASH_DI();
//	}
//	
////    M45_FLASH_CLK_HIGH();
//    
//	return result;
//}
static int m45_read_byte(void)
{
    #define  DELAY_SF()   {int j; for(j = 0; j < 50000; j++);}

    uint8_t   i, data = 0;
    
    for (i = 8; i != 0; i--)
    {
        M45_FLASH_CLK_LOW();
        data <<= 1;
        M45_FLASH_CLK_HIGH();
        
        data |= M45_FLASH_DI();
    }
    
    M45_FLASH_CLK_LOW();
    
    return data;
}
/*
 * 函数名：read_id_m45pe16
 * 描述  ：读FLASH ID
 * 输入  ：空
 * 输出  ：空
 * 返回  ：FLASH ID
 */
static uint32_t read_id_m45pe16(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3;
	
	flash_cs_en_m45pe16();
	m45_send_byte(M45_CMD_RDID);            /* 发送读ID命令 */
	id1 = m45_read_byte();					/* 读ID的第1个字节 */
	id2 = m45_read_byte();					/* 读ID的第2个字节 */
	id3 = m45_read_byte();					/* 读ID的第3个字节 */
    
	flash_cs_dis_m45pe16();
	
	uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;
    
	return uiID;
}

/*
 * 函数名：init_m45pe16_spi_flash
 * 描述  ：初始化FLASH
 * 输入  ：空
 * 输出  ：空
 * 返回  ：无
 */
void init_m45pe16_spi_flash(void)
{
	m45_gpio_init();
    
    flash_cs_set_m45pe16(2);
//	m45_flash_unlock();
	
	m45pe16_info.ChipID = read_id_m45pe16();
    
    
	if(m45pe16_info.ChipID == 0x204015)
	{
		strcpy(m45pe16_info.ChipName, "M45PE16");
		m45pe16_info.TotalSize = 2 * 1024 * 1024; /* 总容量 = 2M */
		m45pe16_info.PageSize = 256; /* 页面大小 = 256B */
	}
    
//   m45_flash_lock();
}

/*
 * 函数名：m45_write_enable
 * 描述  ：发送m45写使能打开
 * 输入  ：空
 * 输出  ：空
 * 返回  ：无
 */
static void m45_write_enable(void)
{
	flash_cs_en_m45pe16(); /* 使能片选 */
	m45_send_byte(M45_CMD_WREN); /* 发送命令 */
	flash_cs_dis_m45pe16(); /* 禁能片选 */
}

/*
 * 函数名：m45_write_disable
 * 描述  ：发送m45写使能关闭
 * 输入  ：空
 * 输出  ：空
 * 返回  ：无
 */
static void m45_write_disable(void)
{
	flash_cs_en_m45pe16(); /* 使能片选 */
	m45_send_byte(M45_CMD_DISWR); /* 发送命令 */
	flash_cs_dis_m45pe16(); /* 禁能片选 */
}

/*
 * 函数名：m45_wait_for_write_end
 * 描述  ：发送m45读状态指令，并等待FLASH工作结束
 * 输入  ：空
 * 输出  ：空
 * 返回  ：无
 */
static void m45_wait_for_write_end(void)
{
	flash_cs_en_m45pe16(); /* 使能片选 */
	m45_send_byte(M45_CMD_RDSR); /* 发送命令， 读状态寄存器 */
    /* 给FLASH工作提供时钟 */
    while((m45_read_byte() & M45_WIP_FLAG) == SET);/* 判断状态寄存器的忙标志位 */
    
	flash_cs_dis_m45pe16(); /* 禁能片选 */
}

/*
 * 函数名：m45_page_write
 * 描述  ：写一整页数据，地址是页对齐的
 * 输入  ：_pBuf 数据缓存 _uiWriteAddr 地址是页对齐的
 * 输出  ：空
 * 返回  ：无
 */
void m45_page_write(uint8_t * _pBuf, uint32_t _uiWriteAddr)
{
	uint32_t i = 0;
    
    /* 检查如果不是页对齐的就退出 */
    if(_uiWriteAddr % m45pe16_info.PageSize != 0)
    {
        return;
    }
    
    m45_flash_unlock();/* 解锁写使能 */
    
    m45_write_enable(); /* 发送写使能命令 */
    
    flash_cs_en_m45pe16(); /* 使能片选 */
    
    m45_send_byte(M45_CMD_PAGE_PROGRAM); /* 发送页写指令 M45_CMD_PAGE_WRITE*/
    m45_send_byte((_uiWriteAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
    m45_send_byte((_uiWriteAddr & 0xFF00) >> 8); /* 发送扇区地址中间8bit */
    m45_send_byte(_uiWriteAddr & 0xFF); /* 发送扇区地址低8bit */
    
    for (i = 0; i < 256; i++)
    {
        m45_send_byte(*_pBuf++); /* 发送数据 */
    }
    
    flash_cs_dis_m45pe16(); /* 禁止片选 */
    
    m45_flash_lock();/* 上锁写使能 */
    
    m45_wait_for_write_end(); /* 等待串行Flash内部写操作完成 */
	
	m45_write_disable();/* 退出写保护状态 */
    
	m45_wait_for_write_end(); /* 等待串行Flash内部写操作完成 */
}

/*
 * 函数名：m45_read_buffer
 * 描述  ：读数据，地址是任意的
 * 输入  ：_pBuf 数据缓存 _uiWriteAddr 地址 _uiSize 读的大小
 * 输出  ：空
 * 返回  ：无
 */
void m45_read_buffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > m45pe16_info.TotalSize)
	{
		return;
	}
    
	/* 擦除扇区操作 */
	flash_cs_en_m45pe16(); /* 使能片选 */
	m45_send_byte(M45_CMD_READ); /* 发送读命令 */
	m45_send_byte((_uiReadAddr & 0xFF0000) >> 16); /* 发送扇区地址的高8bit */
	m45_send_byte((_uiReadAddr & 0xFF00) >> 8); /* 发送扇区地址中间8bit */
	m45_send_byte(_uiReadAddr & 0xFF); /* 发送扇区地址低8bit */
    
	while (_uiSize--)
	{
		*_pBuf++ = m45_read_byte(); /* 读一个字节并存储到pBuf，读完后指针自加1 */
	}
    
	flash_cs_dis_m45pe16(); /* 禁能片选 */
}

/*
 * 函数名：m45_page_erase
 * 描述  ：擦除页，地址是任意的但是向下对页取整，
 * 输入  ：_pBuf 数据缓存 _uiWriteAddr 地址 _uiSize 擦除的页数
 * 输出  ：空
 * 返回  ：无
 */
void m45_page_erase(uint32_t _uiReadAddr, uint32_t _uiSize)
{
    int32_t i = 0;
	/* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > m45pe16_info.TotalSize)
	{
		return;
	}
    
    m45_flash_unlock();/* 解锁写使能 */
    m45_write_enable(); /* 发送写使能命令 */
    m45_wait_for_write_end(); /* 等待串行Flash内部写操作完成 */
    
	/* 擦除操作 */
    for(i = 0; i < _uiSize; i++)
    {
        flash_cs_en_m45pe16(); /* 使能片选 */
        
        m45_send_byte(M45_CMD_PAGE_ERASE); /* 发送页擦除命令 */
        m45_send_byte((_uiReadAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
        m45_send_byte((_uiReadAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
        m45_send_byte(_uiReadAddr & 0xFF);				/* 发送扇区地址低8bit */
        
        flash_cs_dis_m45pe16(); /* 禁能片选 */
        
        m45_wait_for_write_end(); /* 等待串行Flash内部写操作完成 */
        
    }
    
	m45_write_disable();/* 退出写保护状态 */
    m45_flash_lock();/* 上锁写使能 */
    
	m45_wait_for_write_end(); /* 等待串行Flash内部写操作完成 */
}

/*
 * 函数名：write_page_and_check_ok
 * 描述  ：写入一页并对写入的数据进行检查
 * 输入  ：buf 数据缓存 addr 地址页对齐的
 * 输出  ：空
 * 返回  ：无
 */
static void write_page_and_check_ok(uint8_t *buf, uint32_t addr)
{
    uint8_t buf_cmp[256];
    uint32_t page_size = m45pe16_info.PageSize;
    uint32_t retry_count = 0;
    
    do
    {
        m45_page_erase(addr, 1);
        m45_page_write(buf, addr);
        m45_read_buffer(buf_cmp, addr, page_size);
        if(++retry_count > 5)
        {
            RECORD_EXCEP("M45PE16 WRITE ERR");
            break;
        }
    }while(0 != memcmp(buf, buf_cmp, page_size));
}

/*
 * 函数名：m45_buffer_write
 * 描述  ：缓冲写，地址是2Mb内任意地址，
 * 输入  ：pBuffer 数据缓存 WriteAddr 地址页对齐的 NumByteToWrite 写入的字节数只要空间足够的任意字节数
 * 输出  ：空
 * 返回  ：无
 */
void m45_buffer_write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
	uint32_t num_of_p_first = 0;//第一页个数
	uint32_t num_of_p_n = 0;//整页数
	uint32_t num_of_p_last = 0;//最后一页个数
    uint32_t addr_of_p_first = 0;//第一页的首地址
	uint32_t addr = 0;
    uint32_t page_size = m45pe16_info.PageSize;
    static uint8_t buf[256];
    
    int32_t i = 0;
    uint32_t start;
    uint32_t first_addr_of_page;//页的首地址
    
    if(page_size == 0)
    {
        return;
    }
    
    addr_of_p_first = WriteAddr / page_size * page_size;
	addr = WriteAddr % page_size;//写入地址是每页的第几位
    
	num_of_p_first = (addr==0? 0:(page_size - addr));//在开始的一页还可以写入的个数
    
    /* 如果开始页放不下就计算后面需要的页信息 */
    if(NumByteToWrite > num_of_p_first)
    {
        num_of_p_n = (NumByteToWrite - num_of_p_first) / page_size;//要写入的整页数
        num_of_p_last = (NumByteToWrite - num_of_p_first) % page_size;//最后一页的个数
    }
    
    first_addr_of_page = addr_of_p_first;
    
    /* 第一页 */
    if(num_of_p_first > 0)
    {
        m45_read_buffer(buf, first_addr_of_page, page_size);
        
        start = addr;
        
        for(i = 0; i < num_of_p_first; i++)
        {
            buf[start + i] = *pBuffer++;
        }
        
        write_page_and_check_ok(buf, first_addr_of_page);
        
        first_addr_of_page += page_size;
    }
    /* 中间n页 */
    if(num_of_p_n > 0)
    {
        for(i = 0; i < num_of_p_n; i++)
        {
            write_page_and_check_ok(pBuffer, first_addr_of_page);
            pBuffer += page_size;
            first_addr_of_page += page_size;
        }
    }
    /* 最后一页 */
    if(num_of_p_last > 0)
    {
        m45_read_buffer(buf, first_addr_of_page, page_size);
        
        for(i = 0; i < num_of_p_last; i++)
        {
            buf[i] = *pBuffer++;
        }
        
        write_page_and_check_ok(buf, first_addr_of_page);
    }
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
