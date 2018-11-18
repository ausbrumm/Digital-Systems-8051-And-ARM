public _BallDelay

MainSeg SEGMENT CODE

RSEG MainSeg

_BallDelay:
	mov r7, #255
	DelayLoop: 	mov r6, #40
				; mov r6, #20
	DelayLoop1:
				djnz r6, DelayLoop1
				djnz r7, DelayLoop
				ret
end
	