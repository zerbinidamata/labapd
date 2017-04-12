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

                   mov 	 	#matr, R6
          call 		#W16_ASC
          jmp 		$

W16_ASC:
          mov		#0x2403,R5
          mov		#4, R7

lb1:
          mov		#4, R8
          mov		#0,R9

loop:
          rra		R6
          rrc		R9
          dec		R8
          jnz		loop
          swpb		R9
          rra		R9
          rra		R9
          rra		R9
          rra		R9

lb2:
          mov		#0x30,R10
          cmp.b		#0xA,R10
          jl		lb3
          add.b		#7,R10

lb3:
	      add.b		R9,R10
          mov.b		R10,0(R5)
          dec		R5
          dec		R7
          jnz		lb1
          ret

          .data
matr: .set 15015

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
            
