//interrupt.c
#include <lpc21xx.h>
#include "types.h"
#include "defines.h"
#include "mini_pro_defines.h"
#include "headers.h"
vu8 menu_flag = 0 ;
vu8 pause_flag = 0;

void eint0_isr(void)__irq
{
	menu_flag = 1;
	//delay for debouncing 
	delay_ms(200);
	EXTINT =(1<<0);
	//DAMY 
	VICVectAddr = 0;
}

void eint1_isr(void)__irq
{
	pause_flag =1;
	//delay for debouncing 
	delay_ms(200);	
	EXTINT =(1<<1);
	//DAMY 
	VICVectAddr = 0;
}

void enable_Eint0(void)
{
	
	//cfg 3:2 P0.1 -> 11(3) as eint0 pin
	PINSEL0 = ((PINSEL0 & ~(3<<2)) | (3<<2));
	//SELECT EINT0 AS IRQ
	VICIntSelect &=~ (1<<EINT0_CH);
	//ENABLE THE EINT0 SOURCE
	VICIntEnable |= (1<<EINT0_CH);
	//SELECT SLOT0 FOR EINT0
	VICVectCntl1 = (1<<5)|EINT0_CH;
	//LOAD ISR ADDR INTO ADD REG
	VICVectAddr1 = (u32)eint0_isr;
	//EINT0 TRIGGER AT EDGE
	EXTMODE |= 1<<0;
	//FALLING EDGE
	EXTPOLAR &=~ (1<<0);
	
}

void enable_Eint1(void)
{
	
	//cfg 7:6 P0.3 ->11(3) as eint2 pin
	PINSEL0 = ((PINSEL0 & ~(3<<6)) | (3<<6));
	//SELECT EINT0 AS IRQ
	VICIntSelect &=~ (1<<EINT1_CH);
	//ENABLE THE EINT0 SOURCE
	VICIntEnable |= (1<<EINT1_CH);
	//SELECT SLOT0 FOR EINT0
	VICVectCntl2 = (1<<5)|EINT1_CH;
	//LOAD ISR ADDR INTO ADD REG
	VICVectAddr2 = (u32)eint1_isr;
	//EINT0 TRIGGER AT EDGE
	EXTMODE |= 1<<1;
	//FALLING EDGE
	EXTPOLAR &=~ (1<<1);
	
}

