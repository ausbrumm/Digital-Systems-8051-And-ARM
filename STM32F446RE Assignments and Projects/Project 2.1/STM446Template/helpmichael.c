#include "stm32f446.h"


unsigned int LMatrixRow[8] = {0x020, 0x0A0, 0x120, 0x1A0, 0x220, 0x2A0, 0x320, 0x3A0};
unsigned int RMatrixRow[8] = {0x040, 0x0C0, 0x140, 0x1C0, 0x240, 0x2C0, 0x340, 0x3C0};
unsigned int Column[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
void Setup(void);
void Delay(void);
void Paddles(void);


int main(){
	Setup();
	while(1){
		Paddles();
	}
}


void Setup(){
	RCC_AHB1ENR |= 7;
	RCC_APB2ENR |= 0x300;
	GPIOA_MODER |= 0x3C00;
	GPIOB_MODER |= 0x55400;
	GPIOC_MODER |= 0x05555;
	ADC1_CR2 |= 1; 
	ADC2_CR2 |= 1;
	ADC1_CR2 |= 400;
	ADC2_CR2 |= 400;
	ADC_CCR |= 0x30000;
	ADC1_SQR3 |= 0x5;
	ADC2_SQR3 |= 0x6;
}

void Delay(){
	for(int i = 0; i < 200; i++);
}


void Paddles(){

	ADC1_CR2 |= 0x400000000;
	while((ADC1_SR & 0x2) == 0);
	if(ADC1_DR > 0x0 && ADC1_DR < 0x200){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[0];
		Delay();
		GPIOB_ODR = RMatrixRow[1];
		Delay();
	}
	else if(ADC1_DR >= 0x200 && ADC1_DR < 0x400){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[0];
		Delay();
		GPIOB_ODR = RMatrixRow[1];
		Delay();
		GPIOB_ODR = RMatrixRow[2];
		Delay();
	}
	
	else if(ADC1_DR >= 0x400 && ADC1_DR < 0x600){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[1];
		Delay();
		GPIOB_ODR = RMatrixRow[2];
		Delay();
		GPIOB_ODR = RMatrixRow[3];
		Delay();
	}
		
	else if(ADC1_DR >= 0x600 && ADC1_DR < 0x800){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[2];
		Delay();
		GPIOB_ODR = RMatrixRow[3];
		Delay();
		GPIOB_ODR = RMatrixRow[4];
		Delay();
	}
	
	else if(ADC1_DR >= 0x800 && ADC1_DR < 0xA00){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[3];
		Delay();
		GPIOB_ODR = RMatrixRow[4];
		Delay();
		GPIOB_ODR = RMatrixRow[5];
		Delay();
	}
	
	else if(ADC1_DR >= 0xA00 && ADC1_DR < 0xC00){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[4];
		Delay();
		GPIOB_ODR = RMatrixRow[5];
		Delay();
		GPIOB_ODR = RMatrixRow[6];
		Delay();
	}
	
	else if(ADC1_DR >= 0xC00 && ADC1_DR < 0xE00){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[5];
		Delay();
		GPIOB_ODR = RMatrixRow[6];
		Delay();
		GPIOB_ODR = RMatrixRow[7];
		Delay();
	}
	
	else if(ADC1_DR >= 0xE00 && ADC1_DR < 0xFFF){
		GPIOC_ODR = Column[7];
		GPIOB_ODR = RMatrixRow[6];
		Delay();
		GPIOB_ODR = RMatrixRow[7];
		Delay();
	}
	GPIOB_ODR = 0x000;
	GPIOC_ODR = 0x000;
	
	ADC2_CR2 |= 0x40000000;
	while((ADC2_SR & 0x2) == 0);
	if(ADC2_DR > 0x0 && ADC2_DR < 0x200){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[0];
		Delay();
		GPIOB_ODR = LMatrixRow[1];
		Delay();
	}
	
	else if(ADC2_DR >= 0x200 && ADC2_DR < 0x400){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[0];
		Delay();
		GPIOB_ODR = LMatrixRow[1];
		Delay();
		GPIOB_ODR = LMatrixRow[2];
		Delay();
	}
	
	else if(ADC2_DR >= 0x400 && ADC2_DR < 0x600){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[1];
		Delay();
		GPIOB_ODR = LMatrixRow[2];
		Delay();
		GPIOB_ODR = LMatrixRow[3];
		Delay();
	}
		
	else if(ADC2_DR >= 0x600 && ADC2_DR < 0x800){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[2];
		Delay();
		GPIOB_ODR = LMatrixRow[3];
		Delay();
		GPIOB_ODR = LMatrixRow[4];
		Delay();
	}
	
	else if(ADC2_DR >= 0x800 && ADC2_DR < 0xA00){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[3];
		Delay();
		GPIOB_ODR = LMatrixRow[4];
		Delay();
		GPIOB_ODR = LMatrixRow[5];
		Delay();
	}
	
	else if(ADC2_DR >= 0xA00 && ADC2_DR < 0xC00){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[4];
		Delay();
		GPIOB_ODR = LMatrixRow[5];
		Delay();
		GPIOB_ODR = LMatrixRow[6];
		Delay();
	}
	
	else if(ADC2_DR >= 0xC00 && ADC2_DR < 0xE00){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[5];
		Delay();
		GPIOB_ODR = LMatrixRow[6];
		Delay();
		GPIOB_ODR = LMatrixRow[7];
		Delay();
	}
	
	else if(ADC2_DR >= 0xE00 && ADC2_DR < 0xFFF){
		GPIOC_ODR = Column[0];
		GPIOB_ODR = LMatrixRow[6];
		Delay();
		GPIOB_ODR = LMatrixRow[7];
		Delay();
	}
	GPIOB_ODR = 0x000;
	GPIOC_ODR = 0x000;
	
//	GPIOC_ODR = Column[7];
//	GPIOB_ODR = RMatrixRow[3];
//	Delay();
//	GPIOB_ODR = RMatrixRow[4];
//	Delay();
//	GPIOB_ODR = RMatrixRow[5];
//	Delay();
//	GPIOC_ODR = 0;
//	Delay();
//	GPIOC_ODR = Column[0];
//	GPIOB_ODR = LMatrixRow[3];
//	Delay();
//	GPIOB_ODR = LMatrixRow[4];
//	Delay();
//	GPIOB_ODR = LMatrixRow[5];
//	Delay();
//	GPIOC_ODR = 0;
}
