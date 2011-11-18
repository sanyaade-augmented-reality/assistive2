#include "transmatrix.h"
#include <math.h>

#define PI 3.14159265

TransMatrix::TransMatrix()
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			if (i!=j)
				m[i][j] = 0;
			else
				m[i][j] = 1; // macierz jednostkowa
}

TransMatrix::TransMatrix(double angleY, double distZ)
{
	this->TransMatrix::TransMatrix(); // inicjalizacja zerami
	m[0][0] = cos(angleY*PI/180);
	m[2][2] = cos(angleY*PI/180);
	m[0][2] = sin(angleY*PI/180);
	m[2][0] = -sin(angleY*PI/180);
	m[2][3] = cm2units(distZ*100);
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

TransMatrix TransMatrix::inverse() const
{
	TransMatrix result;

	double mainDet = determinant();
	
	// wyznaczniki skrócone
	for (int i=0; i<3; i++) // wiersz
	{
		for (int j=0; j<3; j++) // kolumna
		{
			double plusProduct = 1;
			double minusProduct = 1;
			for (int a=0,b=0,d=2; a<3 && b<3 && d>=0; a++,b++,d--) // nr czynnika, w [4][4] jest jedynka wiec mnozenie do 3
			{
				// omijam kolumne i wiersz liczonego dopelnienia
				if (a==i) a++;
				if (b==j) b++;
				if (d==j) d--;
				plusProduct  *= m[a][b];
				minusProduct *= m[a][d];
			}
			// transpozycja - wiêc j zamienione z i
			result.m[j][i] = 1/mainDet * pow(-1.0,i+j) * (plusProduct - minusProduct);
		}
	}

	// jeszcze 3 wyznaczniki w 4 kolumnie (po transpozycji)
	for (int a=0; a<3; a++) // liczony wyznacznik
	{
		double det = 0;
		for (int i=0; i<3; i++)
		{
			double plusProduct = 1;
			double minusProduct = 1;
			for (int row=0, j1=(i<=a?i:i+1), j2=((7-i)%4>=a?7-i:6-i); row<3; row++, j1++, j2--)
			{
				if (j1%4 == a) j1++;
				if (j2%4 == a) j2--;
				plusProduct *= m[row][j1%4];
				minusProduct*= m[row][j2%4];
			}
			det += plusProduct;
			det -= minusProduct;
		}
		result.m[a][3] = 1/mainDet * pow(-1.0,a+3) * det;
	}



	return result;
}

double TransMatrix::determinant() const
{
	// z twierdzenia Cauchiego o macierzach blokowych Adam Cmiel - Algebra Wyklad07
	// wyznacznik tej macierzy to wyznacznik macierzy 3x3 z lewego gornego rogu
	double det = 0;
	for (int i=0; i<3; i++)
	{
		double plusProduct = 1;
		double minusProduct = 1;
		for (int j=0; j<3; j++)
		{
			plusProduct *= m[j][(i+j)%3];
			minusProduct*= m[j][(5-i-j)%3];
		}
		det += plusProduct;
		det -= minusProduct;
	}
	return det;
}

double TransMatrix::getYAngle()
{
	// sprawdzic z ktorej cwiartki jest kat
	double a = m[0][2];
	double b = m[2][2];
	double add = 0; // kat dodawany w zaleznosci od cwiartki 0,90,180 lub 270
	double basicAngle = 180/PI*atan(abs(a)/abs(b));

	if (b<0)
		add = 180;

	if ( (a>0 && b>0) || (a<0 && b<0) )
		return add + basicAngle;
	else
		return (add - basicAngle + 360)  - 360*floor((add - basicAngle + 360)/360);
}