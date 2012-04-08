#ifndef GLWINDOW_H
#define GLWINDOW_H

/**
* @file glwindow.h
* @brief klasa GLWindow odpowiedzialna za wizualizacj�
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <list>
#include <string>

#include "pattern.h"

using namespace std;

/**
* @class GLWindow
*
* @brief Klasa odpowiadaj�ca za wizualizacj�
*
*/ 
class GLWindow
{
public:
	/* @brief destructor */
	~GLWindow(void);

	/** @brief initializing function
	 *
	 *  @param imageData wska�nik na wska�nik z przechwyconym obrazem
	 *  @param cameraParam parametry kalibracji kamery
	 *  @param xSize rozmiar X obrazu z kamery
	 *  @param ySize rozmiar Y obrazu z kamery
	 */
	void init(ARUint8** imageData, ARParam &cameraParam, int xSize, int ySize);

	/** @brief Rysuje obraz z kamery */
	void drawVideo();

	/** @brief Rysuje sze�ciany w miejscach wykrytych marker�w
	 *
	 *  @param scene lista obiekt�w do narysowania
	 */
	void draw3DScene(list<Pattern*> &scene);

	/** @brief Rysuje t�o do tekstu - bia�y prostok�t
	 *
	 *  @param startX wsp�rz�dna x pocz�tku prostok�ta
	 *  @param startY wsp�rz�dna y pocz�tku prostok�ta
	 *  @param sizeX rozmiar X
	 *  @param sizeY rozmiar Y
	 */
	void drawBackground(float startX, float startY, float sizeX, float sizeY);

	/** @brief Wypisuje tekst na ekranie.
	 *
	 *  @param str The text
	 *  @param x wsp�rz�dna x
	 *  @param y wsp�rz�dna y
	 */
	void printString(string str, float x, float y);

	/** @brief Rysuje strza�k�
	 *
	 *  @param yAngle k�t wzgl�dem osi pionowej, kt�ry ma wskazywa� z przedzia�u 0-360st, 0 - na wprost
	 *  @param distance odleg�o�� do celu
	 */
	void drawArrow(double yAngle);

	/* wska�nik na instancj� klasy (obiekt jest singletonem) */
	static GLWindow* instance();

private:
	GLWindow(void);

	ARUint8** imageData; /**< obraz z kamery*/
	int xSize;	/**< rozmiar obrazu (os X) */
	int ySize;	/**< rozmiar obrazu (os Y) */

	static GLWindow* s_instance; /**< wska�nik na instancj� obiektu*/
};

#endif //GLWINDOW_H