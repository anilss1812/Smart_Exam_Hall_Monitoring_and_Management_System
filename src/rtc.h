//rtc.h

#include "types.h"
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







