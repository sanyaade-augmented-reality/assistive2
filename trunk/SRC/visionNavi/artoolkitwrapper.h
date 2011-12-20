#ifndef ARTOOLKITWRAPPER_H
#define ARTOOLKITWRAPPER_H

/**
* @file graph.h
* @brief plik nag³ówkowy klasy ARToolKitWrapper
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <AR/ar.h>
#include <AR/video.h>
#include <AR/param.h>
#include <string>
#include <list>

#include "pattern.h"

using namespace std;

/**
* @class ARToolKitWrapper
*
* @brief Klasa zajmuj¹ca siê przetwarzaniem obrazu za pomoc¹ ARToolkit
*
*/ 
class ARToolKitWrapper
{
public:
	/** @brief konstruktor*/
	ARToolKitWrapper() : threshold(100) {};
	/** @brief destruktor*/
	~ARToolKitWrapper();

	/** @brief funkcja inicjalizuj¹ca ARToolkita
	 *
	 *  @param videoConfFile nazwa pliku konfiguracji urz¹dzenia wejœciowego
	 *  @param imageData wskaŸnik na wskaŸnik danych obrazu
	 *  @param patternList wskaŸnik na listê markerów
	 */
	int init(string &videoConfFile, ARUint8** imageData, list<Pattern*>* patternList);

	/** @brief pobierz now¹ klatkê z urz¹dzenia*/
	bool grabFrame();

	/** @brief znajdŸ markery na klatce*/
	void findMarkers();

	/** @brief dopasuj znalezione wzory do wzorów wczytanych do programu*/
	list<Pattern*> getScene();

	/** @brief numer przetwarzanej ramki*/
	int getFrameNumber() { return frameNumber; }

	/** @brief rozmiar poziomy obrazu z kamery*/
	int getSizeX() { return xSize; }

	/** @brief rozmiar pionowy obrazu z kamery*/
	int getSizeY() { return ySize; }

private:
	ARUint8**       imageData;		/**< obraz */
    ARMarkerInfo*   markerInfo;		/**< informacje o wykrytych markerach */
    int             marker_num;		/**< iloœæ markerów w ramce */
	int             frameNumber;	/**< nr ramki */
	int				xSize;			/**< rozmiary obrazu */
	int				ySize;
	char*			videoConfFile;	/**< konfiguracja kamery */
	char*			cameraCalibFile;/**< plik kalibracji kamery */
	list<Pattern *>* patternList;	/**< lista markerow do wykrywania */
	int				threshold;		/**< prog */
};

#endif //ARTOOLKITWRAPPER_H