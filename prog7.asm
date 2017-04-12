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

			mov		#vetor, R5
			call	#ord
			jmp 	$

ord:		mov 	R5, R8
			mov.b 	@R5,R6
			incd	R8
			dec		R6

lb1:
			mov		R6, R7
			mov 	R8, R5

lb2:
			jmp		bolha

lb3:
			dec		R7
			jnz		lb2
			dec		R6
			jnz		lb1
			ret

bolha:
			mov		@R5,R9
			incd 	R5
			mov		@R5,R10
			cmp		R9, R10
			jge		lb3
			decd	R5
			mov		R10,0(R5)
			incd 	R5
			mov		R9,0(R5)
			jmp		lb3
			nop

	.data
vetor: 	.word	8, 121, 234, 567, -1990, 117, 867, 45, -1980

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
            
