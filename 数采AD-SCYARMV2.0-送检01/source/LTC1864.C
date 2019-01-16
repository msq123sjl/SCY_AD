#include "STC15F2K60S2.H"
#include "LTC1864.H" 
#include "delay.h"
sbit AD_SDO = P3^4;
sbit AD_SCK = P3^3;
sbit AD_CONV = P3^2;
sbit AD_A0 = P3^5;
sbit AD_A1 = P3^6;
sbit AD_A2 = P3^7;

#define N 	41			  //中值法采样点个数
//xdata float real_data=0;
xdata uchar a1,a2,a3;	//主函数调用	
//xdata uint buff[N],vtemp1;
xdata float buff[N],vtemp1;
xdata float voltage;
//xdata float buff[N],vtemp1,voltage;
float k1=0,k2=0,k3=0;

void LTC1864_init()
{
	AD_CONV=1;
	AD_SCK=1;
	delay1us(1);   //误差 -0.095775462963us

} 



float LTC1864_Read(uchar channel)	//0-7
{
	uint AD_data=0;
	uchar i=0;

	AD_A0=channel&0x01;
	AD_A1=(channel&0x02)>>1;
	AD_A2=(channel&0x04)>>2;
 	Delay10ms();            
 	Delay10ms();            
 	Delay10ms();            
 	Delay10ms();            
 	Delay10ms();            
	for(a1=0;a1<N;a1++)	  //一次采集N个数据放入数组中
	{
		LTC1864_init();	
		AD_CONV=0;
		for(i=0; i<16; i++)   
		 { 
			AD_SCK=0; 
			AD_SCK=1;
		 	delay1us(1);            //  keep level for 1 us
		 	AD_data=(AD_data<<1)|AD_SDO;	
		 }
		delay1us(1);
		//real_data=(float)(AD_data)/65535*5  ;
		//buff[a1] =real_data;
		buff[a1] = (float)AD_data;
	}
	for(a2=0;a2<N-1;a2++)	   //采样值由小到大排列
	{
		for(a3=0;a3<N-a2-1;a3++)
		{
			if(buff[a3]>buff[a3+1])
			{
			  vtemp1=buff[a3];
			  buff[a3]=buff[a3+1];
			  buff[a3+1]=vtemp1;
			}
		}		
	
	}
	voltage = (buff[(N-1)/2-3] + buff[(N-1)/2-2] \
					+ buff[(N-1)/2-1] + buff[(N-1)/2]\
					+ buff[(N-1)/2+1] + buff[(N-1)/2+2]\
					+ buff[(N-1)/2+3] + buff[(N-1)/2+4])/(float)8;
	
	//voltage = (buff[(N-1)/2] + buff[(N-1)/2+1])>>1;
	/*voltage = (buff[(N-1)/2-3]>>3) + (buff[(N-1)/2-2]>>3) \
			+ (buff[(N-1)/2-1]>>3) + (buff[(N-1)/2]>>3)\
			+ (buff[(N-1)/2+1]>>3) + (buff[(N-1)/2+2]>>3)\
			+ (buff[(N-1)/2+3]>>3) + (buff[(N-1)/2+4]>>3);*/

////////////////01-4.99899//////////////////////
	/*if(voltage<=0.0055)voltage=0;
	k1=1.0005;					   //对比2V得来
	if(voltage>0.0055)voltage*=k1;
	k2=0.0005;				   //2V以下修正系数
	if(voltage<2&&voltage>0.0055*k1)voltage=voltage+(2-voltage)*k2;
	k3=0.0006;					   //2V以上修正系数
	if(voltage>2&&voltage<(voltage+k3*(voltage-2)))voltage=voltage-(voltage-2)*k3;
	if(voltage>5)voltage=5;*/



	
//	if(voltage<=0.0055)voltage=0;
//	if(voltage>0)voltage+=0.001;
////	if(voltage>5)voltage=voltage-0.002;
////	if(voltage<5.001)voltage=voltage*0.998216;
//	if(voltage>5)voltage=5;

	AD_CONV=1;
	return voltage;
}






