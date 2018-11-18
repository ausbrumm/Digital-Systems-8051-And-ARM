// Austin Brummett
// Assignment 3
// September 27, 2017
//
// This program turns on all 4 leds seeminlgy at the same time, but
// they are blinking at 50 times a second,meaning they are blinking
// on and off every 5 msec
//
#include <at89c51cc03.h> // CC03 library file


// Function:BlinkRate
// Takes: 	unsigned char that is a loop variable
// Purpose: creates a software delay of x msec based on
//					input
// Returns: nothing
void BlinkRate(unsigned char x); 
//
void main(void) //This is the start of the main program
 {//Declare all variables here
 
	while(1)			// endless loop
	{
		if(P3_4 == 0)		// if switch is pushed
		{
			P1_0 = ~P1_0;   // set on = 0
			BlinkRate(20);	// blink
			P1_0 = 1; 			// set off = 1
			BlinkRate(20); // blink

			P1_1 = ~P1_1;	  // set on = 0
			BlinkRate(20);	// blink
			P1_1 = 1;				// set off = 1
			BlinkRate(20);  // blink
		
			P1_2 = ~P1_2;	  // set on = 0
			BlinkRate(20); 	// blink
			P1_2 = 1;				// set off = 1
			BlinkRate(20);	//blink

			P1_3 = ~P1_3;	  // set on = 0
			BlinkRate(20);	// blink
			P1_3 = 1;				// set off = 1
			BlinkRate(20);	// blink
			
		}		
	}
 }
//
void BlinkRate(unsigned char x)
{
	unsigned char lp,lp2;	// create loop variables
	for(lp = 0; lp < x; lp++) // increment up the first loop
		for(lp2 = x; lp2 > 0; lp2--); // decrement the second loop
}