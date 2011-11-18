#pragma once

#include <string>
#include <AR/ar.h>

class Node;

class Pattern
{
public:
	Pattern(void);
	Pattern(std::string filename, int direction);
	~Pattern(void);

	bool init(std::string filename, int direction);

	int             id;
	double          width;
	double          center[2];
	double          transformation[3][4];
	bool initialized;
	
	// wskaünik na informacje o znalezionym markerze
	ARMarkerInfo* markerInfo;
	bool found;

	// info o polozeniu -----------------------

	// kπt osi Z (wychodzacej z markera w strone obserwatora) wzgl umowionego kierunku N
	// zakres 0-359
	int direction;
	float distance; // [m]
	Node* node;
	// ----------------------------------------

private:
	std::string file;
};
