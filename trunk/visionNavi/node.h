#ifndef NODE_H
#define NODE_H

#include <AR/ar.h>
#include <list>
#include <string>

#include "pattern.h"

class Pattern;
class Node;

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
	Node(int id) : id(id) {}
	~Node() {};

	int id;	// nr wierzcholka
	std::list<gConn> connections;	// polaczenia z danego wierzcholka
	std::list<Pattern*> patterns;	// markery przyporzadkowane do danego wierzcholka
};

#endif // NODE_H
