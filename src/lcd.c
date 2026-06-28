//lcd.c
#include<lpc21xx.h>
#include "types.h"
#include "mini_pro_defines.h"
#include "defines.h"
#include "headers.h"

void writeLcd(u8 byte)
{
 	 //write to data
	 WRITEBYTE(IOPIN0,byte,data);
	 //SELECT WRITE OPERATION
	 IOCLR0 = 1<<EN;
	 //PROVIDE HIGH TO LOW EN
	 IOSET0 = 1<<EN;
	 delay_us(1); //at least 470ns
	 IOCLR0 =1<<EN;
	 delay_us(50); //at least 37µs
}

void cmdLcd(u8 opcode)
{
	//clr rs pin for cmd resis select
	IOCLR0 =1<<RS;
	//WRITE TO CMD REGISTER VIA D8 TO D15
	writeLcd(opcode);
	//for clear and return to home requires 2ms delay
	if(opcode == 0x01 || opcode == 0x02) 
		 delay_ms(2); 
}

void lcd_Init(void)
{
 	//cfg p0.8 to p0.15 rs,rw,en as gpio out
	IODIR0 |=((1<<RS) |(1<<EN));
  IODIR0 |= (0XFF<<data);
	delay_ms(15);
	cmdLcd(MODE_8BIT_1LINE);
	delay_ms(4);
	delay_us(2);
  cmdLcd(MODE_8BIT_1LINE);
	delay_us(100);
	cmdLcd(MODE_8BIT_1LINE);
	cmdLcd(MODE_8BIT_2LINE);
	cmdLcd(DSP_ON_CUR_OFF);
	cmdLcd(CLEAR_LCD);
	cmdLcd(SHIFT_CUR_RIGHT);
}
void charLcd(u8 ascii)
{
 	//set rs pin for data
	IOSET0 = 1<<RS;
	//WRITE TO DDRAM VIA DATA
	writeLcd(ascii);
}

void strLcd(s8 *str)
{
	while(*str)
	charLcd(*str++);
}

void U32Lcd(u32 num)
{
	u8 a[10];
	s32 i=0;
	if(num==0)
	{
		charLcd('0');
	}
	else
	{
		 while(num>0)
		 {
		 	a[i++]=(num%10)+48;
			num/=10;
		 }
			for(--i;i>=0;i--)
   			{	
				charLcd(a[i]);
			}
	}
}

void S32Lcd(s32 num)
{
	if(num<0)
	{
		charLcd('-');
		num=-num;

	}
	U32Lcd(num);
}

void F32Lcd(f32 fnum,u32 ndp)
{
	u32 num;
	s32 i;
 	if(fnum<0.0)
	{
		charLcd('-');
		fnum=-fnum;
	}
	num = fnum;
	U32Lcd(num);
	charLcd('.');
	for(i=0;i<ndp;i++)
	{
		fnum = (fnum-num)*10;
		num = fnum;
		charLcd(num+48);
	}
}

/*void buildCgram(s8 *p,u32 nbytes)
{
	u32 i;
	//goto to cgram 
	cmdLcd(GOTO_CGRAM_START);
	//RS =1 for data mode
	IOSET0 = 1<<RS;
	for(i=0;i<nbytes;i++)
	{
		writeLcd(p[i]);
	}
	//goto to ddram
	cmdLcd(GOTO_LINE1_POS0);

	//For love symbol 
	//0x0A,0x15,0x11,0x11,0x00,0x0A,0x04,0x00

}*/

