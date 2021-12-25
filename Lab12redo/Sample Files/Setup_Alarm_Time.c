#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "I2C_Support.h"
#include "Setup_Alarm_Time.h"
#include "Setup_Time.h"
#include "Main_Screen.h"
#include "Main.h"
#include "ST7735_TFT.h"
#include "utils.h"

extern char setup_alarm_time[]; 
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;
extern unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
extern char found;
extern char *txt;
extern short nec_ok;

char Select_Alarm_Field;  
char In_Alarm_Time_Setup;

void Do_Setup_Alarm_Time(void)
{
    Select_Alarm_Field = 0;
    In_Alarm_Time_Setup = 1;
    DS3231_Read_Alarm_Time();                   // Read alarm time
    alarm_second = bcd_2_dec(alarm_second);     // convert to dec for use
    alarm_minute = bcd_2_dec(alarm_minute);
    alarm_hour   = bcd_2_dec(alarm_hour);
    setup_alarm_second = alarm_second;          // Beginning setup alarm time as current set alarm time
    setup_alarm_minute = alarm_minute;
    setup_alarm_hour = alarm_hour;
    Initialize_Setup_Alarm_Time_Screen();       // Initialize the alarm setup screen
    Update_Setup_Alarm_Time_Screen();           // Update the alarm screen with latest info
    while (In_Alarm_Time_Setup == 1)
    {   
         if (check_for_button_input() == 1)
         {
             if (found >= Prev && found <= EQ)
             {
                Do_Beep_Good();
                if (found == Prev) Go_Prev_Alarm_Field();
                // add code here for the remaining buttons like in Setup_Time())
                found = 0xff;
             }
             else
             {
                 Do_Beep_Bad();
             }
        }                   
    }
}

void Increase_Alarm_Time()
{
            switch (Select_Alarm_Field)             // Increment the proper field
            {
                case 0:
                    setup_alarm_hour++;
                    if (setup_alarm_hour == 24) setup_alarm_hour = 0; 
                    break;

                case 1:
                    // add code for alarm_minute increment

                case 2:
                    // add code for alarm_second increment 

                default:
                    break;
            }    
            Update_Setup_Alarm_Time_Screen();   // Update alarm screen with latest info
                                                // Clear the increment flag
}

void Decrease_Alarm_Time()
{       
            switch (Select_Alarm_Field)               // Decrement the proper field
            {
                case 0:
                    if (setup_alarm_hour == 0) setup_alarm_hour = 23;
                    else --setup_alarm_hour;
                    break;

                case 1:
                    // add code for alarm_minute decrement

                case 2:
                    // add code for alarm_second decrement    

                default:
                break;
            }                
            Update_Setup_Alarm_Time_Screen();           // Update alarm screen with latest info
                         
} 

void Go_Next_Alarm_Field()
{
     
    Select_Alarm_Field++;                                   // Move to next field
    if (Select_Alarm_Field == 3) Select_Alarm_Field = 0;    // Wrap around if necessary
    Update_Setup_Screen_Cursor_Forward(Select_Alarm_Field); // Update cursor
                                     
}  

void Go_Prev_Alarm_Field()
{
    if (Select_Alarm_Field == 0) Select_Alarm_Field = 2;
    else Select_Alarm_Field--;                                // Move to next field
    Update_Setup_Screen_Cursor_Backward(Select_Alarm_Field);  // Update cursor
} 
          
void Exit_Setup_Alarm_Time()
{
    DS3231_Read_Time();                                     // Read time
    Initialize_Screen();                                    // Initialize the screen before returning  
    In_Alarm_Time_Setup = 0;
}

void Do_Save_New_Alarm_Time()
{
    DS3231_Write_Alarm_Time();                              // Write alarm time
    DS3231_Read_Alarm_Time();                               // Read alarm time
    DS3231_Read_Time();                                     // Read current time
    Initialize_Screen();                                    // Initialize main screen before returning
    In_Alarm_Time_Setup = 0;
}
     
void Initialize_Setup_Alarm_Time_Screen(void) 
{ 
    fillScreen(ST7735_BLACK);                               // Fills background of screen with color passed to it
 
    strcpy(txt, "ECE3301L F'21 Final");                     // Text displayed 
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);
                                                                // X and Y coordinates of where the text is to be displayed

    strcpy(txt, "  Alarm");
    drawtext(start_x+5 , start_y+10, txt, ST7735_MAGENTA, ST7735_BLACK, TS_2);
    strcpy(txt, "  Setup");                                  // Text displayed 
    drawtext(start_x+5 , start_y+25, txt, ST7735_MAGENTA, ST7735_BLACK, TS_2); 
    
    strcpy(txt, "Time");
    drawtext(time_x  , time_y , txt, ST7735_BLUE   , ST7735_BLACK, TS_1);
    fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_CYAN);
}

void Update_Setup_Alarm_Time_Screen(void)
{
    printf ("%x:%x:%x\r\n", setup_alarm_hour,setup_alarm_minute,setup_alarm_second);
    setup_alarm_time[0]  = (setup_alarm_hour/10)  + '0';
    setup_alarm_time[1]  = 0;// add code here 
    setup_alarm_time[3]  = 0;// add code here
    setup_alarm_time[4]  = 0;// add code here
    setup_alarm_time[6]  = 0;// add code here
    setup_alarm_time[7]  = 0;// add code here   // Put your code here
    drawtext(data_time_x, data_time_y, setup_alarm_time, ST7735_CYAN, ST7735_BLACK, TS_2);
}
 
