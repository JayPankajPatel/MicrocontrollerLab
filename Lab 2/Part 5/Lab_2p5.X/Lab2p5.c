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
void Flash_LED(void);                      // Function Prototyping area
void Delay_One_Sec(void);                  //

void main(void)
{   ADCON1 = 0x0F;                         // configure A/D control register 1 Digtial
    TRISC = 0x00;                          // make PORTC as all outputs
    TRISD = 0x00;                          // make PORTD as all outputs
    while (1)
    {
        Flash_LED();                       // call routine to flash the LED
    }
}

void Flash_LED()                           // Function Defintion
{
    
    unsigned int D[] = {0xA6, 0x23, 0x05, 0x82, 0xC0, 0x67, 0x41, 0xE4};               
         
        for (char i = 0; i < 8; i++)       // routine to change colors of D1,D2,D3
        {      
            LATC = i;                      // set PORTC to counter (D1)
            LATD = D[i];                   // Set PORTD to Array value at i index
    
            Delay_One_Sec();               // delay
                                        
        }
}
void Delay_One_Sec()                       //Function Definition
{
    for (int I = 0; I < 17000; I++);
}
