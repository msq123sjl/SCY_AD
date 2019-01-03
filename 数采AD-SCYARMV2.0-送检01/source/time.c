#include "STC15F2K60S2.H"
#include "time.h"
#include "STC15_SPI.h"
#include "LTC1864.H" 
bit Flag_safe=0;   //=1ҧ   =0��ҧ
bit dog_state=1;   //=0ҧ֮ǰ =ҧ֮��
xdata unsigned long int timecount=0;
xdata unsigned long int timecount1=0;
xdata unsigned int  io_press_timer;
xdata unsigned char Trg,Continuous;  

xdata uint count_spi=0;


void Timer1Init(void)		//20����@11.0592MHz
{
	AUXR&=0xBF;			//	12��Ƶ
	TMOD &= 0x0F;		    //���ö�ʱ��ģʽ
	TL1 = 0x00;				//���ö�ʱ��ֵ
	TH1 = 0xB8;				//���ö�ʱ��ֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1=1;					//������ʱ���ж�1
	EA=1;
}

void IO_Read( void )  
{   
	xdata unsigned char ReadData,io_temp; 
	io_temp=P5|0xdf;
	ReadData = io_temp^0xFF;   // 1      
	Trg = ReadData & (ReadData ^ Continuous);      // 2      
	Continuous = ReadData;                                // 3  
}  

void Timer1Interrupt(void) interrupt 3
{
	ET1=0;
	TR1=0;
	IO_Read();
	if(Trg)	io_press_timer=1;
	if(Continuous&&io_press_timer)
	{
		io_press_timer++;
		if(io_press_timer>=5)
		{
		
			dog_state=0;
			Flag_safe=0;
			timecount=0;
			io_press_timer=0;
		}
	}
	if(dog_state==0)timecount++;
	if(dog_state)timecount1++;	   
	if(dog_state)				// ҧ֮��
	{
		timecount=0;
		if(timecount1>=15000)	    // 20*15000=300000ms��ʱ	  
		{	
			timecount1=0;
			dog_state=0;
		}
	}
	if(dog_state==0)				 //ҧ֮ǰ
	{	
		if(timecount>=15000)	    //20*15000=150000ms��ʱ
		{	
			timecount=0;
			Flag_safe=1;			
		}
	}

	ET1=1;
	TR1=1;
}






