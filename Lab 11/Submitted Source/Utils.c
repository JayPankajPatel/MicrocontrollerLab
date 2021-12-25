#include <p18f4620.h>
#include "Main.h"
#include "Utils.h"
#include "stdio.h"


void Wait_Half_Second()
{
    for(int i = 0; i < 8800; i++);              // Hardware delay
}

void Wait_One_Sec()
{
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    Wait_Half_Second();  
}

void Activate_Buzzer()                         // This routine uses PWM in order to activate a connected buzzer
{
    PR2 = 0b11111001;                              
    T2CON = 0b00000101;                           
    CCPR2L = 0b01001010;                         
    CCP2CON = 0b00111100;                          
}

void Deactivate_Buzzer()                       // This routine clears the register so the duty cycle is zero/ the buzzer is off
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0; 
}

void Do_Beep()
{
    Activate_Buzzer();
    Wait_Half_Second();
    Deactivate_Buzzer();
    
}

