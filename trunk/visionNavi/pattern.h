#pragma once

#include <string>
#include <AR/ar.h>

class Node;

class Pattern
{
public:
	Pattern(void);
	Pattern(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );
	~Pattern(void);

	bool init(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );
	int             id;
	double          width;
	double          center[2];
	double          transformation[3][4];
	bool initialized;
	
	// wskaŸnik na informacje o znalezionym markerze
	ARMarkerInfo* markerInfo;
	bool found;

	// info o polozeniu -----------------------

	
	// k¹t i odleg³oœæ po³o¿enia wzglêdem markera
	float directionAngle;
	float distanceFromNode; // [m]
	Node* node;

	// k¹t osi Z (wychodzacej z markera w strone obserwatora) wzgl umowionego kierunku N
	float faceAngle;
	// ----------------------------------------

private:
	std::string file;
	void autoFaceAngle();
};
