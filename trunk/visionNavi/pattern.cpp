#include <math.h>

#include "pattern.h"
#include "mod.h"

Pattern::Pattern()
{
	initialized = 0;
	width = 80.0; // dl boku markera
	center[0] = 0.0;
	center[1] = 0.0;
	found = 0;
	id = -1;
	markerInfo = NULL;
}

Pattern::Pattern(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle )
{
	this->Pattern::Pattern();
	init(filename, node, distanceFromNode, directionAngle, faceAngle);
}

bool Pattern::init(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle )
	// faceAngle = -1.0 to automatyczny k¹t, bêdzie ok, gdy marker jest powieszony na œcianie w pomieszczeniu prostokatnym
	// ze œcianami równolegle i prostopadle do kierunku N
{
	if( (id = arLoadPatt(filename.c_str()) ) < 0 || !(directionAngle >= 0 && directionAngle < 360) || !((faceAngle >= 0 && faceAngle < 360) || faceAngle == -1.0) )
		initialized = 0;
	else
	{
		initialized = 1;
		file = filename;
		this->directionAngle = directionAngle;
		if (faceAngle = -1)
			autoFaceAngle();
		else
			this->faceAngle = faceAngle;
		this->distanceFromNode = distanceFromNode;
		this->node = node;
	}
	return initialized;
}

void Pattern::autoFaceAngle()
{
	float tempAngle = directionAngle + 45;
	int i = (int)floor( mod(tempAngle,360) /90);
	faceAngle = (float)((2 + i)%4*90);
}