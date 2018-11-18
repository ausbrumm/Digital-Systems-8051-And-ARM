// Austin Brummett
// pong.c
// December 4, 2017
#include "stm32f446.h"

// PA5 -> Potentiometer 1
// PA6 -> Potentiometer 2
// PB0-PB2 -> Decoder 1 & 2 input to set rows
// PB5 & PB6 -> Decoder 1 & Decoder 2 Enable,respectively
// PC0-7 -> Sets columns


void player1Move(void); // checks player one's position
void player2Move(void); // checks player two's position
void Paddles(void); //initializes paddles to starting position
void InitializeClock(void); // reset clock speed to 168 MHz
void Delay(void); // simple delay function

int main()
{
	RCC_AHB1ENR |= 0x111;			// Enable GPIOA,B, and C
	RCC_APB2ENR |= 0x111; // Enable bit 8 ADC1 Clock

	GPIOA_MODER &= 0x3C00; // set PA5 & PA6 up for analog input
	GPIOB_MODER &= 0x1415;  // set PB0-2 & PB5-6 up for output 
	GPIOC_MODER &= 0x5555; // set up PC0-7 for output
	
	Paddles();
}
void Paddles()
{
	GPIOC_ODR = 0x0000;
	GPIOB_ODR = 0xC30;
	Delay();
	GPIOB_ODR = 0x0000;
}
void player1Move()
{
	ADC1_CR2 |= 1;
	ADC1_CR2 |= 0x400;
	ADC_CCR |= 0x30000;
	ADC1_SQR3 |= 0;
}

void player2Move()
{
	ADC2_CR2 |= 1;
	ADC2_CR2 |= 0x400;
	ADC_CCR |= 0x30000;
	ADC2_SQR3 |= 1;
}

 void InitializeClock()
{
	 RCC_CFGR = 0x00000000;         //Reset Clock Configuration Register
	 RCC_CR &= 0xFEF6FFFF;          //Reset HSEON, CSSON and PLLON Bits
	 RCC_CR |= (1 << 16);           //Turn on HSE clock
	 while((RCC_CR & (1 << 17)) == 0); //Wait until HSE is ready
	 RCC_CR |= (1 << 19);
	 FLASH_ACR &= 0xFFFFFFF8;       //Set flash wait states to 5
	 FLASH_ACR |= 0x5;  
	 RCC_PLLCFGR = 0x27405A08;      //Set PLLP = 0, PLLN = 360, PLLM = 8, 
																	//PLLQ = 7, PLL Src = HSE
	 RCC_CR |= (1 << 24);           //Enable PLL on
	 while((RCC_CR & (1 << 25)) == 0); //Wait for PLL to lock on
	 RCC_CFGR = 0x9402;             // APB2/2, APB1/4, AHB/1 

}

void Delay()
{
	for(int i =0; i < 100000; i++);
}
