#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define SIZE 8 //Matrix Size

#define CLK 2 //Clock
#define DT 0 //Data
#define LTC 1 //Latch

volatile uint32_t tm = 0;

//Display Data
uint8_t sBuffer[SIZE];/* =
 {
 0b00000000,
 0b00000000,
 0b01000000,
 0b00100000,
 0b11100000,
 0b00000000,
 0b00000000,
 0b00000000
 };*/

//Store Tetris Data in EEPROM
uint8_t EEMEM Iblk[5] = { 2, 4,0b11110000, 2,0b10101010 };
uint8_t EEMEM Jblk[9] = { 4, 3,0b10011100, 2,0b11101000, 3,0b11100100, 2,0b01011100 };
uint8_t EEMEM Lblk[9] = { 4, 3,0b00111100, 2,0b10101100, 3,0b11110000, 2,0b11010100 };
uint8_t EEMEM Oblk[3] = { 1, 2,0b11110000 };
uint8_t EEMEM Sblk[5] = { 2, 3,0b01111000, 2,0b10110100 };
uint8_t EEMEM Tblk[9] = { 4, 3,0b01011100, 2,0b10111000, 3,0b11101000, 2,0b01110100 };
uint8_t EEMEM Zblk[5] = { 2, 3,0b11001100, 2,0b01111000 };

uint8_t trs[9]; //Current Tetris Peace
uint8_t xS, yS = 0;

static void Render() {
	int8_t iY, iX = 0; //For Loops Primary XY

	cli();

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

	sei();
}

uint8_t p = 7;

static void Logic() {
//	for (int i = 0, j = trs[3]; i < 8; i++) {
//		if (((trs[p+2] & (1 << (7 - i))) >> (7 - i)) == 1)
//		{
//			sBuffer[j + yS-1] &= ~(1 << (7 - (i % trs[p+1]) - xS));
//			sBuffer[j + yS] |= (1 << (7 - (i % trs[p+1]) - xS));
//		}
//
//		if (i % trs[p+1] == (trs[p+1] - 1))
//			j--;
//	}
//
//	yS++;

	//uint8_t id = 0;

	//uint8_t h = trs[id+1];
	//uint8_t s = trs[id+2];
}

ISR (TIM0_COMPA_vect) {
	tm++;
}

//Button1 233
//Button2 42
//Button3 45

ISR (ADC_vect) {
	//sBuffer[0] = ADCH;

	ADCSRA |= (1 << ADSC);
}

int main(void) {
	DDRB = 0b00000111; //Set PortB as Output

	//Setup Timer
	TCCR0A |= (1 << WGM02); //Set Timer Register
	OCR0A = 98; //Set Interval
	TIMSK0 |= (1 << OCIE0A); //Set Timer

	//sei(); //Enable Global Interupt

	TCCR0B |= (1 << CS00) | (1 << CS02);

	//Setup ADC for Randomization
	ADMUX |= (1 << MUX0) | (1 << MUX1); //Set Input Pin PB3
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Set Division Factor To 128
	ADCSRA |= (1 << ADEN); //Enable ADC
	ADCSRA |= (1 << ADSC); //Start Conversion
	while (ADCSRA & (1 << ADSC))
		; //Wait until conversion ended
	uint8_t seed = ADCL; //Generated Seed

	//for (int i = 0; i < SIZE; i++) {
	//sBuffer[i] = 0; //seed;
	//seed = ((seed*seed)+seed)%100;
	//}

	//Setup ADC for Button Input
	ADMUX = 0; //Clear Multiplexer Selection Register
	ADCSRA = 0; //Clear Control and Status Register A

	ADMUX |= (1 << MUX1) | (1 << ADLAR);
	//ADCSRA |= (1<<ADPS2) | (1<<ADPS1); // | (1<<ADPS0); //Set Division Factor To 128
	ADCSRA |= (1 << ADEN); //Enable ADC
	ADCSRA |= (1 << ADIE); //Enable ADC Interupt

	sei();
	//Enable Global Interupt
	ADCSRA |= (1 << ADSC); //Start ADC

	eeprom_read_block((void*) &trs, (const void*) &Lblk, 5);

	sBuffer[2] = p;

	while (1) {
		if (tm > 5) {
			Logic();
			tm = 0;
		}

		Render();
	}

	return 0;
}
