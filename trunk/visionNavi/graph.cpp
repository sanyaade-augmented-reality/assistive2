#include "graph.h"
#include <vector>

using namespace std;

std::list<Pattern*>* Graph::init()
	// wcztywanie danych - tu powinno siê znaleŸæ wczytywanie danych z pliku,
	// którego nazwa bêdzie podana jako argument tej funkcji
{
	list<Pattern*> *patternList = new std::list<Pattern*>;

	// zestaw testowy:
	//   N - w strone obserwatora, hiro po lewej od kanji
	//     
	//   kanji(B) ------------>  hiro(A)
	//   dirA=0,  dirAB = 90 (bo oœ Z do obserwatora),   dirB = 0

	Node* aNode = addNode();
	Pattern *hiro = new Pattern("Data/patt.hiro\0", aNode, 0.5, 0);
	if (!hiro->initialized)
		return NULL; // zwraca pusta liste jako znak bledu
	patternList->push_back(hiro);
	aNode->patterns.push_back(hiro);
	
	Node* bNode = addNode();
	Pattern *kanji = new Pattern("Data/patt.kanji\0", bNode, 0.5, 0);	
	if (!kanji->initialized)
		return NULL;
	patternList->push_back(kanji);
	bNode->patterns.push_back(kanji);

	//Node* cNode = addNode();
	
	addConnection(aNode, bNode, 5, 270);
	//addConnection(bNode, cNode, 5, 90);
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