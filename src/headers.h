//headers.h
#ifndef _HEADERS_H
#define _HEADERS_H

#include "types.h"

//-------------lcd.h---------------------------
#include "types.h"
void writeLcd(u8 byte);
void cmdLcd(u8 opcode);
void lcd_Init(void);
void charLcd(u8 ascii);
void strLcd(s8 *str);
void U32Lcd(u32 num);
void S32Lcd(s32 num);
void F32Lcd(f32 fnum,u32 ndp);
void buildCgram(s8 *p,u32 nbytes);

//--------------------kpm.h---------------------

void kpm_Init(void);
u32 Colscan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 keyScan(void);
u32 ReadNum(u8*);
u32 keyInput(u8 nodigit,u8 *cancle);
u32 passInput(u8 nodigit,u8 *cancle);
u32 menu_sel(void);


//-----------seg.h-------------------------------

extern volatile u8 seg_value;
void seg_Init(void);
void disp_2_mux_seg(u8 num);
void refreshSeg(void);


//--------------lm35.h-----------------------------
#include "types.h"
f32 read_Lm35Degc(void);
f32 read_Lm35Degf(void);
void read_Lm35NT(f32 *tdegC,f32 *tdegF);


//----------------------delay.h---------------------
#include "types.h"
void delay_us(u32);
void delay_ms(u32);
void delay_s(u32);

//---------------------rtc.h--------------------------

void rtc_Init(void);
void setRtcTime(vu8 hour,vu8 min,vu8 sec);
void setRtcDate(u8 day,u8 mon,u32 year);
void setRtcDay(s32 day);
void getRtcTime(vu32* hour,vu32 *min,vu32* sec);
void getRtcDate(vu32* dom,vu32* mon,vu32* year);
void getRtcDay(s8* day);
void dispRtcTime(u8);
void dispRtcDate(u8);
void dispRtcDow(u8);

//--------------------interrupt.h-------------------

extern vu8 menu_flag;
extern vu8 pause_flag;

void enable_Eint0(void);
void enable_Eint1(void);

//---------------------adc.h-----------------------------

void adc_Init(void);
void Read_acd(u32 CHNO,u32* adcDval,f32* ear);

//--------------------timer-------------------------
void timer_Init(void);


#endif
