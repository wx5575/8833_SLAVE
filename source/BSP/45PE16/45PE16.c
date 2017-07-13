/****************************************************************************** 
 *  项目名称 :                      
 *                                                               
 *  项目功能 :  
 *                                                                          
 *  应用软件 :  WinAVR                                                
 *                                                                       
 *  版    本 :  WinAVR-20060214-install 
 *                                                          
 *  硬    件 :  
 *                                                                
 *  创建时间 :  
 *               
 *  编    写 :             
 *               
 *  附    注 :                                                                 
 *                                                                                     
 *                                                                                           
******************************************************************************/ 

/******************************************************************************
 *                          本文件所引用的头文件
******************************************************************************/ 
 
#include    "45PE16.h"

/******************************************************************************
 *                       本文件所定义的静态全局变量
******************************************************************************/ 

static uint8 _ChipSelectForM45PE16  = 0;

/******************************************************************************
 *                       本文件所定义的全局全局变量
******************************************************************************/ 

/******************************************************************************
 *                       本文件所定义的全局结构体
******************************************************************************/

/******************************************************************************
 *                       本文件所定义的静态数据结构
******************************************************************************/ 

/******************************************************************************
 *                            本文件内部宏定义
******************************************************************************/

/******************************************************************************
 *                          本文件内部宏函数定义
******************************************************************************/
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

#define SPI_SCK_HIGH()              GPIO_SetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)
#define SPI_SCK_LOW()               GPIO_ResetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)

#define SPI_OUTPUT_HIGH()           GPIO_SetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)
#define SPI_OUTPUT_LOW()            GPIO_ResetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)

//#define SPI_INPUT_PORT              (FIO0PIN)               // SPI输入端口
//#define SPI_INPUT_PORT_INDEX        (17)                    // 输入端口索引号

#define PARAM_MEM_WRT_EN()          GPIO_ResetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)
#define PARAM_MEM_WRT_DISEN()       GPIO_SetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)

#define PARAM_MEM_CS_EN()           GPIO_ResetBits(M45_FLASH_CS1_GPIO,   M45_FLASH_CS1_PIN)
#define PARAM_MEM_CS_DISEN()        GPIO_SetBits(M45_FLASH_CS1_GPIO, M45_FLASH_CS1_PIN)

#define CAL_PARAM_MEM_CS_EN()       GPIO_ResetBits(M45_FLASH_CS2_GPIO,   M45_FLASH_CS2_PIN)
#define CAL_PARAM_MEM_CS_DISEN()    GPIO_SetBits(M45_FLASH_CS2_GPIO, M45_FLASH_CS2_PIN)

#define RESULT_MEM1_CS_EN()         GPIO_ResetBits(M45_FLASH_CS3_GPIO,   M45_FLASH_CS3_PIN)
#define RESULT_MEM1_CS_DISEN()      GPIO_SetBits(M45_FLASH_CS1_GPIO, M45_FLASH_CS1_PIN)

#define RESULT_MEM2_CS_EN()         GPIO_ResetBits(M45_FLASH_CS3_GPIO,   M45_FLASH_CS3_PIN)
#define RESULT_MEM2_CS_DISEN()      GPIO_SetBits(M45_FLASH_CS3_GPIO, M45_FLASH_CS3_PIN)

#define M45_FLASH_DI()			GPIO_ReadInputDataBit(M45_FLASH_MISO_GPIO, M45_FLASH_MISO_PIN)


/******************************************************************************
 *                           本文件静态函数声明
******************************************************************************/

static uint8 _M45PE16BusyStateQuery(void);

static void _M45PE16ChipSelServ(uint8 cmd); 

/******************************************************************************
 *                            文件接口函数声明
******************************************************************************/ 

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  模拟SPI接口发送一字节                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
static void _SPISendByte(uint8  data)
{
    uint8   i;
    
    for (i = 8; i != 0; i-- )
    {
        SPI_SCK_LOW();
        if (data & 0x80)
        {
            SPI_OUTPUT_HIGH();
        }                                                                
        else
        {
            SPI_OUTPUT_LOW();
        }                                                                
        SPI_SCK_HIGH();                                          
        data <<= 1;                                                      
        
    }                                                                    
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  模拟SPI接口接收一字节                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
static uint8 _SPIGetByte(void)
{
    uint8   i, data = 0;                                                   
    
    for (i = 8; i != 0; i--)
    {
        SPI_SCK_LOW();
        data <<= 1;
        SPI_SCK_HIGH();
        if (M45_FLASH_DI() != RESET)
        {
            data++;
        }
    }
    
    return data;                                                         
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  45PE16写使能标志位置为使能状态                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static void _M45PE16WriteEnable(void)
{
    _M45PE16ChipSelServ(EN);
    _SPISendByte(M45PE16_CMD_WREN);
    _M45PE16ChipSelServ(DISEN);                                              
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  45PE16写使能标志位置为禁能状态                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static void _M45PE16WriteDisenable(void)
{
    _M45PE16ChipSelServ(EN);
    _SPISendByte(M45PE16_CMD_WRDI);
    _M45PE16ChipSelServ(DISEN);                                              
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  读取器件ID                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

uint32 HAL_ReadM45PE16ID(void)
{
    uint8   data2, data1, data0;                                         
    uint32  data    = 0;                                                        
    
    _M45PE16ChipSelServ(EN);
    _SPISendByte( M45PE16_CMD_RDID );                          
    data2 = _SPIGetByte();                                
    data1 = _SPIGetByte();                                
    data0 = _SPIGetByte();
    _M45PE16ChipSelServ(DISEN);                                              
    
    data  = ((uint32)data2)<<16;                                         
    data += ((uint32)data1)<<8;                                          
    data += data0;                                                       
    
    return data;                                                         
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  读取器件状态寄存器                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _ReadM45PE16State(void)
{
    uint8   data;                                                        
    
    _M45PE16ChipSelServ(EN);
    _SPISendByte( M45PE16_CMD_RDSR );                          
    data            = _SPIGetByte();
    _M45PE16ChipSelServ(DISEN);                                              

    return data;                                                         
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  读取指定物理地址的一页数据                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16ReadPhyPage(uint16 page, uint8 *pdata)
{
    uint16  i;                                                           

    // 页物理地址错误
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
    
    _M45PE16ChipSelServ(EN);
    _SPISendByte( M45PE16_CMD_READ );                          
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( 0x00 );                                  

    for (i = 0; i < 256; i++ )
    {                                           
        pdata[i] = _SPIGetByte();                          
    }
    _M45PE16ChipSelServ(DISEN);                                              
    
    return M45PE16_OK;                                                   
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  擦除指定物理地址的一页数据                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16ErasePhyPage(uint16 page)
{
    uint8   rdata;                                                       
    
    // 页物理地址错误
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
        
    // 开启 FLASH 写允许
    _M45PE16WriteEnable();                                         
    rdata = _ReadM45PE16State();                                    
    // 写允许未打开，返回错误
    if ((rdata & 0x02) == 0x00)
    {                                    
        _M45PE16WriteDisenable();                                           
        return M45PE16_ERROR;                                            
    }                                                                    

    _M45PE16ChipSelServ(EN);
    // 页擦除指令                                               
    _SPISendByte( M45PE16_CMD_PE );                           
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( 0x00 );
    _M45PE16ChipSelServ(DISEN);
    
    return _M45PE16BusyStateQuery();                                                
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  编程指定物理地址的一页数据                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16ProgPhyPage(uint16 page, uint8 *pdata)
{
    uint16  i;                                                           
    uint8   rdata;                                                       
    
    // 页物理地址错误
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
    
    // 开启 FLASH 写允许
    _M45PE16WriteEnable();                                         
    rdata = _ReadM45PE16State();                                    
    // 写允许未打开，返回错误
    if ((rdata & 0x02) == 0x00)
    {                                    
        _M45PE16WriteDisenable();                                           
        return M45PE16_ERROR;                                            
    }                                                                    

    _M45PE16ChipSelServ(EN);
    // 页编程指令
    _SPISendByte( M45PE16_CMD_PP );                           
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( 0x00 );                                  
    for (i = 0; i < 256; i++)
    {                                             
        _SPISendByte(pdata[i]);                           
    }
    _M45PE16ChipSelServ(DISEN);
    
    return _M45PE16BusyStateQuery();                                                
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  写指定物理地址页 包括擦除、编程过程                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16WritePhyPage(uint16 page, uint8 addrInPage, uint8 data[256], uint16 dataLen)
{
    uint16          i;                                                           
    uint8           rdata;                                                       
    
    // 页物理地址错误
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
    
    // 开启 FLASH 写允许
    _M45PE16WriteEnable();                                         
    rdata = _ReadM45PE16State();                                    
    // 写允许未打开，返回错误
    if ((rdata & 0x02) == 0x00)
    {                                    
        _M45PE16WriteDisenable();                                           
        return M45PE16_ERROR;                                            
    }                                                                    

    _M45PE16ChipSelServ(EN);
    // 页写入指令
    _SPISendByte( M45PE16_CMD_PW );                           
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( addrInPage );                                  
    for (i = 0; i < dataLen; i++)
    {                                             
        _SPISendByte(data[i]);                           
    }
    _M45PE16ChipSelServ(DISEN);
    
    return _M45PE16BusyStateQuery();                                                
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  判断M45PE16是否处于内部操作时间 即忙状态                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16BusyStateQuery(void)
{
    uint16          i;
    uint8           rdata;

    // 确保 11ms 延迟，检测 写入 完毕标志
    i = 50000;                                                          
    while (i--)
    {                                                       
        rdata = _ReadM45PE16State();                                
        if ((rdata & 0x01) == 0x00)
        {                                   
            _M45PE16WriteDisenable();                                       
            return M45PE16_OK;                                           
        }                                                                
        NOP();                                             
        NOP();                                             
        NOP();                                             
        NOP();                                             
        NOP();                                             
    }                                                                    
    
    // 超时，应答错误
    _M45PE16WriteDisenable();                                               
    return M45PE16_ERROR;
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  片选信号的使能与禁能                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

static void _M45PE16ChipSelServ(uint8 cmd)
{
    //指定存储器的片选使能
    if (EN == cmd)
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            
                CAL_PARAM_MEM_CS_EN();

                break;

            case 1:
            
                PARAM_MEM_CS_EN();

                break;

            case 2:
            
                RESULT_MEM1_CS_EN();

                break;

            case 3:
            
                RESULT_MEM2_CS_EN();

                break;

            default:
                break;
        }
    }
    //指定存储器的片选禁能
    else
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            
                CAL_PARAM_MEM_CS_DISEN();

                break;

            case 1:
            
                PARAM_MEM_CS_DISEN();

                break;

            case 2:
            
                RESULT_MEM1_CS_DISEN();

                break;

            case 3:
            
                RESULT_MEM2_CS_DISEN();

                break;

            default:
                break;
        }
    }
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  写保护信号的使能与禁能                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

void HAL_M45PE16WriteProtectCtrl(uint8 cmd)
{
    // 指定存储器的片选使能
    if (M45PE16_WP_ENABLE == cmd)
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            case 1:
            case 2:
            
                PARAM_MEM_WRT_EN();
                break;

            default:
                break;
        }
    }
    // 指定存储器的片选禁能
    else
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            case 1:
            case 2:
            
                PARAM_MEM_WRT_DISEN();
                break;

            default:
                break;
        }
    }
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  M45PE16输入输出控制                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

void HAL_M45PE16IoCtrl(uint8 cmd, void *pparam)
{
    if (M45PE16_CHIP_SELECT == cmd)
    {
        _ChipSelectForM45PE16       = (uint8)pparam;
    }
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  M45PE16连续字节写入                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
uint8   M45PE16_ContinuousBytesWrt(uint32 destAddr, uint8 *prscData, uint32 dataLen)
{
    uint16      writeLen;
    uint16      startPage                   = destAddr / M45PE16_PAGE_SIZE;
    uint8       addrInPage                  = destAddr % M45PE16_PAGE_SIZE;

    // 写使能管脚置为使能状态
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_ENABLE);

    while (dataLen > 0)
    {
        writeLen                            = M45PE16_PAGE_SIZE - addrInPage;
        if (writeLen > dataLen)
        {
            writeLen                        = dataLen;
        }
        if (M45PE16_ERROR == _M45PE16WritePhyPage(startPage, addrInPage, prscData, writeLen))
        {
            return M45PE16_ERROR;
        }
        startPage++;
        prscData                           += writeLen;
        addrInPage                          = 0;
        dataLen                            -= writeLen;
    }

    // 写使能管脚置为写禁能状态
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_DISENABLE);

    return M45PE16_OK;
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  任意地址读取任意长度数据                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

void   M45PE16_ContinuousBytesRd(uint32 rscAddr, uint8 *pdestData, uint32 dataLen)
{
    uint32      i;

    _M45PE16ChipSelServ(EN);
    // 页写入指令
    _SPISendByte( M45PE16_CMD_READ );                           
    _SPISendByte( (uint8)(rscAddr>>16) );                      
    _SPISendByte( (uint8)(rscAddr>>8)&0x000000FF );                  
    _SPISendByte( (uint8)(rscAddr&0x000000FF) );                                  
    for (i = 0; i < dataLen; i++)
    {                                             
        pdestData[i]    = _SPIGetByte();
    }
    _M45PE16ChipSelServ(DISEN);
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  模拟SPI接口所用GPIO端口初始化                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
static void _GPIOForM45PE16Init(void)
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
    
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  M45PE16 器件初始化                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
void M45PE16Init(void)
{
    _GPIOForM45PE16Init();    

    //所使用端口初始化
    PARAM_MEM_CS_DISEN();
    CAL_PARAM_MEM_CS_DISEN();
    RESULT_MEM1_CS_DISEN();
    RESULT_MEM2_CS_DISEN();
    
    HAL_ReadM45PE16ID();
}

/******************************************************************************
 *  函数名称 :                                                                
 *                                                                           
 *  函数功能 :  45PE16功能调试函数                                                            
 *                                                                           
 *  入口参数 :                                                                
 *                                                                             
 *  出口参数 :                                                                
 *                                                                              
 *  编 写 者 :                                                                
 *                                                                                 
 *  日    期 :                                                                 
 *                                                                              
 *  审 核 者 :                                                                                                                               
 *                                                                             
 *  日    期 :                                                                
 *                                                                                                                                       
 *  附    注 :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
#ifdef M45PE16_DEBUG
uint8       txdData[1000]     = {0};
uint8       rxdData[2000]    = {0};
void M45PE16_M45PE16FuncDebug(void)
{
    uint16      i;
    uint32      memId;

    HAL_M45PE16IoCtrl(M45PE16_CHIP_SELECT, (void *)3);

    memId   = HAL_ReadM45PE16ID();

    for (i = 0; i < 1000; i++)
    {
        txdData[i] = 1;
    }
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_ENABLE);
    M45PE16_ContinuousBytesWrt(250, txdData, sizeof(txdData));
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_DISENABLE);

    M45PE16_ContinuousBytesRd(250, rxdData, sizeof(txdData));
}

#endif

/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
