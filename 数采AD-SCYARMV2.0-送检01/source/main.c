#include "STC15F2K60S2.H"
#include "delay.h"
#include "time.h"
#include "uart.h"
#include "LTC1864.H"
#include "STC15_SPI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

sbit IO_RST	= P5^4;
sbit TEST1 = P2^6;
sbit TEST0 = P2^5;
xdata uchar sendbuf[200]="0";
//xdata unsigned char tempbuf[50]="0";
unsigned char *strtemp;
xdata unsigned int  crc,len,CRC_16;
xdata int kk;

int String_Search(unsigned char *substr,unsigned char *mainstr,unsigned char n);
unsigned int CRC16(unsigned char *databuff, unsigned int len); 
void Hex_To_Str(unsigned int x,char *Str);
xdata char info[6];
//xdata char result[11]={'0'};
//xdata int point_front=0;
//xdata int point_back=0;
//void  float_to_2int(float f);
//void Int_To_Str(int x,char *Str,int flag);


xdata unsigned char CRC_Buf[5];
char valueToHexCh(unsigned int value);

xdata unsigned char ad_road=0;
xdata uint AD0=0,AD1=0,AD2=0,AD3=0,AD4=0,AD5=0,AD6=0,AD7=0;
xdata uchar sendbuf_spi[200]="0";
void main()
{
	uint iLoop;
	Delay500ms();
	WDT_CONTR = 0x3e;
//	P1M1|=(1<<4);
//	P1M0|=(1<<4);
//	P1M1|=(1<<3);
//	P1M0|=(1<<3);
//	IO_RST=0;
//	Delay500ms();
	P3M1=0;
	P3M0=0;
	TEST1=0;
	Timer1Init();
	init_uart1();
    InitSPI();                  //初始化SPI    
	IO_RST=1;

	while(1)
	{
//		ad_road=4;
		switch (ad_road)
		{
			case 0:AD0=LTC1864_Read(ad_road);
					break;
			case 1:AD1=LTC1864_Read(ad_road);
					break;
			case 2:AD2=LTC1864_Read(ad_road);
					break;
			case 3:AD3=LTC1864_Read(ad_road);
					break;
			case 4:AD4=LTC1864_Read(ad_road);
					break;
			case 5:AD5=LTC1864_Read(ad_road);
					break;
			case 6:AD6=LTC1864_Read(ad_road);
					break;
			case 7:AD7=LTC1864_Read(ad_road);
					break;
			default:
				break;
		}
		ad_road++;
		if(ad_road>=8) ad_road=0;
		if(Flag_safe==1)
		{
			TEST1=~TEST1;
			IO_RST=0;
			Delay500ms();
			IO_RST=1;
			dog_state=1;
			Flag_safe=0;
		}
		WDT_CONTR = 0x3e; //喂狗
		TEST0 =~TEST0;

		memset(sendbuf,0,200);
		//memset(tempbuf,0,50);
		memset(CRC_Buf,0,5);
		//strcat(sendbuf,"##FC=2;U0=");
		iLoop  = sprintf(sendbuf,"##FC=2;U0=%u",AD0);
		iLoop += sprintf(sendbuf + iLoop,",U1=%u",AD1);
		iLoop += sprintf(sendbuf + iLoop,",U2=%u",AD2);
		iLoop += sprintf(sendbuf + iLoop,",U3=%u",AD3);
		iLoop += sprintf(sendbuf + iLoop,",U4=%u",AD4);
		iLoop += sprintf(sendbuf + iLoop,",U5=%u",AD5);
		iLoop += sprintf(sendbuf + iLoop,",U6=%u",AD6);
		iLoop += sprintf(sendbuf + iLoop,",U7=%u;CRC=",AD7);

		CRC_16=CRC16(&sendbuf[2], strlen(sendbuf)-2);
		CRC_Buf[0]=valueToHexCh((CRC_16&0xF000)>>12);
		CRC_Buf[1]=valueToHexCh((CRC_16&0x0F00)>>8);
		CRC_Buf[2]=valueToHexCh((CRC_16&0xF0)>>4);
		CRC_Buf[3]=valueToHexCh(CRC_16&0x0F);
		CRC_Buf[4]='\0';
		strcat(sendbuf,CRC_Buf);
		strcat(sendbuf,"\r\n");
		strcpy (sendbuf_spi,sendbuf);

		if(sendok_flg)
		{
			ET1=0;				

			SPI_Send_String(sendbuf_spi);
			ET1=1;				
			sendok_flg=0;
			receive_flag=0;	

		}

		if(uart1_sendok_flg)
		{
			send_string_uart1(sendbuf_spi);
			uart1_sendok_flg=0;
		}

//		clear_buf_spi();
//		SPI_Send_String(sendbuf);

//			}
//		}
//		sendok_flg=0;
//		receive_flag=0;	
//		EA=1;	
//	}
	}


}

//int String_Search(unsigned char *substr,unsigned char *mainstr,unsigned char n)
//{	
//	unsigned int i=0,max_len=0; 
//
//	max_len = strlen(mainstr);
//			
//	for(i=0;i<max_len;i++)		
//	{
//		if(*(mainstr+i) == *substr)
//		{
//			if(strncmp(mainstr+i,substr,n) == 0)
//			{
//				return (i+n);
//			}
//		}		
//	}
//	return (-1);
//}

unsigned int CRC16(unsigned char *databuff, unsigned int len) 
{          
unsigned char   i;
    unsigned int  wkg = 0xFFFF;

  	while( len-- )
  	{
       wkg  = (wkg>>8)&0x00FF;
       wkg ^= *databuff++;

   for ( i = 0 ; i < 8; i++ )
   {
      if ( wkg & 0x0001 )
      {
        wkg = ( wkg >> 1 ) ^ 0xa001;
      }
      else
      {
        wkg = wkg >> 1;
      }
   }
  	}
  	return( wkg );
}

char valueToHexCh(unsigned int value)
{
  char result = '\0';
  if(value >= 0 && value <= 9){
    result = (char)(value + 48); //48为ascii编码的‘0’字符编码值
  }
  else if(value >= 10 && value <= 15){
    result = (char)(value - 10 + 65); //减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
  }
  else{
    ;
  }

  return result;
}

/*void  float_to_2int(float f)
{
	point_front= (int)f;
	point_back=(int)((f-point_front)*10000);
}*/

/*void Int_To_Str(int x,char *Str,int flag)
{
    int t;
    char *Ptr,Buf[5];
    int i = 0;
	int n = 0;
	int int_temp=0;
    Ptr = Str;
	memset(Buf,0x30,5);
	int_temp =x;
    if(x < 10)		// 当整数小于10时,转化为"0x"的格式
    {
		if(flag==0)		 //处理小数点后面的数据
		{
	        for(i=2;i >= 0;i --) *Ptr++=0x30;		
	        *Ptr ++ = x+0x30;
		}
		else			//处理小数点前面的数据
		{
	        *Ptr ++ = x+0x30;
		
		}

    }
    else
    {
		if(flag==0)
		{
	        while(x > 0)
	        {
	            t = x % 10;
	            x = x / 10;
	            Buf[i++] = t+0x30;	// 通过计算把数字转化成ASCII码形式
	        }
	        i -- ;
	        for(i=3;i >= 0;i --) 		// 将得到的字符串倒序
	        {
	            *(Ptr++) = Buf[i];
	        }  
		}
		else
		{			
			while(int_temp)
			
			{	
				int_temp=int_temp/10;
				++n;
			}
			n--;

	        while(x > 0)
	        {
	            t = x % 10;
	            x = x / 10;
	            Buf[i++] = t+0x30;	// 通过计算把数字转化成ASCII码形式
	        }
	        i -- ;
	        for(i=n;i >= 0;i --) 		// 将得到的字符串倒序
	        {
	            *(Ptr++) = Buf[i];
	        }  		
		
		}
    }
    *Ptr = '\0';

}*/

