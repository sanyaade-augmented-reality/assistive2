/**
* @file transmatrix.cpp
* @brief metody klasy TransMatrix
*
* @author Kamil Neczaj
*
* @date 9.12.2011
*/

#include <AR/ar.h>
#include <math.h>

#include "transmatrix.h"
#include "mod.h"

#define PI 3.14159265

void TransMatrix::makeIdentityMat()
	// author Kamil Neczaj
{
	// macierz jednostkowa
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			if (i!=j)
				m[i][j] = 0;
			else
				m[i][j] = 1;
}

TransMatrix::TransMatrix()
	// author Kamil Neczaj
{
	makeIdentityMat();
}

void TransMatrix::constructorHelper(double directionAngle, double distance, double faceAngle)
	// author Kamil Neczaj
{
	makeIdentityMat();
	//this->TransMatrix::TransMatrix(); // inicjalizacja zerami
	// rotation matrix depending on marker's orientation
	m[0][0] = cos(faceAngle*PI/180);
	m[2][2] = cos(faceAngle*PI/180);
	m[0][2] = sin(faceAngle*PI/180);
	m[2][0] = -sin(faceAngle*PI/180);
	
	m[0][3] = m2units( sin(directionAngle*PI/180) * distance); // X axis transformation
	m[2][3] = m2units( cos(directionAngle*PI/180) * distance); // Z axis transformation
	// Y asis is the vertical axis - X,Z horizontal
}

TransMatrix::TransMatrix(double directionAngle, double distance, double faceAngle)
	// author Kamil Neczaj
{
	constructorHelper(directionAngle, distance, faceAngle);
}

//TransMatrix::TransMatrix(Pattern &p)
//{
//	constructorHelper(p.directionAngle, p.distanceFromNode, p.faceAngle);
//}

//TransMatrix::TransMatrix(gConn &c)
//{
//	constructorHelper(c.direction, c.cost, 0);
//}

double TransMatrix::getDistance() const
	// author Kamil Neczaj
{
	return units2m(sqrt(pow(m[0][3],2) /*+ pow(m[1][3],2)*/ + pow(m[2][3],2)));
}

TransMatrix::~TransMatrix(void)
{
}

TransMatrix TransMatrix::operator*(TransMatrix const &matrix) const
	// author Kamil Neczaj
{
	TransMatrix result;
	double sum = 0;
	for (int i=0; i<4; i++) // row
	{
		for (int j=0; j<4; j++) // col
		{
			for (int k=0; k<4; k++) // no of sum element
				sum += m[i][k]*matrix.m[k][j];
			result.m[i][j] = sum;
			sum = 0;
		}
	}
	return result;
}

TransMatrix TransMatrix::inverse()
	// author Kamil Neczaj
{
	TransMatrix result;
	arUtilMatInv(this->m, result.m);
	return result;
}

double TransMatrix::getYAngle() const // for transformation
	// author Kamil Neczaj
{
	// which quater the angle is from
	double a = m[0][3]; // sin
	double b = m[2][3]; // cos
	double add = 0; // added angle depending on the halve 0 or 180
	double basicAngle = 180/PI*atan(abs(a)/abs(b));
	
	if (b<0)
		add = 180;
	
	if ( (a>0 && b>0) || (a<0 && b<0) )
		return add + basicAngle;
	else
		return mod(add - basicAngle + 360, 360);
}
