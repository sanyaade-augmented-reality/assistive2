#ifndef NODE_H
#define NODE_H

#include <AR/ar.h>
#include <list>
#include <string>

#include "pattern.h"

class Node;

// graph connection - węzeł docelowy, koszt
class gConn
{
public:
	gConn(Node* dest, float cost, int direction)
    {
        this->dest = dest;
        this->cost = cost;
		this->direction = direction;
    }

    Node* dest; // węzeł docelowy
    float cost;   // koszt (droga w metrach)    

	// kat tak jak w pattern.h
	int direction; // kierunek połączenia
};

class Node
{
public:
	Node(int id)
	{
		this->id = id;
	}
	~Node();

	int id;

	std::list<gConn> connections;
	std::list<Pattern*> patterns;
};

#endif // NODE_H
