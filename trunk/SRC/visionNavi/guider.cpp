#include <sstream>
#include "guider.h"

/**
* @file guider.cpp
* @brief metody klasy Guider
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

const char Guider::dictionary[8][32] = 
{
	"zawroc",
	"skrec mocno w lewo",
	"skrec w lewo", 
	"skrec lekko w lewo",
	"idz prosto",
	"skrec lekko w prawo",
	"skrec w prawo",
	"skrec mocno w prawo"
};

void Guider::update(const list<Pattern*> scene)
{
	// jesli znaleziono marker
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
		TransMatrix node2aim; // domyslnie macierz jednostkowa
		if (!path.empty())	// nie doszlismy do ostatniego wierzcholka
			node2aim = TransMatrix(path.front()->direction, path.front()->cost, 0);
		
		user2aim = node2user.inverse()*node2aim;
		angle = user2aim.getYAngle();
		nextNodeDistance = user2aim.getDistance();
		
		aimDistance = nextNodeDistance;
		for (list<gConn*>::iterator i=path.begin(); i != path.end(); i++)
		{
			if (i != path.begin()) // dla pierwszego wierzcholka mamy koszt wyliczony z macierzy przejscia
				aimDistance += (*i)->cost;
		}

		getDirection();
		if (!_atAim)
			makeHint();
	}
}

Direction Guider::getDirection()
{
	// kat w stopniach 0-360
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
{
	ostringstream os;
	os.precision(2);
	os.setf(ios::fixed);
	if (aimDistance > aimAchivedDist)
	{
		os << "znajdujesz sie w " << nearestNode->placeName << ", " << dictionary[direction] << ",\ndo celu pozostalo " << aimDistance << "m";
		if (!path.empty())
			os << ", do najblizszego punktu kontrolnego " << nextNodeDistance << "m";
	}
	else
	{
		_atAim = 1;
		os << "Jestes u celu!";
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