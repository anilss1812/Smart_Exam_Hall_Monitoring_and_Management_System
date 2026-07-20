//adc_defines.h

//ADC CLOCK DEFINES
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)
#define ADCLK 3000000
#define DIVIDER ((PCLK/ADCLK)-1)

//ADC SFRS

//ADCR
#define ADC_CLK_DIV (DIVIDER<<8)
#define PDN_BIT (1<<21)
#define START_CONV (1<<24)
//CHANNELS
#define CH0 (1<<0)  //p0.27
#define CH1 (1<<1)  //p0.28 //using
#define CH2 (1<<2)  //p0.29
#define CH3 (1<<3)  //p0.30

//ADDR
#define RESULT 6
#define DONE_BIT 31
