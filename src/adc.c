//ADC.C

#include <lpc21xx.h>
#include "types.h"
#include "mini_pro_defines.h"
#include "headers.h"

void adc_Init(void)
{
	//cfg p0.27,28,29,30 as AIN0
	PINSEL1 |= 0x15400000;
	//TAKE ADC ADC INTO OPERATIONAL MODE AND SET ADC_CLK
	ADCR = PDN_BIT | ADC_CLK_DIV;
}
void Read_acd(u32 CHNO,u32* adcDval,f32* ear)
{
	//clear the channel bits
	ADCR &=~(255<<0);
	//select channel & start conv 
	ADCR |= CHNO | START_CONV;
	//convertion time
	delay_us(3);
	//wait for done_bit status
	while(!((ADDR>>DONE_BIT)&1)) ;
	//stop convertion
	ADCR &=~ START_CONV;
	//extract result
	*adcDval = ((ADDR>>RESULT)&1023);
	//analog readings
	//stepsize * digital value
	*ear = (3.3/1024)*(*adcDval);
}
