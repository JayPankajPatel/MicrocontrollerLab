#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
ORG 0x0000
START:
    MOVLW   0x0F	;Load W with 0x0F0
    MOVWF   ADCON1	;Make ADCON1 to be all digital
    
    MOVLW   0xFF
    MOVWF   TRISA	;Set PORTA as all inputs
    
    MOVLW   0x00	;Load W with 0x00
    MOVWF   TRISC	;Set PORTC as outputs
    
MAIN_LOOP:
    MOVF    PORTA, W	;Read from PORTA and move to W
    ANDLW   0x07	;Mask with 0x0F
    MOVWF   PORTC	;Move from W to PORTC
    
    GOTO    MAIN_LOOP	;Loop forever
END


