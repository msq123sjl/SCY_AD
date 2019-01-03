#ifndef _UART_H_
#define _UART_H_
#define uchar unsigned char
#define uint unsigned int
 
void send_string_uart1(unsigned char * S);
void init_uart1();	
void send_data_uart1(unsigned char word);
//void clear_buf_uart1();

extern bit uart1_sendok_flg;
extern unsigned char receivedbuf[20];

#endif
