
#ifndef __MB_MASTER_H__
#define __MB_MASTER_H__

#ifndef NULL
#define NULL 0
#endif

#include "app.h"

#define DATA_MAX_LON  100

typedef enum{
    MB_MASETER_SENDING,/* 正在发送 */
    MB_MASETER_SEND_END,/* 发送结束 */
    MB_MASETER_SEND_BREAK,/* 终止发送并清空队列 */
}MB_MASTER_SEND_ST;/* MODBUS主机发送状态 */

typedef struct{
    uint16_t data_f[DATA_MAX_LON];/* 数据缓冲 */
    uint8_t slave_addr;/* 从机地址 */
    uint8_t read_addr;/* 读出的地址 */
    uint8_t read_lon;/* 读出的长度 半字的个数 = 字节数/2 */
    uint8_t write_addr;/* 写入的地址 */
    uint8_t write_lon;/* 写入的数据长度 半字的个数 = 字节数/2 */
    uint8_t type;/* 通信包的类型是广播帧还是普通帧 */
}MB_PACK_T;


#define QUEUE_DEEP    MDBUS_BUF_SIZE  /* 队列的深度 */
typedef struct{
    MB_PACK_T node[QUEUE_DEEP];
    uint32_t top;/* 队列的顶部 */
}QUEUE_T;


extern void mb_send_cmd(MB_PACK_T *mb_pack);
extern int32_t mb_query_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t* data, uint16_t data_len, uint16_t len);
extern int32_t mb_set_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t *data, uint16_t len);


#endif //__MB_MASTER_H__
