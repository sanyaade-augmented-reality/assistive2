#pragma once

#include "math.h"

#include "pattern.h"
#include "node.h"

class TransMatrix
	// macierz transformacji - przechowuje tablice [3][4], a 4 wiersz to zawsze [0 0 0 1]
	// to mo¿e byc przydatne:
	// http://pl.wikibooks.org/wiki/Metody_matematyczne_fizyki/Obr%C3%B3t_uk%C5%82adu_wsp%C3%B3%C5%82rz%C4%99dnych
	// http://pl.wikipedia.org/wiki/Elementarne_macierze_transformacji
{
public:
	TransMatrix(void);
	TransMatrix(double directionAngle, double distance, double faceAngle); // kat w stopniach, odleglosc w metrach
	TransMatrix(Pattern &p);
	TransMatrix(gConn &c);
	~TransMatrix(void);
	
	double m[4][4];

	// funkcje zwracajace katy obrotu w stopniach wzgl osi - TODO
	double getXAngle();
	double getYAngle();
	double getZAngle();

	// liczy odleglosc przesuniecia w linii prostej w cm
	double getDistance();

	TransMatrix inverse();
	TransMatrix operator*(TransMatrix const &matrix) const;
private:
	// double determinant() const;
	// przeliczanie z jednostek openGLa na cm
	double units2cm(double u) const { return u/190*46; }
	double cm2units(double cm) const { return cm/46*190;}
};
