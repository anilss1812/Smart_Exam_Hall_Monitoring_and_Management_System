//mini_pro_defines.h

//---------LCD DEFINES----------
//Lcdcammands
#define CLEAR_LCD 		  0x01
#define RET_CUR_HOME   	0x02
#define SHIFT_CUR_RIGHT 0x06
#define DSP_OFF			    0x08
#define MOV_CUR_RIGHT   0x14
#define MOV_CUR_LEFT	  0x10
#define DSP_ON_CUR_OFF	0x0C
#define DSP_ON_CUR_ON		0x0E
#define SHIFT_DSP_LEFT	0x10
#define SHIFT_DSP_RIGHT	0x14
#define MODE_8BIT_1LINE	0x30
#define MODE_8BIT_2LINE	0x38
#define GOTO_LINE1_POS0	0x80
#define GOTO_LINE2_POS0	0xC0
#define GOTO_LINE3_POS0	0x94
#define GOTO_LINE4_POS0	0xD4


#define RS 16  //p0.16
#define EN 17 //p0.17
#define data 8 //p0.8 to p0.15

//----------Interrupt defines-----------

/*InterruptVIC Channel EINT0 - 14
										 EINT1 - 15
										 EINT2 - 16
										 EINT3 - 17*/
#define EINT0_CH 14 // 14 channel for eint1
#define EINT1_CH 15 //15 channel for eint2


//----------MATRIX KEYBOARD--------------

#define ROW0 16 //P1.16
#define ROW1 17
#define ROW2 18
#define ROW3 19

#define COL0 20 //P1.20
#define COL1 21
#define COL2 22
#define COL3 23

//----------ADC DEFINES-------------------

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

//------------SEVEN SEG DEFINES--------------

#define	CA7SEG_2_MUX 18 //p0.18 to p0.25
#define DSEL1 24 //P1.24
#define DSEL2 25//P1.25

//-------------RTC DEFINES-------------------

#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))
//CCR
#define RESET_RTC (1<<1)
#define ENABLE_RTC (1<<0)
//clock src is set to extranal rtc oscillator and enable the rtc
#define RTC_CLK_SRC (1<<4)|(1<<0) 

//------------LED'S AND BUZZER-------------

#define LED_GREEN 26 //P1.26
#define LED_YELLOW 27 //P1.27
#define LED_RED    28 //P1.28

#define BUZZER 29 //P1.29

