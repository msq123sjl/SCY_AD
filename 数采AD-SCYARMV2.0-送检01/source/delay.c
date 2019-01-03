#include "STC15F2K60S2.H"
#include "delay.h"
#include <intrins.h>

#define  uint unsigned int

//==============================================================================
// —” ±
//==============================================================================

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void delay1us(uint t)   //ŒÛ≤Ó -0.095775462963us

{
	while(t--)
	{
		;
	}


}

