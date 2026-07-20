//kpm.h
#include "types.h"

void kpm_Init(void);
u32 Colscan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 keyScan(void);
u32 ReadNum(u8*);
u32 keyInput(u8 nodigit,u8 *cancle);
u32 passInput(u8 nodigit,u8 *cancle);
u32 menu_sel(void);
