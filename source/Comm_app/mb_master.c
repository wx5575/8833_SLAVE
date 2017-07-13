
#include "stm32f10x.h"
#include "app.h"
#include "mb_master.h"
#include "mb_cmd_define.h"
#include "mb.h"
#include "mb_m.h"


QUEUE_T send_queue;/* 发送队列 */

/*
 * 函数名：mb_send_cmd
 * 描述  ：modbus主机发送函数
 * 输入  ：mb_pack 发送数据帧包
 * 输出  ：无
 * 返回  ：无
 */
void mb_send_cmd(MB_PACK_T *mb_pack)
{
    if(mb_pack == NULL)
    {
        return;
    }
    
    eMBMasterReqReadWriteMultipleHoldingRegister(mb_pack->slave_addr,
                           mb_pack->read_addr, mb_pack->read_lon,
                           mb_pack->data_f,
                           mb_pack->write_addr, mb_pack->write_lon);
//     OSTimeDlyResume(MODBUS_SEND_TASK_PRIO);
}

/*
 * 函数名：mb_query_slave_info
 * 描述  ：modbus主机发送询问从机信息指令，并将数据帧放入modbus的发送队列中
 * 输入  ：slave_addr 从机地址
 *          cmd 命令
 *          data 发个从机的数据
 *          data_len 发给从机数据长度
 *          len 询问从机数据的程度 -2
 * 输出  ：无
 * 返回  ：无
 */
int32_t mb_query_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t* data, uint16_t data_len, uint16_t len)
{
    uint16_t i = 0;
    uint32_t j = 0;
    MB_PACK_T  *mb_pack;
    uint8_t type;
    uint8_t err = 0;
    
    OSMutexPend(MdbusMutex, 0, &err);
    mb_pack = &send_queue.node[send_queue.top];
    send_queue.top = (send_queue.top + 1) % QUEUE_DEEP;
    
    if(slave_addr == 0)
    {
        type = MB_NO_WAIT_ACK;
    }
    else
    {
        type = MB_YES_WAIT_ACK;
    }
    
    mb_pack->slave_addr = slave_addr;
    mb_pack->read_addr = 0;
    mb_pack->read_lon = len + 2;
    mb_pack->write_addr = 0;
    mb_pack->write_lon = data_len + 2;
    mb_pack->type = type;
    
    mb_pack->data_f[i++] = cmd;
    mb_pack->data_f[i++] = MB_READ_PAR;
    
    for(j = 0; j < data_len; j++)
    {
        mb_pack->data_f[i++] = data[j];
    }
    
    OSQPost(MdbusQSem, mb_pack);
	OSMutexPost(MdbusMutex);
    
    return 0;
}


/*
 * 函数名：mb_set_slave_info
 * 描述  ：modbus主机发送设置从机信息指令，并将帧数据放入modbus的发送队列中
 * 输入  ：slave_addr 从机地址
 *          cmd 命令
 *          data 发个从机的数据
 *          data_len 发给从机数据长度
 *          len 询问从机数据的程度 -2
 * 输出  ：无
 * 返回  ：无
 */
int32_t mb_set_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t *data, uint16_t len)
{
    uint16_t i = 0;
    uint32_t j = 0;
    MB_PACK_T  *mb_pack;
    uint8_t type;
    uint8_t err = 0;
    
    OSMutexPend(MdbusMutex, 0, &err);
    mb_pack = &send_queue.node[send_queue.top];
    send_queue.top = (send_queue.top + 1) % QUEUE_DEEP;
    
    if(slave_addr == 0)
    {
        type = MB_NO_WAIT_ACK;
    }
    else
    {
        type = MB_YES_WAIT_ACK;
    }
    
    mb_pack->slave_addr = slave_addr;
    mb_pack->read_addr = 0;
    mb_pack->read_lon = 3;
    mb_pack->write_addr = 0;
    mb_pack->write_lon = len + 2;
    mb_pack->type = type;
    
    mb_pack->data_f[i++] = cmd;
    mb_pack->data_f[i++] = MB_SET_PAR;
    
    for(j = 0; j < len; j++)
    {
        mb_pack->data_f[i++] = data[j];
    }
    
    OSQPost(MdbusQSem, mb_pack);
	OSMutexPost(MdbusMutex);
    
    return 0;
}


/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/


