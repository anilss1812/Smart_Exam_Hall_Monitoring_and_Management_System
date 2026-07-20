//interrupt.h

#include "types.h"

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

extern vu8 menu_flag;
extern vu8 pause_flag;

void enable_Eint0(void);
void enable_Eint1(void);

#endif
