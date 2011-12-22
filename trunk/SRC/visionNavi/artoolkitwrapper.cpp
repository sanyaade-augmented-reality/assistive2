#include "artoolkitwrapper.h"
#include <AR/video.h>

/**
* @file artoolkitwrapper.cpp
* @brief metody klasy ARToolKitWrapper
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

ARToolKitWrapper::~ARToolKitWrapper()
{
	arVideoCapStop();
	arVideoClose();
}

int ARToolKitWrapper::init(string &sVideoConfFile, ARUint8** imageData, list<Pattern *>* patternList)
{
	frameNumber = 0;
	this->imageData = imageData;
	this->patternList = patternList;
	videoConfFile = new char[sVideoConfFile.size()];
	videoConfFile = (char *)sVideoConfFile.c_str();

	// Otwiera strumien wideo
	if( arVideoOpen( videoConfFile ) < 0 )
		return 1;

    // Ustawia rozdzielczosc strumienia
    if( arVideoInqSize(&xSize, &ySize) < 0 )
		return 2;

	return 0;
}


bool ARToolKitWrapper::grabFrame()
{
	// grab a video frame
    if( ((*imageData) = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return 0;
    }
    if( frameNumber == 0 )
		arUtilTimerReset();
	frameNumber++;
	return 1;
}

void ARToolKitWrapper::findMarkers()
{
	// detect the markers in the video frame
    if( arDetectMarker(*imageData, threshold, &markerInfo, &marker_num) < 0 ) 
        exit(0);
}

list<Pattern*> ARToolKitWrapper::getScene()
{
	std::list<Pattern*> scene; // Przechowuje wszystkie rozpoznane markery do zwrotu

	for (list<Pattern*>::iterator pat = patternList->begin() ; pat != patternList->end(); pat++ )
	{
		for(int j = 0; j < marker_num; j++ )
			// sprawdza czy marker w polu j to ten, który ma sprawdzane teraz id
			if(( (*pat)->id == markerInfo[j].id) && 
				// i czy jeszcze nigdzie go nie dopasowano
				// lub jeœli wczesniej dopasowano taki sam to sprawdza czy to dopasowanie nie jest lepsze
				((*pat)->markerInfo == NULL || markerInfo[j].cf > (*pat)->markerInfo->cf))
			{
				(*pat)->markerInfo = &markerInfo[j];
				(*pat)->matchProbability = markerInfo[j].cf;
			}
	
		// znaleziono wzor, wiec dodaj do sceny - do narysowania
		if ((*pat)->markerInfo)
		{
			// dodaje wzor
			scene.push_back(*pat);
			// liczy transformacje pomiedzy markerem a kamera
			arGetTransMat(scene.back()->markerInfo, scene.back()->center, scene.back()->width, scene.back()->transformation.m);
		}
	}
	return scene;
}