#include "graph.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

// Do konwersji string na liczby
template <class T>
	bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
	{
		std::istringstream iss(s);
		return !(iss >> f >> t).fail();
	}

std::list<Pattern*>* Graph::init()
	// wcztywanie danych - tu powinno siê znaleŸæ wczytywanie danych z pliku,
	// którego nazwa bêdzie podana jako argument tej funkcji
{
	string buf;
	ifstream file("mapa.graph");
	int numberOfNodes = 0;
	int nodeIndex;

	list<Pattern*> *patternList = new std::list<Pattern*>;

	if (!file)
	{
        // Print an error and exit
        cerr << "File mapa.graph couldn't be opened for reading!" << endl;
        exit(1);
	}

	getline(file, buf);
	if(buf.compare(0,2,"GR") != 0)
	{
		// Print an error and exit
        cerr << "File mapa.graph is not proper graph file!" << endl;
        exit(1);
	}
	/*
	vector<string> patFiles; = 
	{
		"Data/patt.hiro",
		"Data/patt.kanji",
		"Data/patt.sample1",
		"Data/patt.sample2"
	};*/

	/*
	vector<string> placeNames; 
	{
		"hiro",
		"kanji",
		"sample1",
		"sample2"
	};

	vector<double> patDist; 
	{
		0.5,
		0.5,
		0.5,
		0.5
	};

	vector<double> patDirAngle = 
	{
		0,
		0,
		0,
		0
	};


	vector<int> indexes;*/

	//Loading number of nodes
		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<int>(numberOfNodes, buf, std::dec);
		cout << numberOfNodes;
		
	while(getline(file, buf) && !buf.compare(0,1,"#"));

	if(!buf.empty())
	{
		cerr << "Error in file data.graph, improper format" << endl;
		exit(1);
	}
	Node **tNode = 
		new Node*[numberOfNodes];
	bool nodesEnd = false;

	while (nodesEnd == false)
	{
		string patFile;
		double patDist;
		double patDirAngle;

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		if(buf.compare(0,1,"*"))
		{
			nodesEnd = true;
			continue;
		}

		from_string<int>(nodeIndex, buf, std::dec);
		while(getline(file, buf) && !buf.compare(0,1,"#"));
		tNode[nodeIndex] = addNode(buf);
	
		while(getline(file, buf) && !buf.compare(0,1,"#"));
		while(!buf.empty())
		{
			patFile = buf;

			while(getline(file, buf) && !buf.compare(0,1,"#"));
			from_string<double>(patDist, buf, std::dec);

			while(getline(file, buf) && !buf.compare(0,1,"#"));
			from_string<double>(patDirAngle, buf, std::dec);

			Pattern* pattern = tNode[nodeIndex]->addPattern(patFile, patDist, patDirAngle);
			if(!pattern)
			{
				delete nodeList.back();
				nodeList.pop_back();
				delete patternList;
				return NULL; // zwraca NULL jako znak bledu
			}
			patternList->push_back(pattern);

			while(getline(file, buf) && !buf.compare(0,1,"#"));
		}
		while(getline(file, buf) && !buf.compare(0,1,"#"));
	}
	
	bool connEnd = false;

	while (connEnd == false)
	{
		int fromIndex,toIndex;
		double cost;
		double angle;

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		if(buf.compare(0,1,"*"))
		{
			connEnd = true;
			continue;
		}

		from_string<int>(fromIndex, buf, std::dec);
		
		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<int>(toIndex, buf, std::dec);

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<double>(cost, buf, std::dec);

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<double>(angle, buf, std::dec);

		addConnection(tNode[fromIndex], tNode[toIndex], cost, angle);
	}

	file.close();

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

	// zwracanie wyniku przez referencje path
	// Tu wstawiæ algorytm Dijkstry
void Graph::getPath(Node* start, Node* stop, list<gConn*>& path)
{
	Node* act;
	Node* next;

	priority_queue<Node*> toExplore;

	for(list<Node*>::iterator it = nodeList.begin(); it != nodeList.end(); it++)
	{
		act = *it;
		act->prevNode = NULL;
		act->prevConn = NULL;
		act->pathCost = 1e20;
		act->explored = false;
	}

	toExplore.push(start);
	path.clear();

	while(act != stop && !toExplore.empty())
	{
		act = toExplore.top();
		toExplore.pop();
		
		if(act->explored == true)
			continue;

		act->explored = true;
		
		for(list<gConn>::iterator it = act->connections.begin(); it != act->connections.end(); it++)
		{
			next = (*it).dest;
			if(next->explored == false)
			{
				if(next->pathCost > act->pathCost + (*it).cost)
				{
					next->pathCost = act->pathCost + (*it).cost;
					next->prevNode = act;
					next->prevConn = &(*it);
					toExplore.push(next);
				}
			}
		}
	}
	
	//Teraz rekonstrukcja sciezki
	act = stop;
	if(stop->prevNode != NULL)
	{
		//znaleziono sciezke
		while(act != start)
		{
			path.push_back(act->prevConn);
			act = act->prevNode;
		}
	}

	// algorytm tymczaowy
	//while (currentNode!=stop)
	//{
	//	path.push_back(&currentNode->connections.back());
	//	currentNode = path.back()->dest;
	//}
}

ostream& operator<<(ostream &out, const Graph &g)
{
	for (list<Node*>::const_iterator i=g.nodeList.begin(); i != g.nodeList.end(); i++)
		out << **i << endl;
	return out;
}