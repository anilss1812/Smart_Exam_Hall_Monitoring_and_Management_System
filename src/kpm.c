//kpm.c

#include "types.h"
#include <lpc21xx.h>
#include "mini_pro_defines.h"
#include "headers.h"


u8 KpmLUT[4][4] =	{{'1','2','3','A'},

			             		{'4','5','6','B'},

			              	{'7','8','9','C'},

				             	{'c','0','=','D'}};


void kpm_Init(void)
{

	//cfg rows (p1.16-p1.19) as o/p

	IODIR1 |= 15<<ROW0;

}


u32 Colscan(void)
{

	if(((IOPIN1>>COL0)&15)<15)
	{

	  return 0;

	}

	else 
	{

	  return 1;

  }

}


u32 RowCheck(void)
{

	u32 rno;

	for(rno=0;rno<4;rno++)
	{

		IOPIN1=(IOPIN1 &~ (0xFF<<ROW0))|((~(1<<rno))<<ROW0);

		if(Colscan()==0)
		{

			break;

		}
	}
	
	//make row as defaults
	IOCLR1 = 15<<ROW0;
	
	return rno;
}

u32 ColCheck(void)
{

		u32 cno;

		for(cno=0;cno<4;cno++)
	{

		if(((IOPIN1>>(COL0+cno))&1)==0)

			break;
	}

	return cno;
}

u32 keyScan(void)
{

	u32 rno,cno,key;

	//wait for switch press
	while(Colscan());

	//find rno
	rno=RowCheck();

	//find cno
	cno = ColCheck();

	//get key value from KPMLUT[][]
	key = KpmLUT[rno][cno];

	//wait for switch release
	while(!Colscan());

	//return keyvalue
	return key;

}

u32 ReadNum(u8 *endop)
{
	u8 key;
	u32 sum=0;
	
	while(1)
	{
		key = keyScan();
		delay_ms(100);
		
		if(key>='0' && key<='9')
		{
			sum = (sum*10)+(key-'0');//'0' = 48
		}
		
	else
	{
		*endop = key;
		break;
	}
  }
	return sum;
}

u32 keyInput(u8 nodigit,u8 *cancle)
{
	u32 value=0;
	u8 cnt=0;
	u32 key;
	*cancle =0;
	while(cnt <= nodigit) //it iterates upto number of digits you want to enter
	{

		//waiting for key scan
		key = keyScan();
	//	delay_ms(200);
		
		//here c is for exit 
		if(key == 'c')
		{
			*cancle = 1;
			return 0;
		}
		//here its for delete the value like backspace -
		else if(key == 'D')
		{
			if(cnt>0)
			{
				cnt--;
				value = value/10; //removes the last entered value

				//move lcd cur left and erase char in lcd
				cmdLcd(MOV_CUR_LEFT);
				charLcd(' ');
				cmdLcd(MOV_CUR_LEFT);
			}
		}
		
		//stores the entered values in one variable
		else if(cnt < nodigit)
		{
			if(key >= '0' && key <='9')
			{
			value = (value*10) + (key - '0');
			 
			cnt++;

			charLcd(key);
			}
		 }

	else if(key == '=')
	{
		break ;
	}

	}
	return value;
}


u32 passInput(u8 nodigit,u8 *cancle)
{
	u32 value=0;
	u8 cnt=0;
	u32 key;
	*cancle =0;
	while(cnt <= nodigit) //it iterates upto number of digits you want to enter
	{

		//waiting for key scan
		key = keyScan();
	//	delay_ms(200);
		
		//here c is for exit 
		if(key == 'c')
		{
			*cancle = 1;
			return 0;
		}
		//here its for delete the value like backspace -
		else if(key == 'D')
		{
			if(cnt>0)
			{
				cnt--;
				value = value/10; //removes the last entered value

				//move lcd cur left and erase char in lcd
				cmdLcd(MOV_CUR_LEFT);
				charLcd(' ');
				cmdLcd(MOV_CUR_LEFT);
			}
		}
		
		//stores the entered values in one variable
		else if(cnt < nodigit)
		{
			if(key >= '0' && key <='9')
			{
			value = (value*10) + (key - '0');
			 
			cnt++;

			charLcd('*');
			}
		 }

	else if(key == '=')
	{
		break ;
	}

	}
	return value;
}


u32 menu_sel(void) //it returns key value 1,2,3,4..
{

	u32 key;

	do
	{
	key = keyScan(); //scans the input from keypad and stores in the key variables

	delay_ms(100);

	}while(key < '0' || key >'9'); //accepts only digit values

	return key-'0';
}
