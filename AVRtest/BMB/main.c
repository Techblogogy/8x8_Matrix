#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

//Timer Ticks
#define TMR_TCK 78 //156

#define SIZE 8 //Matrix Size

#define CLK 5 // 2 //Clock
#define DT 3 // 0 //Data
#define LTC 4 // 1 //Latch

volatile uint32_t tm = 0;
volatile uint32_t bTm = 0; //Buttons Timer Tick

uint8_t bl = 0; //To Run Simulation
uint8_t id = 0; //Current Selection Id

//Display Data
uint8_t tBuffer[SIZE];
uint8_t sBuffer[SIZE];

#define OPT 20

uint8_t EEMEM bfr[OPT][SIZE] = {
		{0b00000000,
		 0b01001011,
		 0b01001010,
		 0b01111011,
		 0b01001010,
		 0b01001010,
		 0b00000000,
		 0b00000000},

		 {0b00000000,
		  0b00101111,
		  0b00101001,
		  0b11101111,
		  0b00101001,
		  0b00101001,
		  0b00000000,
		  0b00000000},

		 {0b00000000,
  		  0b10111101,
  		  0b10100101,
  		  0b10111101,
  		  0b10100101,
 		  0b10100101,
  		  0b00000000,
		  0b00000000},

		  {0b00000000,
		   0b11110111,
		   0b10010101,
		   0b11110111,
		   0b10010100,
		   0b10010100,
		   0b00000000,
		   0b00000000},

		   {0b00000000,
		  	0b11011101,
		  	0b01010101,
		  	0b11011101,
		  	0b01010001,
		  	0b01010001,
		  	0b00000000,
		  	0b00000000},

		   {0b00000000,
			0b01110111,
			0b01010101,
			0b01110111,
			0b01000100,
			0b01000100,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b11011101,
			0b01010101,
			0b11011101,
			0b00010000,
			0b00010001,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01110111,
			0b01010100,
			0b01110111,
			0b01000001,
			0b01000111,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b11011101,
			0b01010001,
			0b11011101,
			0b00000101,
			0b00011101,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01110111,
			0b01000101,
			0b01110101,
			0b00000101,
			0b01110111,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b11011101,
			0b00010101,
			0b11010101,
			0b00010101,
			0b11011101,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01110111,
			0b01010101,
			0b01010101,
			0b01010101,
			0b01110111,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b11011101,
			0b01010101,
			0b01010101,
			0b01010100,
			0b11011101,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01110111,
			0b01010100,
			0b01010111,
			0b01010001,
			0b01110111,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b11011101,
			0b01010001,
			0b01011101,
			0b01000101,
			0b11011101,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01110101,
			0b01000101,
			0b01110101,
			0b00010101,
			0b01110111,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b11010101,
			0b00010101,
			0b11010101,
			0b01010101,
			0b11011101,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01010111,
			0b01010101,
			0b01010110,
			0b01010101,
			0b01110111,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01011101,
			0b01010101,
			0b01011001,
			0b01010100,
			0b11011101,
			0b00000000,
			0b00000000},

		   {0b00000000,
			0b01110111,
			0b01010100,
			0b01100111,
			0b01010001,
			0b01110111,
			0b00000000,
			0b00000000}
};

static void Render() {
	int8_t iY, iX = 0; //For Loops Primary XY

	for (iY = 0; iY < SIZE; iY++) {
		for (iX = 7; iX >= 4; iX--) {
			if (((sBuffer[iX] & (1 << (7 - iY))) >> (7 - iY)) == 1)
				PORTB |= _BV(DT);
			else
				PORTB &= ~_BV(DT);

			PORTB |= _BV(CLK); //Clock HIGH
			PORTB &= ~_BV(CLK); //Clock LOW
		}

		for (iX = 3; iX >= 0; iX--) {
			if (iX == iY)
				PORTB &= ~_BV(DT);
			else
				PORTB |= _BV(DT);

			PORTB |= _BV(CLK); //Clock HIGH
			PORTB &= ~_BV(CLK); //Clock LOW
		}

		for (iX = 0; iX < 4; iX++) {
			if (((sBuffer[iX] & (1 << (7 - iY))) >> (7 - iY)) == 1)
				PORTB |= _BV(DT);
			else
				PORTB &= ~_BV(DT);

			PORTB |= _BV(CLK); //Clock HIGH
			PORTB &= ~_BV(CLK); //Clock LOW
		}
		for (iX = 4; iX < 8; iX++) {
			if (iX == iY)
				PORTB &= ~_BV(DT);
			else
				PORTB |= _BV(DT);

			PORTB |= _BV(CLK); //Clock HIGH
			PORTB &= ~_BV(CLK); //Clock LOW
		}

		PORTB |= _BV(LTC); //Latch HIGH
		PORTB &= ~_BV(LTC); //Latch LOW
	}
}

static void Logic() {
	id++;
	eeprom_read_block((void*) &sBuffer, (const void*) &bfr[id], SIZE);
}

ISR (TIMER0_COMPA_vect) {
	tm++;
}

int main(void) {
	DDRB = 0xff; //Set PortB as Output

	//Setup Timer
	TCCR0A |= (1 << WGM01); //Set Timer To CTC Mode
	TCCR0B |= (1 << CS00) | (1 << CS02); //Set Scale Factor To Clk/1024
	OCR0A = TMR_TCK; //Set Timer Interval
	TIMSK0 |= (1 << OCIE0A); //Set Timer Interrupt On OCR0A Match

	sei();

	eeprom_read_block((void*) &sBuffer, (const void*) &bfr[id], SIZE);

	while (1) {

		if (tm > 80 && id+1<OPT) {
			Logic();
			tm = 0;
		}

		Render();
	}

	return 0;
}
