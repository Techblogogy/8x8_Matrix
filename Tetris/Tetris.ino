#include<avr/io.h>

int latchPin = 13;
int clockPin = 12;
int dataPin = 11;

#define WIDTH 8
#define HEIGHT 8

//byte* tBuffer = new byte[WIDTH*HEIGHT]; //Screen Buffer Copy
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

struct Bitmap
{
	Bitmap() { }

	Bitmap(int size, byte data[])
	{
		//w = width;
		//h = height;

		s = size;

		dt = new byte[s*s];
		for (int i=0; i<s*s; i++) dt[i] = data[i];
	}

	void Render()
	{
		for (int y0=0; y0<s; y0++)
		{
			for (int x0=0; x0<s; x0++)
			{
				int i = y0*s+x0;
				int i2 = (y0+y)*WIDTH+(x0+x);

				if (dt[i] == 1)
					sBuffer[i2] = dt[i];
			}
		}
	}
	
	void Clean()
	{
		for (int y0=0; y0<s; y0++)
		{
			for (int x0=0; x0<s; x0++)
			{
				int i = y0*s+x0;
				int i2 = (y0+y)*WIDTH+(x0+x);

				if (dt[i] == 1)
					sBuffer[i2] = 0;
			}
		}
	}

	bool Check(int xOff, int yOff)
	{
		bool col = false;

		for (int y0=0; y0<s; y0++)
		{
			for (int x0=0; x0<s; x0++)
			{
				int i = y0*s+x0;

				if (dt[i]==1)
				{
					if (y0+y+yOff >= HEIGHT || x0+x+xOff >= WIDTH || y0+y+yOff < 0 || x0+x+xOff < 0) col = true;

					i = (y0+yOff)*s+(x0+xOff);
					if ( (dt[i]==1 && (y0+yOff < s && x0+xOff < s)) )
					{
						continue;
					}
					else 
					{
						int i2 = (y0+y+yOff)*WIDTH+(x0+x+xOff);

						if (sBuffer[i2] == 1) col = true;
					}
				}
			}
		}

		return col;
	}

	void RotateB()
	{
		byte* tempDt = new byte[s*s];

		bool br = true;

		int oX = 1;
		int oY = 1;

		for (int y0=0; y0<s; y0++)
		{
			for (int x0=0; x0<s; x0++)
			{
				//dt[(s-y0-1)*s+(x0)] = tempDt[y0*s+x0];

				//dt[0] = 0;

				//if (tempDt[y0*s+x0] == 0) continue;

				int i = y0*s+x0;

				if (sBuffer[i] == 1) br = false;

				int lX = x0 - oX; //Relative X
				int lY = y0 - oY; //Relative Y

				int rX = lX * 0 + lY * (-1);
				int rY = lX * 1 + lY * 0;

				lX = rX + oX;
				lY = rY + oY;

				tempDt[lY*s+lX] = dt[y0*s+x0];
			}
		}

		if (br)
		{
			for (int i=0; i<s*s; i++)
			{
				dt[i] = tempDt[i];
			}
		}	
	}

	int x,y; //Position
	//int w,h;
	int s; //Size
	byte* dt; //Bitmap Data
};

byte tIA[4*4] = 
{
	0,0,0,0,
	1,1,1,1,
	0,0,0,0,
	0,0,0,0
};

byte tJA[3*3] = 
{
	0,0,0,
	1,1,1,
	0,0,1
};

byte tLA[3*3] = 
{
	0,0,0,
	1,1,1,
	1,0,0
};

byte tOA[2*2] = 
{
	1,1,
	1,1
};

byte tSA[3*3] = 
{
	0,1,1,
	1,1,0,
	0,0,0
};

byte tTA[3*3] = 
{
	0,0,0,
	1,1,1,
	0,1,0
};

byte tZ[3*3] = 
{
	1,1,0,
	0,1,1,
	0,0,0
};

#define B_SIZE 7
Bitmap blocks[B_SIZE] = {Bitmap(3,tZ), Bitmap(3,tTA), Bitmap(3,tSA), Bitmap(2,tOA), Bitmap(3,tLA), Bitmap(3,tJA), Bitmap(4,tIA)};

Bitmap b; //= blocks[0];

void setup() 
{
	//Empty Temporary Buffer
	//for (int i=0; i<WIDTH*HEIGHT; i++) tBuffer[i] = 0;

	randomSeed(analogRead(5));

	//Bitmap test
	RandomBlock();



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
const long interval = 1000; //Logic Time Interval 

void loop()
{
	for (int i=0; i<WIDTH*HEIGHT; i++) sBuffer[i] = 0;
	b.Clean();

	unsigned long currentMils = millis(); //Current Time

	//tone(10, tan(millis()));
	
	if (currentMils-prevMillis > interval) //Delays logic by INTERVAL
	{
		prevMillis = currentMils; //Set last time
		Logic(); //Ticks logic
	}	 
	
	if (GetButton(A0) && !b.Check(-1,0))
	{
		b.x--;
		b.Clean();

		b.RotateB();

		//sBuffer[0] = 1;
	}
	if (GetButton(A1) && !b.Check(1,0))
	{
		b.x++;
		b.Clean();

		//sBuffer[1] = 1;
	}
	if (GetButton(A2) && !b.Check(1,0))
	{
		//b.x++;
		b.Clean();

		//sBuffer[2] = 1;
	}
	if (GetButton(A3) /*&& !b.Check(1,0)*/)
	{
		//b.x++;
		//b.Clean();

		//sBuffer[3] = 1;
	}
	if (GetButton(A4) /*&& !b.Check(1,0)*/)
	{
		//b.x++;
		b.Clean();

		//sBuffer[4] = 1;
	}

	b.Render();	
   
	Render(); //Outputs To Display
}

//Pin 9834

void Logic()
{
	if (b.Check(0,1))
	{
		b.Render();

		//Logic Test
		RowTest();

		RandomBlock();

		return;
	}

	b.y++;
}

void RowTest()
{
	for (int y=HEIGHT-1; y>=0; )
	{
		int cc = 0; //Coloum Count

		for (int x=0; x<WIDTH; x++)
		{
			int i = y*WIDTH+x;

			if (sBuffer[i] == 0) break;
			if (sBuffer[i] == 1) cc++;
		}

		if (cc == WIDTH)
		{
			for (int x=0; x<WIDTH; x++) sBuffer[y*WIDTH+x] = 0;

			for (int y0=y; y0>=0; y0--)
			{
				for (int x=0; x<WIDTH; x++)
				{
					if (y0-1 >= 0)
						sBuffer[y0*WIDTH+x] = sBuffer[(y0-1)*WIDTH+x];
					else 
						sBuffer[y0*WIDTH+x] = 0;
				}
			}
		}
		else
		{
			y--;
		}
	}
}

void Render()
{
	for (int y=0; y<HEIGHT; y++)
    {
		for (int y1=HEIGHT-1; y1>=0; y1--) //Shift Out Column data
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
		}

		//Move Data To Output
		PORTB |= _BV(PORTB5); //Set latch pin HIGH
      
		delayMicroseconds(16); //Multiplexing Delay
		PORTB &= ~_BV(PORTB5); //Set latch pin LOW
    }
}

int bInterval = 250;
int bTime = 0;
boolean GetButton(int b)
{
	long cTime = millis();
	int state = digitalRead(b);

	if (state == HIGH && cTime - bTime >= bInterval)
	{
		bTime = cTime;
		return true;
	}
	else
		return false;
}

void RandomBlock()
{
	bool reset = false;
	int id = random(B_SIZE); //6;

	b = blocks[id];
	b.x = 5;
	b.y = 0;
}