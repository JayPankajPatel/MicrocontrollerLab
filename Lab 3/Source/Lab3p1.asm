; THIS FIRST ASSEMBLY LANGUAGE PROGRAM WILL FLASH AN LED CONNECTED
; TO THE PINS 0 THROUGH 3 OF PORT C
#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
; Constant declarations
Delay1 equ 0xFF
Delay2 equ 0xFF
ORG 0x0000
; CODE STARTS FROM THE NEXT LINE
START:
    MOVLW	0x0F	           ; Load W with 0x0F0
    MOVWF	ADCON1	           ; Make ADCON1 to be all digital

    MOVLW	0x00	           ; Load W with 0x00
    MOVWF	TRISB	           ; Make PORT B as outputs
MAIN_LOOP:     
    MOVLW	0x05	           ; Load W with value 0x05
    MOVWF	PORTB	           ; Output to PORT B

    CALL	DELAY_ONE_SEC      ; delay one second
    MOVLW	0x0A	           ; Load W with value 0x0A
    MOVWF	PORTB	           ; Output to PORT B (flipping the LEDs)
    CALL	DELAY_ONE_SEC      ; delay one second

    GOTO	MAIN_LOOP	       ; go back to repeat the main loop
DELAY_ONE_SEC:
    MOVLW	Delay1	           ; Load constant Delay1 into W
    MOVWF	0x28	           ; Load W to memory 0x21
LOOP_1_OUTER:
    NOP		                   ; Do nothing
    MOVLW	Delay2	           ; Load constant Delay2 into W
    MOVWF	0x29	           ; Load W to memory 0x29
LOOP_1_INNER:
    NOP		                   ; Do nothing
    DECF	0x29,F	           ; Decrement memory location 0x20
    BNZ	    LOOP_1_INNER       ; If value not zero, go back to
    DECF	0x28,F	           ; Decrement memory location 0x28
    BNZ	    LOOP_1_OUTER       ; If value not zero, go back to
RETURN
END


