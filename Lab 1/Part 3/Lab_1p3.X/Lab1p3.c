#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
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
// Prototype Area
void Init_ADC(void);
unsigned int Get_Full_ADC(void);
float Read_Ch_Volt(char);
void Init_ADC(void)
{
ADCON0=0x01; // select channel AN0, and turn on the ADDC subsystem
ADCON1=0x0a; // set pins 2,3,4,5 & 7 as analog signal, VDD-VSS as ref voltage
ADCON2=0xA9; // Set the bit conversion time (TAD) and acquisition time
}
unsigned int Get_Full_ADC(void)
{
int result;
ADCON0bits.GO=1; // Start Conversion
while(ADCON0bits.DONE==1); // Wait for conversion to be completed (DONE=0)
result = (ADRESH * 0x100) + ADRESL; // Combine result of upper byte and lower byte into
return result; // return the most significant 8- bits of the result.
}
float Read_Ch_Volt(char ch_num)
{
ADCON0 = ch_num * 0x4 + 1;
int ADC_Result = Get_Full_ADC();
float Volt = ADC_Result / 1024.0 * 5.0;
return (Volt);
}
void main(void)
{
float Volt;
init_UART();
Init_ADC();
while(1)
{
Volt = Read_Ch_Volt( 0);
printf ("Volt at AN0 is %f\r\n", Volt);
Volt = Read_Ch_Volt( 1);
printf ("Volt at AN1 is %f\r\n", Volt);
Volt = Read_Ch_Volt( 2);
printf ("Volt at AN2 is %f\r\n", Volt);
Volt = Read_Ch_Volt( 4);
printf ("Volt at AN4 is %f\r\n\n", Volt);
Volt = Read_Ch_Volt( 3);
printf ("Reference voltage at AN3 is %f\r\n\n", Volt);
}
}