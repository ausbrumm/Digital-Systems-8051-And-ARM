public _PaddleDelay
	
MainSeg SEGMENT CODE

RSEG MainSeg

_PaddleDelay:
	mov a, r7;
	DelayLoop:
				djnz r7, DelayLoop
				ret
	ret
end
	
	
	