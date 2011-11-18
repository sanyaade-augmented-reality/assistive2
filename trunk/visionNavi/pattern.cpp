#include "pattern.h"

Pattern::Pattern()
{
	initialized = 0;
	width = 80.0;
	center[0] = 0.0;
	center[1] = 0.0;
	found = 0;
	id = -1;
	markerInfo = NULL;
}

Pattern::Pattern(std::string filename, int direction)
{
	this->Pattern::Pattern();
	init(filename, direction);
}

bool Pattern::init(std::string filename, int direction)
{
	if( (id = arLoadPatt(filename.c_str()) ) < 0 || !(direction >= 0 && direction < 360) )
		initialized = 0;
	else
	{
		initialized = 1;
		file = filename;
		this->direction = direction;
	}
	return initialized;
}
