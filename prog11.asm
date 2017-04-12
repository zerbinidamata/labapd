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

            call	#leds
			jmp		$

leds:		; setando os leds
			mov.b	#0x0,P2DIR
			mov.b	#0x1,P1DIR
			mov.b	#0x80,P4DIR
			mov.b	#0x2,P1OUT
			mov.b	#0x2,P2OUT
			mov.b	#0x2,P1REN
			mov.b	#0x2,P2REN

loop:
			bit.b	#2,P1IN
			jnz		lb
			call	#debounce
			bit.b	#2,P1IN
			jz		S1

lb:
			bit.b	#2,P2IN
			jnz		loop
			call	#debounce
			bit.b	#2,P2IN
			jz		S2
			jmp		loop
			ret


S1:
			xor.b	#0x80,P4OUT
			jmp		led1

S2:
			xor.b	#0x1,P1OUT
			jmp		led2

debounce:
			mov		#0xFFF,R15

debounce_loop:

			dec		R15
			jnz		debounce_loop
			ret

led1:
			bit.b	#2,P1IN
			jnz		loop
			bit.b	#2,P2IN
			jz		save
			jmp		led1



led2:
			bit.b	#2,P2IN
			jnz		loop
			bit.b	#2,P1IN
			jz		save
			jmp		led2




save: ;salvar a saida dos leds na pilha(registrador deu ruim)

		push.b		P1OUT
		push.b		P2OUT
		bis.b		#1,P1OUT
		bic.b		#0x80,P4OUT

		mov			#0xFFFF,R5

lb1:
		dec			R5
		nop
		nop
		nop
		nop
		nop
		jnz			lb1
		xor.b		#1,P1OUT
		xor.b		#0x80,P4OUT
		bit.b		#2,P1IN
		jnz			lb2
		bit.b		#2,P1IN
		jnz			lb2
		bit.b		#2,P2IN
		jnz			lb2
		jmp			lb1

lb2:
		pop.b		P2OUT
		pop.b		P1OUT
		jmp 		loop
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
            
