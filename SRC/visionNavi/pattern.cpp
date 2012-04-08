/**
* @file pattern.cpp
* @brief metody klasy Pattern
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <math.h>

#include "pattern.h"
#include "mod.h"

Pattern::Pattern() : 
	initialized(0),	
	width(200.0), 
	id(-1), 
	markerInfo(NULL)
{
	center[0] = 0.0;
	center[1] = 0.0;
}

Pattern::Pattern(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle )
{
	this->Pattern::Pattern();
	init(filename, node, distanceFromNode, directionAngle, faceAngle);
}

bool Pattern::init(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle )
	// faceAngle = -1.0 -> automatic face angle (works in common situaltions)
{
	if( (id = arLoadPatt(filename.c_str()) ) < 0 || !(directionAngle >= 0 && directionAngle < 360) || !((faceAngle >= 0 && faceAngle < 360) || faceAngle == -1.0) )
		initialized = 0;
	else
	{
		initialized = 1;
		file = filename;
		this->directionAngle = directionAngle;
		if (faceAngle < -0.5)
			autoFaceAngle();
		else
			this->faceAngle = faceAngle;
		this->distanceFromNode = distanceFromNode;
		this->node = node;
	}
	return initialized;
}

void Pattern::autoFaceAngle()
	// automatic face angle - ok when a pattern sheet is hung on a wall in rectangular room
	// with walls parallel and ortogonal to N direction
{
	float tempAngle = directionAngle + 45;
	int i = (int)floor( mod(tempAngle,360) /90);
	faceAngle = (float)((2 + i)%4*90);
}