#ifdef _MATRIX_
#define _MATRIX_

#include "Arduino.h"

class Matrix
{
public:
	Matrix(byte _bSize, int _lathP, int _clockP, int _dataP);
private:
	byte* sBuff[]; //Screen Buffer

	int bSize;
	int latchP,clockP,dataP; //Shift Register Pins
};

#endif