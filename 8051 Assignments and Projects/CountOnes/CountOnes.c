//CountOnes
#include<at89c51cc03.h>
void main(void)
{
	unsigned char count = 0;
	unsigned char x, i;
	x = P1;
	for(i = 0; i < 8; i++)
	{
		if((x & 0x80) != 0)
			count++;
		x = x << 1;
	}
	P2 = count;
	while(1);
}