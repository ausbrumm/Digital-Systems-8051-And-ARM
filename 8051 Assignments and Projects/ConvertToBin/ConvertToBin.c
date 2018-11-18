//Convert to BCD
#include<at89c51cc03.h>

void main(void)
{
	unsigned char xBCD,xBinary;
	xBCD = P1;
	xBinary = (xBCD >> 4)*10 +(xBCD &0x0f);
	P2 = xBinary;
	while(1);
}