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

	mov		#vetor,R5
	call	#bolha
	jmp			$

bolha:
	mov.b	@R5,R6
	inc		R5
	mov		R5,R8
	dec		R6

lb1:
	mov		R6,R7
	mov		R8,R5

lb2:
	call	#ord
	dec		R7
	jnz		lb2
	dec		R6
	jnz		lb1
	ret

ord:
	mov.b	@R5,R9
	inc		R5
	cmp.b	@R5,R9
	jlo		lb3
	mov.b	@R5,R10
	mov.b	R9,0(R5)
	dec		R5
	mov.b	R10,0(R5)
	inc		R5
lb3:
	ret

	.data
vetor:   .byte 13, 'M','A','R','I','A','N','A','R','A','F','A','E','L'

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
            
