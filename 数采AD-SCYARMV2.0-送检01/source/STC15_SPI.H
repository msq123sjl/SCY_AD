#ifndef _STC15_SPI_H_
#define _STC15_SPI_H_
#define uchar unsigned char
#define uint unsigned int
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
void InitSPI();
BYTE SPISwap(BYTE dat);         //主机与从机之间交换数据
void SPI_Send_Byte(unsigned char  dat);
typedef struct _SPI_BUF
{
     unsigned char  buf [20];   // 多申请一个字符保证最后一个字符永远是'0'
     unsigned int index ;
}SPI_BUF;

extern bit MSSEL;                      //1: master 0:slave
extern bit  sendok_flg;
extern bit	receive_flag;
extern SPI_BUF spibuf ;

//void clear_buf_spi();
void SPI_Send_String(unsigned char * buf);

#endif
