// Austin Brummett
// Project 1: 8051 Pong
// Date Assigned: August 23,2017
// Date Due: October 20, 2017
//
// This Program runs a variant of Pong
// Port Descriptions
// P1.5 = Player 1 Paddle
// P1.6 = Player 2 Paddle
// P1.0-P0.3 Decoder 1 controls column 1
// P1.0-1.2, P1.4 Decoder 2 controls column 2
// P3.0-P3.7 Driver Controls Rows

#include <at89c51cc03.h> // CC03 library file
//Declare any global variable here
unsigned char ballRow;
unsigned char ballCol;
unsigned char ballDir ;


//Subprograms
void paddlesRus();
void aBallisOkayIGuess();
void BlinkRate(unsigned char); // C Delay for the ball
extern void BallDelay(unsigned int); //assembly delay for balls
extern void PaddleDelay(unsigned int); // assembly delay for paddles
void movePaddles();



//
void main(void) //This is the start of the main program
{
	unsigned char counter = 0;
	ballDir = 1;
	ballRow = 4;
	ballCol = 3;
	TMOD = 0x01;    //Timer 0 mode = not gated, internal clock, 16-bit mode
                    //Timer 1 mode = not gated, internal clock, 16-bit mode
	TH0 = 0xB1;         //Timer 0 set to B1E0 = 45536
	TL0 = 0xE0;
	EA=1;
	ET0 = 1;
	
	// loop forever
	while(1)
		{
		paddlesRus();	// set paddles			
		aBallisOkayIGuess(); // add & bounce the ball
	}
	
}

//creates paddles
void paddlesRus()
{
		//set up left paddle
		P3 = 0x01;
		P1 = 0x0b;
		//PaddleDelay(1);
		BlinkRate(10);
		P1 = 0x0c;
	BlinkRate(10);
		//PaddleDelay(1);
		P1 = 0x0d;
	BlinkRate(10);
		//PaddleDelay(1);

		//set up right paddle
	  P3 = 0x80;
	  P1 = 0x13;		  
	BlinkRate(10);
		//PaddleDelay(1);
	  P1 = 0x14;
	BlinkRate(10);
	  //PaddleDelay(1);
		P1 = 0x15;
		BlinkRate(10);
		//PaddleDelay(1);
	
	//	movePaddles();
}
void aBallisOkayIGuess()
{
	
	unsigned char t = 0x01;
	
	if(ballDir == 1)
	{
		if((ballCol <= 7) )
		{
			P1 = ballRow;
			P1_3 = 1;
			t =  t << ballCol;
			P3 = t;
			ballCol++;
		}
		else if(ballCol >= 8)
		{
			P1 = ballRow;
			P1_4 = 1;
			t = t << (ballCol-8);
			P3 = t;
			ballCol++;
		
		if(ballCol == 15)
		{
			ballDir = ~ballDir;
		}
	}
	}
	else
	{
		if(ballCol >= 8)
		{
			P1 = ballRow;
			P1_4 = 1;
			t = t << (ballCol-8);
			P3 = t;
			ballCol--;
		}
		else if(ballCol <= 7)
		{
			P1 = ballRow;
			P1_3 = 1;
			t = t << ballCol;
			P3 = t;
			ballCol--;
		
		if(ballCol == 0)
		{
			ballDir = ~ballDir;
		}
	}
	}
	BallDelay(1);
}
	
void BlinkRate(unsigned char x)
{
	unsigned char lp,lp2;	// create loop variables
	for(lp = 0; lp < x; lp++) // increment up the first loop
		for(lp2 = x; lp2 > 0; lp2--); // decrement the second loop
}		

void movePaddles()
{
	unsigned char tmp;
	ADCF = 0x20; // P1.4 = ADC[4]
	ADCON = 0x20; // Enable ADC Function
	EA =  0;
 ADCON &= 0xF8; // Reset ADC Channel Select
 ADCON |= 0x20; // Select ADC = Ch0
 ADCON |= 0x20; // Use Standard mode
 ADCON |= 0x08; // Start ADC Convert
	if(ADDH + ADDL <= 0x85)
	{
		P1 = P1++;
	}
	else if(ADDH + ADDL > 0x85 | ADDH+ADDL <=0xAA)
		P1 = P1--;
	else if(ADDH+ADDL > 0xAA)
		P1 = P1--;
 ADCON &= 0xEF; // Clear ADEOC = 0
 
	
	ADCF = 0x40; // P1.5 = ADC[5]
	ADCON = 0x20; // Enable ADC Function
	EA = 0;
ADCON &= 0xF8; // Reset ADC Channel Select
 ADCON |= 0x40; // Select ADC = Ch0
 ADCON |= 0x20; // Use Standard mode
 ADCON |= 0x08; // Start ADC Convert
 	if(ADDH + ADDL <= 0x85)
		P1 = ADDH + ADDL;
	else if(ADDH + ADDL > 0x85 | ADDH+ADDL <=0xAA)
		P1 = ADDH+ADDL;
	else if(ADDH+ADDL > 0xAA)
		P1 = ADDH+ADDL;
 ADCON &= 0xEF; // Clear ADEOC = 0
 
 ADCON &= 0xEF; // Clear ADEOC = 0
 
}
