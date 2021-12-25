; THIS SECOND ASSEMBLY LANGUAGE PROGRAM WILL READ THE VALUES OF
; ALL THE BITS 0-3 OF PORT A AND OUTPUT THEM
; TO THE PINS 0 THROUGH 3 OF PORT B
#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
ORG     0x0000
START:
    MOVLW   0x0F	    ; Load W with 0x0F0
    MOVWF   ADCON1	    ; Make ADCON1 to be all digital

    MOVLW   0xFF	    ; Load W with 0xFF
    MOVWF   TRISA	    ; Set PORT A as all inputs

    MOVLW   0x00	    ; Load W with 0x00
    MOVWF   TRISB	    ; Make PORT B as outputs
MAIN_LOOP:	            ; Start of While LOOP
    MOVF    PORTA, W	    ; Read from PORT A and move it into W
    ANDLW   0x0F	    ; Mask with 0x0F
    
    MOVWF   PORTB	    ; Move from W to PORT B
    GOTO    MAIN_LOOP	    ; Loop forever
END
