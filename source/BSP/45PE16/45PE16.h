/******************************************************************************
 * 文件信息 : 
 * 
 * 创 建 者 :  
 *
 * 创建日期 : 
 * 
 * 原始版本 :  
 *     
 * 修改版本 :   
 *    
 * 修改日期 :  
 *
 * 修改内容 : 
 * 
 * 审 核 者 : 
 *
 * 附    注 : 
 *
 * 描    述 :   源代码
 *
 * 版    权 :   南京长盛仪器有限公司 , Copyright Reserved
 * 
******************************************************************************/

/******************************************************************************
 *                              头文件卫士
******************************************************************************/ 
 
#ifndef _45PE16_H_
#define _45PE16_H_

#if defined(__cplusplus)

    extern "C" {     /* Make sure we have C-declarations in C++ programs */

#endif

/******************************************************************************
 *                             包含文件声明
******************************************************************************/ 

#ifndef    _INCLUDES_H_

        #include "stm32f10x.h"
        typedef uint8_t uint8;
        typedef uint16_t uint16;
        typedef uint32_t uint32;
    
#endif

/******************************************************************************
 *                           文件接口信息宏定义
******************************************************************************/
#define EN      1
#define DISEN   0
#define NOP()   {int x; x=x;}
//----------------------------------------------------------------------------
// M45PE16 状态定义
//----------------------------------------------------------------------------

#define         M45PE16_ERROR                   (0)         // 操作错误
#define         M45PE16_OK                      (1)         // 操作成功
#define         M45PE16_MAX_PHY_PAGE            (8192)      // 最大物理页数
#define         M45PE16_PAGE_SIZE               (256)       // 页大小 

//----------------------------------------------------------------------------
// M45PE16D 命令定义
//----------------------------------------------------------------------------

#define         M45PE16_CMD_WREN                (0x06)       // 写允许
#define         M45PE16_CMD_WRDI                (0x04)       // 写禁止
#define         M45PE16_CMD_RDID                (0x9F)       // 读ID     0x20 40 15
#define         M45PE16_CMD_RDSR                (0x05)       // 读状态寄存器
#define         M45PE16_CMD_READ                (0x03)       // 读取     时钟小于  33MHz
#define         M45PE16_CMD_FAST_READ           (0x0B)       // 快速读取 时钟可以到 50MHz
#define         M45PE16_CMD_PW                  (0x0A)       // 页写   包含 擦除 和 编程 指令
#define         M45PE16_CMD_PP                  (0x02)       // 页编程    1-->0
#define         M45PE16_CMD_PE                  (0xDB)       // 页擦除    0-->1
#define         M45PE16_CMD_SE                  (0xD8)       // 扇区擦除  0-->1
#define         M45PE16_CMD_DP                  (0xB9)       // 深度掉电
#define         M45PE16_CMD_RDP                 (0xAB)       // 释放深度掉电

//----------------------------------------------------------------------------
// M45PE16D 写保护使能与禁能宏定义
//----------------------------------------------------------------------------

#define         M45PE16_WP_ENABLE               (0)
#define         M45PE16_WP_DISENABLE            (1)

#define         M45PE16_CHIP_SELECT             (0)


#define         DELAY_US(dlyTime)               (sysDelayUS(dlyTime))         



//#define        M45PE16_DEBUG

/******************************************************************************
 *                         文件接口数据结构体声明
******************************************************************************/ 


/******************************************************************************
 *                            文件接口函数声明
******************************************************************************/

#ifdef      M45PE16_DEBUG
    
    void M45PE16_M45PE16FuncDebug(void);

#endif

void    M45PE16Init(void);

void    HAL_M45PE16IoCtrl(uint8 cmd, void *pparam);

void    HAL_M45PE16WriteProtectCtrl(uint8 cmd);

uint8   M45PE16_ContinuousBytesWrt(uint32 destAddr, uint8 *prscData, uint32 dataLen);

void    M45PE16_ContinuousBytesRd(uint32 rscAddr, uint8 *pdestData, uint32 dataLen);



#if defined(__cplusplus)

    }

#endif 

/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
#endif

