#include <stdlib.h>
#include <time.h>

#define SIZE 8

#define _BV(bit) \
	(1 << (bit))

#define CLK 2 //Clock
#define DT 0 //Data
#define LTC 1 //Latch

uint8_t tBuffer[SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t sBuffer[SIZE] =
{
	0b00000000,
	0b00000000,
	0b00000001,
	0b00000101,
	0b00000011,
	0b00000000,
	0b00000000,
	0b00000000
};

uint8_t PORTB = 0b00000000;

int main() {
	int8_t iY, iX = 0;
	clock_t clk1, clk2;

	clk1 = clock();

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

	clk2 = clock();

	float tm = ((float)clk2 - (float)clk1) / CLOCKS_PER_SEC;
	printf("%f \n", tm);

	return 0;
}