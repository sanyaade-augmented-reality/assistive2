/**
* @file mod.cpp
* @brief funkcje reszty z dzielenia dla typów float i double
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <math.h>

float mod(float a, float b)
	// zwraca reszte z dzielenia a/b dla float
{
	return a - floor(a/b)*b;
}

double mod(double a, double b)
	// zwraca reszte z dzielenia a/b dla double
{
	return a - floor(a/b)*b;
}