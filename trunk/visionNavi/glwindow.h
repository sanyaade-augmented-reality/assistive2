#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <list>
#include <string>

#include "pattern.h"

using namespace std;

class GLWindow
{
public:
	~GLWindow(void);

	void init(ARUint8** imageData, ARParam &cameraParam, int xSize, int ySize);
	void drawVideo();
	void draw3DScene(list<Pattern*> &scene);
	void drawBackground(float startX, float startY, float sizeX, float sizeY);
	void printString(string str, float x, float y);
	void drawArrow(double yAngle);

	static GLWindow* instance();

private:
	GLWindow(void);
	ARUint8** imageData; // obraz z kamery
	int xSize;
	int ySize;

	static GLWindow* s_instance;
};

#endif //GLWINDOW_H