#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#include "I2C.h"
#include "I2C_Support.h"
#include "Interrupt.h"
#include "Fan_Support.h"
#include "Main.h"
#include "ST7735_TFT.h"


#pragma config OSC = INTIO67
#pragma config BOREN =OFF
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config CCP2MX = PORTBE

void Initialize_Screen();
                                                    // I2C register addresses for the DS3231 start
char second = 0x00;                                 // I2C register address for second
char minute = 0x01;                                 // I2C register address for minute
char hour = 0x02;                                   // I2C register address for hour
char dow = 0x03;                                    // I2C register address for day of week
char day = 0x04;                                    // I2C register address for day
char month = 0x05;                                  // I2C register address for month  
char year = 0x06;                                   // I2C register address for year
                                                    // I2C register addresses for the DS3231 end

char found;
char tempSecond = 0xff; 
signed int DS1621_tempC, DS1621_tempF;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
unsigned char setup_fan_temp = 75;
unsigned char Nec_state = 0;

short nec_ok = 0;
char Nec_code1;
char FAN;
char duty_cycle;
int rps;
int rpm;
int ALARMEN;

char buffer[33]     = " ECE3301L Sp'21 L12\0";     // This block focuses on the data that will be displayed on the LCD
char *nbr;
char *txt;
char tempC[]        = "+25";
char tempF[]        = "+77";
char time[]         = "00:00:00";
char date[]         = "00/00/00";
char alarm_time[]   = "00:00:00";
char Alarm_SW_Txt[] = "OFF";
char Fan_SW_Txt[]   = "OFF";                        // text storage for Heater Mode

char array1[21]=                                    // IR codes
{
    0xA2,0x62,0xE2,
    0x22,0x02,0xC2,
    0xE0,0xA8,0x90,
    0x68,0x98,0xB0,
    0x30,0x18,0x7A,
    0x10,0x38,0x5A,
    0x42,0x4A,0x52,
};

//char txt1[21][4] =                                  // Text to display on LCD corresponding to button press      
//{
//    "CH-\0","CH\0","CH+\0",
//    
//    "|<<\0",">>|\0",">||\0",
//    
//    " - \0"," + \0","EQ \0",
//    
//    " 0 \0","100\0","200\0",
//    
//    " 1 \0"," 2 \0"," 3 \0",
//    
//    " 4 \0"," 5 \0"," 6 \0",
//    
//    " 7 \0"," 8 \0"," 9 \0"
//};

//int color[21]=                                      // Color of LCD circle that is displayed corresponding to button press
//{
//    RD,RD,RD,
//    BU,BU,GR,
//    MA,MA,MA,
//    BK,BK,BK,
//    BK,BK,BK,
//    BK,BK,BK,
//    BK,BK,BK,
//};

    
char DC_Txt[]       = "000";                // text storage for Duty Cycle value
char RTC_ALARM_Txt[]= "0";                  //
char RPM_Txt[]      = "0000";               // text storage for RPM

char setup_time[]       = "00:00:00";
char setup_date[]       = "01/01/00";
char setup_alarm_time[] = "00:00:00"; 
char setup_fan_text[]   = "075F";


void putch (char c)
{   
    while (!TRMT);       
    TXREG = c;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}

void Do_Init()                      // Initialize the ports 
{ 
    init_UART();                    // Initialize the uart
    OSCCON=0x70;                    // Set oscillator to 8 MHz 
    
    ADCON1=0x0F;                    // In this lab we are only working with digital signals
    TRISA = 0x00;
    TRISB = 0x01;                   // Our only input is the IR receiver, everything is an output
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0x00;


    RBPU=0;                         // Turn on the weak internal pull-up located in PORTB ports
    TMR3L = 0x00;                   // Timer values for PWM
    T3CON = 0x03;                   // Timer values for PWM
    I2C_Init(100000); 

    DS1621_Init();
    init_INTERRUPT();
    FAN = 0;                        
}

void main() 
{
    Do_Init();                       // Initialization  
    Initialize_Screen();
    FAN = 1;
    FAN_EN = 1;
    FAN_LED = 1;
    FAN_PWM = 1;
    duty_cycle = 50;
    do_update_pwm(duty_cycle);
    
//    while (1)
//    {
//        DS3231_Read_Time();
//        Set_RPM_RGB(rpm);
//
//        if(tempSecond != second)
//        {   
//            rpm = get_RPM();
//            tempSecond = second;
//            DS1621_tempC = DS1621_Read_Temp();
//            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;
//            
//            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
//            printf (" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);
//            printf ("RPM = %d  dc = %d\r\n", rpm, duty_cycle);
//        }
//    }
    
//    FAN_EN = 1;
//    duty_cycle = 50;
//    do_update_pwm(duty_cycle);
//        DS3231_Read_Time();
//
//        if(tempSecond != second)
//        {
//            tempSecond = second;
//            DS1621_tempC = DS1621_Read_Temp();
//            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;
//            rpm = get_RPM();
//            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
//            printf (" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);
//            printf ("RPM = %d  dc = %d\r\n", rpm, duty_cycle);
//        }    
//    
//    FAN_EN = 1;
//    duty_cycle = 50;    
//    do_update_pwm(duty_cycle);
//    while (1)
//    {
//        DS3231_Read_Time();
//
//        if(tempSecond != second)
//        {
//            tempSecond = second;
//            DS1621_tempC = DS1621_Read_Temp();
//            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;
//            rpm = get_RPM();
//            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
//            printf (" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);
//            printf ("RPM = %d  dc = %d\r\n", rpm, duty_cycle);
//        }
//    }
    
    while (1)
    {

        DS3231_Read_Time();
        Set_RPM_RGB(rpm);
        Set_DC_RGB(duty_cycle);
        Set_RPM_RGB(rpm);
        if(tempSecond != second)                                 // Check if the time has changed
        {
            tempSecond = second;                                 // If it has, change the time to the current time                                
            DS1621_tempC = DS1621_Read_Temp();                   // Also, read the temperature and calculate the temperature in F
            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;
            rpm = get_RPM();                                     // Run the get_RPM routine in order to save the value of rpm every second
            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
            printf (" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);
            printf ("RPM = %d  dc = %d\r\n", rpm, duty_cycle);
            Update_Screen();                                    // Finally, Update the screen values with the date,time,rpm and duty cycle
        }
        
       if (nec_ok == 1)                                         // If a button press has occured
        {
            nec_ok = 0;
            printf ("NEC_Code = %x\r\n", Nec_code1);            // make sure to use Nec_code1
    
            INTCONbits.INT0IE = 1;                              // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;                            // Edge programming for INT0 falling edge
    
            found = 0xff;
            for (int j=0; j< 21; j++)                           // Linear search to scan for button presses
            {
                if (Nec_code1 == array1[j]) 
                {
                    found = j;
                    j = 21;
                }
            }
          
            if (found == 0xff)                                     // If a button could not be found, print out a message in TeraTerm
            {
                printf ("Cannot find button \r\n");
            }
            
            else                                                    // If a button was found, run the code below
            {
                Do_Beep();
                printf ("button = %d \r\n", found);
                
                if ( found == 5 )                                   // If the power button is pressed (mapped to button 5)
                {
                    if (FAN == 0 )                                  // Check the previous state of the fan, if it was off
                    {
                        FAN = 1;                                    // Turn the fan on
                    }
                    else                                            // If it was on, 
                    {
                        FAN = 0;                                    // Turn off the fan
                    }       
                    Toggle_Fan();                                   // Run the toggle fan routine
                    
                }

                if ( found == 6 )                                   // If the '-' button is pressed (mapped to button 6)
                {    
                    Decrease_Speed();                               // Run the decrease fan speed routine
                } 

                if ( found == 7 )                                   // If the '+' button is pressed (mapped to button 7)
                { 
                    Increase_Speed();                               // Run the increase fan speed routine
                } 
                
                if ( found == 8 )                                   // If EQ (the eight button on the IR remote) is pressed, Reset the time according to the time 
                {    
                    printf("\r\n New Time Set! \r\n \r\n");         // defined in the DS3231_Setup_Time() function 
                    DS3231_Setup_Time(); 
                }
                
            }            
        }
    }
}







