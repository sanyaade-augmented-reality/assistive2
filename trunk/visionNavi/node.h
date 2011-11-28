#ifndef NODE_H
#define NODE_H

#include <AR/ar.h>
#include <list>
#include <string>

#include "pattern.h"

class Pattern;
class Node;

using namespace std;

// graph connection - węzeł docelowy, koszt
class gConn
{
public:
	gConn(Node* dest, float cost, int direction) : dest(dest), cost(cost), direction(direction) {}

    Node* dest;		// węzeł docelowy
    float cost;		// koszt (droga w metrach)
	int direction;	// kierunek połączenia - kat 0-360st wzgl kierunku N
};

class Node
{
public:
	Node(int id, string placeName) : id(id), placeName(placeName) {}
	~Node() {};

	Pattern* addPattern(string filename, double distance, double directionAngle, double faceAngle = -1.0);

	int id;							// nr wierzcholka
	string placeName;				// nazwa miejsca
	std::list<gConn> connections;	// polaczenia z danego wierzcholka
	std::list<Pattern*> patterns;	// markery przyporzadkowane do danego wierzcholka

	friend ostream& operator<<(ostream &out, const Node &n);
};

#endif // NODE_H
