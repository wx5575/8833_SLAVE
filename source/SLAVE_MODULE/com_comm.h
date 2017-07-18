/**
  ******************************************************************************
  * @file    com_comm.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   串口的结构信息
  ******************************************************************************
  */


#ifndef __COM_COMM_H__
#define __COM_COMM_H__



#include "cs99xx_struct.h"
typedef enum{
    MODULE_COMM_RECEIVE,///<模块通信处于接收状态
    MODULE_COMM_SEND,///<模块通信处于发送状态
}MODULE_COMM_STATUS;

/**
  * @brief  串口编号
  */
typedef enum{
    ROAD1_COM,///<第一路串口编号
    ROAD2_COM,///<第二路串口编号
    ROAD3_COM,///<第三路串口编号
    ROAD4_COM,///<第四路串口编号
}COM_NUM;

#define RESEND_NUM          3 ///<最大重发次数
#define WAIT_TIMEOUT_MS     500 ///<等待从机响应的超时时间 ms
typedef struct COM_STRUCT_ COM_STRUCT;
typedef void (*COM_SERVE_FUN)(COM_STRUCT *);
typedef uint8_t (*COM_SERVE_R_U8_FUN)(COM_STRUCT *);
typedef uint32_t (*COM_SERVE_R_U32_FUN)(COM_STRUCT *);
typedef uint8_t* (*COM_SERVE_R_PU8_FUN)(COM_STRUCT *);

struct COM_STRUCT_{
    MODULE_COMM_STATUS status;///<串口状态 空闲状态 发送状态 接收状态
    void (*send_fun)(COM_STRUCT *,uint8_t*,uint32_t);///<发送函数
    COM_SERVE_FUN resend_fun;///<重发函数
    
    COM_SERVE_FUN receive_dispose_fun;///<接收完成后的处理函数
    COM_SERVE_R_U8_FUN get_receive_over_flag;///<获取接收完成标志
    
    COM_SERVE_R_PU8_FUN get_com_frame;///<获取串口数据帧
    COM_SERVE_R_U32_FUN get_com_frame_len;///<获取串口数据帧长度
    COM_SERVE_R_U8_FUN get_com_send_status;///<获取串口通信的发送状态
    uint8_t frame_buf[512];///<通信缓冲区
};

void com_set_wait_ack_timeout(COM_STRUCT *com);
void receive_over_dispose(COM_STRUCT *com);
void com_set_wait_ack_timeout(COM_STRUCT *com);
void com_wait_ack_timeout(COM_STRUCT *com);


#endif ///__COM_COMM_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
