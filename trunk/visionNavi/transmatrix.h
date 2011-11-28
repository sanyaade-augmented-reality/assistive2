#ifndef TRANSMATRIX_H
#define TRANSMATRIX_H

#include "math.h"

//#include "pattern.h"
//#include "node.h"

class TransMatrix
	// macierz transformacji
	// http://pl.wikibooks.org/wiki/Metody_matematyczne_fizyki/Obr%C3%B3t_uk%C5%82adu_wsp%C3%B3%C5%82rz%C4%99dnych
	// http://pl.wikipedia.org/wiki/Elementarne_macierze_transformacji
{
public:
	TransMatrix();
	TransMatrix(double directionAngle, double distance, double faceAngle); // kat w stopniach, odleglosc w metrach
//	TransMatrix(Pattern &p);
//	TransMatrix(gConn &c);
	~TransMatrix();
	
	double m[4][4];

	double getYAngle() const;	// zwraca kat przesuniecia wzgl osi Y (gora-dol)
	double getDistance() const;	// liczy odleglosc przesuniecia w linii prostej w cm

	TransMatrix inverse();
	TransMatrix operator*(TransMatrix const &matrix) const;
private:
	void makeIdentityMat();
	void constructorHelper(double directionAngle, double distance, double faceAngle);
	// przeliczanie z jednostek openGLa na m 
	// okazuje sie ze po skalibrowaniu i odp wpisaniu rozmiaru markera jednostki to mm
	double units2m(double u) const { return u/1000; }
	double m2units(double m) const { return m*1000;}
};

#endif //TRANSMATRIX_H
