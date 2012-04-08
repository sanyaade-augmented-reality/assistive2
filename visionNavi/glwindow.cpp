#ifdef _WIN32
#include <windows.h>
#endif
#include <AR/gsub.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "GLWindow.h"

/**
* @file glwindow.cpp
* @brief methods of GLWindow class
*
* @author Kamil Neczaj
*
* @date 9.12.2011
*/

GLWindow* GLWindow::s_instance = 0;

GLWindow::GLWindow(void)
{
}

GLWindow::~GLWindow(void)
{
	argCleanup();
}

void GLWindow::init(ARUint8** imageData, ARParam &cameraParam, int xSize, int ySize)
{
	this->imageData = imageData;
	this->xSize = xSize;
	this->ySize = ySize;
	// open the graphics window
    argInit( &cameraParam, 1.0, 0, 0, 0, 0 );
}

void GLWindow::drawVideo()
{
	argDrawMode2D();
    argDispImage(*imageData, 0,0 );
}

void GLWindow::printString(string str, float x, float y)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // position
  glTranslatef(x, y-28.0/(float)ySize, 0.0);

  // text
  glColor3f(0.75, 0.0, 0.0);
  glRasterPos2i(0.05, 0.0);
  for (unsigned int i=0; i<str.length(); i++)
      if(str[i] != '\n' )
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
      else
	  {
          glTranslatef(0.0, -32.0/(float)ySize, 0.0);
          glRasterPos2i(0.0, 0.0);
      }
}

void GLWindow::drawBackground(float startX, float startY, float sizeX, float sizeY)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// position
	glTranslatef(startX, startY, 0.0);

	// background
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	{
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, sizeY);
		glVertex2f(sizeX, sizeY);
		glVertex2f(sizeX, 0.0);
	}
	glEnd();
}

void GLWindow::drawArrow(double yAngle)
{
    argDrawMode3D();
    argDraw3dCamera(0, 0);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
		
	// loads an identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	// space transformation --------------------------
	
	// axis Y 35 deg down, Z - 180 forward
	glTranslatef(0.0, 35.0, 180.0);
	
	// angles in degrees
	// slightly to up (10 deg - X axis)
	glRotated(10.0, 1.0, 0.0, 0.0);
	glRotated(yAngle, 0.0, 1.0, 0.0);
	glRotated(0.0, 0.0, 0.0, 1.0);

	// ----------------------------------------------------
		

	// color, lightnings, material -----------------------

	GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 0.3};
	GLfloat   mat_flash[]       = {0.0, 5.0, 5.0, 1.0};
	GLfloat   mat_flash_shiny[] = {100.0};
	GLfloat   light_position[]  = {80.0, 120.0,120.0,3.0};
	GLfloat   ambi[]            = {30.0, 0.5, 0.1, 0.1};
	GLfloat   lightZeroColor[]  = {0.8, 0.8, 0.8, 0.1};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	// ------------------------------------------------------

	// rysowanie strzalki -----------------------------------
	// korpus strza³ki
	glBegin(GL_QUADS);
	{
		// back
		glVertex3f(-10.0, -4.0, -20.0);
		glVertex3f(10.0, -4.0, -20.0);
		glVertex3f(10.0, 4.0, -20.0);
		glVertex3f(-10.0, 4.0, -20.0);

		// left
		glVertex3f(10.0, -4.0, 20.0);
		glVertex3f(10.0, 4.0, 20.0);
		glVertex3f(10.0, 4.0, -20.0);
		glVertex3f(10.0, -4.0, -20.0);
		// right
		glVertex3f(-10.0, -4.0, 20.0);
		glVertex3f(-10.0, 4.0, 20.0);
		glVertex3f(-10.0, 4.0, -20.0);
		glVertex3f(-10.0, -4.0, -20.0);
		// bottom
		glVertex3f(-10.0, -4.0, 20.0);
		glVertex3f(10.0, -4.0, 20.0);
		glVertex3f(10.0, -4.0, -20.0);
		glVertex3f(-10.0, -4.0, -20.0);
		// top
		glVertex3f(-10.0, 4.0, 20.0);
		glVertex3f(10.0, 4.0, 20.0);
		glVertex3f(10.0, 4.0, -20.0);
		glVertex3f(-10.0, 4.0, -20.0);
		// arrowhead - vertical front
		glVertex3f(0.0, 4.0, 35.0);
		glVertex3f(20.0, 4.0, 20.0);
		glVertex3f(20.0, -4.0, 20.0);
		glVertex3f(0.0, -4.0, 35.0);
		
		glVertex3f(0.0, 4.0, 35.0);
		glVertex3f(-20.0, 4.0, 20.0);
		glVertex3f(-20.0, -4.0, 20.0);
		glVertex3f(0.0, -4.0, 35.0);
		// arrowhead - vertical back
		glVertex3f(10.0, 4.0, 20.0);
		glVertex3f(20.0, 4.0, 20.0);
		glVertex3f(20.0, -4.0, 20.0);
		glVertex3f(10.0, -4.0, 20.0);

		glVertex3f(-10.0, 4.0, 20.0);
		glVertex3f(-20.0, 4.0, 20.0);
		glVertex3f(-20.0, -4.0, 20.0);
		glVertex3f(-10.0, -4.0, 20.0);
	}
	glEnd();

	// the top and the bottom of the arrowhead
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(-20.0, -4.0, 20.0);
		glVertex3f(20.0, -4.0, 20.0);
		glVertex3f(0.0, -4.0, 35.0);

		glVertex3f(-20.0, 4.0, 20.0);
		glVertex3f(20.0, 4.0, 20.0);
		glVertex3f(0.0, 4.0, 35.0);
	}
	glEnd();
	// ---------------------------------------

	glDisable( GL_LIGHTING );

	glDisable( GL_DEPTH_TEST );
}

GLWindow* GLWindow::instance()
{
	if (!s_instance)
		s_instance = new GLWindow;
	return s_instance;
}
