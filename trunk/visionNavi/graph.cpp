#include "graph.h"
#include <vector>

using namespace std;

std::list<Pattern*>* Graph::init()
	// wcztywanie danych - tu powinno siê znaleŸæ wczytywanie danych z pliku,
	// którego nazwa bêdzie podana jako argument tej funkcji
{
	list<Pattern*> *patternList = new std::list<Pattern*>;

	string patFiles[4] = 
	{
		"Data/patt.hiro",
		"Data/patt.kanji",
		"Data/patt.sample1",
		"Data/patt.sample2"
	};

	string placeNames[4] = 
	{
		"hiro",
		"kanji",
		"sample1",
		"sample2"
	};

	double patDist[4] =
	{
		0.5,
		0.5,
		0.5,
		0.5
	};

	double patDirAngle[4] =
	{
		0,
		0,
		0,
		0
	};

	Node *tNode[4];

	for (int i=0; i<4; i++)
	{
		tNode[i] = addNode(placeNames[i]);
		Pattern* pattern = tNode[i]->addPattern(patFiles[i], patDist[i], patDirAngle[i]);
		if(!pattern)
		{
			delete nodeList.back();
			nodeList.pop_back();
			delete patternList;
			return NULL; // zwraca NULL jako znak bledu
		}
		patternList->push_back(pattern);
	}
	
	addConnection(tNode[0], tNode[1], 5, 270);
	addConnection(tNode[1], tNode[2], 5, 270);
	addConnection(tNode[2], tNode[3], 5, 270);

	return patternList;
}

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
        if ((*i)->id == id)
			return (*i);
    return 0;
}

Node* Graph::addNode(string placeName)
{
	Node *node = new Node(nodeList.size(), placeName);
	nodeList.push_back(node);
	return node;
}

void Graph::getPath(Node* start, Node* stop, list<gConn*>& path)
	// zwracanie wyniku przez referencje path
	// Tu wstawiæ algorytm Dijkstry
{
	Node *currentNode = start;
	path.clear();
	
	// algorytm tymczaowy
	while (currentNode!=stop)
	{
		path.push_back(&currentNode->connections.back());
		currentNode = path.back()->dest;
	}
}

ostream& operator<<(ostream &out, const Graph &g)
{
	for (list<Node*>::const_iterator i=g.nodeList.begin(); i != g.nodeList.end(); i++)
		out << **i << endl;
	return out;
}