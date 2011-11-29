#ifndef ARTOOLKITWRAPPER_H
#define ARTOOLKITWRAPPER_H

#include <AR/ar.h>
#include <AR/video.h>
#include <AR/param.h>
#include <string>
#include <list>

#include "pattern.h"

using namespace std;

class ARToolKitWrapper
{
public:
	ARToolKitWrapper() : threshold(100) {};
	~ARToolKitWrapper();

	int init(string &videoConfFile, ARUint8** imageData, list<Pattern*>* patternList);
	bool grabFrame();
	void findMarkers();
	list<Pattern*> getScene();

	int getFrameNumber() { return frameNumber; }
	int getSizeX() { return xSize; }
	int getSizeY() { return ySize; }

private:
	ARUint8**       imageData;		// obraz
    ARMarkerInfo*   markerInfo;		// informacje o wykrytych markerach
    int             marker_num;		// iloœæ markerów w ramce
	int             frameNumber;	// nr ramki
	int				xSize;			// rozmiary obrazu
	int				ySize;
	char*			videoConfFile;	// konfiguracja kamery
	char*			cameraCalibFile;// plik kalibracji kamery
	list<Pattern *>* patternList;	// lista markerow do wykrywania
	int				threshold;		// prog
};

#endif //ARTOOLKITWRAPPER_H