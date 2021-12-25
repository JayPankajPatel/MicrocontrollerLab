#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>
#pragma config OSC =   INTIO67
#pragma config WDT=    OFF
#pragma config LVP=    OFF
#pragma config BOREN=  OFF
#pragma config CCP2MX= PORTBE

#define NS_RED        PORTAbits.RA5
#define NS_GREEN      PORTBbits.RB0

#define EW_RED        PORTBbits.RB4
#define EW_GREEN      PORTBbits.RB5

#define NSLT_RED      PORTBbits.RB1
#define NSLT_GREEN    PORTBbits.RB2

#define EWLT_RED      PORTEbits.RE1
#define EWLT_GREEN    PORTEbits.RE2

#define OFF           0
#define RED           1
#define GREEN         2
#define YELLOW        3

#define NSPED_SW      PORTAbits.RA1
#define NSLT_SW       PORTAbits.RA2

#define EWPED_SW      PORTAbits.RA3
#define EWLT_SW       PORTAbits.RA4

#define MODE_LED      PORTBbits.RB7
#define SEC_LED       PORTDbits.RD7

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


void Wait_Half_Second(void);                                                        // Start of Function Prototyping Area
void Wait_One_Second(void);                                                         //
void Wait_N_Seconds (char);                                                         //
void Wait_One_Second_With_Beep(void);                                               //
                                                                                    //
void Activate_Buzzer(void);                                                         //
void Deactivate_Buzzer(void);                                                       //
                                                                                    //
void DISPLAY_UPPER_7SEG(char);                                                      //
void DISPLAY_LOWER_7SEG(char);                                                      //  
                                                                                    //
void Set_NS(char);                                                                  //
void Set_EW(char);                                                                  //
void Set_NSLT(char);                                                                //
void Set_EWLT(char);                                                                //
                                                                                    //
void PED_Control ( char ,char );                                                    //
                                                                                    //
void Day_Mode(void);                                                                //
void Night_Mode(void);                                                              // End of Function Prototyping Area

void init_ADC(void)
{
    ADCON0 = 0x01;                                            
    ADCON1=0x0E;                                                                    // Select pins AN0 as analog signal, VDD-VSS as
                                                                                    // Reference voltage
    ADCON2=0xA9;                                                                    // Right justify the result. Set the bit conversion time (TAD) and
                                                                                    // Acquisition time
}       
        
unsigned int get_full_ADC(void)     
{       
int result;         
    ADCON0bits.GO=1;                                                                // Start Conversion
    while(ADCON0bits.DONE==1);                                                      // Wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                             // Combine result of upper byte and
                                                                                    // Lower byte into result   
return result;                                                                      // Return the result.
}       
        
//void Select_ADC_Channel(char channel)     
//{     
//    ADCON0 = channel * 4 + 1;     
//          
//}     
        
        
int segDisplay[] = { 0x01,0x4F,0x12,0x06,                                           // Index of array is the number that will be displayed 
                                                                
0x4C,0x24,0x20,0x0F,0x00,0x04};

void main(void) 
{
  
    init_ADC();
    init_UART();
    
    TRISA = 0x1F;
    TRISB = 0x00; 
    TRISC = 0x00; 
    TRISD = 0x00; 
    TRISE = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0xFF;
    PORTD = 0xFF; 
    PORTE = 0x01;
    
         
    
    

    while (1)
    {
        //Select_ADC_Channel(0);
        int step = get_full_ADC();
        float volt = (step * 4.88)/1000.0;

        printf("VOLT = %f\r\n", volt); 

        if ( volt < 2.5 )                                                          // Test if the photo resistor's incoming voltage is enough to be considered day or night mode
        {   MODE_LED = 1;
            Day_Mode();
        }
        if ( volt > 2.5 ) 
        {   MODE_LED = 0;
            Night_Mode(); 
        }

   }
    
  
}

void Wait_Half_Second()
{    
    T0CON = 0x02;                                                                   // Timer 0, 16-bit mode, prescaler 1:8
    TMR0L = 0xDB;                                                                   // set the lower byte of TMR                    
    TMR0H = 0x0B;                                                                   // set the upper byte of TMR                    
    INTCONbits.TMR0IF = 0;                                                          // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                                                           // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0);                                                 // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                                                           // turn off the Timer 0
}
void Wait_One_Second()
{
    SEC_LED = 0;                                                                    // First, turn off the SEC LED
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
    SEC_LED = 1;                                                                    // then turn on the SEC LED
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
}

void Wait_N_Seconds (char seconds) 
{
  char I;
  
    for( I = 0; I < seconds; I++)
    {
        Wait_One_Second();
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

void Wait_One_Second_With_Beep()
{
    SEC_LED = 1;                                                                    // First, turn on the SEC LED
    Activate_Buzzer();                                                              // Activate the buzzer
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                                                    // then turn off the SEC LED
    Deactivate_Buzzer();                                                            // Deactivate the buzzer
    Wait_Half_Second();                                                             // Wait for half second (or 500 msec)
}

void Set_NS(char color)
{
    switch (color)
   {
    case OFF: NS_RED =0;NS_GREEN=0;break;                                           // Turns off the NS LED
    case RED: NS_RED =1;NS_GREEN=0;break;                                           // Sets NS LED RED
    case GREEN: NS_RED =0;NS_GREEN=1;break;                                         // sets NS LED GREEN
    case YELLOW: NS_RED =1;NS_GREEN=1;break;                                        // sets NS LED YELLOW
   }            
}

void Set_NSLT(char color)
{
    switch (color)
   {
    case OFF: NSLT_RED =0;NSLT_GREEN=0;break;                                       // Turns off the NSLT LED
    case RED: NSLT_RED =1;NSLT_GREEN=0;break;                                       // Sets NSLT LED RED
    case GREEN: NSLT_RED =0;NSLT_GREEN=1;break;                                     // sets NSLT LED GREEN
    case YELLOW: NSLT_RED =1;NSLT_GREEN=1;break;                                    // sets NSLT LED YELLOW
   }
}

void Set_EW(char color)
{
    switch (color)
   {
    case OFF: EW_RED =0;EW_GREEN=0;break;                                           // Turns off the EW LED
    case RED: EW_RED =1;EW_GREEN=0;break;                                           // Sets EW LED RED
    case GREEN: EW_RED =0;EW_GREEN=1;break;                                         // sets EW LED GREEN
    case YELLOW: EW_RED =1;EW_GREEN=1;break;                                        // sets EW LED YELLOW
   }
}

void Set_EWLT(char color)
{
    switch (color)
   {
    case OFF: EWLT_RED =0;EWLT_GREEN=0;break;                                       // Turns off the EWLT LED
    case RED: EWLT_RED =1;EWLT_GREEN=0;break;                                       // Sets EWLT LED RED
    case GREEN: EWLT_RED =0;EWLT_GREEN=1;break;                                     // sets EWLT LED GREEN
    case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break;                                    // sets EWLT LED YELLOW
   }
}

void PED_Control ( char Direction, char Num_Sec ) 
{
    
        
        if(Direction == 0)
        {   PORTD = 0xFF;                                                           // Clear the upper 7 Seg display
            
                for(char i = 1; i < Num_Sec; i++ )             
                {
                    DISPLAY_UPPER_7SEG(Num_Sec - i);                                // Countdown will begin at Num_sec - 1
                    Wait_One_Second_With_Beep(); 
                }
                
                
        
                PORTC = 0xFF;                                                       // Clear the lower 7 seg display 
                PORTEbits.RE0 = 1;
                Wait_One_Second_With_Beep(); 
        }
    
        if(Direction == 1)
        {   PORTC = 0xFF;                                                           // Clear the lower 7 seg display
            PORTEbits.RE0 = 1;
            
                for(char i = 1; i < Num_Sec; i++ )  
                {
                    DISPLAY_LOWER_7SEG( Num_Sec - i);
                    Wait_One_Second_With_Beep(); 

                }
                
                
             
                PORTD = 0xFF;
                Wait_One_Second_With_Beep(); 
    
        }   
     
        PORTC = 0xFF;                                                               // Clear the lower and upper 7 seg
        PORTEbits.RE0 = 1;
        PORTD = 0xFF;
}

void Day_Mode()
{
    
    
    Set_NS(GREEN);                                                                  // Set NS light green and the rest to red
                                        
    Set_EW(RED);
    Set_NSLT(RED);
    Set_EWLT(RED);
    
    if(NSPED_SW == 1)                                                               // Check for pedestrian request
    {
        PED_Control(0,8);                                                           // If true, start pedestrian counter
        
    }
    

    
        Wait_N_Seconds(7);
        
        Set_NS(YELLOW);
        Wait_N_Seconds(3);
        
        Set_NS(RED);
        
 
        
        if(EWLT_SW == 1)                                                            // Check left turn request in East West direction 
        {                                                                           // If true, start left turn request sequence 
            Set_EWLT(GREEN);
            Wait_N_Seconds(8);
            
            Set_EWLT(YELLOW);
            Wait_N_Seconds(3);
            
            Set_EWLT(RED);
            
        }
        
        Set_EW(GREEN);
        
        if( EWPED_SW == 1 )                                                         // Check pedestrian request
        {
            PED_Control(1,9);                                                       // If true, start pedestrian counter
        }
        
  
        
        Wait_N_Seconds(9);
        
        Set_EW(YELLOW);
        Wait_N_Seconds(3);
        
        Set_EW(RED);
        
   
        
        if(NSLT_SW == 1)                                                            // Check left turn request in East West direction
                                                                                    // If true, start left turn request sequence
        {           
            Set_NSLT(GREEN);
            Wait_N_Seconds(8);
            
            Set_NSLT(YELLOW);
            Wait_N_Seconds(3);
            
            Set_NSLT(RED);
            
        }
        

    
}

void Night_Mode() 
{
    
    Set_NS(GREEN);
    
    Set_EW(RED);
    Set_NSLT(RED);
    Set_EWLT(RED);
    
    Wait_N_Seconds(6);
    
    Set_NS(YELLOW); 
    Wait_N_Seconds(3);
    
    Set_NS(RED);
    
    
    if (EWLT_SW == 1)                                                               // Check left turn request in East West direction
                                                                                    // If true, start left turn request sequence 
    {
        Set_EWLT(GREEN);
        Wait_N_Seconds(6);
        
        Set_EWLT(YELLOW);
        Wait_N_Seconds(3);
        
        Set_EWLT(RED);
        
    }
    
    Set_EW(GREEN);
    Wait_N_Seconds(6);
    
    Set_EW(YELLOW);
    Wait_N_Seconds(3);
    
    Set_EW(RED);
    
   
    
    if ( NSLT_SW == 1 )                                                             // Check left turn request in North South direction
                                                                                    // If true, start left turn request sequence 
    {
        Set_NSLT(GREEN);
        Wait_N_Seconds(8);
        
        Set_NSLT(YELLOW);
        Wait_N_Seconds(3);
        
        Set_NSLT(RED);

    }
    
    
    
    
}

void DISPLAY_UPPER_7SEG(char Upper)
{
    PORTC = segDisplay[Upper] & 0x3F;
    char testbit = segDisplay[Upper] & 0x40;
    if( testbit == 0 ) PORTEbits.RE0 = 0; 
    else PORTEbits.RE0 = 1;
                                                               
//    int LED1 = segDisplay[Upper];                                                 // 7 seg Display Hex Code was mapped to Index
//    PORTC = LED1 & 0x3F;                                                          // Using AND to the 6th bit
//                          
//    int testbit = (LED1 & 0x40) >> 0x06;                                          // Keep the 6th bit and shift it to bit position 0
//                          
//    //PORTEbits.RE0 = testbit;                                                    // The bit in the 0 position will be sent to PORTE RE0
//   
 }

void DISPLAY_LOWER_7SEG(char Lower)
{

    int LED2 = segDisplay[Lower];                            

    PORTD = LED2;                                                                   // Display Value on 7 seg

}


