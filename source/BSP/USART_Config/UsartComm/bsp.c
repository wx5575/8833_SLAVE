// #include "stm32f10x.h"
// #include "stm32f10x_conf.h"
// #include "comm_usart2.h"


// void bps_config(void)
// {
//     Usart2Config();
// }

// /*
//     功能    读取服务器接收到的数据
//     描述    本机为服务器端，接收客户机发送的指令数据，从接收队列的头结点读取至数据的尾节点，
//                 修改接收数据缓冲队列的头结点为当前尾节点的下一个节点
//     参数    用于接收数据的头指针
//     返回    接收数据的个数
// */
// uint8_t bspReadServerData(uint8_t * data)
// {
//     uint16_t i, j;

//     i = Usart2_front;
//     j = 0;
//     while(1) {
//         data[j] = UsartReceive_Data[i];
//         i = (i + 1) % BUFFER_SIZE_USART2;
//         j++;
//         
//         if(i == Usart2_rear)
//             break;
//     }
//     Usart2_front = Usart2_rear;
//     return j;
// }

// /*
//     功能    服务器端发送数据
// */
// void bspSendServerData(uint8_t * data, uint8_t data_num)
// {
//     Usart2_SendData((char *)data, data_num);
// }

// /*
//     功能    客户端发送数据
// */
// void bspSendClientData(uint8_t * data, uint8_t data_num)
// {
//     Usart2_SendData((char *)data, data_num);
// }

// /*
//     功能    客户端接收数据
// */
// uint8_t bspReadClientData(uint8_t * data)
// {
//     uint16_t i, j;

//     i = Usart2_front;
//     j = 0;
//     while(1) {
//         data[j] = UsartReceive_Data[i];
//         i = (i + 1) % BUFFER_SIZE_USART2;
//         j++;

//         if(i == Usart2_rear)
//             break;
//     }

//     return j;
// }







