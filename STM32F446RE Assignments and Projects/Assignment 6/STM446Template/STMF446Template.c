#include "stm32f446.h"
/*stm446Template.c              July 1, 2017
  This program toggles a bit on PA7 as fast as possible
*/
void InitializeClock(void);
int main()
  {int i, tmp;
   //Clock bits
	 InitializeClock();	
	 RCC_AHB1ENR |= 1;         //Bit 0 is GPIOA clock enable bit 
   //I/O bits
   GPIOA_MODER |= 0x4000;    //Bits 15-14 = 01 for digital output on PA7
   //OTYPER register resets to 0 so it is push/pull by default
   GPIOA_OSPEEDER |= 0xC000;  //Bits 15-14 = 11 for high speed on PA7
   //PUPDR defaults to no pull up no pull down    
   //Main program loop
   tmp = 0;
   while(1)
     {GPIOA_ODR = tmp;       //Only PA7 is set up for output so other bits
      tmp = ~tmp;            //  have no effect.
//			for(i=0;i<10000;i++);
     }
  }
	//This function resets the system clock to 168 MHz.
  void InitializeClock()
      {RCC_CFGR = 0x00000000;         //Reset Clock Configuration Register
       RCC_CR &= 0xFEF6FFFF;          //Reset HSEON, CSSON and PLLON Bits
       RCC_CR |= (1 << 16);           //Turn on HSE clock
       while((RCC_CR & (1 << 17)) == 0); //Wait until HSE is ready
       RCC_CR |= (1 << 19);
       RCC_PLLCFGR = 0x27405A08;      //Set PLLP = 0, PLLN = 360, PLLM = 8, 
                                      //PLLQ = 7, PLL Src = HSE
       RCC_CR |= (1 << 24);           //Enable PLL on
       while((RCC_CR & (1 << 25)) == 0); //Wait for PLL to lock on
       RCC_CFGR = 0x9402;             // APB2/2, APB1/4, AHB/1 
       FLASH_ACR &= 0xFFFFFFF8;       //Set flash wait states to 5
       FLASH_ACR |= 0x5;          
      }
