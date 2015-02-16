#include <avr/io.h>

const int latchPin = 13; //GREEN
const int clockPin = 12; //ORANGE
const int dataPin = 11; //BLUE

#define WIDTH 8
#define HEIGHT 8

byte* tBuffer = new byte[WIDTH*HEIGHT]; //Screen Buffer Copy
byte sBuffer[WIDTH*HEIGHT] = //Screen Buffer
{ 
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 
};

/* 

	Writing Steps:

	1) Last 4 X-coords from Array
	2) 
	3) First 4 X-coords from Array
	4) 
*/

byte tBf[16] = { 0,0,0,0, 0,0,0,0,  1,0,0,0, 0,0,0,0 };

int iX = 0;
int iY = 0;

int yH = 3;

void setup() 
{
	//Empty Temporary Buffer
		for (int i=0; i<WIDTH*HEIGHT; i++){ tBuffer[i] = 0; }

	//Randomize Screen
	randomSeed(analogRead(5));
	for (int i=0; i<WIDTH*HEIGHT; i++)
		//sBuffer[i] = random(2);
	
	//Sets Shift Register Pins
	// pinMode(latchPin, OUTPUT);
	// pinMode(clockPin, OUTPUT);
	// pinMode(dataPin, OUTPUT);

	DDRB = 0xFF;

	//for (int y=0; y<HEIGHT; y++)
	//{
		PORTB &= ~_BV(PORTB5); //Set latch pin LOW
		

		/*for (int y1=HEIGHT-1; y1>=0; y1--) //Shift Out Column data
		{
			if ( (HEIGHT-1-y) == y1 )
				PORTB &= ~_BV(3);
			else 
				PORTB |= _BV(3);

			PORTB |= _BV(4); //Clock HIGH
			PORTB &= ~_BV(4); //Clock LOW
		}
		for (int x=WIDTH-1; x>=0; x--) //Shift Out Row data
		{
			if (sBuffer[y*WIDTH+x] == 1)
				PORTB |= _BV(3);
			else 
				PORTB &= ~_BV(3);

			PORTB |= _BV(4); //Clock HIGH
			PORTB &= ~_BV(4); //Clock LOW
		}*/

		//Move Data To Output
		
		//delayMicroseconds(16); //Multiplexing Delay

		// for (int i=0; i<16; i++)
		// {
		// 	if ( tBf[i] == 0 )
		// 		PORTB &= ~_BV(3); 
		// 	else 
		// 		PORTB |= _BV(3);

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iX=7; iX>=4; iX--)
		// {
		// 	if ( tBuffer[yH*WIDTH+iX] == 1 )
		// 		PORTB &= ~_BV(3); 
		// 	else 
		// 		PORTB |= _BV(3);

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iY=3; iY>=0; iY--)
		// {
		// 	if ( iY == 0 )
		// 		PORTB |= _BV(3);
		// 	else
		// 		PORTB &= ~_BV(3); 

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iX=0; iX<4; iX++)
		// {
		// 	if ( tBuffer[yH*WIDTH+iX] == 1 )
		// 		PORTB &= ~_BV(3); 
		// 	else 
		// 		PORTB |= _BV(3);

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iY=4; iY<8; iY++)
		// {
		// 	if ( iY == 0 )
		// 		PORTB |= _BV(3);
		// 	else
		// 		PORTB &= ~_BV(3); 

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iY=7; iY>=4; iY--)
		// {
		// 	if ( iY == yH )
		// 		PORTB |= _BV(3);
		// 	else
		// 		PORTB &= ~_BV(3); 

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iX=3; iX>=0; iX--)
		// {
		// 	if ( sBuffer[yH*WIDTH+iX] == 1 )
		// 		PORTB &= ~_BV(3);
		// 	else 
		// 		PORTB |= _BV(3);

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iY=0; iY<4; iY++)
		// {
		// 	if ( iY == yH )
		// 		PORTB |= _BV(3);
		// 	else
		// 		PORTB &= ~_BV(3); 

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }

		// for(iX=4; iX<8; iX++)
		// {
		// 	if ( sBuffer[yH*WIDTH+iX] == 1 )
		// 		PORTB &= ~_BV(3); 
		// 	else 
		// 		PORTB |= _BV(3);

		// 	PORTB |= _BV(4); //Clock HIGH
		// 	PORTB &= ~_BV(4); //Clock LOW
		// }


		// PORTB |= _BV(PORTB5); //Set latch pin HIGH
		// PORTB &= ~_BV(PORTB5); //Set latch pin LOW
	//}
	
	//Sets Analog Inputs
	// pinMode(A0, INPUT);
	
	//Sets Rows
	// for (int i=0; i<4+4; i++)
	// {
	// 	pinMode(i, OUTPUT);
	// 	PORTD |= _BV(i);
	// }
}

long prevMillis = 0; //Last Tick Time
const long interval = 500; //Logic Time Interval

void loop()
{
	unsigned long currentMils = millis(); //Current Time

	if (currentMils-prevMillis > interval) //Delays logic by INTERVAL
	{
		prevMillis = currentMils; //Set last time
		//Logic(); //Ticks logic
	}

	Render(); //Outputs To Display
}

void Logic()
{
	//Copy temp buffer with screen buffer
	for (int i=0; i<WIDTH*HEIGHT; i++)
	{
		tBuffer[i] = sBuffer[i];
	}

	//Calculate
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			int n = 0; //Neighbours
			int i = y*WIDTH+x; //Id Of Cell
			
			//Calculate Neigbours
			for (int y0=y-1; y0<=y+1; y0++)
			{
				for (int x0=x-1; x0<=x+1; x0++)
				{
					if ( (y!=y0 || x!=x0) && (x0>=0 && x0<WIDTH && y0>=0 && y0<HEIGHT))
					{
						if (tBuffer[y0*WIDTH+x0]) n++;
					}
				}
			}

			//Apply Rules
			if (tBuffer[i])
			{
				if (n<2)
					sBuffer[i] = 0;
				else if (n==2||n==3)
					sBuffer[i] = 1;
				else if (n>3)
					sBuffer[i] = 0;
			}
			else
			{
				if (n==3)
					sBuffer[i] = 1;
			}
		}
	}
}

//Clock Pin 4
//Data Pin 3

void Render()
{
	for (yH=0; yH<HEIGHT; yH++)
	{
		for(iY=7; iY>=4; iY--)
		{
			if ( iY == yH )
				PORTB |= _BV(3);
			else
				PORTB &= ~_BV(3); 

			PORTB |= _BV(4); //Clock HIGH
			PORTB &= ~_BV(4); //Clock LOW
		}

		for(iX=3; iX>=0; iX--)
		{
			if ( sBuffer[yH*WIDTH+iX] == 1 )
				PORTB &= ~_BV(3);
			else 
				PORTB |= _BV(3);

			PORTB |= _BV(4); //Clock HIGH
			PORTB &= ~_BV(4); //Clock LOW
		}

		for(iY=0; iY<4; iY++)
		{
			if ( iY == yH )
				PORTB |= _BV(3);
			else
				PORTB &= ~_BV(3); 

			PORTB |= _BV(4); //Clock HIGH
			PORTB &= ~_BV(4); //Clock LOW
		}

		for(iX=4; iX<8; iX++)
		{
			if ( sBuffer[yH*WIDTH+iX] == 1 )
				PORTB &= ~_BV(3); 
			else 
				PORTB |= _BV(3);

			PORTB |= _BV(4); //Clock HIGH
			PORTB &= ~_BV(4); //Clock LOW
		}


		PORTB |= _BV(PORTB5); //Set latch pin HIGH

		delayMicroseconds(565);
		//delay(500);

		PORTB &= ~_BV(PORTB5); //Set latch pin LOW
	}
}