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

#define D7_RED    PORTAbits.RA4
#define D7_GREEN  PORTAbits.RA5
#define D7_BLUE   NULL  

#define D8_RED    PORTBbits.RB0
#define D8_GREEN  PORTBbits.RB1
#define D8_BLUE   PORTBbits.RB2

#define D9_RED    PORTBbits.RB5
#define D9_GREEN  PORTBbits.RB6
#define D9_BLUE   PORTBbits.RB7

void SET_D7_RED(void);
void SET_D7_GREEN(void);
void SET_D7_YELLOW(void);

void SET_D8_OFF(void);
void SET_D8_RED(void);
void SET_D8_GREEN(void);
void SET_D8_YELLOW(void);
void SET_D8_BLUE(void);
void SET_D8_PURPLE(void);
void SET_D8_CYAN(void);
void SET_D8_WHITE(void);

void SET_D9_OFF(void);
void SET_D9_RED(void);
void SET_D9_GREEN(void);
void SET_D9_BLUE(void);
void SET_D9_WHITE(void);

void init_ADC(void);
unsigned int get_full_ADC(void);

void init_UART()
{
OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
USART_BRGH_HIGH, 25);
OSCCON = 0x60;
}


void main(void)
{   
    
    ADCON1 = 0x0F;                         // configure A/D control register 1 Digtial
    TRISC = 0x00;                          // make PORTC as all outputs
    TRISD = 0x00;                          // make PORTD as all outputs
    TRISE = 0x00; 
    
    init_UART();
    init_ADC(); 
    
    int tempDigits[2];
    int LED2;                                                   // Declaration of the Tens place value 7 seg display 
    int LED1;                                                   // Declaration of the Ones place value 7 seg display
    int testbit;                                                // Storage of 6th bit to test for PORTE  
    
    int segDisplay[] = { 0x01,0x4F,0x12,0x06,
0x4C,0x24,0x20,0x0F,0x00,0x04};                               // Index is the number that will be displayed b/c
                                                               // 7 seg Display Hex Code was mapped to Index
    
    
    
    while(1)
    {
      
        for(int i = 0; i <= 99; i++)
        {
         int arr[1] = {0};
         tempDigits[1] = arr[1] % 10;                          
         tempDigits[0] = ((arr[1] - tempDigits[1])/10) % 10;   

         
         
         LED2 = segDisplay[tempDigits[0]];  
         LED1 = segDisplay[tempDigits[1]];
         
         PORTD = LED2;                                          // Output Hex to Display
                                                        
         PORTC = LED1 & 0x3F;                                   // Using AND to the 6th bit
         
         testbit = (LED1 & 0x40) >> 0x06;                       // Keep the 6th bit and shift it to bit position 0
         
         PORTEbits.RE1 = testbit;                               // Move the 0 or 1 that was in the 6th bit into PORTE RE1
         
         arr[1]++;
         
                                                                // Keep the 6th bit and shift it to bit position 0
         
                                                                 // Move the 0 or 1 that was in the 6th bit into PORTE RE1
         for(int i = 0; i < 17000; i++);
         for(int i = 0; i < 17000; i++);
         for(int i = 0; i < 17000; i++);
         for(int i = 0; i < 17000; i++);
         
   
        }
         
         
         
         
         
            
    }
}



void init_ADC(void)
{
    ADCON0=0x01;                                               // Select channel AN0, and turn on the ADC subsystem
    ADCON1=0x1B;                                               // Select pins AN0 through AN3 as analog signal, VDD-VSS as
                                                               // Reference voltage
    ADCON2=0xA9;                                               // Right justify the result. Set the bit conversion time (TAD) and
                                                               // Acquisition time
}

unsigned int get_full_ADC(void)
{
int result; 
    ADCON0bits.GO=1;                                           // Start Conversion
    while(ADCON0bits.DONE==1);                                 // Wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                        // Combine result of upper byte and
                                                               // Lower byte into result   
return result;                                                 // Return the result.
}



void SET_D7_RED()
{
    D7_RED = 1;
    D7_GREEN = 0;
    
}
void SET_D7_GREEN()
{   
    D7_RED = 0;
    D7_GREEN = 1;
   
}
void SET_D7_YELLOW()
{
    D7_RED = 1;
    D7_GREEN = 1;
    
}



void SET_D8_OFF()
{
    D8_RED = 0;
    D8_GREEN = 0;
    D8_BLUE = 0;
    
}
void SET_D8_RED()
{
    D8_RED = 1;
    D8_GREEN = 0;
    D8_BLUE = 0;
    
}
void SET_D8_GREEN()
{
    D8_RED = 0;
    D8_GREEN = 1;
    D8_BLUE = 0;
    
}
void SET_D8_YELLOW()
{
    D8_RED = 1;
    D8_GREEN = 1;
    D8_BLUE = 0;
}
void SET_D8_BLUE()
{
    D8_RED = 0;
    D8_GREEN = 0;
    D8_BLUE = 1;
    
}
void SET_D8_PURPLE()
{
    D8_RED = 1;
    D8_GREEN = 0;
    D8_BLUE = 1;
}
void SET_D8_CYAN()
{
    D8_RED = 0;
    D8_GREEN = 1;
    D8_BLUE = 1;
    
}
void SET_D8_WHITE()
{
    D8_RED = 1;
    D8_GREEN = 1;
    D8_BLUE = 1;
    
}

void SET_D9_OFF()
{
    D9_RED = 0;
    D9_GREEN = 0;
    D9_BLUE = 0;
    
}


void SET_D9_RED()
{
    D9_RED = 1;
    D9_GREEN = 0;
    D9_BLUE = 0;
}

void SET_D9_GREEN()
{
    D9_RED = 0;
    D9_GREEN = 1;
    D9_BLUE = 0;
}

void SET_D9_BLUE()
{
    D9_RED = 0;
    D9_GREEN = 0;
    D9_BLUE = 1;
}
void SET_D9_WHITE()
{
    D9_RED = 1;
    D9_GREEN = 1;
    D9_BLUE = 1;
}
