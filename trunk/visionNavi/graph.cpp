#include "graph.h"
#include <vector>

//const int Graph::start = 2;
//const int Graph::stop = 5;

using namespace std;

bool Graph::addConnection(Node* a, Node* b, int distance, int directionAB)
{
	if (!a || !b || directionAB < 0 || directionAB >= 360)
		return 0;
	a->connections.push_back(gConn(b, distance, directionAB));
    b->connections.push_back(gConn(a, distance, (directionAB + 180)%360));
	return 1;
}



Node* Graph::searchFor(int id)
{
    for (list<Node*>::iterator i=nodeList.begin(); i != nodeList.end(); i++)
    {
        if ((*i)->id == id)
			return (*i);
    }
    return 0;
}

list<Node*> Graph::getPath(Node* start, Node* stop)
	// Tu wstawi� algorytm Dijkstry
{
	list<Node *> result;
	result.push_back(stop);
	//vector<int> d;
	//vector<int> p;
	//for (int i=0; i<connectionNumber; i++)
	//{
	//	d[i] = -1;
	//	p[i] = -1;
	//}
	//int s = 0;
	//d[s] = 0;
	//list<Node *> Q = list<Node*>(nodeList);
	//while(!Q.empty())
	//{
	//	u = 
	//}

	return result;
}