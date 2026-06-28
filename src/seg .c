//seg.c
#include <lpc21xx.h>
#include "headers.h"
#include "types.h"
#include "defines.h"
#include "mini_pro_defines.h"
 

const u8 seg_LUT[10] =
   {
		  0xc0,0xf9,0xa4,0xb0,0x99,
		  0x92,0x82,0xf8,0x80,0x98
	 };

void seg_Init(void)
{
	WRITEBYTE(IODIR0,0XFF,CA7SEG_2_MUX);
	IODIR1 = (IODIR1 &~ (3<<DSEL1)) | (1<<DSEL1) | (1<<DSEL2);
}	
void disp_2_mux_seg(u8 num)
{
	  //for writing 1st digit to 1st seg
	  WRITEBYTE(IOPIN0,seg_LUT[num/10],CA7SEG_2_MUX);
	  //enable and disabl seg1
	  IOSET1 =1<<DSEL1;
	  delay_ms(1);
	  IOCLR1 = 1<<DSEL1	;
	  //FOR WRITING 2ND DIGIT TO 2ND SEG
	  WRITEBYTE(IOPIN0,seg_LUT[num%10],CA7SEG_2_MUX);
	  //ENABLE AND DISABLE SEG2
	  IOSET1 =1<<DSEL2;
	  delay_ms(1);
	  IOCLR1 = 1<<DSEL2;
}

volatile u8 seg_digit_sel = 0;
volatile u8 seg_value = 0;   // main loop just sets this, ISR reads it

void refreshSeg(void)
{
    if(seg_digit_sel == 0)
    {
        IOCLR1 = 1<<DSEL2;
        WRITEBYTE(IOPIN0, seg_LUT[seg_value/10], CA7SEG_2_MUX);
        IOSET1 = 1<<DSEL1;
        seg_digit_sel = 1;
    }
    else
    {
        IOCLR1 = 1<<DSEL1;
        WRITEBYTE(IOPIN0, seg_LUT[seg_value%10], CA7SEG_2_MUX);
        IOSET1 = 1<<DSEL2;
        seg_digit_sel = 0;
    }
}

