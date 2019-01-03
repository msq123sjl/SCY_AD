#include "STC15F2K60S2.H"

#include "uart.h"

//#define FOSC 22118400L  // <  晶振
#define FOSC 11059200L  // <  晶振
#define BAUD1 9600 		// < 波特率
#define S2RI  0X01
#define S2TI  0X02
#define S3RI  0X01
#define S3TI  0X02
#define S4RI  0X01
#define S4TI  0X02

#define	Timer2_Reload	(65536UL -(FOSC / 4 / BAUD1))		//Timer 2 重装值， 对应300KHZ
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

bit uart1_sendok_flg=0;
unsigned char receivedbuf[20];
static unsigned int index=0;

//void clear_buf_uart1()
//{
//    xdata unsigned int i = 0 ,c ;
//    c = 20 ;
//    ES = 0 ;
//    for( i = 0 ; i < c ; i ++)
//    {
//		receivedbuf[i] = 0x00 ;
//    }
//
//    index = 0 ;
//
//    ES = 1 ;
//}
//



/*初始化串口1*/
void init_uart1()
{
	SCON  |= 0X50;
	T2H = (u8)(Timer2_Reload >> 8);
	T2L = (u8)Timer2_Reload;
	AUXR|=0X15;
	ES    = 1;               		  //允许串口1中断 
    EA = 1 ;
}



void send_data_uart1(unsigned char word)
{
	ES=0;
    SBUF = word ;
    while(!TI);
    TI = 0 ;
	ES=1;
}

void send_string_uart1(unsigned char * S)
{
    while(*S)
    {
        send_data_uart1(*S++);
    }
}


//void send_buf_uart1(unsigned char buf[] , unsigned int len)
//{
//	unsigned int i ;
//    //ES=0;
//    for(i=0;i<len;i++)
//        send_data_uart1(buf[i]);
//	//ES=1;
//}


//串口接收
void Uart1() interrupt 4 using 1	    
{
    if(RI)
    {
		if(uart1_sendok_flg==0)
		{
			if(SBUF=='#')index=0;
			receivedbuf[index]=SBUF;
			if((index>1)&&(receivedbuf[index]==0x0A))
			{
				uart1_sendok_flg=1;
			}
			else
			{
				if(index>=19)index=0;
				index++;
			}
		}

	    RI=0;

    }
	if(TI)
    {
	    TI=0;
	}

}



