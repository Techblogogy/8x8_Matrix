#include <avr/io.h>
#include <avr/interrupt.h>

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

static void Logic() {

}

ISR (TIM0_COMPA_vect) {
	tm++;
}

//Button1 233
//Button2 42
//Button3 45

ISR (ADC_vect) {
	sBuffer[0] = ADCH;

	ADCSRA |= (1<<ADSC);
}

int main(void) {
	DDRB = 0b00000111; //Set PortB as Output

	//Setup Timer
	TCCR0A |= (1 << WGM02);
	OCR0A = 98;
	TIMSK0 |= (1 << OCIE0A);

	sei();

	TCCR0B |= (1 << CS00) | (1 << CS02);

	//Setup ADC for Randomization
	ADMUX |= (1<<MUX0) | (1<<MUX1); //Set Input Pin PB3
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //Set Division Factor To 128
	ADCSRA |= (1<<ADEN); //Enable ADC
	ADCSRA |= (1<<ADSC); //Start Conversion
	while (ADCSRA & (1<<ADSC)); //Wait until conversion ended
	uint8_t seed = ADCL; //Generated Seed

	//for (int i = 0; i < SIZE; i++) {
		//sBuffer[i] = 0; //seed;
		//seed = ((seed*seed)+seed)%100;
	//}

	//Setup ADC for Button Input
	ADMUX = 0; //Clear Multiplexer Selection Register
	ADCSRA = 0; //Clear Control and Status Register A

	ADMUX |= (1<<MUX1) | (1<<ADLAR);
	//ADCSRA |= (1<<ADPS2) | (1<<ADPS1);  // | (1<<ADPS0); //Set Division Factor To 128
	ADCSRA |= (1<<ADEN); //Enable ADC
	ADCSRA |= (1<<ADIE); //Enable ADC Interupt

	sei();
	ADCSRA |= (1<<ADSC);

	while (1) {
		if (tm > 1) {
			Logic();
			tm = 0;
		}

		Render();
	}

	return 0;
}