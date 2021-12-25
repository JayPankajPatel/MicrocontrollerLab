#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include <p18f4620.h>
#include "utils.h"

extern char found;
extern char Nec_code1;
extern short nec_ok;
extern char array1[21];
extern char duty_cycle;

extern char colors[8]; 

char check_for_button_input()
{       
    if (nec_ok == 1)
    {
            nec_ok = 0;

            printf ("NEC_Code = %x\r\n", Nec_code1);
            INTCONbits.INT0IE = 1;          // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;        // Edge programming for INT0 falling edge

            found = 0xff;
            for (int j=0; j< 21; j++)
            {
                if (Nec_code1 == array1[j]) 
                {
                    found = j;
                    j = 21;
                }
            }
            
            if (found == 0xff) 
            {
                printf ("Cannot find button \r\n");
                return (0);
            }
            else
            {
                return (1);
            }
    }
}

char bcd_2_dec (char bcd)
{
    int dec;
    dec = ((bcd>> 4) * 10) + (bcd & 0x0f);
    return dec;
}

int dec_2_bcd (char dec)
{
    int bcd;
    bcd = ((dec / 10) << 4) + (dec % 10);
    return bcd;
}

void Do_Beep()
{
    Activate_Buzzer();
    Wait_One_Sec();
    Deactivate_Buzzer();
    Wait_One_Sec();
    do_update_pwm(duty_cycle);
}

void Do_Beep_Good()
{
// add code here using Activate_Buzzer_2KHz()
    Activate_Buzzer_2KHz();
    Wait_One_Sec();
    Deactivate_Buzzer();
    Wait_One_Sec();
    do_update_pwm(duty_cycle);
}

void Do_Beep_Bad()
{
// add code here using Activate_Buzzer_500Hz()
    Activate_Buzzer_500Hz();
    Wait_One_Sec();
    Deactivate_Buzzer();
    Wait_One_Sec();
    do_update_pwm(duty_cycle);
}

void Wait_One_Sec()
{
    for (int k=0;k<0xffff;k++);
}

void Activate_Buzzer()
{
    PR2 = 0b11111001; 
    T2CON = 0b00000101;
    CCPR2L = 0b01001010;
    CCP2CON = 0b00111100;
}

void Activate_Buzzer_500Hz()
{
    // add code here
    PR2 = 0b11111001;
    T2CON = 0b00000111;
    CCPR2L = 0b01111100;
    CCP2CON = 0b00111100;
}

void Activate_Buzzer_2KHz()
{
    // add code here
    PR2 = 0b11111001;
    T2CON = 0b00000101;
    CCPR2L = 0b01111100;
    CCP2CON = 0b00111100;
}

void Activate_Buzzer_4KHz()
{
    // add code here
    PR2 = 0b01111100;
    T2CON = 0b00000101;
    CCPR2L = 0b00111110;
    CCP2CON = 0b00011100;
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
	PORTBbits.RB3 = 0;
}

void do_update_pwm(char duty_cycle) 
{ 
	float dc_f;
	int dc_I;
	PR2 = 0b00000100 ;                      // Set the frequency for 25 Khz 
	T2CON = 0b00000111 ;                    // As given in website
	dc_f = ( 4.0 * duty_cycle / 20.0) ;     // calculate factor of duty cycle versus a 25 Khz signal
	dc_I = (int) dc_f;                      // Truncate integer
	if (dc_I > duty_cycle) dc_I++;          // Round up function
	CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
	CCPR1L = (dc_I) >> 2;
}

void Set_RGB_Color(char color)
{
    // add code here
    PORTA = colors[color];
    Wait_One_Sec(); 
    
}

float read_volt()
{
    // add code here
    int step = get_full_ADC(); 
    float volt = step * 5 / 1024.0; 
    return volt;
}

unsigned int get_full_ADC()
{
    int result; 
    ADCON0bits.GO=1;                                           // Start Conversion
    while(ADCON0bits.DONE==1);                                 // Wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                        // Combine result of upper byte and
                                                               // Lower byte into result   
    return result;                                             // Return the result.
}

void Init_ADC()
{
    // add code here
    ADCON0 = 0x01; 
    ADCON1 = 0x0E; 
    ADCON2 = 0xA9;
    
}

void Set_RPM_RGB(int rpm)
{
    if ( rpm == 0 ) 
    {
        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 0;
    }
    
    if ( (rpm > 0) && (rpm < 1800) ) 
    {
        PORTDbits.RD5 = 1;
        PORTDbits.RD6 = 0;
    }
    
    if ( (rpm >= 1800) && (rpm < 2700) ) 
    {
        PORTDbits.RD5 = 1;
        PORTDbits.RD6 = 1;
    }
    
    if ( (rpm >= 2700) ) 
    {
        // handout says to change value to blue, however blue is not hooked up
        // so i set it to red
        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 1;
    }
    
}


