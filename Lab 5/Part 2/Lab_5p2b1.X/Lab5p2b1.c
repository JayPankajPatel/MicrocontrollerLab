#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>
#pragma config OSC = INTIO67
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOREN =OFF
#pragma config CCP2MX = PORTBE

#define D_POINT     PORTDbits.RD7                               // Defining the port of the Decimal Point 

#define D2_RED      PORTBbits.RB0
#define D2_GREEN    PORTBbits.RB1
#define D2_BLUE     PORTBbits.RB2

void DISPLAY_UPPER_7SEG(char);                                 // Function Prototyping Area Start
void DISPLAY_LOWER_7SEG(char);                                 //
                                                               //
void Select_ADC_Channel(char);                                 //
                                                               //
                                                               //
void Activate_Buzzer(void);                                    //
void Deactivate_Buzzer(void);                                  //
void WAIT_ONE_SEC(void);                                       //
void DO_DISPLAY_D2(int);                                       //
                                                               //
void SET_D1_RED(void);                                         //                                
void SET_D1_GREEN(void);                                       //                               
void SET_D1_YELLOW(void);                                      //
                                                               //
void SET_D2_OFF(void);                                         //
void SET_D2_RED(void);                                         //
void SET_D2_GREEN(void);                                       //
void SET_D2_BLUE(void);                                        //
void SET_D2_WHITE(void);                                       //
void SET_D2_YELLOW(void);                                      //
void SET_D2_PURPLE(void);                                      //
void SET_D2_CYAN(void);                                        // Function Prototyping Area End


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




int segDisplay[] = {0x01,0x4F,0x12,0x06,                       // Index of array is the number that will be displayed                                 
0x4C,0x24,0x20,0x0F,0x00,0x04};


void main(void)
{   
    init_ADC(); 
    init_UART(); 
    
    TRISA = 0xFF;                                              // Set PORTA as input and the rest as outputs
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTB = 0x00;                                               // Initialization of the ports
                                                                //
    PORTC = 0xFF;                                               //
    PORTD = 0xFF;                                               //
    PORTE = 0xFF;                                               //
     
    
    char U;                                                     // Declaration of the Value that will be displayed in the Lower 7 Seg Display
                                                                // Declaration of the Value that will be displayed in the Upper 7 Seg Display
    char L;
    char ON = D_POINT= 0; 
    char OFF = D_POINT = 1; 
    float RREF = 0.1;                                           // Resistance used to measure unknown resistor in kOhms
    float VREF = 4.096;
    
        while(1)
        {
            Select_ADC_Channel(1);                              // Select AN1 for ADC conversion
            int STEP = get_full_ADC(); 
            float VOLT = (round((STEP * 4.0)))/1000;
            float RL = (RREF * VOLT) / (VREF - VOLT);           // Calculation of Resistance using Voltage Divider Rule
            U = (int) VOLT;                                     // Extract Upper Value of Voltage
                                                                
            L =  ((int)((VOLT - U) * 10) % 10);                 // Extract Lower Value of Voltage
                 
            
            if(RL < 10) 
            {
                 U = (int) RL;
                 L =  ((int)((RL - U) * 10) % 10);
                 
                 DISPLAY_UPPER_7SEG(U);
            
                 DISPLAY_LOWER_7SEG(L);
                 
                 D_POINT= 0;                                    // Turn Decimal Point On
            }
            
            else 
            {
                U = (int) RL/10;
                L =  ((int)((RL - U) * 10) % 10);
                DISPLAY_UPPER_7SEG(U);
            
                DISPLAY_LOWER_7SEG(L);
                
                 D_POINT= 1;                                    // Turn Decimal Point Off
                
            }
            
            if(RL < 70)
            {
                Activate_Buzzer();
            }
            else
            {
                Deactivate_Buzzer();
            }
            DO_DISPLAY_D2(RL); 
            printf("RL = %f\r\n\n\n", RL ); 
            WAIT_ONE_SEC();
          
            
   
        }
    
    
}

void WAIT_ONE_SEC()
{
    for(int i = 0; i < 0xFFFF; i++);
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

void DO_DISPLAY_D2(int RL)
{
     PORTB = (RL >= 70) ? 7 : (RL/10); 
      
}


void Select_ADC_Channel(char channel)
{
    ADCON0 = channel * 4 + 1;
    
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


void SET_D2_OFF()
{
    D2_RED = 0;
    D2_GREEN = 0;
    D2_BLUE = 0;
    
}

void SET_D2_RED()
{
    D2_RED = 1;
    D2_GREEN = 0;
    D2_BLUE = 0;
}
void SET_D2_GREEN()
{
    D2_RED = 0;
    D2_GREEN = 1;
    D2_BLUE = 0;
}
void SET_D2_BLUE()
{
    D2_RED = 0;
    D2_GREEN = 0;
    D2_BLUE = 1;
}
void SET_D2_WHITE()
{
    D2_RED = 1;
    D2_GREEN = 1;
    D2_BLUE = 1;
}

void SET_D2_YELLOW()
{
D2_RED = 1;
D2_GREEN = 1;
D2_BLUE = 0;
}

void SET_D2_PURPLE()
{
    D2_RED = 1;
    D2_GREEN = 0;
    D2_BLUE = 1;
    
}

void SET_D2_CYAN()
{
    D2_RED = 0;
    D2_GREEN = 1;
    D2_BLUE =  1;
}
