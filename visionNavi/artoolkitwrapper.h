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
* @brief class responsible for image processing using ARToolkit
*
*/ 
class ARToolKitWrapper
{
public:
	/** @brief constructor*/
	ARToolKitWrapper() : threshold(100) {};
	/** @brief destructor*/
	~ARToolKitWrapper();

	/** @brief ARToolkit initialization
	 *
	 *  @param videoConfFile file with input device configuration
	 *  @param imageData pointer to pointer of image data
	 *  @param patternList pointer to pattern list
	 */
	int init(string &videoConfFile, ARUint8** imageData, list<Pattern*>* patternList);

	/** @brief get new frame*/
	bool grabFrame();

	/** @brief find patterns on the loaded frame*/
	void findMarkers();

	/** @brief assign patterns from frame to loaded patterns*/
	list<Pattern*> getScene();

	/** @brief frame number*/
	int getFrameNumber() { return frameNumber; }

	/** @brief horizontal size of the image from video recorder*/
	int getSizeX() { return xSize; }

	/** @brief vertical size of the image from video recorder*/
	int getSizeY() { return ySize; }

private:
	ARUint8**       imageData;		/**< image */
    ARMarkerInfo*   markerInfo;		/**< info about recognized patterns */
    int             marker_num;		/**< no of patterns on the frame */
	int             frameNumber;	/**< frame no */
	int				xSize;			/**< image size */
	int				ySize;
	char*			videoConfFile;	/**< video rec configuration */
	char*			cameraCalibFile;/**< calibration file */
	list<Pattern *>* patternList;	/**< available pattern list */
	int				threshold;		/**< treshold */
};

#endif //ARTOOLKITWRAPPER_H