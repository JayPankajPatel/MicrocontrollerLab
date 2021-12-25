
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Main.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"

extern char buffer[31];
extern char *nbr;
extern char *txt;
extern char tempC[];
extern char tempF[];
extern char time[];
extern char date[];
extern char alarm_time[];
extern char Alarm_SW_Txt[];
extern char Heater_Set_Temp_Txt[];
extern char Heater_SW_Txt[];
 
extern char DC_Txt[];
extern char Volt_Txt[];
extern char RPM_Txt[];
extern char RTC_ALARM_Txt[];
extern signed int DS1621_tempC, DS1621_tempF;
extern unsigned char second, minute, hour, dow, day, month, year;
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;
extern unsigned char heater_set_temp;
extern char duty_cycle;
extern int ALARMEN;
extern char HEATER;
extern int rpm;
extern float volt;

void Initialize_Screen(void) 
{ 
    LCD_Reset();                                    // Screen reset
    TFT_GreenTab_Initialize();         
    fillScreen(ST7735_BLACK);                       // Fills background of screen with color passed to it
 
    strcpy(txt, " ECE3301L F'21 Final\0");          // Text displayed 
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);   
                                                    // X and Y coordinates of where the text is to be displayed
    strcpy(txt, "Temperature:");
    drawtext(temp_x  , temp_y , txt, ST7735_MAGENTA, ST7735_BLACK, TS_1);                                                                                               // including text color and the background of it
    drawCircle(circ_x, circ_y , 2  , ST7735_YELLOW);
    strcpy(txt, "C/");
    drawtext(tempc_x , tempc_y, txt, ST7735_YELLOW , ST7735_BLACK, TS_2); 
    strcpy(txt, "F");         
    drawCircle(cirf_x, cirf_y , 2  , ST7735_YELLOW);
    drawtext(tempf_x , tempf_y, txt, ST7735_YELLOW , ST7735_BLACK, TS_2);
    strcpy(txt, "Time");
    drawtext(time_x  , time_y , txt, ST7735_BLUE   , ST7735_BLACK, TS_1);
    strcpy(txt, "Date");
    drawtext(date_x  , date_y , txt, ST7735_RED    , ST7735_BLACK, TS_1);
    strcpy(txt, "Alarm Time");
    drawtext(alarm_time_x  , alarm_time_y , txt, ST7735_YELLOW, ST7735_BLACK, TS_1); 
    strcpy(txt, "Alarm SW");
    drawtext(alarm_sw_x, alarm_sw_y, txt, ST7735_YELLOW, ST7735_BLACK, TS_1);    
    strcpy(txt, "HTR Set Temp");
    drawtext(heater_set_temp_x, heater_set_temp_y, txt, ST7735_BLUE  , ST7735_BLACK, TS_1); 
    strcpy(txt, "HTR SW");
    drawtext(heater_sw_x, heater_sw_y, txt, ST7735_BLUE  , ST7735_BLACK, TS_1);
    strcpy(txt, "DC");
    drawtext(dc_x, dc_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);
    strcpy(txt, "RM");
    drawtext(rtc_x, rtc_y, txt, ST7735_WHITE       , ST7735_BLACK  , TS_1);
    strcpy(txt, "Volt");
    drawtext(volt_x, volt_y, txt, ST7735_WHITE       , ST7735_BLACK  , TS_1);
    strcpy(txt, "RPM");
    drawtext(rpm_x, rpm_y, txt, ST7735_WHITE       , ST7735_BLACK  , TS_1); 
    
}


void Update_Screen(void)
{
    if ((DS1621_tempC & 0x80) == 0x80) 
    {
        tempC[0] = '-';
        char neg_DS1621_tempC = 0x80 - (DS1621_tempC & 0x7f);
        tempC[1]  = (neg_DS1621_tempC)/10  + '0';// Tens digit of C
        tempC[2]  = (neg_DS1621_tempC)%10  + '0';// Ones digit of C
    }
    else
    {
        tempC[0] = '+';
        tempC[1]  = DS1621_tempC/10  + '0';         // Tens digit of C
        tempC[2]  = DS1621_tempC%10  + '0';         // Ones digit of C
    }
    
    if (DS1621_tempF < 0) 
    {
        tempF[0] = '-';
        int neg_tempF = 0x80 - (DS1621_tempF & 0x7f);
        tempF[1]  = neg_tempF/10  + '0';            // Tens digit of C
        tempF[2]  = neg_tempF%10  + '0';            // Ones digit of C
    }
    else
    {
        tempF[0] = '+';
        tempF[1]  = DS1621_tempF/10  + '0';         // Tens digit of C
        tempF[2]  = DS1621_tempF%10  + '0';         // Ones digit of C
    }    
    
    time[0]  = (hour>>4)  + '0';                    // Hour MSD
    time[1]  = (hour & 0x0f)+ '0';                  // Hour LSD
    time[3]  = 0; // add code                   // Minute MSD
    time[4]  = 0; // add code               // Minute LSD
    time[6]  = 0; // add code                   // Second MSD
    time[7]  = 0; // add code               // Second LSD
    date[0]  = 0; // add code                   // Month MSD
    date[1]  = 0; // add code                // Month LSD
    date[3]  = 0; // add code                      // Day MSD
    date[4]  = 0; // add code                   // Day LSD
    date[6]  = 0; // add code                     // Year MSD
    date[7]  = 0; // add code                  // Year LSD
    alarm_time[0]  = (alarm_hour>>4)  + '0';        // Alarm Hour MSD
    alarm_time[1]  = (alarm_hour & 0x0f)+ '0';      // Alarm Hour LSD
    alarm_time[3]  = 0; // add code       // Alarm Minute MSD
    alarm_time[4]  = 0; // add code   // Alarm Minute LSD
    alarm_time[6]  = 0; // add code       // Alarm Second MSD
    alarm_time[7]  = 0; // add code    // Alarm Second LSD
    
    if (ALARMEN == 1) 
    {   
        strcpy(Alarm_SW_Txt, "ON ");
    }
    else 
    {
        strcpy(Alarm_SW_Txt, "OFF");
    }
 
    Heater_Set_Temp_Txt[0] = 0; // add code
    Heater_Set_Temp_Txt[1] = 0; // add code 
    Heater_Set_Temp_Txt[2] = 0; // add code 
    
    if (HEATER == 1)
    {
        strcpy(Heater_SW_Txt, "ON ");
    }
    else 
    {
        strcpy(Heater_SW_Txt, "OFF");
    }
    
    if (RTC_ALARM_NOT == 0) RTC_ALARM_Txt[0] = '1';
    else RTC_ALARM_Txt[0] = '0';
       
    
    DC_Txt[0] = 0; // add code
    DC_Txt[1] = 0; // add code 
    DC_Txt[2] = 0; // add code     
    
    Volt_Txt[0] = ((int)  volt        ) + '0';
    Volt_Txt[2] = 0; // add code 
    Volt_Txt[3] = 0; // add code 
    
    RPM_Txt[0] = 0; // add code
    RPM_Txt[1] = 0; // add code   
    RPM_Txt[2] = 0; // add code 
    RPM_Txt[3] = 0; // add code          
    
    drawtext(data_tempc_x, data_tempc_y, tempC , ST7735_YELLOW , ST7735_BLACK , TS_2);       
    drawtext(data_tempf_x, data_tempf_y, tempF , ST7735_YELLOW , ST7735_BLACK , TS_2);
    drawtext(data_time_x , data_time_y , time  , ST7735_CYAN   , ST7735_BLACK , TS_2);
    drawtext(data_date_x , data_date_y , date  , ST7735_GREEN  , ST7735_BLACK , TS_2);
    drawtext(data_alarm_time_x , data_alarm_time_y , alarm_time  , ST7735_CYAN   , ST7735_BLACK , TS_1); 
    drawtext(data_alarm_sw_x , data_alarm_sw_y , Alarm_SW_Txt  , ST7735_CYAN   , ST7735_BLACK , TS_1); 
    drawtext(data_heater_set_temp_x, data_heater_set_temp_y ,Heater_Set_Temp_Txt, ST7735_RED, ST7735_BLACK, TS_1);
    drawtext(data_heater_sw_x, data_heater_sw_y, Heater_SW_Txt, ST7735_RED    , ST7735_BLACK , TS_1);    
    drawtext(data_dc_x, data_dc_y, DC_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);
    drawtext(data_rtc_x, data_rtc_y, RTC_ALARM_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);    
    drawtext(data_volt_x, data_volt_y, Volt_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);    
    drawtext(data_rpm_x, data_rpm_y, RPM_Txt, ST7735_GREEN, ST7735_BLACK, TS_1);
}


