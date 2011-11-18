#pragma once

#include "math.h"

class TransMatrix
	// macierz transformacji - przechowuje tablice [3][4], a 4 wiersz to zawsze [0 0 0 1]
	// to mo¿e byc przydatne:
	// http://pl.wikibooks.org/wiki/Metody_matematyczne_fizyki/Obr%C3%B3t_uk%C5%82adu_wsp%C3%B3%C5%82rz%C4%99dnych
	// http://pl.wikipedia.org/wiki/Elementarne_macierze_transformacji
{
public:
	TransMatrix(void);
	TransMatrix(double angleY, double distZ); // kat w stopniach, odleglosc w metrach
	~TransMatrix(void);
	
	double m[4][4];

	// funkcje zwracajace katy obrotu w stopniach wzgl osi - TODO
	double getXAngle();
	double getYAngle();
	double getZAngle();

	// liczy odleglosc przesuniecia w linii prostej w cm
	double getDistance() {return units2cm(sqrt(pow(m[0][3],2) + pow(m[1][3],2) + pow(m[2][3],2)));}

	TransMatrix inverse() const;
	TransMatrix operator*(TransMatrix const &matrix) const;
private:
	double determinant() const;
	// przeliczanie z jednostek openGLa na cm
	double units2cm(double u) const { return u/190*46; }
	double cm2units(double cm) const { return cm/46*190;}
};
