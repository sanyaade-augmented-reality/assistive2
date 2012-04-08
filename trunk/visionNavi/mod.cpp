/**
* @file mod.cpp
* @brief functions to compute rest after division of types float and double
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <math.h>

float mod(float a, float b)
	// computes rest after division of float by float
{
	return a - floor(a/b)*b;
}

double mod(double a, double b)
	// computes rest after division of double by double
{
	return a - floor(a/b)*b;
}