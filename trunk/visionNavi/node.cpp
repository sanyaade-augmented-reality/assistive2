#include "node.h"

Pattern* Node::addPattern(string filename, double distance, double directionAngle, double faceAngle)
{
	Pattern *pattern = new Pattern(filename+"\0", this, distance, directionAngle, faceAngle);
	if (!pattern->initialized)
	{
		delete pattern;
		return NULL; // zwraca pusta liste jako znak bledu
	}
	patterns.push_back(pattern);
	return pattern;
}

ostream& operator<<(ostream &out, const Node &n)
{
	out << n.id << ". " << n.placeName;
	return out;
}

bool Node::operator<(Node & rhs){
	return this->pathCost > rhs.pathCost;
}