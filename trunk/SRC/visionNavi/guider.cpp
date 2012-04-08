#include <sstream>
#include "guider.h"

/**
* @file guider.cpp
* @brief methods of Guider class
*
* @author Kamil Neczaj
*
* @date 9.12.2011
*/

const char Guider::dictionary[8][32] = 
{
	"turn back",
	"turn left tightly",
	"turn left", 
	"turn left slightly",
	"go forward",
	"turn right	slightly",
	"turn right",
	"turn right tightly"
};

void Guider::update(const list<Pattern*> scene)
{
	// if any pattern was found
	if (!scene.empty())
	{
		Pattern * nearestPatTemp = NULL;
		for (list<Pattern*>::const_iterator i=scene.begin(); i!=scene.end(); i++)
			if (!nearestPatTemp || (*i)->transformation.getDistance() < nearestPatTemp->transformation.getDistance() )
				nearestPatTemp  = *i;

		nearestPattern = nearestPatTemp;
		nearestNode = nearestPattern->node;
		
		if (aim)
			graph->getPath(nearestNode, aim, path);

		TransMatrix node2marker = TransMatrix(nearestPattern->directionAngle,nearestPattern->distanceFromNode,nearestPattern->faceAngle);
		TransMatrix user2marker = nearestPattern->transformation;
		TransMatrix node2user = node2marker*user2marker.inverse();
		TransMatrix node2aim; // by default identity matrix
		if (!path.empty())	// the last vertex has not been reached
			node2aim = TransMatrix(path.front()->direction, path.front()->cost, 0);
		
		user2aim = node2user.inverse()*node2aim;
		angle = user2aim.getYAngle();
		nextNodeDistance = user2aim.getDistance();
		
		aimDistance = nextNodeDistance;
		for (list<gConn*>::iterator i=path.begin(); i != path.end(); i++)
		{
			// for the first node, there is a cost computed from user2aim matrix 
			// (matrix of conversion the current position to the position of the next node)
			if (i != path.begin())
				aimDistance += (*i)->cost;
		}

		getDirection();
		if (!_atAim)
			makeHint();
	}
}

Direction Guider::getDirection()
{
	// angle in degrees 0-360
	if (180.0-45.0*0.5 < angle && angle < 180.0 + 45.0*0.5)
		direction =  TURNAROUND;
	else if (180.0 + 45.0*0.5 < angle && angle < 180.0 + 45.0*1.5)
		direction =  STRLEFT;
	else if (180.0 + 45.0*1.5 < angle && angle < 180.0 + 45.0*2.5)
		direction =  LEFT;
	else if (180.0 + 45.0*2.5 < angle && angle < 180.0 + 45.0*3.5)
		direction =  SOFTLEFT;
	else if (180.0 - 45.0*1.5 < angle && angle < 180.0 - 45.0*0.5)
		direction =  STRRIGHT;
	else if (180.0 - 45.0*2.5 < angle && angle < 180.0 - 45.0*1.5)
		direction =  RIGHT;
	else if (180.0 - 45.0*3.5 < angle && angle < 180.0 - 45.0*2.5)
		direction =  SOFTRIGHT;
	else
		direction =  FORWARD;
	return direction;
}

void Guider::makeHint()
	// text hint
{
	ostringstream os;
	os.precision(2);
	os.setf(ios::fixed);
	if (aimDistance > aimAchivedDist)
	{
		os << "you are in " << nearestNode->placeName << ", " << dictionary[direction] << ",\nthere is " << aimDistance << "m left to the aim";
		if (!path.empty())
			os << ", " << nextNodeDistance << "m left to the nearest pass point";
	}
	else
	{
		_atAim = 1;
		os << "You have reached the aim!";
	}
	hint = os.str();
}

string Guider::aimName()
{
	if (aim)
		return aim->placeName;
	else
		return string();
}