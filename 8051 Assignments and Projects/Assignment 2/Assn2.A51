; Austin Brummett
; Assignment 2
; Sept 18, 2017
;
; Assn2.A51
; This program should turn on the speaker at P1.0 when
; the switch at 3.4 is pressed

MainSeg SEGMENT CODE
	;
	OutBit MACRO x	; output a bit to Port 1 to turn it on
	mov a, #x		; 
	mov P1, a
	clr a
	endm
CSEG at 0h				; Start code segment at 0
	jmp START				; Jump to Start
RSEG MainSeg
	START:
	loop:
	JNB P3.4, Switch_34 ; Jump to Switch_34 when it P3.4 is 0
	jmp loop ; go back to the loop
	Switch_34:
	; Get those cycles for the delay
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	
	OutBit 254 ; turn Port 1 on
	
	; Get those cycles for the delay
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	acall WAIT
	
	OutBit 255 ; turn Port 1 off
	jmp loop
	
	WAIT:	
	MOV P2,0x00
	MOV A,0x00
	Lp: MOV R6,#0h
	LOOP1:DJNZ R6,LOOP1
      CPL A
      MOV P2,A
      ret
     
		
END 				
