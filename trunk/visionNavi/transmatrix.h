#ifndef TRANSMATRIX_H
#define TRANSMATRIX_H

/**
* @file transmatrix.h
* @brief plik nag³ówkowy klasy TransMatrix
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include "math.h"

//#include "pattern.h"
//#include "node.h"

/**
* @class TransMatrix
*
* @brief Klasa zawieraj¹ca macierz transformacji
*
*/
class TransMatrix
	// macierz transformacji
	// http://pl.wikibooks.org/wiki/Metody_matematyczne_fizyki/Obr%C3%B3t_uk%C5%82adu_wsp%C3%B3%C5%82rz%C4%99dnych
	// http://pl.wikipedia.org/wiki/Elementarne_macierze_transformacji
{
public:
	TransMatrix();
	
	/** @brief Konstruktor
	 *
	 *  @param directionAngle k¹t kierunku pomiêdzy wektorem ruchu, a kierunkiem N
	 *  @param distance przesuniêcie
	 *  @param faceAngle k¹t w stopniach pomiêdzy kierunkiem N, a docelowym kierunkiem osi Z
	 */
	TransMatrix(double directionAngle, double distance, double faceAngle); // kat w stopniach, odleglosc w metrach

	/** @brief Destruktor */
	~TransMatrix();
	
	/**< macierz transformacji */
	double m[4][4];

	/** @brief oblicza k¹t obrotu osi Z wzglêdem osi pionowej Y */
	double getYAngle() const;	// zwraca kat przesuniecia wzgl osi Y (gora-dol)

	/** @brief oblicza przesuniêcie transformacji */
	double getDistance() const;	// liczy odleglosc przesuniecia w linii prostej w cm

	/** @brief odwraca macierz */
	TransMatrix inverse();
	
	/** @brief Mno¿enie macierzy
	 *
	 *  @param druga macierz
	 */
	TransMatrix operator*(TransMatrix const &matrix) const;
private:
	/** @brief tworzy macierz jednostkow¹ */
	void makeIdentityMat();

	/** @brief pomocnik konstruktora
	 *
	 *  @param directionAngle k¹t kierunku pomiêdzy wektorem ruchu, a kierunkiem N
	 *  @param distance przesuniêcie
	 *  @param faceAngle k¹t w stopniach pomiêdzy kierunkiem N, a docelowym kierunkiem osi Z
	 */
	void constructorHelper(double directionAngle, double distance, double faceAngle);
	// 
	// okazuje sie ze po skalibrowaniu i odp wpisaniu rozmiaru markera jednostki to mm

	/** @brief przeliczanie z jednostek openGLa na m
	 *
	 *  @param odleg³oœæ w jednostkach openGL
	 */
	double units2m(double u) const { return u/1000; }

	/** @brief przeliczanie metrów na jednostki OpenGL
	 *
	 *  @param odleg³oœæ w metrach
	 */
	double m2units(double m) const { return m*1000;}
};

#endif //TRANSMATRIX_H
