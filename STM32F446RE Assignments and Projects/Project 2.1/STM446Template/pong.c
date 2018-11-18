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
unsigned int RowR[] = 
			{0x100,0x120,0x140,0x160,0x180,0x1A0,0x1C0,0x1E0};//Left rows bottom->top
unsigned int RowL[] = 
			{0x200,0x220,0x240,0x260,0x280,0x2A0,0x2C0,0x2E0};//right rows bottom->top
unsigned int PaddlePos1,PaddlePos2,CPaddlePos; // current position of paddles, 
																							 //CPaddle for Computer
unsigned int Score[] =
			{0x1, 0x2, 0x3};		// score values
int p1Score, p2Score; // player scores
int up, down; // Control Ball direction
unsigned int ballRow;	// ball starting row

// Functions
void Game(void);	// run game with two players
void compMoveBall(void); // moves ball and updates score
void compPaddle(void); // computer paddle
void compGame(void); // runs 1player game
void Setup(void); // Set up ports and ADC
void Paddles(void); // initializes paddles to starting position
void Paddle1(void); // rhs paddle
void Paddle2(void); // lhs paddle
void OnePlayerPaddles(void); // setups two player paddles
void moveBall(void); // move ball and intialize paddles
void score1(void);	// player 1 score
void score2(void);  // player 2 score
void Delay(int); // simple Delay, approximately 1.1ms*x
void Delay20ms(void); // Delays for approx.20ms

//UART Declarations
void ConfigureUART(unsigned int baudDivisor);

void UARTPutChar(char ch);

void SendMsg(const char msg[]);


int main()
{
	
	//UART setup
	//Clock bits
Setup();	// setup ports
//UART/USART setup
	RCC_APB1ENR |= (1 << 4);  	//Enable peripheral timer for timer 6

  RCC_AHB1ENR |= 1; //enables Port A clock

	RCC_APB1ENR |= (1<<19); //Enable UART4 clock

	//USART PIN Bits

	GPIOA_AFRL = (8	<< 0| 8<< 4); //Alternate Func PA0 to UART4

	GPIOA_MODER |= (2<<0|2<<2); //Bits 4-1 = 1010 for Alt Func on PA0

	//OTYPER register resets to 0 so it is push/pull by default

	GPIOA_OSPEEDER |= (3<<0|3<<2); //Bits 4-1 = 1111 for high speed on PA0
	GPIOA_MODER |= 0x40000; 	//Bits 18-19 are 01 for digital output on PA9
	GPIOA_OSPEEDER |= 0xC0000; 	//Bits 18-19 are 11 for high speed on PA9
	NVICISER0 |= 1 << 28;
	TIM2_DIER |= 1;
	TIM2_CR1 |=  1 << 7;
	TIM2_ARR = 16000;
	TIM2_PSC = 0;
	TIM2_EGR |= 1;				//Enable update
	TIM2_CR1 |= 1;				//Turn on Timer2



	ConfigureUART(4800);	//openlog's baud is 9600, so 16000000/((8*(2-0))*9600) = ~104

	ballRow = 4; //initialize ball row
	//intialize player scores
	p1Score = 0; 
	p2Score = 0;
	
	
	while(1){
	Game();
	}
			
}

// Runs 2 player game
void Game()
{
	Paddles(); // setup paddles
	moveBall(); // ball game
}

// Runs player vs computer
void compGame()
{
	Setup(); // setup ports
	OnePlayerPaddles();
	compMoveBall(); // play game & update scoring
}
// Creates paddles for single player
void OnePlayerPaddles()
{
	Paddle1(); // lhs  paddle
	compPaddle(); // rhs paddle
}

// sets up ADC & Output Ports
void Setup()
{
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
void Paddles()
{
	Paddle1(); // lhs paddle
	Paddle2(); // rhs paddle
}

// Creates a paddle for the 2nd matrix(rhs)
void Paddle2()
{
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
void Paddle1()
{
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


// Paddle for the computer
void compPaddle()
{
	int x = rand()%7; // select a  random row
	CPaddlePos = x; // set Computer Paddle position
	// check the x value to determine how many leds to turn on
	if(x-1 >= 1 || x+1 <= 7){
		GPIOC_ODR = Col[0];
		GPIOB_ODR = RowR[x-1];
		Delay(2);
		GPIOB_ODR = RowR[x];
		Delay(2);
		GPIOB_ODR = RowR[x+1];
		Delay(2);
		GPIOC_ODR = 0x000;
	}
	if(x-1 < 1){
		GPIOC_ODR = Col[0];
		GPIOB_ODR = RowR[x];
		Delay(2);
		GPIOB_ODR = RowR[x+1];
		Delay(2);
		GPIOC_ODR = 0x000;
	}
	if(x+1 > 7){
		GPIOC_ODR = Col[0];
		GPIOB_ODR = RowR[x-1];
		Delay(2);
		GPIOB_ODR = RowR[x];
		Delay(2);
		GPIOC_ODR = 0x000;
	}
	// 5 ms delay between paddle movement
	Delay(50);
}

// Moves ball for the two player mode
void moveBall()
{	//GPIOB_ODR == ROWS(y)
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



//Main Game Ran with Player vs Computer
//Runs just like the moveBall Functions
void compMoveBall()
{
	//GPIOB_ODR == ROWS(y)
	//GPIOC_ODR == COLUMNS(x)

	unsigned char h,i,returnCol,l,m;
	
	//if ball direction 1 go right
	for(h=1; h<8; h++){
		GPIOC_ODR = Col[h];
		GPIOB_ODR = RowR[ballRow]; // across right matrix
		Delay20ms();
		for(int k = 0; k<5; k++)
			Paddle1();
		compPaddle();

		GPIOB_ODR = 0x000;
		if(up == 1)
			ballRow--;
		if(down == 1)
			ballRow++;
		if(ballRow==7 && down==1){
			up = 1;
			down = 0;
		}
		if(ballRow==1 && up==1){
			up = 0;
			down = 1;
		}		
	}
	Delay(50);	
	OnePlayerPaddles();
	
	for(i=0; i<7; i++){
		GPIOC_ODR = Col[i];
		GPIOB_ODR = RowL[ballRow]; // across left matrix
		Delay20ms();
		for(int k = 0; k<5; k++)
				Paddle1(); // lhs  paddle
	compPaddle(); // rhs paddle
		
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
			else if(PaddlePos1 == ballRow){	//go straigt
				up = 0;
				down = 0;}
			else if(PaddlePos1 == ballRow-1){ // go down
				up = 0;
				down = 1;
			}
			//no collision? update score for p1
			else{
				Delay(10);
				score1();
				Delay(10);
			}
		}
	}
	returnCol = 6;  // set the column in order to
									// send the ball back
	// go left
	for(l=7;l>0;l--){
		GPIOB_ODR = RowL[ballRow];	
		GPIOC_ODR = Col[returnCol];
		Delay20ms();
		returnCol--;
		for(int k = 0; k<5; k++)
				Paddle1(); // lhs  paddle
	compPaddle(); // rhs paddle
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
	
	returnCol = 7;
	for(m=8;m>1;m--){
		GPIOB_ODR = RowR[ballRow];
		GPIOC_ODR = Col[returnCol];
		Delay20ms();
		returnCol--;
		for(int k = 0; k<10; k++)
				Paddle1(); // lhs  paddle
	compPaddle(); // rhs paddle
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

			if(CPaddlePos == ballRow+1){ // go up
				up = 1;
				down = 0;
			}
			if(CPaddlePos == ballRow){	// go straight
				up = 0;
				down = 0;
			}
			if(CPaddlePos == ballRow-1){ // go down
				up = 0;
				down = 1;
			}
			//no collision? update score for p2
			else{
				Delay(10);
				score2();
				Delay(10);
			}
		}
	}
}

// update score for player 1
void score1()
{
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
void score2()
{
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
void Delay(int x)
{
	unsigned int j;
	for(j=2500*x; j>0; j--);
}



// Delays for 20ms
void Delay20ms()
{
	for(unsigned int i = 50000; i>0; i--);
}

void TIM2_IRQHandler(){
	// dont do loops or adc checks in interrupt
	UARTPutChar('/');
	TIM2_CNT = 0; 	 // reset count of TIM3

	SendMsg(" Bitch");
	
	TIM2_SR &= 0xFFFE; // reset update flag
	TIM2_CR1 |= 1;
	
}

void ConfigureUART(unsigned int baudDivisor)
  {
		USART4_CR1 &= 1;        //Disable during set up. Wd len = 8, Parity = off
   USART4_BRR = baudDivisor;  //Set up baud rate
   USART4_CR2 = 0;            //1 stop bit
		USART4_CR3 = 0;            //Disable interrupts and DMA
		USART4_CR1 = 0x200C; 
	 
  }   
// 
void UARTPutChar(char ch)
  {//Wait for empty flag 
   while((USART4_SR & 0x80)==0);
   USART4_DR = ch; 

  }   
// 
void SendMsg(const char msg[])
  {int i = 0;
   while(msg[i] != 0) 
      {UARTPutChar(msg[i]);
       i++; 
      }
  }    
	
	