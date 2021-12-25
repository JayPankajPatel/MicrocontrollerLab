#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"
#include "stdio.h"

unsigned char bit_count;
unsigned int Time_Elapsed;

extern unsigned char Nec_state;
extern short nec_ok;
unsigned long long Nec_code;
extern char Nec_code1;

void init_INTERRUPT()
{

}

void interrupt  high_priority chkisr() 
{    
 
    
}

void TIMER1_isr(void)
{

}

void force_nec_state0()
{

}

void INT0_isr() 
{    

}






