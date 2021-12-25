#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#define delay 5
// Prototype Area to place all the references to the routines used in the program
void Init_ADC(void);
unsigned int Get_Full_ADC(void);
void Flash_LED(unsigned char);
void main(void)
{
    unsigned char ADC_Result; // local variable to store the result
    Init_ADC();               // initialize the A2D converter
    TRISB = 0x00;             // make PORTB as all outputs
    while (1)
    {
        ADC_Result = Get_Full_ADC(); // call routine to measure the A2D port
        Flash_LED(ADC_Result);       // call routine to flash the LED based on the delay
                                     // indicated by ADC_Result
    }
}
void Init_ADC(void)
{
    ADCON0 = 0x01; // select channel AN0, and turn on the A2D subsystem
    ADCON1 = 0x0E; // set pin 2 as analog signal, VDD-VSS as reference voltage
                   // and right justify the result
    ADCON2 = 0xA9; // Set the bit conversion time (TAD) and acquisition time
}
unsigned int Get_Full_ADC(void)
{
    int result;
    ADCON0bits.GO = 1;                  // Start Conversion
    while (ADCON0bits.DONE == 1)
        ;                               // Wait for conversion to be completed (DONE=0)
    result = (ADRESH * 0x100) + ADRESL; // Combine result of upper byte and lower byte into
    return result;                      // return the most significant 8- bits of the result.
}
void Flash_LED(unsigned char ADC_result)
{
    unsigned int counter1, counter2;
    LATB = 0x0A; // output to PORTB the pattern 00001010
                 // delay loop
    for (counter2 = delay; counter2 > 0; --counter2)
    {
        for (counter1 = ADC_result; counter1 > 0; --counter1);
    }
    LATB = 0x05; // output to PORTB the pattern 00000101
                 // delay loop
    for (counter2 = delay; counter2 > 0; --counter2)
    {
        for (counter1 = ADC_result; counter1 > 0; --counter1);
    }
}