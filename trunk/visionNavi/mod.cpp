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