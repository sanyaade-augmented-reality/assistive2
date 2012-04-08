#ifndef GLWINDOW_H
#define GLWINDOW_H

/**
* @file glwindow.h
* @brief klasa GLWindow odpowiedzialna za wizualizacjê
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
* @brief Klasa odpowiadaj¹ca za wizualizacjê
*
*/ 
class GLWindow
{
public:
	/* @brief destructor */
	~GLWindow(void);

	/** @brief initializing function
	 *
	 *  @param imageData wskaŸnik na wskaŸnik z przechwyconym obrazem
	 *  @param cameraParam parametry kalibracji kamery
	 *  @param xSize rozmiar X obrazu z kamery
	 *  @param ySize rozmiar Y obrazu z kamery
	 */
	void init(ARUint8** imageData, ARParam &cameraParam, int xSize, int ySize);

	/** @brief Rysuje obraz z kamery */
	void drawVideo();

	/** @brief Rysuje szeœciany w miejscach wykrytych markerów
	 *
	 *  @param scene lista obiektów do narysowania
	 */
	void draw3DScene(list<Pattern*> &scene);

	/** @brief Rysuje t³o do tekstu - bia³y prostok¹t
	 *
	 *  @param startX wspó³rzêdna x pocz¹tku prostok¹ta
	 *  @param startY wspó³rzêdna y pocz¹tku prostok¹ta
	 *  @param sizeX rozmiar X
	 *  @param sizeY rozmiar Y
	 */
	void drawBackground(float startX, float startY, float sizeX, float sizeY);

	/** @brief Wypisuje tekst na ekranie.
	 *
	 *  @param str The text
	 *  @param x wspó³rzêdna x
	 *  @param y wspó³rzêdna y
	 */
	void printString(string str, float x, float y);

	/** @brief Rysuje strza³kê
	 *
	 *  @param yAngle k¹t wzglêdem osi pionowej, który ma wskazywaæ z przedzia³u 0-360st, 0 - na wprost
	 *  @param distance odleg³oœæ do celu
	 */
	void drawArrow(double yAngle);

	/* wskaŸnik na instancjê klasy (obiekt jest singletonem) */
	static GLWindow* instance();

private:
	GLWindow(void);

	ARUint8** imageData; /**< obraz z kamery*/
	int xSize;	/**< rozmiar obrazu (os X) */
	int ySize;	/**< rozmiar obrazu (os Y) */

	static GLWindow* s_instance; /**< wskaŸnik na instancjê obiektu*/
};

#endif //GLWINDOW_H