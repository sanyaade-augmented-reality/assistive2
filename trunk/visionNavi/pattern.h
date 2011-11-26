#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <AR/ar.h>

#include "transmatrix.h"
#include "node.h"

class TransMatrix;
class Node;

class Pattern
{
public:
	Pattern(void);
	Pattern(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );
	~Pattern(void);

	bool init(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );

	int			id;
	double		width;
	double		center[2];
	// je�li kompilator zatrzymuje sie na tej linijce to
	// zamiast TransMatrix transformation wpisac double transformation[4][4],
	// sprobowac skompilowac, po niepowodzeniu zmienic z powrotem
	// i teraz juz sie skompiluje :)
	TransMatrix transformation; //user2marker
	bool		initialized;
	
	// wska�nik na informacje o znalezionym markerze --------
	ARMarkerInfo* markerInfo;
	double matchProbability;
	//-------------------------------------------------------

	// info o polozeniu -----------------------	
	// k�t i odleg�o�� po�o�enia markera wzgl�dem najbli�szego wierzcho�ka grafu
	float directionAngle;	// [st]
	float distanceFromNode; // [m]
	Node* node;

	// k�t osi Z (wychodzacej z markera w strone obserwatora) wzgl umowionego kierunku N
	float faceAngle;	// [st]
	// ----------------------------------------

private:
	std::string file;
	void autoFaceAngle();
};

#endif //PATTERN_H