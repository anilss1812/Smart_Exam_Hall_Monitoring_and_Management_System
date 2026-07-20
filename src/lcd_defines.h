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


