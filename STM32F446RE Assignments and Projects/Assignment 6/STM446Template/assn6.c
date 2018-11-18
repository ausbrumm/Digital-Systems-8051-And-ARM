#include "stm32f446.h"
/*assn6.c
   this program controls 6 LEDS via a potentiometer to show an approximation of the voltage
	 PB0-PB5 = LED output
	 PA5 = Analog Potentiometer Input
*/
void InitializeClock(void);
int main()
{	
	
		RCC_AHB1ENR |= 3;         //Bit 1&2 is GPIOA&B clock enable bits
	  RCC_APB2ENR |= 0x100;		// turnson ADC1 Clock
		
		GPIOA_MODER |= 0xC00;		// set PA5 to analog
	  GPIOB_MODER |= 0x5555;   // set PB0-PB3,PB5-PB7 to output
		GPIOA_PUPDR &= 0xFFFFF0FF;		// PA5 no PUPD

		
		ADC1_CR2 |= 1;					// turn ADC on
		ADC1_CR2 |= 0x400;			// allows EOC after conversion
		ADC_CCR  |= 0x30000;		//bits16-17 = 11 so clock divided by 8
		ADC1_SQR3 |= 0x5;				// channel set to 5 which is PA5
		
		while(1)
		{
			ADC1_CR2 |= 0x40000000;  // software start of A/D conversion
			while((ADC1_SR&0x2)==0); // bit 1 is end of conversion
			
			if(ADC1_DR >0x000 && ADC1_DR <=0x2AB){
				GPIOB_ODR = 0xFFFE;
			}
			if(ADC1_DR >= 0x2AC && ADC1_DR <= 0x557){
				GPIOB_ODR = 0xFFFC;
				for(int j = 0; j<100000;j++);
				GPIOB_ODR= 0xFFFF;
			}
			if(ADC1_DR >= 0x558 && ADC1_DR <= 0x802){
				GPIOB_ODR = 0xFFF8;
				for(int j = 0; j<100000;j++);
				GPIOB_ODR = 0xFFFF;
			}
			if(ADC1_DR >= 0x803 && ADC1_DR <= 0xAAE){
				GPIOB_ODR = 0xFFB8;
				for(int j = 0; j<100000;j++);
				GPIOB_ODR = 0xFFFF;
			}
			if(ADC1_DR >= 0xAAF && ADC1_DR <= 0xD5A){
				GPIOB_ODR = 0xFF38;
				for(int j = 0; j<100000;j++);
				GPIOB_ODR = 0xFFFF;
			}
			if(ADC1_DR >= 0xD5B && ADC1_DR <= 0xFFF){
				GPIOB_ODR = 0xFF18;
				for(int j = 0; j<100000;j++);
				GPIOB_ODR = 0xFFFF;
			}
		}
		
				
		}
	//This function resets the system clock to 168 MHz.
  void InitializeClock()
      {RCC_CFGR = 0x00000000;         //Reset Clock Configuration Register
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
