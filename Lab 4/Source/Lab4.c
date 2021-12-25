#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>
#pragma config OSC = INTIO67
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOREN =OFF

#define D3_RED    PORTBbits.RB5
#define D3_GREEN  PORTBbits.RB6
#define D3_BLUE   PORTBbits.RB7

#define D2_RED      PORTBbits.RB0
#define D2_GREEN    PORTBbits.RB1
#define D2_BLUE     PORTBbits.RB2


#define D1_RED    PORTAbits.RA4
#define D1_GREEN  PORTAbits.RA5


  

void Select_ADC_Channel(char channel)                          // Function to select analog port for ADC conversion 
{
    ADCON0 = channel * 4 + 1;
    
}


void init_ADC(void)
{
                                                 
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
void init_UART()                                               // Initialize UART for Debugging
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

void DISPLAY_UPPER_7SEG(char);                                 // Function Prototyping Area Start
void DISPLAY_LOWER_7SEG(char);                                 //
                                                               //
                                                               //        
                                                               //
void DO_DISPLAY_D1(int);                                       //
void DO_DISPLAY_D2(int);                                       //
void DO_DISPLAY_D3(int);                                       //
                                                               //
void SET_D1_RED(void);                                         //                                        
void SET_D1_GREEN(void);                                       //                                      
void SET_D1_YELLOW(void);                                      //                                      
                                                               //
                                                               //
                                                               //
void SET_D3_OFF(void);                                         //
void SET_D3_RED(void);                                         //
void SET_D3_GREEN(void);                                       //
void SET_D3_BLUE(void);                                        //
void SET_D3_WHITE(void);                                       //
                                                               //
void SET_D2_OFF(void);                                         //
void SET_D2_RED(void);                                         //
void SET_D2_GREEN(void);                                       //
void SET_D2_BLUE(void);                                        //
void SET_D2_WHITE(void);                                       //
void SET_D2_YELLOW(void);                                      //
void SET_D2_PURPLE(void);                                      //
void SET_D2_CYAN(void);                                        //
                                                               //
void init_ADC(void);                                           //
unsigned int get_full_ADC(void);                               //
void init_UART(void);                                          //
void Select_ADC_Channel(char);                                 //
                                                               //
void WAIT_ONE_SEC(void);                                       // Function Prototyping Area End

int segDisplay[] = { 0x01,0x4F,0x12,0x06,                      // Index of array is the number that will be displayed 
                                                                
0x4C,0x24,0x20,0x0F,0x00,0x04};


void main(void)

{   init_UART();
    init_ADC();
    
    TRISA = 0x0F;                                              // Make RA0-RA3 inputs, the rest outputs
    TRISB = 0x00;                                              // Make PORTB as all outputs
    TRISC = 0x00;                                              // Make PORTC as all outputs
    TRISD = 0x00;                                              // Make PORTD as all outputs
    TRISE = 0x00;                                              // Make PORTE as all outputs
                    
    PORTB = 0x00;                                              // Initialization of the ports
                        
    PORTC = 0xFF;                      
    PORTD = 0xFF;                                               
    PORTE = 0xFF;                                               
                        
                        
                        
    char U;                                                    // Declaration of the Value that will be displayed in the Lower 7 Seg Display
                                                               // Declaration of the Value that will be displayed in the Upper 7 Seg Display
    char L;
    
    
                                  
    
                                          
    while(1)
    {   
        Select_ADC_Channel(0);                                 // Select RA0 for analog conversion                                 
        int num_step = get_full_ADC();                         // Obtain the steps of the ADC coversion
        float voltage_mv = num_step * 4.0;                     // Convert the Analog to Digital conversion to a useable voltage measurement
        
        float tempInC = (1035.0 - voltage_mv )/ 5.50;
        float temperatureInF = (1.80* tempInC) + 32.0; 
        int tempInF = (int) temperatureInF;
        
        L = tempInF % 10;                                      // Extract the Ones Place and initialize it to L
        U = ((tempInF-L)/10);                                  // Extract the Tens Place and initialize it to U
        
        
        //printf("voltage_mv = %f \r\n\n\n",voltage_mv);
        //printf("Temperature = %d \r\n\n\n",tempInF);
        
        DISPLAY_UPPER_7SEG(U);
        DISPLAY_LOWER_7SEG(L);
        
        DO_DISPLAY_D2(tempInF);             
        DO_DISPLAY_D3(tempInF);
        
        
        
      
        
        Select_ADC_Channel(2);                                 // Select RA0 for analog conversion                                                   
        num_step = get_full_ADC();                             // Obtain the steps of the ADC coversion                    
        float voltage_mv2 = num_step * 4.0;                    // Convert the Analog to Digital conversion to a useable voltage measurement  
        
        DO_DISPLAY_D1(voltage_mv2);
        //printf("voltage_mv2 = %f \r\n\n\n",voltage_mv2);
        //printf("voltagePHOTO = %f \r\n\n\n",voltage);
        
        
        
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

void DO_DISPLAY_D3(int tempInF)
{
         if(tempInF < 45) 
         {
             SET_D3_OFF();
         }
         
         if(46 < tempInF && tempInF <= 55) 
         {
             SET_D3_RED();
         }
         
         if(55 < tempInF && tempInF <= 65) 
         {
             SET_D3_GREEN();
         }
         
         if(66 < tempInF && tempInF <= 75) 
         {
             SET_D3_BLUE();
         }
         
         if(tempInF > 76 ) 
         {
             SET_D3_WHITE();
         }
         
         WAIT_ONE_SEC();
        
}
  
void DO_DISPLAY_D2(int tempInF)
{
     PORTB = (tempInF >= 70) ? 7 : (tempInF/10);               // Truthy output is 7 and Falsy output is tempInF/10
            
      
}

void DO_DISPLAY_D1(int voltage)
{
    
    
    if(voltage < 2500)
    {
        SET_D1_RED();
    }
    if(voltage >= 2500 && voltage < 3400)
    {
        SET_D1_GREEN();
    }
    if(voltage >= 3400)
    {
        SET_D1_YELLOW();
    }
    
     WAIT_ONE_SEC(); 
   
    
}






void SET_D3_OFF()
{
    D3_RED = 0;
    D3_GREEN = 0;
    D3_BLUE = 0;
    
}

void SET_D3_RED()
{
    D3_RED = 1;
    D3_GREEN = 0;
    D3_BLUE = 0;
    
}
void SET_D3_GREEN()
{
    D3_RED = 0;
    D3_GREEN = 1;
    D3_BLUE= 0;
    
}
void SET_D3_BLUE()
{
    D3_RED = 0;
    D3_GREEN = 0;
    D3_BLUE = 1;
    
}
void SET_D3_WHITE()
{
    D3_RED = 1;
    D3_GREEN = 1;
    D3_BLUE = 1;
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





void SET_D1_RED()
{
    D1_RED = 1; 
    D1_GREEN = 0; 
}

void SET_D1_GREEN()
{
    D1_RED = 0; 
    D1_GREEN = 1; 
}

void SET_D1_YELLOW()
{
    D1_RED = 1; 
    D1_GREEN = 1; 
}