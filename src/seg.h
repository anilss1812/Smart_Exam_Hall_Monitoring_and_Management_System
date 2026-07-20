//seg.h

#include "types.h"

extern volatile u8 seg_value;
void seg_Init(void);
void disp_2_mux_seg(u8 num);
void refreshSeg(void);


