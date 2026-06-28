//tim_dly_seg.c

#include <lpc21xx.h>
#include "types.h"
#include "headers.h"

volatile u32 ms_tick = 0; 

void timer0_isr(void) __irq
{
	  // increment every 1ms
    ms_tick++;      
		if(ms_tick % 5 == 0)
			refreshSeg();
	  // clear match flag
    T0IR  = 1;      
    // end of interrupt	
    VICVectAddr = 0;   
}

void timer_Init(void)
{
    T0PR  = 14999;    // you know this from tdelay_ms
    T0MR0 = 1;    // match every 1ms
    T0MCR = 0x03;    // interrupt + reset
    T0TCR = 1;    // start timer
    
    VICVectAddr0 = (u32)timer0_isr;
    VICVectCntl0 = 0x20|4;
    VICIntEnable = 1<<4;
}


