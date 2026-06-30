//mini_project_main.c
#include <LPC21xx.h>
#include "mini_pro_defines.h"
#include "headers.h"

//------------------GLOBALAS---------------------

//for rtc time

vu32 hour,min,sec,dom,mon,year,day;

//to update values for every sec 

u32 last_sec = 99; //any damy value which is not possible ,sec are 0 to 60

//-----------cfg_MODE_var's----------------------

u8 choice,sub_rtc_menu,ex_time_submenu;

//setting password as 1234

u32 password = 1234;

//for entered password

u32 given_pass;

//for exiting from cfg mode
u8 hole_exit;

u32 ex_time_submenu_run;

u8 check = 0; //for checking max range for time and day 

u8 main_menu;

//-------------------------------------------------------------

//----------------EXAM RUN VAR'S----------------------------------

//for exam start time and end time

u8 ex_start_hour = 0;  //variable for exam starting hour
u8 ex_start_min = 0;		//variable for exam minitue 
u8 ex_duration_min = 0;  //exam duration for entered by user

u8 remaining_time =0;  //remaining time of exam 

vu8 paused =0;	//for interrupt 2 is time paused or resume

u8 sec_cnt = 0; //for the second count for knowing the one minitue

//-----------------------------------------------------------------
//enum for system state variables for switching the cases 
typedef enum
{
	idle,
	cfg,
	ex_runing,
	ex_end
}sys_state;

//starting state 

volatile sys_state current_state = idle;

//var for temp values

f32 temp;

//---------------------------------------------------------------

void default_screen(void)
{
	cmdLcd(CLEAR_LCD);
  	strLcd("TIME: ");

	cmdLcd(GOTO_LINE2_POS0);
	strLcd("DATE: ");

	cmdLcd(GOTO_LINE3_POS0);
 	strLcd("TEMP: ");

}

int main()
{
	//for leds and buzzer as output and off condition
	IODIR1 |= (1<<LED_GREEN)|(1<<LED_YELLOW)|(1<<LED_RED)|(1<<BUZZER);
	IOSET1 = (1<<LED_GREEN)|(1<<LED_YELLOW)|(1<<LED_RED);
	IOCLR1 =1<<BUZZER;

	//initialition
		rtc_Init();
		seg_Init();
  lcd_Init();
		adc_Init();
		kpm_Init();
		timer_Init();
  enable_Eint0();
 	enable_Eint1();
	//default time 10:10:00
		setRtcTime(10,10,0);

	//after turn on display condition
   cmdLcd(CLEAR_LCD);         //clearing the display 
			cmdLcd(GOTO_LINE1_POS0+5); 
   strLcd("EXAM HALL");
			cmdLcd(GOTO_LINE2_POS0+2);
   strLcd("MONITORING SYSTEM");
			cmdLcd(GOTO_LINE3_POS0+4);
   strLcd("BY V25HE9T1");
			cmdLcd(GOTO_LINE4_POS0+5);
			strLcd("LOADING...");
			delay_ms(1000);
			default_screen();  //function call for default screen (time,date,temperature)

  while(1)     //infinite loop
		{

		//taking rtc values 
		getRtcTime(&hour,&min,&sec);
		getRtcDate(&dom,&mon,&year);
		//checks the state is in exam runinig then the seted duration value is assigned with 7segment value	
		if(current_state == ex_runing)
		{
			seg_value = remaining_time;   // ISR handles the actual display refresh automatically
		}
		else
		{
			seg_value = 0;		//in idle time shows zero on 7segment
		}
		
		//take switch cases for different oparations or stages
		switch(current_state)
		{

					case idle :
					{
							if(sec != last_sec)
							{
								last_sec = sec;
								//what we have to update for every sec here
								//1.display rtc time
								dispRtcTime(0x86);

								cmdLcd(GOTO_LINE2_POS0);

								dispRtcDate(0xC6);

								//3.disp temp
								temp=read_Lm35Degc();
								cmdLcd(GOTO_LINE3_POS0+5);
								F32Lcd(temp,2);
								charLcd(0xDF);
								charLcd('C');
								if(ex_time_submenu_run==0)
								{
									//check exam start time

									if((hour == ex_start_hour) && (min == ex_start_min) && (sec == 0))
									{
										paused = 0;                         //for there is no switch press
										remaining_time = ex_duration_min;   //duration minitues is assigned with remaining time variable
										current_state = ex_runing;          //changing current state to exam runing state
									}
								}
							}
							if(menu_flag)
							{
								// change interrupt flag to default
								menu_flag = 0;
								//switch to configuration state from current state
								current_state = cfg;
							}
				break;
			}
//--------------------------------------------------------------------
			case cfg :
			{
				ex_time_submenu_run = 1; 
			 	main_menu =0;
				//clear the lcd
			  cmdLcd(CLEAR_LCD);
				//disp menu mode
				cmdLcd(GOTO_LINE2_POS0+5); //lcd line 2 pos 3
				strLcd("MENU MODE"); //dsp menu mode
				cmdLcd(GOTO_LINE3_POS0); //lcd line 3 pos 0
				strLcd("Password:");
				//scaning the password from keypad 
				given_pass = passInput(4,&hole_exit);
				
				//check the condition if its returns 0 then it means exit to idle
				if(!(given_pass))
				{
					cmdLcd(CLEAR_LCD);
					current_state = idle;
					break;
				}
				
				//if given password and setted password is wrong or incorrect shows the wrong password and exit to idle
				else if(password != given_pass)
				{
				  cmdLcd(CLEAR_LCD); //clears the lcd
				  cmdLcd(GOTO_LINE3_POS0+3); //lcd line 1 pos 1
					strLcd("WRONG PASSWORD"); //dsp wrong password
					delay_ms(1000);          // delay for 1 sec
					current_state = idle;    //jumps to idle		
					default_screen();	//for showing the lcd idle default screen 
				}
				
				//if password is correct shows menu options
				else
				{
				 main_menu = 1;
					//password sucess msg 
				  cmdLcd(CLEAR_LCD);          //clears the lcd
				  cmdLcd(GOTO_LINE2_POS0+3); //lcd line 1 pos 1
					strLcd("AUTHENTICATION");       //dsp password msg
				  cmdLcd(GOTO_LINE3_POS0+5); //lcd line 2 position 3
					strLcd("SUCESSFUL");       //dsp MENU option 1
					delay_ms(500); 
					
					while(main_menu)
					{
						  //display the menu					
						main:cmdLcd(CLEAR_LCD);          		//clears the lcd
							strLcd("1.RTC_EDIT");      		 //dsp MENU option 1
							cmdLcd(GOTO_LINE2_POS0); 					//lcd line 2 position 0
							strLcd("2.SET_EXAM_TIME");    //dsp MENU option 2
							cmdLcd(GOTO_LINE3_POS0); 					//lcd line 3 postion 0
							strLcd("3.EDIT_PASSWORD");    //dsp MENU option 3
							cmdLcd(GOTO_LINE4_POS0); 					//lcd line 4 postion 0
							strLcd("4.EXIT");            //dsp MENU option 4
							choice = menu_sel();
						
						if(choice == 1)
						{
							u8 rtc_submenu_run = 1;
							
							while(rtc_submenu_run == 1)
							{
						  	cmdLcd(CLEAR_LCD);          //clears the lcd
						  	strLcd("1.EDIT TIME");           //dsp MENU option 1
								 cmdLcd(GOTO_LINE2_POS0);  //lcd line 1 postion 11
								 strLcd("2.EDIT DATE");        //dsp MENU option 2
								 cmdLcd(GOTO_LINE3_POS0);    //lcd line 2 position 0
								  strLcd("3.EXIT");            //dsp MENU option 3
								sub_rtc_menu = menu_sel();
								
								if(sub_rtc_menu == 1)
								{
									h:cmdLcd(CLEAR_LCD);          //clears the lcd
						    	strLcd("HOUR(23):");        //dsp MENU option 2
									  check  = keyInput(2,&hole_exit);
									
									if(check <=23)
									{
										hour = check;
									}
									else
									{
									 cmdLcd(CLEAR_LCD);
									 cmdLcd(GOTO_LINE3_POS0+3);
									 strLcd("INVALID INPUT");
									 delay_ms(1500);
									 goto h;
									}

									mi:cmdLcd(GOTO_LINE2_POS0);
									   strLcd("MINUTES(59):");        //dsp MENU option 2									
									   check  = keyInput(2,&hole_exit);
									if(check <=59)
									{
										min = check;
									}
									else
									{
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("INVALID INPUT");
									delay_ms(1500);
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("                 ");
									cmdLcd(GOTO_LINE2_POS0);
									strLcd("                 "); 
									goto mi;
									}
									setRtcTime(hour,min,0); // its sets the rtc time
								}
								
								else if(sub_rtc_menu == 2)
								{
								  	d:cmdLcd(CLEAR_LCD);          //clears the lcd
	   	   		strLcd("DAY:");        //dsp MENU option day
									check = keyInput(2,&hole_exit);// taking input from keypad
									if(check >= 1 && check <=31)
									{
										dom = check;
									}
									else
									{
									cmdLcd(CLEAR_LCD);
									cmdLcd(GOTO_LINE2_POS0+3);
									strLcd("INVALID INPUT");
									delay_ms(1500);
									goto d;
									}
									mo:cmdLcd(GOTO_LINE2_POS0);
									strLcd("MONTH:");        //dsp MENU option 2									
									check  = keyInput(2,&hole_exit);
									if(check >= 1 && check <=12)
									{
										mon = check;
									}
									else
									{
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("INVALID INPUT");
									delay_ms(1500);
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("                 ");
									cmdLcd(GOTO_LINE2_POS0);
									strLcd("                 ");
									goto mo;
									}
									cmdLcd(GOTO_LINE3_POS0);
									strLcd("YEAR:");        //dsp MENU option 2									
									year  = keyInput(4,&hole_exit);
									setRtcDate(dom,mon,year); // its sets the rtc time
								}
								
								else if(sub_rtc_menu==3)
								{
									rtc_submenu_run = 0;
								}
							}
						}
						
						else if(choice == 2)
						{
							ex_time_submenu_run = 1;

							while(ex_time_submenu_run == 1)
							{	
								cmdLcd(CLEAR_LCD);              //clears the lcd
								strLcd("1.EXAM_START_TIME");      //dsp SUBMENU option 1
								cmdLcd(GOTO_LINE2_POS0);        //lcd line 2 postion 0
								strLcd("2.EXAM_DURATION");           //dsp SUBMENU option 2
								cmdLcd(GOTO_LINE3_POS0 );     //lcd line 3 position 0
								strLcd("3.EXIT");                //dsp MENU option 2
								ex_time_submenu = menu_sel();					
								
								if(ex_time_submenu == 1)
								{
									xh:cmdLcd(CLEAR_LCD);          //clears the lcd
									strLcd("EXAM_START_HR(23):");     //dsp SUBMENU option 1
									check  = keyInput(2,&hole_exit);
									if(check <=23)
									{
										ex_start_hour = check;
									}
									else
									{
									cmdLcd(CLEAR_LCD);
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("INVALID INPUT");
									delay_ms(1500);
									goto xh;
									}
									xmi:cmdLcd(GOTO_LINE2_POS0);    //lcd line 2 postion 0
									strLcd("EXAM_START_MIN(59):");     // for exam in string
									check  = keyInput(2,&hole_exit);
									if(check <=59)
									{
										ex_start_min = check;
									}
									else
									{
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("INVALID INPUT");
									delay_ms(1500);
									cmdLcd(GOTO_LINE3_POS0+3);
									strLcd("                 ");
									cmdLcd(GOTO_LINE2_POS0);
									strLcd("                 ");
									goto xmi;
									}

								}
								else if(ex_time_submenu == 2)
								{
									cmdLcd(CLEAR_LCD);          //clears the lcd
									strLcd("DURATION(MIN):");        //dsp SUBMENU option 1
									ex_duration_min  = keyInput(2,&hole_exit);								
  						}
								else if(ex_time_submenu ==3)
								{
									ex_time_submenu_run = 0;
								}
							}
						}
						else if(choice ==3)
						{
							u8 cnt=0;
							 	pass:cmdLcd(CLEAR_LCD);          //clears the lcd
									strLcd("Curr Password:");
									//scaning the password from keypad 
									given_pass = passInput(4,&hole_exit);
									if(password == given_pass)
									{
										cmdLcd(CLEAR_LCD);
										strLcd("NEW PASSWORD:");
										password = keyInput(4,&hole_exit);
										cmdLcd(CLEAR_LCD);          //clears the lcd
						 			cmdLcd(GOTO_LINE3_POS0+2);										
										strLcd("PASSWORD CHANGED");
										delay_ms(1000);
										goto main;
									}
									else
									{
										cnt++;
										cmdLcd(CLEAR_LCD);          //clears the lcd
						 			cmdLcd(GOTO_LINE2_POS0+3);										
										strLcd("ACCESS DENINED");			//IF WRONG PASSWORD 
						 			cmdLcd(GOTO_LINE3_POS0+5);										
										strLcd("TRY AGAIN");		     	//IF WRONG PASSWORD 
										delay_ms(1000);	
										if(cnt >= 3)
									 {
											cmdLcd(CLEAR_LCD);          //clears the lcd
											cmdLcd(GOTO_LINE3_POS0+3);										
											strLcd("SO MANY TIMES");			//IF WRONG PASSWORD 
											delay_ms(1000);
										// exit menu after 3 wrong attempts
										goto main;
								 	}	
										goto pass;										
									}
																	
						}
						else if(choice == 4)
					 {
									current_state = idle;
									main_menu = 0;
									menu_flag = 0;
									default_screen();	          //for showing the lcd idle default screen 
						}
				}
				current_state = idle;
				}
			break;
			}

 //--------------------------------------------------------------------------------------------------------------

			case ex_runing :
			{
				if(pause_flag)
				{
					pause_flag = 0 ;//make it as defalut
					//toggles the paused variable to know the pause and resume condition 0->resume,paused->1
					paused = !paused;
					//LCD UPDATES
					cmdLcd(GOTO_LINE2_POS0);
					if(paused)
					{
						//dsp paused

					 //lcd line 2 position 0
						strLcd("       PAUSED");           //dsp MENU option 2
						delay_ms(1000);
					pause_flag = 0 ;//make it as defalut			
					}
					else
					{
						strLcd("       RESUMED");
						delay_ms(1000);  
						pause_flag = 0 ;//make it as defalut
						cmdLcd(GOTO_LINE2_POS0);
						strLcd("REMAINING:");           //dsp remaining time
						U32Lcd(remaining_time);   //shows remaining time on dsp
						strLcd("          ");
						last_sec = sec; 
					}
				}
			  if(sec != last_sec)
     {
    			 last_sec = sec;

							dispRtcTime(0x86);
							temp=read_Lm35Degc();
			   	cmdLcd(GOTO_LINE3_POS0+5);
				  	F32Lcd(temp,2);
			  		charLcd(0xDF);
			  		charLcd('C');
					if(paused == 0)
					{
            		sec_cnt++;
            	if(sec_cnt >= 60)
            {
                sec_cnt = 0;
                remaining_time--;
            }
            if(remaining_time == 0)
            {
                current_state = ex_end;
            }
						cmdLcd(GOTO_LINE2_POS0);
						strLcd("REMAINING:");           //dsp remaining time
						U32Lcd(remaining_time);   //shows remaining time on dsp
						strLcd("        ");
					}
				//LED STATUS INDICATION
				if(remaining_time >= 3)
				{

					IOCLR1 = 1<<LED_GREEN;
				 IOSET1 = (1<<LED_YELLOW)|(1<<LED_RED);
				}
				else if(remaining_time >= 1)
				{
					IOCLR1 = 1<<LED_YELLOW;
		   IOSET1 = (1<<LED_GREEN)|(1<<LED_RED);
				}
				else
				{
					IOCLR1 = 1<<LED_RED;
		   IOSET1 = (1<<LED_GREEN)|(1<<LED_YELLOW);
				}
			}		
			break;
			}
	//-------------------------------------------------------------------------------------------
			case ex_end:
   {
    static u8 end_shown = 0;
    
    if(!end_shown)
    {
					// RED ON (active low)
        IOCLR1 = (1<<LED_RED);      
					// GREEN/YELLOW OFF
        IOSET1 = (1<<LED_GREEN)|(1<<LED_YELLOW);
					// BUZZER ON (active HIGH)
        IOSET1 |= (1<<BUZZER);                        
        
        cmdLcd(CLEAR_LCD);
        cmdLcd(GOTO_LINE2_POS0+5);
        strLcd("EXAM OVER");
        cmdLcd(GOTO_LINE3_POS0+4);
        strLcd("TIME IS UP");
        
        delay_ms(5000);
					// BUZZER OFF
        IOCLR1 |= (1<<BUZZER);                        
        end_shown = 1;
    }
    
    if(menu_flag)
    {
        menu_flag = 0;
        end_shown = 0;
				 	// RED OFF
        IOSET1 |= (1<<LED_RED);                       
        last_sec = 99;
        default_screen();
        current_state = idle;
    }
    break;
   }		
			default :
			{
				current_state = idle;
				break;
			}	
		}
 } 
}



