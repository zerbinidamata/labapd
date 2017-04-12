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

            call	#conta
			jmp		$

conta:
			mov.b	#0x1,P1DIR ;input
			mov.b	#0x80,P4DIR ; p4.7 output
			mov.b	#2,P1OUT ;pull up
			mov.b	#0,P4OUT
			mov.b	#2,P1REN
			mov		#0,R5 ; contador do led

loop: ; rodar ate estar pressionado
			mov.b	P1IN,R6; entrada
			bit		#2,R6 ; nao usa and pra nao apagar o conteudo de r6
			jz		press; se ta pressionado
			jmp		loop

press:
			inc		R5
			bit		#3,R5 ; ver se os dois primeiros bits estao setados
			jz		none ; se nao tiverem, vai pra none, que representa nenhuma chave pressionada
			bit		#2,R5 ; se o segundo bit esta setado
			jz		one; se nao estiver, apenas um esta ativo
			bit		#1,R5 ; ver se o primeiro bit esta setado
			jz		two ; se nao estiver, o o segundo esta ativo ou nenhum
			bis.b	#0x80,P4OUT ; acender o led p4.7
			jmp 	pullup

none:
			bic.b	#1,P1OUT ; desliga
			bic.b	#0x80,P4OUT ;desliga
			jmp		pullup

one:
			bis.b 	#0x80,P4OUT ;acende
			jmp		pullup

two:
			bic.b	#0x80,P4OUT
			bis.b	#1,P1OUT

pullup:
			mov.b	P1IN,R6
			bit		#2,R6
			jnz		cont; se continuar pressionado entra no contador
			jmp 	pullup

cont:
			mov		#0xFFF,R7
cont_loop:
			dec		R7
			jnz		cont_loop
			jmp		loop
			ret





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
            
