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
void Delay_One_Sec(void);               // Function Prototyping area 

void main(void)               
{   
                                        //setup
    TRISC = 0x00;                       // make PORTC as output to LED
    ADCON1 = 0x0F;                      // configure A/D control register 1 as all digital
    
    while (1)                 
    {                                   // for-each routine to loop through LED colors
        for (int i = 0; i < 8; i++) 
        {
            PORTC = i;                  //PORTC is initialized as counter
                                        // Note: the counter's value in binary
                                        // match the binary output to pins needed 
                         
            Delay_One_Sec();            //delay 
            Delay_One_Sec();            //delay
        }
    }
}

void Delay_One_Sec()                     //Function Definition
{
    for (int I = 0; I < 17000; I++);
}
