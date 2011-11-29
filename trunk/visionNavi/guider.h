#pragma once

#include <string>

#include "pattern.h"
#include "graph.h"

using namespace std;

enum Direction
{
	TURNAROUND,
	STRLEFT,
	LEFT,
	SOFTLEFT,
	FORWARD,
	SOFTRIGHT,
	RIGHT,
	STRRIGHT
};

class Guider
	// podaje wskazowki nawigacji na podstawie macierzy transformacji
{
public:
	Guider(Graph* graph) : graph(graph), aim(NULL) {};
	~Guider(void) {};

	bool update(list<Pattern*> scene);		// uaktualnia polozenie
	string getHint();						// wskazowka slowna
	Node* findUs() { return nearestNode; }	// znajduje wierzcho³ek najbli¿szy lokalizacji
	double getAngle() { return angle; }		// kat w strone ktorego masz isc
	//double getDistance() { return distance; }
	void setAim(Node* aim) { this->aim = aim; }
	string aimName();

private:
	Direction getDirection();	// kierunek do nastepnego wierzcholka
	
	double nextNodeDistance;	// odleglosc do nastepnego wierzcholka
	Direction direction;		// kierunek do nastepnego wierzcholka
	Node* nearestNode;			// najblizszy wierzcholek
	TransMatrix user2aim;		// macierz transformacji do nastepnego wierzcholka
	Pattern* nearestPattern;	// najblizszy marker

	Graph *graph;

	list<gConn*> path;			// aktualna sciezka do celu
	Node* aim;					// cel
	double angle;				// kat do celu
	double aimDistance;			// odlegosc do celu

	int lastUpdate;				// nr klatki w której ostatnio by³y uaktualniane wyniki
	
	static const char dictionary[8][32]; // slownik kierunkow
};
