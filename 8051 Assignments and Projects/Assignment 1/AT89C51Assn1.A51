; Austin Brummett
; Assignment 1
; Sept 6, 2017
;
; AT89C51Assn1.A51
; This program should turn on the LED at P1.0 & P1.1 when P3.6 or P3.7
; are closed, respectively, and when both are closed both lights blink at
; a blink rate @ 10 blinks/second
;  P3.7 switch P3.6 switch P1.0 LED P1.1 LED
;    open 1		open 1 		off 	off
;	 open 1		closed0		off 	on
; 	 closed0 	open1 		on 		off
; 	 closed0 	closed0		blinks 	blink

MainSeg SEGMENT CODE
	;
	CSEG at 0h					; Start code segment at 0
		ljmp Start				; Jump to Start
	RSEG MainSeg
		Start: 	
		loop:					
		JNB P3.6, Switch_36 ; Jump to Switch_3.6 when it P3.6 is 0
		JNB P3.7, Switch_37 ; Jump to Switch_3.7 when it P3.7 is 0
		jmp loop
		
		Switch_36:
		JNB P3.7, BothSwitches ; Jump to BothSwitches when it is 0
		mov P1, #1				; Set P1 to 00000001 to turn on P1.1
		mov P1, #3			   ; Set P1 to 00000011 to turn on P1.1
		jmp loop			   ; Jump back to the start of the loop
		
		Switch_37:
		JNB P3.6, BothSwitches ; Jump to BothSwitches when it is 0
		mov P1, #2			   ; Set P1 to 00000010 to turn on P1.0 
		mov P1, #3			   ; Set P1 to 00000011 to turn off P1.0 
		jmp loop			   ; Jump back to the start of the loop
		
		BothSwitches:
		ACALL WAIT			  ; Delay off
		mov P1, #4			  ; Turn both switches on
		ACALL WAIT			  ; Delay on
		mov P1, #3	          ; Turn off switches
		jmp loop			  ; jump tp start of loop
		
		WAIT:  
		mov R4,#255H			  ; Set R4 to 0001
		
		WAIT1: 
		mov R3,#00H			  ; Set R3 to 0000
		WAIT2: 
		mov R2,#00H			  ; Set R2 to 0000
		WAIT3:  
		djnz R2,WAIT3		  ; R2 <- R2-1, not 0 jump to Wait 3 256 
		djnz R3,WAIT2		  ; R3 <- R3-1, not 0 jump to Wait 2 256 
		djnz R4,WAIT1		  ; R4 <- R3-1, not 0 jump to Wait 1 255
		ret				  ; Return from Wait 3
		
End