/**
* @file transmatrix.cpp
* @brief metody klasy TransMatrix
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <AR/ar.h>
#include <math.h>

#include "transmatrix.h"
#include "mod.h"

#define PI 3.14159265

void TransMatrix::makeIdentityMat()
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
{
	makeIdentityMat();
}

void TransMatrix::constructorHelper(double directionAngle, double distance, double faceAngle)
{
	makeIdentityMat();
	//this->TransMatrix::TransMatrix(); // inicjalizacja zerami
	// macierz obrotu w zaleznosci od kierunku markera w przestrzeni
	m[0][0] = cos(faceAngle*PI/180);
	m[2][2] = cos(faceAngle*PI/180);
	m[0][2] = sin(faceAngle*PI/180);
	m[2][0] = -sin(faceAngle*PI/180);
	
	m[0][3] = m2units( sin(directionAngle*PI/180) * distance); // przesuniecie wzdluz osi x
	m[2][3] = m2units( cos(directionAngle*PI/180) * distance); // przesuniecie wzdluz osi z
}

TransMatrix::TransMatrix(double directionAngle, double distance, double faceAngle)
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
{
	return units2m(sqrt(pow(m[0][3],2) /*+ pow(m[1][3],2)*/ + pow(m[2][3],2)));
}

TransMatrix::~TransMatrix(void)
{
}

TransMatrix TransMatrix::operator*(TransMatrix const &matrix) const
{
	TransMatrix result;
	double sum = 0;
	for (int i=0; i<4; i++) // wiersz
	{
		for (int j=0; j<4; j++) // kolumna
		{
			for (int k=0; k<4; k++) // nr skladnika sumy
				sum += m[i][k]*matrix.m[k][j];
			result.m[i][j] = sum;
			sum = 0;
		}
	}
	return result;
}

TransMatrix TransMatrix::inverse()
{
	TransMatrix result;
	arUtilMatInv(this->m, result.m);
	return result;
}

double TransMatrix::getYAngle() const // dla przesuniecia
{
	// sprawdzic z ktorej cwiartki jest kat
	double a = m[0][3];
	double b = m[2][3];
	double add = 0; // kat dodawany, w zaleznosci od polowy 0 lub 180
	double basicAngle = 180/PI*atan(abs(a)/abs(b));
	
	if (b<0)
		add = 180;
	
	if ( (a>0 && b>0) || (a<0 && b<0) )
		return add + basicAngle;
	else
		return mod(add - basicAngle + 360, 360);
}
