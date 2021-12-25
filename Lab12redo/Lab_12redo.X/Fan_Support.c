#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

extern char HEATER;
extern char duty_cycle;
extern char heater_set_temp;
extern signed int DS1621_tempF;

extern char FAN; 

int get_duty_cycle(signed int temp, int set_temp)
{
// add code to check if temp is greater than set_temp. If so, dc = 0. Else dc = 2 times of difference of set_temp and temp
// check if dc is greater than 100. If so, set it to 100
// return dc
    if (temp > set_temp) 
    {
        return 0; 
    }
    else 
    {
        int dc = 2 * (set_temp - temp);
        if (dc > 100) 
        {
            return 100; 
        }
        else 
        {
            return dc; 
        }
    }
}

void Monitor_Heater()
{
    duty_cycle = get_duty_cycle(DS1621_tempF, heater_set_temp);
    do_update_pwm(duty_cycle);
    if (HEATER == 1) 
    {
        FAN_EN = 1;
    }
    else FAN_EN = 0;
}

void Toggle_Heater()
{
// add code just to flip the variable FAN
    HEATER = (!HEATER);
    FAN_LED = !FAN_LED;

}

int get_RPM()
{
// add old code
    int RPS = TMR3L / 2; // read the count. Since there are 2 pulses per rev
                         // then RPS = count /2
    TMR3L = 0;           // clear out the count
    return (RPS * 60);   // return RPM = 60 * RPS
}

void Turn_Off_Fan()
{
    FAN = 0;
    HEATER = 0; 
    FAN_LED = 0;
     
}

void Turn_On_Fan()
{
    FAN = 1;
    do_update_pwm(duty_cycle);
    HEATER = 1;
    FAN_LED = 1; 
}









