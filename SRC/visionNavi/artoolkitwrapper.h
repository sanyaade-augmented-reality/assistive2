#ifndef ARTOOLKITWRAPPER_H
#define ARTOOLKITWRAPPER_H

/**
* @file graph.h
* @brief plik nag��wkowy klasy ARToolKitWrapper
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
* @brief Klasa zajmuj�ca si� przetwarzaniem obrazu za pomoc� ARToolkit
*
*/ 
class ARToolKitWrapper
{
public:
	/** @brief konstruktor*/
	ARToolKitWrapper() : threshold(100) {};
	/** @brief destruktor*/
	~ARToolKitWrapper();

	/** @brief funkcja inicjalizuj�ca ARToolkita
	 *
	 *  @param videoConfFile nazwa pliku konfiguracji urz�dzenia wej�ciowego
	 *  @param imageData wska�nik na wska�nik danych obrazu
	 *  @param patternList wska�nik na list� marker�w
	 */
	int init(string &videoConfFile, ARUint8** imageData, list<Pattern*>* patternList);

	/** @brief pobierz now� klatk� z urz�dzenia*/
	bool grabFrame();

	/** @brief znajd� markery na klatce*/
	void findMarkers();

	/** @brief dopasuj znalezione wzory do wzor�w wczytanych do programu*/
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
    int             marker_num;		/**< ilo�� marker�w w ramce */
	int             frameNumber;	/**< nr ramki */
	int				xSize;			/**< rozmiary obrazu */
	int				ySize;
	char*			videoConfFile;	/**< konfiguracja kamery */
	char*			cameraCalibFile;/**< plik kalibracji kamery */
	list<Pattern *>* patternList;	/**< lista markerow do wykrywania */
	int				threshold;		/**< prog */
};

#endif //ARTOOLKITWRAPPER_H