//rtc.c
#include <lpc21xx.h>
#include "mini_pro_defines.h"
#include "types.h"
#include "headers.h"

s8 week[][4] = {"SUN","MON","TUS","WED","THR","FRI","SAT"};
extern vu32 hour,min,sec,dom,mon,year,day;

void rtc_Init(void)
{
	
	  CCR = RESET_RTC;  //RESET 
		#if (CPU == LPC2129)
	  // Set prescaler integer and fractional parts
		PREINT = PREINT_VAL;
		PREFRAC = PREFRAC_VAL;
	  //ENABLE CTC
		CCR = ENABLE_RTC;  
	
		#elif (CPU == LPC2148)
	  // Enable the RTC with external clock source
		CCR = RTC_CLK_SRC;
	  #endif
}

void setRtcTime(vu8 hour,vu8 min,vu8 sec)
{
	HOUR = hour;
	MIN = min;
	SEC = sec;
}

void setRtcDate(u8 day,u8 mon,u32 year)
{
	DOM = day;
	MONTH = mon;
	YEAR = year;
}

void setRtcDay(s32 day)
{
	DOW = day;
}


void getRtcTime(vu32* hour,vu32* min,vu32* sec)
{
	*hour = HOUR;
	*min = MIN;
	*sec = SEC;
}

void getRtcDate(vu32* dom,vu32* mon,vu32* year)
{
	*dom = DOM;
	*mon = MONTH;
	*year = YEAR;
}

void getRtcDay(s8* day)
{
	*day = DOW;
}

void dispRtcTime(u8 lcdAdd)
{
	  getRtcTime(&hour,&min,&sec);
	  cmdLcd(lcdAdd);
		//HH:MM:SS
		charLcd(hour/10+'0');
		charLcd(hour%10+'0');
		strLcd(":");
		charLcd(min/10+'0');
		charLcd(min%10+'0');
		strLcd(":");
    charLcd(sec/10+'0');
		charLcd(sec%10+'0');
}
	
void dispRtcDate(u8 lcdAdd)
{
	  //DOM/MONTH/YEAR
	  cmdLcd(lcdAdd);  
		charLcd(DOM/10+'0');
		charLcd(DOM%10+'0');
		charLcd('/');
		charLcd(MONTH/10+'0');
		charLcd(MONTH%10+'0');
		charLcd('/');
		U32Lcd(YEAR);
}
void dispRtcDow(u8 lcdAdd)
{
	cmdLcd(lcdAdd);
	strLcd("- ");
  strLcd(week[day]);

}
