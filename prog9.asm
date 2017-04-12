;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

            call	#chaves
			jmp		$

chaves:		mov.b	#0x0,P2DIR ; output
			mov.b	#0x1,P1DIR ; input
			mov.b	#0x80,P4DIR ; P4.7 output
			mov.b	#0x2,P1OUT ; pull up
			mov.b	#0x2,P2OUT
			mov.b	#0x2,P1REN
			mov.b	#0x2,P2REN ; set pull up

loop:		mov.b	P1IN,R5 ; se está pressionado
			mov.b	P2IN,R6
			bit		#2,R5
			jz		led1
			mov.b	#0,P4OUT

lb1:		bit		#2,R6
			jz		led2
			mov.b	#2,P1OUT
			jmp		loop
			ret

led1:
			mov.b	#0x80,P4OUT ;acende led1
			jmp		lb1
led2:
			mov.b	#0x3,P1OUT ; acende led2
			jmp		loop
			nop

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
