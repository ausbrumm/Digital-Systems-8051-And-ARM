// Austin Brummett
// pong.c
// December 4, 2017
#include "stm32f446.h"
#include <stdlib.h>

// PA5 -> Potentiometer 1 P1
// PA6 -> Potentiometer 2 P2
// PA7 -> Potentiometer 3 Mode select
// PB5-7 -> Decoder 1 & 2 input to set rows
// PB8 & PB9 -> Decoder 1 & Decoder 2 Enable,respectively
// PC0-7 -> Sets columns

// variables
unsigned int Col[] = 
			{0x80, 0x40, 0x20, 0x10, 0x08,0x04, 0x02, 0x01}; // array of column values
																											 // right -> left
unsigned int RowL[] = 
			{0x100,0x120,0x140,0x160,0x180,0x1A0,0x1C0,0x1E0};//Left rows bottom->top
unsigned int RowR[] = 
			{0x200,0x220,0x240,0x260,0x280,0x2A0,0x2C0,0x2E0};//right rows bottom->top
unsigned int PaddlePos1,PaddlePos2,CPaddlePos; // current position of paddles, 
																							 //CPaddle for Computer
unsigned int Score[] =
			{0x1, 0x2, 0x3};		// score values
int p1Score, p2Score; // player scores
int up = 0, down = 0; // Control Ball direction
unsigned int ballRow;	// ball starting row

// Functions
void Game(void);	// run game with two players
void Setup(void); // Set up ports and ADC
void Paddles(void); // initializes paddles to starting position
void Paddle1(void); // rhs paddle
void Paddle2(void); // lhs paddle
void moveBall(void); // move ball and intialize paddles
void score1(void);	// player 1 score
void score2(void);  // player 2 score
void Delay(int); // simple Delay, approximately 1.1ms*x
void Delay20ms(void); // Delays for approx.20ms

								
int main(){
	
	ballRow = 4; //initialize ball row
	//intialize player scores
	p1Score = 0; 
	p2Score = 0;
	Setup();	// setup ports
	while(1){
	Game();
	}		
}

// Runs 2 player game
void Game(){
	Paddles(); // setup paddles
	moveBall(); // ball game
}

// sets up ADC & Output Ports
void Setup(){
	RCC_AHB1ENR |= 7;			// Enable GPIOA,B, and C
	RCC_APB2ENR |= 0x300; // Enable bit 8/9/10 ADC1/2/3 Clock
	GPIOA_MODER |= 0x3C00; // set PA5, PA6 up for analog input
	GPIOB_MODER |= 0x55400;  // set PB5-9 up for General output on decoders(rows)
	GPIOC_MODER |= 0x5555; // set up PC0-7 for output on columns
	// Enable A/D Converters
	ADC1_CR2 |= 1;				 
	ADC2_CR2 |= 1;
	
	 //Bit 10 allows EOC to be set after conversion
	ADC1_CR2 |= 400;
	ADC2_CR2 |= 400;
	
	ADC_CCR |= 0x30000; // Clock divided by 8
	ADC1_SQR3 |= 0x5; // set channel 5 conversion A/D PA5
	ADC2_SQR3 |= 0x6; // set channel 6 conversion A/D PA6
	
}

// Creates both paddles
void Paddles(){
	Paddle1(); // lhs paddle
	Paddle2(); // rhs paddle
}

// Creates a paddle for the 2nd matrix(rhs)
void Paddle2(){
	ADC1_CR2 |= 0x40000000;// bit30 starts a/d conversion in software
	GPIOC_ODR = Col[0]; // initalize paddle column
	// Moves the paddle from bottom to top
	if(ADC1_DR > 0x000 && ADC1_DR <= 0x1C8){
		 PaddlePos2 = 0;  
		 GPIOB_ODR = RowR[1];
		 Delay(2);
	}			
	if(ADC1_DR >= 0x1C9 && ADC1_DR <= 0x391){
				 PaddlePos2 = 1; 
				 GPIOB_ODR = RowR[1];
				 Delay(2);
				 GPIOB_ODR = RowR[2];
				 Delay(2);
	}
	if(ADC1_DR >= 0x392 && ADC1_DR <= 0x55A){
				 PaddlePos2 = 2; 
				 GPIOB_ODR = RowR[1];
				 Delay(2);
				 GPIOB_ODR = RowR[2];;
				 Delay(2);
				 GPIOB_ODR = RowR[3];
				 Delay(2);
				}
	if(ADC1_DR >= 0x55B && ADC1_DR <= 0x723){
				 PaddlePos2 = 3; 
				 GPIOB_ODR = RowR[2];
				 Delay(2);
				 GPIOB_ODR = RowR[3];;
				 Delay(2);
				 GPIOB_ODR = RowR[4];
				 Delay(2);
				}
	if(ADC1_DR >= 0x724 && ADC1_DR <= 0x8EC){
				 PaddlePos2 = 4; 
				 GPIOB_ODR = RowR[3];
				 Delay(2);
				 GPIOB_ODR = RowR[4];
				 Delay(2);
				 GPIOB_ODR = RowR[5];
				 Delay(2);
	}
	if(ADC1_DR >= 0x8ED && ADC1_DR <= 0xAB5){
				 PaddlePos2 = 5; 
				 GPIOB_ODR = RowR[4];
				 Delay(2);
				 GPIOB_ODR = RowR[5];
				 Delay(2);
				 GPIOB_ODR = RowR[6];
				 Delay(2);
				}
	if(ADC1_DR >= 0xAB6 && ADC1_DR <= 0xC7E){
				 PaddlePos2 = 6; 
				 GPIOB_ODR = RowR[5];
				 Delay(2);
				 GPIOB_ODR = RowR[6];
				 Delay(2);
				 GPIOB_ODR = RowR[7];
				 Delay(2);
				}
	if(ADC1_DR >= 0xC7F && ADC1_DR <= 0xE47)
				{PaddlePos2 = 7; 
				 GPIOB_ODR = RowR[6];
				 Delay(1);
				 GPIOB_ODR = RowR[7];
				 Delay(1);
				}
	if(ADC1_DR >= 0xE48 && ADC1_DR <= 0xFFF)
				{PaddlePos2 = 8; 
				 GPIOB_ODR = RowR[7];
				 Delay(2);
				}
				//reset the values of the columns and rows to 0x0000
				GPIOB_ODR = 0x000;
				GPIOC_ODR = 0x000;
}

// Creates a paddle for the 1st matrix (lhs)
void Paddle1(){
	ADC2_CR2 |= 0x40000000;// bit30 starts a/d conversion in software
	GPIOC_ODR = Col[7];	// initialize the column row
	
	// move the paddle up from bottom to top
		if(ADC2_DR > 0x000 && ADC2_DR <= 0x1C8){
		 PaddlePos1 = 0; 
		 GPIOB_ODR = RowL[1];
		 Delay(2);
	}			
	if(ADC2_DR >= 0x1C9 && ADC2_DR <= 0x391){
				 PaddlePos1 = 1;  
				 GPIOB_ODR = RowL[1];
				 Delay(2);
				 GPIOB_ODR = RowL[2];
				 Delay(2);
	}
	if(ADC2_DR >= 0x392 && ADC2_DR <= 0x55A){
				 PaddlePos1 = 2; 
				 GPIOB_ODR = RowL[1];
				 Delay(2);
				 GPIOB_ODR = RowL[2];;
				 Delay(2);
				 GPIOB_ODR = RowL[3];
				 Delay(2);
				}
	if(ADC2_DR >= 0x55B && ADC2_DR <= 0x723){
				 PaddlePos1 = 3; 
				 GPIOB_ODR = RowL[2];
				 Delay(2);
				 GPIOB_ODR = RowL[3];;
				 Delay(2);
				 GPIOB_ODR = RowL[4];
				 Delay(2);
				}
	if(ADC2_DR >= 0x724 && ADC2_DR <= 0x8EC){
				 PaddlePos1 = 4; 
				 GPIOB_ODR = RowL[3];
				 Delay(2);
				 GPIOB_ODR = RowL[4];
				 Delay(2);
				 GPIOB_ODR = RowL[5];
				 Delay(2);
	}
	if(ADC2_DR >= 0x8ED && ADC2_DR <= 0xAB5){
				 PaddlePos1 = 5; 
				 GPIOB_ODR = RowL[4];
				 Delay(2);
				 GPIOB_ODR = RowL[5];
				 Delay(2);
				 GPIOB_ODR = RowL[6];
				 Delay(2);
				}
	if(ADC2_DR >= 0xAB6 && ADC2_DR <= 0xC7E){
				 PaddlePos1 = 6; 
				 GPIOB_ODR = RowL[5];
				 Delay(2);
				 GPIOB_ODR = RowL[6];
				 Delay(2);
				 GPIOB_ODR = RowL[7];
				 Delay(2);
				}
	if(ADC2_DR >= 0xC7F && ADC2_DR <= 0xE47)
				{PaddlePos1 = 7; 
				 GPIOB_ODR = RowL[6];
				 Delay(2);
				 GPIOB_ODR = RowL[7];
				 Delay(2);
				}
	if(ADC2_DR >= 0xE48 && ADC2_DR <= 0xFFF)
				{PaddlePos1 = 8; 
				 GPIOB_ODR = RowR[7];
				 Delay(2);
				}
				// reset the columns and rows to 0x0000
				GPIOB_ODR = 0x000;
				GPIOC_ODR = 0x000;
}

// Moves ball for the two player mode
void moveBall(){	
//GPIOB_ODR == ROWS(y)
	//GPIOC_ODR == COLUMNS(x)
	// Patterns are repeated int the code,  so it will only
	// be commented in once 
	unsigned char h,i,j,l,m, returnCol; // variables to avoid confusion
	// go right first  
	for(h=1; h<8; h++){
		GPIOC_ODR = Col[h]; 			 // set col value
		GPIOB_ODR = RowR[ballRow]; // across right matrix
		Delay20ms();							
		for(j =0; j < 5; j++){		// loop 10 times per one ball movement
		Paddles();
		}
		GPIOB_ODR = 0x000;	// turn off the rows
		
		// check the values of up and down
		if(up == 1)
			ballRow--; // go down a row
		if(down == 1)
			ballRow++; // go up a row
		// if the ballrow is at 7 & down is high
		// swap up to high
		if(ballRow==7 && down==1){ 
			up = 1; 
			down = 0;
		}
		// if the ballrow is at 7 & up is high
		// swap down to high
		if(ballRow==1 && up==1){
			up = 0;
			down = 1;
		}		
	}
	Delay(50);	
	Paddles();	
	for(i=0; i<7; i++){
		GPIOC_ODR = Col[i];	// set col value
		GPIOB_ODR = RowL[ballRow]; // across left matrix
		Delay20ms();
		for(j = 0; j< 5; j++)
			Paddles();
		
		if(up == 1)
			ballRow--;
		if(down == 1)
			ballRow++;
		if(ballRow==1 && up==1){
			up = 0;
			down = 1;
		}
		if(ballRow==7 && down==1){
			up = 1;
			down = 0;
		}
		//check for collision
		if(i==6){			
			if(PaddlePos1 == ballRow+1){ //go up
				up = 1;
				down = 0;
			}
			else if(PaddlePos1 == ballRow){	//go straight
				up = 0;
				down = 0;}
			else if(PaddlePos1 == ballRow-1){ // go down
				up = 0;
				down = 1;
			}
			//no collision? update score for player one
			else if(PaddlePos1 != ballRow && PaddlePos1 != ballRow+1 
							&& PaddlePos2 != ballRow-1){
				Delay(10);
				score1();
				Delay(10);
			}
		}
	}
	returnCol = 6; // set the value of the returning column
	// go left
	for(l=7;l>0;l--){
		GPIOB_ODR = RowL[ballRow];
		GPIOC_ODR = Col[returnCol];
		Delay20ms();
		returnCol--; //decrement column
		for(j = 0; j < 5; j++)
			Paddles();
		GPIOB_ODR = 0x000;
		if(up == 1){
			ballRow--;
		}
		if(down == 1){
			ballRow++;
		}
		if(ballRow==1 && up==1){
			up = 0;
			down = 1;
		}
		if(ballRow==7 && down==1){
			up = 1;
			down = 0;
		}
	}
	Delay(50);
	Paddles();	
	returnCol = 7; // set the value of the returning column
	for(m=8;m>1;m--){
		GPIOB_ODR = RowR[ballRow];
		GPIOC_ODR = Col[returnCol];
		Delay20ms();
		returnCol--;
		for(j = 0; j < 5;j++)
			Paddles();
		GPIOB_ODR = 0x000;
		if(up == 1){
			ballRow--;
		}
		if(down == 1){
		ballRow++;
		}
		if(ballRow==1 && up==1){
			up = 0;
			down = 1;
		}
		if(ballRow==7 && down==1){
			up = 1;
			down = 0;
		}
		//check for collision
		if(m==2){

			if(PaddlePos2 == ballRow+1){ // go up
				up = 1;
				down = 0;
			}
			if(PaddlePos2 == ballRow){	// go straight
				up = 0;
				down = 0;
			}
			if(PaddlePos2 == ballRow-1){ // go down
				up = 0;
				down = 1;
			}
			//no collision? update score for p2
			else if(PaddlePos2 != ballRow && PaddlePos2 != ballRow+1 
							&& PaddlePos2 != ballRow-1){
				Delay(10);
				score2();
				Delay(10);
			}
		}
	}
}


// update score for player 1
void score1(){
		p1Score++; // increment score
	if(p1Score == 1){
		GPIOC_ODR = Score[0];
		GPIOB_ODR = RowR[0];
		Delay(100);
	}
	if(p1Score == 2){
		GPIOC_ODR = Score[1];
		GPIOB_ODR = RowR[0];
		Delay(100);
	}
	if(p1Score == 3){
		GPIOC_ODR = Score[2];
		GPIOB_ODR = RowR[0];
		Delay(100);
	}
	// play winning sequence = X
	if(p1Score == 4){
		while(1){
		for(int i = 0; i < 8; i++){
				GPIOB_ODR = RowR[i];
				GPIOC_ODR = Col[i];
				Delay(1);
				GPIOB_ODR =RowR[7-i];
				GPIOC_ODR = Col[i];
				Delay(1);
		}
	}
}
	Delay(50);
}

//update score for player 2
void score2(){
	p2Score++; // increment player 2 score
	Delay(2);
	if(p2Score == 1){
		GPIOC_ODR = Score[0];
		GPIOB_ODR = RowL[0];
		Delay(100);
		
	}
	if(p2Score == 2){
		GPIOC_ODR = Score[1];
		GPIOB_ODR = RowL[0];
		Delay(100);
	}
	if(p2Score == 3){
		GPIOC_ODR = Score[2];
		GPIOB_ODR = RowL[0];
		Delay(100);
	}
	// play winning sequence = X
	if(p2Score == 4){
		while(1){
			for(int i = 0; i< 8; i++){
				GPIOB_ODR = RowL[i];
				GPIOC_ODR = Col[i];
				Delay(1);
				GPIOB_ODR =RowL[7-i];
				GPIOC_ODR = Col[i];
				Delay(1);
			}
		}
	}
	Delay(50);
}

// simple Delay when x = # of ms
void Delay(int x){
	unsigned int j;
	for(j=2500*x; j>0; j--);
}

// Delays for 20ms
void Delay20ms(){
	for(unsigned int i = 50000; i>0; i--);
}
