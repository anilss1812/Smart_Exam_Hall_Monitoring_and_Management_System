//rtc_defines.h

#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))
//CCR
#define RESET_RTC (1<<1)
#define ENABLE_RTC (1<<0)
//clock src is set to extranal rtc oscillator and enable the rtc
#define RTC_CLK_SRC (1<<4)|(1<<0) 
