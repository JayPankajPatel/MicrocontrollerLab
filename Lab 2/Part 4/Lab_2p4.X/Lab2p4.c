#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#pragma config OSC = INTIO67
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config CCP2MX = PORTBE
void Flash_LED(void);                   // Function Prototyping area 
void Delay_One_Sec(void);               //               
                 

void main(void)
{
    TRISC = 0x00;                       // make PORTC as all outputs
    TRISD = 0x00;                       // make PORTD as all outputs
    ADCON1 = 0x0F;                      // configure A/D control register 1 
                                        // as all digital
    while (1)
    {
        Flash_LED();
    }
}

void Flash_LED()                        // Function Defintion
{       
    char D2;                            // Declartion of 8 bit integer type variable 'D2'
    D2 = 0x07;                          // Color initialized to '111'(white)
    for (int D1 = 0; D1 < 8; D1++) 
                                        // routine to change colors of D1 and D2
    {       
        LATC = D1;                      // set PORTC to counter
        LATD = D2;                      // set PORTD to D2
        Delay_One_Sec();                // delay
        D2--;                           // decrement D2
    }
}
void Delay_One_Sec()                    //Function Definition
{
    for (int I = 0; I < 17000; I++);
}