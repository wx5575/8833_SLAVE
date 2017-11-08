
#ifndef __SEND_TEST_DATA_FPGA_H__
#define __SEND_TEST_DATA_FPGA_H__

#pragma pack(1)
typedef struct
{
    uint16_t vol;//电压
    uint16_t cur;//电流
    uint16_t real;//真实电流
    uint8_t cur_inf;//电流信息
    uint8_t test_flag;//测试标记
    uint8_t test_status;//测试状态
    uint16_t time;//测试时间
}ACW_FRAME;

typedef struct
{
    uint16_t vol;//电压
    uint16_t cur;//电流
    uint8_t cur_inf;//电流信息
    uint8_t test_flag;//测试标记
    uint8_t test_status;//测试状态
    uint16_t time;//测试时间
}DCW_FRAME;

typedef struct
{
    uint16_t vol;//电压
    uint16_t res;//电流
    uint8_t res_inf;//电阻信息
    uint8_t test_flag;//测试标记
    uint8_t test_status;//测试状态
    uint16_t time;//测试时间
}IR_FRAME;

typedef struct
{
    uint16_t cur;//电流
    uint16_t res;//电阻
    uint8_t res_inf;//电阻信息
    uint8_t test_flag;//测试标记
    uint8_t test_status;//测试状态
    uint16_t time;//测试时间
}GR_FRAME;

#pragma pack()


void send_slave_test_data_fpga(void);

#endif //__SEND_TEST_DATA_FPGA_H__
