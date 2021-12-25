#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>
#pragma config OSC = INTIO67
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOREN =OFF
#pragma config CCP2MX = PORTBE

#define D_POINT     PORTDbits.RD7


void Activate_Buzzer(void);                                    // Function Prototyping Area Start
void Deactivate_Buzzer(void);                                  //
void WAIT_ONE_SEC(void);                                       //  
                                                               //
void DISPLAY_UPPER_7SEG(char);                                 //                                  
void DISPLAY_LOWER_7SEG(char);                                 //
                                                               //
void Select_ADC_Channel(char);                                 // Function Prototyping Area End


void init_ADC(void)
{
                                                 
    ADCON1=0x19;                                               // Select pins AN0 through AN3 as analog signal, VDD-VSS as
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
void init_UART()
{
OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
USART_BRGH_HIGH, 25);
OSCCON = 0x60;
}
void putch (char c)
{
while (!TRMT);
TXREG = c;
}




int segDisplay[] = {0x01,0x4F,0x12,0x06,                        // Index of array is the number that will be displayed                                 
0x4C,0x24,0x20,0x0F,0x00,0x04};


void main(void)
{   
    init_ADC(); 
    init_UART(); 
    
    TRISA = 0x0F;                                               // Set PORTA as input and the rest as outputs
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTB = 0x00;                                               // Initialization of the ports
                                                                //
    PORTC = 0xFF;                                               //
    PORTD = 0xFF;                                               //
    PORTE = 0xFF;                                               //
                                                                //
                                                                //
    char U;                                                     // Declaration of the Value that will be displayed in the Lower 7 Seg Display
                                                                // Declaration of the Value that will be displayed in the Upper 7 Seg Display
    char L;
   
    
    
        while(1)
        {
            Select_ADC_Channel(0);                              // Select AN0 for ADC conversion
            int STEP = get_full_ADC();       
            float VOLT = (STEP * 4.0)/1000;         
            U = (int) VOLT;                                     // Extract Upper Value of Voltage
                                                                // Extract Lower Value of Voltage
            L =  ((int)((VOLT) * 10) % 10);
                 
            
            printf("VOLT = %f\r\n\n\n", VOLT ); 
            
            DISPLAY_UPPER_7SEG(U);
        
            DISPLAY_LOWER_7SEG(L);
            
            D_POINT = 0;
            WAIT_ONE_SEC();
       
        }
    
    
}

void DISPLAY_UPPER_7SEG(char Upper)
{
                                                              
    int LED1 = segDisplay[Upper];                              // 7 seg Display Hex Code was mapped to Index
    PORTC = LED1 & 0x3F;                                       // Using AND to the 6th bit
         
    int testbit = (LED1 & 0x40) >> 0x06;                       // Keep the 6th bit and shift it to bit position 0
         
    PORTEbits.RE1 = testbit;                                   // The bit in the 0 position will be sent to PORTE RE1
    
    
  
}

void DISPLAY_LOWER_7SEG(char Lower)
{
    
    int LED2 = segDisplay[Lower];                             
    
    PORTD = LED2;                                              // Display Value on 7 seg
    
}

void Select_ADC_Channel(char channel)
{
    ADCON0 = channel * 4 + 1;
    
}



void WAIT_ONE_SEC()
{
    for(int i = 0; i < 0xFFFF; i++);
}

