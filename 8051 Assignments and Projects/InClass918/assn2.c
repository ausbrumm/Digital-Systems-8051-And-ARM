// Assn02 in C
#include<at89c51cc03.h>

void Delay();

void main(void)
	{
		while(1)
		{
			if(P3_4 == 0)
			{
				P1_0 = ~P1_0;
				Delay();
			}
		}
	}
	
void Delay()
	{
		unsigned char i, j;
		for(i = 0; i < 10; i++)
		{
			for(j = 0; j < 100; j++);
		}
	}