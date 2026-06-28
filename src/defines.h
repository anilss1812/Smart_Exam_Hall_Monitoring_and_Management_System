//defines.h
//MACROCS FOR BIT MANUPULATION
#include "types.h"

#define WRITEBIT(WORD,BIT) \
        WORD=(WORD &~ (1<<BIT) | (1<<BIT))

#define WRITEBYTE(WORD,VAL,STBIT) \
         WORD=(WORD &~ ((u32)0XFF)<<STBIT) | ((u32)VAL<<STBIT)

#define WRITENIBBLE(WORD,VAL,STBIT) \
        WORD=((WORD &~ (15<<STBIT)) | (VAL<<STBIT))

#define READBIT(WORD,BIT) \
        WORD = ((WORD>>BIT)&1)

#define READBYTE(WORD,STBIT) \
       WORD = ((WORD>>STBIT)&0XFF)

#define READNIBBLE(WORD,STBIT) \
       WORD = ((WORD>>STBIT)&15)

        

