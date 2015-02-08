#include<avr/io.h>

int latchPin = 13;
int clockPin = 12;
int dataPin = 11;

#define WIDTH 8
#define HEIGHT 8

byte* tBuffer = new byte[WIDTH*HEIGHT]; //Screen Buffer Copy
byte sBuffer[WIDTH*HEIGHT] = //Screen Buffer
{ 
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0 
};

void setup() 
{
	//Empty Temporary Buffer
	for (int i=0; i<WIDTH*HEIGHT; i++) tBuffer[i] = 0;

	//Randomize Screen
	randomSeed(analogRead(5));
	for (int i=0; i<WIDTH*HEIGHT; i++)
		sBuffer[i] = random(2);
	
	//Sets Shift Register Pins
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	
	//Sets Analog Inputs
	pinMode(A0, INPUT);
	
	//Sets Rows
	for (int i=0; i<4+4; i++)
	{
		pinMode(i, OUTPUT);
		PORTD |= _BV(i);
	}
}

long prevMillis = 0; //Last Tick Time
const long interval = 100; //Logic Time Interval

void loop()
{
	unsigned long currentMils = millis(); //Current Time
  
	if (currentMils-prevMillis > interval) //Delays logic by INTERVAL
	{
		prevMillis = currentMils; //Set last time
		Logic(); //Ticks logic
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
					if ( (y!=y0 || x!=x0) && (x0>=0 && x0<WIDTH && y0>=0 && y0<HEIGHT) )
						if (tBuffer[y0*WIDTH+x0]) n++;
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
	for (int y=0; y<HEIGHT; y++)
    {
		// Ground Row
		if (y-1 >= 0)
			PORTD &= ~_BV(y-1);
		else
			PORTD &= ~_BV(7);
		
		PORTB |= _BV(PORTB5); //Set latch pin HIGH
		for (int x=WIDTH-1; x>=0; x--) //Shift Out Row data
		{
			//digitalWrite(dataPin, tBuffer[y*WIDTH+x]);

			if (sBuffer[y*WIDTH+x] == 1)
				PORTB |= _BV(3);
			else 
				PORTB &= ~_BV(3);

			PORTB |= _BV(4);
			PORTB &= ~_BV(4);
			
			//digitalWrite(clockPin, HIGH);
			//digitalWrite(clockPin, LOW);
		}
		PORTB &= ~_BV(PORTB5); //Set latch pin LOW
      
		delayMicroseconds(16); //Multiplexing Delay
      
		//Disable Row
		if (y-1 >= 0)
			PORTD |= _BV(y-1);
		else
			PORTD |= _BV(7);
    }
}
