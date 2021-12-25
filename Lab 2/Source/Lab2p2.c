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

void main()  
{
    
    char in;                // Declartion of 8 bit integer type variable 'in'
        TRISA = 0xFF;       // Make PORTA an Input 
        TRISB = 0x00;       // Make PORTB an Output
        TRISC = 0x00;       // Make PORTC an Output
        ADCON1 = 0x0F;      // Set pins as a digital signal
        
        while(1) 
        {
            in = PORTA;     // Initialize 'in' with data from PORTA
            in = in & 0x07; // Using bitwise AND mask out the upper three bits
            PORTC = in;     // Assign the data to PORTB
        
        }


}