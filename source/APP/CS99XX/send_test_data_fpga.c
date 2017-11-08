
#include "stm32f10x.h"
#include "send_test_data_fpga.h"
#include "SPI_FPGA.H"
#include "serve_test.h"
#include "string.h"

void send_slave_test_data_fpga(void)
{
    ACW_FRAME acw_frame;
    DCW_FRAME dcw_frame;
    IR_FRAME ir_frame;
    GR_FRAME gr_frame;
    
    
    switch(cur_mode)
    {
        case ACW:
            memset(&acw_frame, 0, sizeof(ACW_FRAME));
            acw_frame.vol = vol_ave;
            acw_frame.cur = cur_ave;
            acw_frame.real = real_ave;
            acw_frame.cur_inf = cur_gear;
            acw_frame.test_flag = cur_work_st;
            acw_frame.test_status = cur_status;
            acw_frame.time = g_dis_time;
            fpga_write((u8 *)&acw_frame, sizeof(ACW_FRAME));
            break;
        case DCW:
            memset(&dcw_frame, 0, sizeof(DCW_FRAME));
            dcw_frame.vol = vol_ave;
            dcw_frame.cur = cur_ave;
            dcw_frame.cur_inf = cur_gear;
            dcw_frame.test_flag = cur_work_st;
            dcw_frame.test_status = cur_status;
            dcw_frame.time = g_dis_time;
            fpga_write((u8 *)&dcw_frame, sizeof(DCW_FRAME));
            break;
        case IR:
            memset(&ir_frame, 0, sizeof(IR_FRAME));
            ir_frame.vol = vol_ave;
            ir_frame.res = res_ave;
            ir_frame.res_inf = cur_gear;
            ir_frame.test_flag = cur_work_st;
            ir_frame.test_status = cur_status;
            ir_frame.time = g_dis_time;
            fpga_write((u8 *)&ir_frame, sizeof(IR_FRAME));
            break;
        case GR:
            memset(&gr_frame, 0, sizeof(GR_FRAME));
            gr_frame.cur = vol_ave;
            gr_frame.res = res_ave;
            gr_frame.res_inf = cur_gear;
            gr_frame.test_flag = cur_work_st;
            gr_frame.test_status = cur_status;
            gr_frame.time = g_dis_time;
            fpga_write((u8 *)&gr_frame, sizeof(GR_FRAME));
            break;
    }
}




