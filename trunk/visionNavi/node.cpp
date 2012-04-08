/**
* @file node.cpp
* @brief metody klasy Node
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include "node.h"

Pattern* Node::addPattern(string filename, double distance, double directionAngle, double faceAngle)
	// author Kamil Neczaj
{
	Pattern *pattern = new Pattern(filename+"\0", this, static_cast<float>(distance), 
		static_cast<float>(directionAngle), static_cast<float>(faceAngle));
	if (!pattern->initialized)
	{
		delete pattern;
		return NULL; // zwraca pusta liste jako znak bledu
	}
	patterns.push_back(pattern);
	return pattern;
}

ostream& operator<<(ostream &out, const Node &n)
	// author Kamil Neczaj
{
	out << n.id << ". " << n.placeName;
	return out;
}

bool relacja(Node * lhs, Node * rhs){
	// author Ernest Staszuk
	return lhs->pathCost < rhs->pathCost;
}