// Austin Brummett
// Assignment 4
// October 13, 2017
//
// This program sets a potentiometer up to dim an LED
// It uses sets the LED up at P1.3 which is PWN0
// and the potentiomenter is set up at P1.0 which is ADC0
// 
//
#include <at89c51cc03.h> // CC03 library file

void main(void) //This is the start of the main program
 {
	 // Variable decleration
	 unsigned char temp; 


	  // PWN Stuff
	  CKCON = 0x01; // x2 mode
		CMOD = 0x02; // cpu clock/2
		CCON = 0x40; //Bit 6 in CCON turns on PCA timer
		CCAPM0 = 0x42; //Bit 6 enables the compare
	 
	  // ADC Stuff
  	ADCF  = 0x01;        // P1.0 = ADC[0]
    ADCON = 0x20;        // Enable ADC Function 
    ADCLK = 0x00;        // Prescaler to 0 
    EA = 0;              // Turn off interrupts
	 
    while(1)             // Loop Forever
     {
			ADCON &= 0xF8;     // Reset ADC Channel Select
      ADCON |= 0x00;     // Select ADC = Ch0            
      ADCON |= 0x20;     // Use Standard mode
			ADCON |= 0x08;     // Start ADC Convert
			CCAP0H = ADDH;		 // send MSB to PWN0
			temp = (ADCON & 0x10);    // Get done bit
      while(temp != 0x10)// Loop until complete
        temp = (ADCON & 0x10);	// set temp
      ADCON &= 0xEF;           // Clear ADEOC = 0

			}
}