#include "STC15F2K60S2.H"
#include "STC15_SPI.h"
#include "uart.h"
#include "LTC1864.H" 


#define     URMD    0           //0:使用定时器2作为波特率发生器
                                //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器

#define SPIF        0x80        //SPSTAT.7                                
#define WCOL        0x40        //SPSTAT.6                                
#define SSIG        0x80        //SPCTL.7                                 
#define SPEN        0x40        //SPCTL.6                                 
#define DORD        0x20        //SPCTL.5                                 
#define MSTR        0x10        //SPCTL.4                                 
#define CPOL        0x08        //SPCTL.3                                 
#define CPHA        0x04        //SPCTL.2                                 
#define SPDHH       0x00        //CPU_CLK/4                               
#define SPDH        0x01        //CPU_CLK/16                              
#define SPDL        0x02        //CPU_CLK/64                              
#define SPDLL       0x03        //CPU_CLK/128                             
sbit SPISS      =   P1^2;       //SPI从机选择口, 连接到其它MCU的SS口
                                //当SPI为一主多从模式时,请使用主机的普通IO口连接到从机的SS口
#define ESPI        0x02        //IE2.1

void InitSPI();
void SPI_Send_Byte(unsigned char  dat);


bit sendok_flg=0;
bit	receive_flag=0;
SPI_BUF spibuf ;


///////////////////////////////////////////////////////////

void spi_isr() interrupt 9 using 1     //SPI中断服务程序 9 (004BH)
{
	unsigned char ch=0x00;
	SPSTAT = 0xc0;
	ch=SPDAT;
	SPDAT=0x00;	
	if(sendok_flg==0)
	{
		if(ch=='#')
		{
			spibuf.index=0;	
			receive_flag=1;
		}
		if(receive_flag)
		{
	        spibuf.buf[spibuf.index] = ch;        //字符存到数组中
		}
		if(ch=='\n'&&receive_flag)
		{
			receive_flag=0;
			sendok_flg=1;
		}
		else
		{
			if(spibuf.index>=19)spibuf.index=0;
			spibuf.index++;
		}

	}


}


///////////////////////////////////////////////////////////

void InitSPI()
{
	SPISS=0;
	SPDAT =0;
	SPSTAT = 0xC0;
	SPCTL =	0x46;	
    IE2 |= ESPI;
	IP2 |=0x02;					  //SPI中断优先级最高
    EA = 1;


}



//void clear_buf_spi()
//{
//    xdata unsigned int i = 0 ,c ;
//    c = 20;
//    spibuf.index = 0 ;
//
//    for( i = 0 ; i < c ; i ++)
//    {
//        spibuf.buf[i] = 0x0 ;
//    }
//
//}

/*************************************************************************
				   SPI发送字节
**************************************************************************/
void SPI_Send_Byte(unsigned char  dat)
{  	
	IE2 &= ~ESPI;
	SPDAT = dat;
	while(!(SPSTAT & SPIF));	
    IE2 |= ESPI;
	SPSTAT = 0xC0; 
}

void SPI_Send_String(unsigned char * buf)
{

    while(*buf)
    {
        SPI_Send_Byte(*buf++);
    }
    IE2 |= ESPI;

}


