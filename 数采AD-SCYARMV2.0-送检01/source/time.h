#ifndef _TIME_H_
#define _TIME_H_

#define uchar unsigned char
#define uint unsigned int

void Timer1Init(void);		//20����@22.1184MHz
//void Timer0Init(void);		//50����@11.0592MHz
//extern xdata uchar sendbuf[200];
extern  bit Flag_safe;   //=1ҧ   =0��ҧ
extern  bit dog_state;   //=0ҧ֮ǰ =ҧ֮��
extern xdata unsigned long int timecount;
extern xdata unsigned long int timecount1;
//extern unsigned  int  count;
//extern xdata uchar sendbuf_spi[200];

#endif