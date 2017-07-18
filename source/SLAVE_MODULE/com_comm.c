/**
  ******************************************************************************
  * @file    com_comm.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   串口的结构信息
  ******************************************************************************
  */

#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART3.H"
#include "Timer5_Config.h"

/**
  * @brief  接收完成的处理函数
  * @param  [in] com 串口类型指针
  * @retval 无
  */
void receive_over_dispose(COM_STRUCT *com)
{
    uint8_t *frame;
    uint32_t len;
    
    frame = com->get_com_frame(com);
    len = com->get_com_frame_len(com);
    
    com_receive_dispose(com, frame, len);
}

/**
  * @brief  串口通信状态机
  * @param  None
  * @retval None
  */
void com_comm_status_machine(COM_STRUCT *com)
{
    switch(com->status)
    {
        case MODULE_COMM_RECEIVE:
            if(com->get_receive_over_flag(com))
            {
                com->receive_dispose_fun(com);
            }
            break;
        case MODULE_COMM_SEND:
            if(com->get_com_send_status(com))
            {
                com->status = MODULE_COMM_RECEIVE;//进入接收状态
            }
            break;
    }
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
