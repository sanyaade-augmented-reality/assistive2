#include "artoolkitwrapper.h"
#include <AR/video.h>

/**
* @file artoolkitwrapper.cpp
* @brief methods of ARToolKitWrapper class
*
* @author Kamil Neczaj
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

	// open video stream
	if( arVideoOpen( videoConfFile ) < 0 )
		return 1;

    // set resolution
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
	std::list<Pattern*> scene; // stores all recognized markers at the view

	for (list<Pattern*>::iterator pat = patternList->begin() ; pat != patternList->end(); pat++ )
	{
		for(int j = 0; j < marker_num; j++ )
			// checks whether the j-th marker is the one which id is checked
			if(( (*pat)->id == markerInfo[j].id) && 
				// and it has not been assigned yet
				// or it has been assigned before, so check whether this assignment is better
				((*pat)->markerInfo == NULL || markerInfo[j].cf > (*pat)->markerInfo->cf))
			{
				(*pat)->markerInfo = &markerInfo[j];
				(*pat)->matchProbability = markerInfo[j].cf;
			}
	
		// a pattern was found so add to the scene
		if ((*pat)->markerInfo)
		{
			// adds pattern
			scene.push_back(*pat);
			// computes the transformation between the pattern and the camera
			arGetTransMat(scene.back()->markerInfo, scene.back()->center, scene.back()->width, scene.back()->transformation.m);
		}
	}
	return scene;
}