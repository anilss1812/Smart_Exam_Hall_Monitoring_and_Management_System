//lm35.c
#include "types.h"
#include "headers.h"
#include "mini_pro_defines.h"

f32 read_Lm35Degc(void)
{
	u32 dval;
	f32 ear;
	Read_acd(CH1,&dval,&ear);
	return (ear*100);
}

f32 read_Lm35Degf(void)
{
	f32 tempc;
	tempc = read_Lm35Degc();
	return ((tempc*(1.8))+32);
}
//for negative temperature

void read_Lm35NT(f32 *tdegC,f32 *tdegF)
{
	u32 dval1,dval2;
	f32 ear1,ear2;
	Read_acd(CH0,&dval1,&ear1);
	Read_acd(CH0,&dval2,&ear2);
	*tdegC=((ear1-ear2)*100);
	*tdegF=((*tdegC)*(1.8))+32;
}

