#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include "node.h"
//#include "individual.h"

using namespace std;

class Graph
{
public:
	Graph(): connectionNumber(0) {}
	std::list<Pattern*>* init();

    Node* searchFor(int id);    // szuka elementu a o danym id
	bool addConnection(Node* a, Node* b, float distance, int directionAB);  // dodaje połączenie między dwoma elementami
	Node* addNode();
	list<Node*> getPath(Node* start, Node* stop);	//znajduje najkrótszą ścieżkę start -> stop

	list<Node*> nodeList;		// lista wierzchołków
    int connectionNumber;       // ilość połączeń w grafie
};

#endif // GRAPH_H