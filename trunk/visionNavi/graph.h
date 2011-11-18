#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include "node.h"
//#include "individual.h"

using namespace std;

class Graph
{
public:
    Graph() { connectionNumber = 0; }

    Node* searchFor(int id);    // szuka elementu a o danym id
	bool addConnection(Node* a, Node* b, int distance, int directionAB);  // dodaje połączenie między dwoma elementami
	list<Node*> getPath(Node* start, Node* stop);	//znajduje najkrótszą ścieżkę start -> stop

	list<Node*> nodeList;		// lista wierzchołków
    int connectionNumber;       // ilość połączeń w grafie
};

#endif // GRAPH_H
