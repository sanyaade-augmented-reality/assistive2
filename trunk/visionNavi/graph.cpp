#include "graph.h"
#include <vector>

using namespace std;

bool Graph::addConnection(Node* a, Node* b, float distance, int directionAB)
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

Node* Graph::addNode()
{
	Node *node = new Node(nodeList.size());
	nodeList.push_back(node);
	return node;
}

list<Node*> Graph::getPath(Node* start, Node* stop)
	// Tu wstawiæ algorytm Dijkstry
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