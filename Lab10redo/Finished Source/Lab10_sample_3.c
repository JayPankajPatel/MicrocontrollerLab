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
#include "ST7735.h"

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTBE

#define Circle_Size     20                          // Size of Circle for Light
#define Circle_X        60                          // Location of Circle
#define Circle_Y        80                          // Location of Circle
#define Text_X          52          
#define Text_Y          77          
#define TS_1            1                           // Size of Normal Text
#define TS_2            2                           // Size of Big Text


// colors
#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_BLACK

#define kp               PORTEbits.RE1              // definition Key Pressed LED      

#define Circle_Size     20                          // Size of Circle for Light
#define Circle_X        60                          // Location of Circle
#define Circle_Y        80                          // Location of Circle
#define Text_X          52          
#define Text_Y          77          
#define TS_1            1                           // Size of Normal Text
#define TS_2            2                           // Size of Big Text

#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_BLACK

char tempSecond = 0xff;                             // Variable to hold the time in order to check if time has elasped

                                                    // I2C register addresses for the DS3231 start
char second = 0x00;                                 // I2C register address for second
char minute = 0x01;                                 // I2C register address for minute
char hour = 0x02;                                   // I2C register address for hour
char dow = 0x03;                                    // I2C register address for day of week
char day = 0x04;                                    // I2C register address for day
char month = 0x05;                                  // I2C register address for month  
char year = 0x06;                                   // I2C register address for year
                                                    // I2C register addresses for the DS3231 end

char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;

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

char txt1[21][4] =                                  // Text to display on LCD corresponding to button press      
{
    "CH-\0","CH\0","CH+\0",
    
    "|<<\0",">>|\0",">||\0",
    
    " - \0"," + \0","EQ \0",
    
    " 0 \0","100\0","200\0",
    
    " 1 \0"," 2 \0"," 3 \0",
    
    " 4 \0"," 5 \0"," 6 \0",
    
    " 7 \0"," 8 \0"," 9 \0"
};

int color[21]=                                      // Color of LCD circle that is displayed corresponding to button press
{
    RD,RD,RD,
    BU,BU,GR,
    MA,MA,MA,
    BK,BK,BK,
    BK,BK,BK,
    BK,BK,BK,
    BK,BK,BK,
};

char TempSecond;

char buffer[31];                                    // general buffer for display purpose
char *nbr;                                          // general pointer used for buffer
char *txt;

short nec_ok = 0;
unsigned long long Nec_code;
char Nec_code1;
char found;
unsigned char Nec_state = 0;

signed char tempF;
signed char tempC;


void Activate_Buzzer();
void Initialize_Screen();
void Initialize_Screen();
void waitHalfSecondWithBeep();  
void Do_LED(); 

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

void Wait_Half_Second()
{
    T0CON = 0x03;                               // Timer 0, 16-bit mode, prescaler 1:16
    TMR0L = 0xDB;                               // set the lower byte of TMR
    TMR0H = 0x0B;                               // set the upper byte of TMR
    INTCONbits.TMR0IF = 0;                      // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                       // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0);             // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                       // turn off the Timer 0
}

void Wait_One_Sec()
{
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    Wait_Half_Second();  
}




void Do_Init()                                  // Initialize the ports 
{           
    init_UART();                                // Initialize the uart
    OSCCON=0x70;                                // Set oscillator to 8 MHz 
    
    ADCON1=0x0F;                                // All Digital Pins

    TRISB = 0x01;                               // One input in PORTB, the rest are outputs
    TRISC = 0x00;                               // PORTC configured as all outputs       
    TRISD = 0x00;                               // PORTD configured as all outputs   
    TRISE = 0x00;                               // PORTE configured as all outputs   

    RBPU=0;                                     // Internal Pull Up Resistor Turned Off
    init_INTERRUPT();                            
    I2C_Init(100000); 
    DS1621_Init();

}


void main() 
{ 
    Do_Init();                                                  // Initialization
    Initialize_Screen();
    //DS3231_Setup_Time(); 
//    while (1)
//    {
//        signed char tempC = DS1621_Read_Temp();
//        signed char tempF = (tempC * 9 / 5) + 32;
//        printf (" Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);
//        Wait_One_Sec();
//    }
    
      while (1)
      {

        DS3231_Read_Time();                                         // Read from the DS3231 forever
        if(tempSecond != second)                                    // If the time was elasped, recalulate the temperature, print out the time, and set temperature LED
        {   
            tempSecond = second;                                    // Update the time
            tempC = DS1621_Read_Temp();                             // Read temperature from the DS1621
            tempF = (tempC * 9 / 5) + 32;                           // Calculate the temperature in Fahrenheit
            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year); // Output the time as shown in the print statement
            printf (" Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);      // Output the temperature in Celsius and Fahrenheit 
            
            Do_LED();                                               // Change the color of the LED according to the handout
        }
    
        if (nec_ok == 1)                                            // If the transmission was sucessful
        {               
            nec_ok = 0;                                             // Clear flag

            Nec_code1 = (char) ((Nec_code >> 8));
            printf ("NEC_Code = %08lx %x\r\n", Nec_code, Nec_code1);
            INTCONbits.INT0IE = 1;                                  // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;                                // Edge programming for INT0 falling edge

            char found = 0xff;                                      // Initialize found variable to hold incoming data

            for( int i = 0; i < 21; i++ )               
            {               
                if( Nec_code1 == array1[i] )                        // Check the array for the transimission 
                {               
                    found = i;                                      // Since all arrays are mapped in the same way, passing the index will give you the correct value for found
                } 
                
            }
            
            if(found == 8)                                          // If EQ (the eight button on the IR remote) is pressed, Reset the time according to the time 
            {   printf("\r\n New Time Set! \r\n \r\n");             // defined in the DS3231_Setup_Time() function 
                DS3231_Setup_Time(); 
            }
            
            
            if (found != 0xff)                                      // If the data was found, do a routine to display to LCD, Turn LEDs to color, and then buzz. 
            {
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]);              // Display LCD Start
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);              //
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK,TS_1); // Display LCD End
                waitHalfSecondWithBeep();                                               // Turn on buzzer and activation (SEC) LED for 500ms
                 

            }
            
        }
      }
            
   
    
    
}



void Activate_Buzzer()
{
    PR2 = 0b11111001;
    T2CON = 0b00000101;
    CCPR2L = 0b01001010;
    CCP2CON = 0b00111100; 
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0; 
}

void Initialize_Screen()
{
    LCD_Reset();                        
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;
    strcpy(txt, "ECE3301L Fall 21-S3");  
    drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    strcpy(txt, "LAB 10 ");  
    drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

void waitHalfSecondWithBeep()
{
    kp = 1;                             // If a button on the IR remote is pressed, turn on an indication LED
    Activate_Buzzer();                  // Activate a buzzer
    Wait_Half_Second();                 // Let 500ms elaspe
    Deactivate_Buzzer();                // Stop the Buzzer 
    kp = 0;                             // Turn off the indication LED
   
}

void Do_LED() 
{
    PORTC = (tempF >= 70) ? 7 : (tempF/10);     // Take the tens place of the temperature in Fahrenheit,
                                                // pass the value into the RGB LED, located on PORTC
}








