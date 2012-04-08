#include "graph.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <queue>

/**
* @file graph.cpp
* @brief methods of Graph class
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

using namespace std;

// For string na figure conversion
template <class T>
	bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
		// author Ernest Staszuk
	{
		std::istringstream iss(s);
		return !(iss >> f >> t).fail();
	}

std::list<Pattern*>* Graph::init()
	// data loading
	// author Ernest Staszuk
{
	string buf;
	ifstream file("Data\\mapa.graph");
	int numberOfNodes = 0;
	int nodeIndex;

	list<Pattern*> *patternList = new std::list<Pattern*>;

	if (!file)
	{
        // Print an error and exit
        cerr << "File Data\\mapa.graph couldn't be opened for reading!" << endl;
        exit(1);
	}

	getline(file, buf);
	if(buf.compare(0,2,"GR") != 0)
	{
		// Print an error and exit
        cerr << "File Data\\mapa.graph is not proper graph file!" << endl;
        exit(1);
	}

	//Loading number of nodes
	while(getline(file, buf) && !buf.compare(0,1,"#"));
	from_string<int>(numberOfNodes, buf, std::dec);
		
	while(getline(file, buf) && !buf.compare(0,1,"#"));

	if(!buf.empty())
	{
		cerr << "Error in file Data\\mapa.graph, improper format" << endl;
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
		double facing;

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		if(!buf.compare(0,1,"*"))
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

			while(getline(file, buf) && !buf.compare(0,1,"#"));
			from_string<double>(facing, buf, std::dec);

			Pattern* pattern = tNode[nodeIndex]->addPattern(patFile, patDist, patDirAngle, facing);

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
		//while(getline(file, buf) && !buf.compare(0,1,"#"));
	}
	
	bool connEnd = false;

	while (connEnd == false)
	{
		int fromIndex,toIndex;
		double cost;
		double angle;

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		if(!buf.compare(0,1,"*"))
		{
			connEnd = true;
			continue;
		}

		// EST wczytywanie linii z polaczeniami - "z do odleglosc kat"
		sscanf_s(buf.c_str(), "%d %d %lf %lf", &fromIndex, &toIndex, &cost, &angle);

		/* EST old version of loading
		from_string<int>(fromIndex, buf, std::dec);
		
		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<int>(toIndex, buf, std::dec);

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<double>(cost, buf, std::dec);

		while(getline(file, buf) && !buf.compare(0,1,"#"));
		from_string<double>(angle, buf, std::dec);
		*/

		addConnection(tNode[fromIndex], tNode[toIndex], static_cast<float>(cost), static_cast<int>(angle));
	}

	file.close();

	return patternList;
}

bool Graph::addConnection(Node* a, Node* b, float distance, int directionAB)
	// author Kamil Neczaj
{
	if (!a || !b || directionAB < 0 || directionAB >= 360)
		return 0;
	a->connections.push_back(gConn(b, distance, directionAB));
    b->connections.push_back(gConn(a, distance, (directionAB + 180)%360));
	return 1;
}

Node* Graph::searchFor(int id)
	// author Kamil Neczaj
{
    for (list<Node*>::iterator i=nodeList.begin(); i != nodeList.end(); i++)
        if ((*i)->id == id)
			return (*i);
    return 0;
}

Node* Graph::addNode(string placeName)
	// author Kamil Neczaj
{
	Node *node = new Node(nodeList.size(), placeName);
	nodeList.push_back(node);
	return node;
}

void Graph::getPath(Node* start, Node* stop, list<gConn*>& path)
	// return of the result using path reference
	// author Ernest Staszuk
{
	Node* act;
	Node* next;

	list<Node*> toExplore;

	for(list<Node*>::iterator it = nodeList.begin(); it != nodeList.end(); it++)
	{
		act = *it;
		act->prevNode = NULL;
		act->prevConn = NULL;
		act->pathCost = 1e20;
		act->explored = false;
	}

	toExplore.push_front(start);
	path.clear();

	start->pathCost = 0.0;
	while(act != stop && !toExplore.empty())
	{
		act = toExplore.front();
		toExplore.pop_front();
		
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
					toExplore.push_front(next);
					toExplore.sort(relacja);
				}
			}
		}
	}

	// path reconstruction now
	act = stop;
	if(stop->prevNode != NULL)
	{
		// path found
		while(act != start)
		{
			path.push_front(act->prevConn);
			act = act->prevNode;
		}
	}
}

ostream& operator<<(ostream &out, const Graph &g)
{
	for (list<Node*>::const_iterator i=g.nodeList.begin(); i != g.nodeList.end(); i++)
		out << **i << endl;
	return out;
}