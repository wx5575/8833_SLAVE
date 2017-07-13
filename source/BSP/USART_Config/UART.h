#ifndef __UART_H__
#define __UART_H__


void uart_init(void);
uint8_t get_send_status_i(void);
void clear_receive_count_i(void);
uint32_t get_uart_timeout_i(void);
uint32_t sub_uart_timeout_i(void);
uint8_t* get_receive_data_i(void);
uint8_t * get_uart_send_buf_i(void);
uint32_t get_receive_data_count_i(void);
void clear_receive_data_count_i(void);
void set_send_data_i(uint8_t *buf, uint16_t c);
void re_send_last_data_i(void);
void close_uart_i(void);

#endif //__UART_H__
