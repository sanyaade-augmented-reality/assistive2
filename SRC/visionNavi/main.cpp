#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>

#include <AR/gsub.h>
#include <AR/param.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

#include "graph.h"
#include "transmatrix.h"
#include "artoolkitwrapper.h"
#include "glwindow.h"
#include "guider.h"
#include "pattern.h"

/**
* @file main.cpp
* @brief main functions
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

using namespace std;

int preChoice = -1;

string	videoConfFile[] = {	"Data\\WDM_camera.xml", 
							"Data\\WDM_demo1.xml",
							"Data\\WDM_demo2.xml",
							"Data\\WDM_demo3.xml",
							"Data\\WDM_demo4.xml",
							"Data\\WDM_demo5.xml",
							"Data\\WDM_demo6.xml",
							"Data\\WDM_demo7.xml",	};


string demoFiles[] = {	"20111208131717300.avi", 
						"20111208132008026.avi",
						"20111208170501762.avi",
						"20111208170616832.avi",
						"20111208170642046.avi",
						"20111208170802344.avi",
						"20111208170847022.avi",};

int defChoices[] = { 11, 2, 2, 2, 13, 11, 14 };

char*	cameraCalibFile = "data\\kalibracja.dat";
string	patt_name[2];

Graph				graph;
ARToolKitWrapper	ar;
Guider*				guider;
ARUint8*			imageData; // image
GLWindow*			gl;

// There was an idea to convert these functions to a class,
// but it was decided that it is only the art for art's sake.
// The idea of such a class would not be coherent, because
// the functions below make many different, not related, general things.

// Global vars are ugly, I know, but I cannot change list of arguments of
// mainLoop and keyEvent (this form is demanded by ARToolkit)
// so it is the only way for the functions to comunicate.
// The only way to get rid of global functions would be making a class.

// menu variables
int				page = 0;
string			userText;
bool			enterToMenu = 1;
bool			confirmed = 0;
int				maxPage;
bool			constrErr = 0; // node beyond the range

static void   init( int choice );
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop();
void menu();
void menuGL();

int main(int argc, char **argv)
	// author Ernest Staszuk
{
	fstream fin;
	bool fileExist = false;
	// Menu:
	int choice = 0;
	do {
		cout << "==============================" << endl
			<<  "    Welcome in Assistive 2" << endl
			<<  "==============================" << endl << endl
			<<  "Please choose the mode" << endl
			<<  "   1. Work with image captured from a camera." << endl
			<<  "   2. Demo 1 - Toilets." << endl
			<<  "   3. Demo 2 - Room 103." << endl
			<<  "   4. Demo 3 - Room 103 II." << endl
			<<  "   5. Demo 4 - Room 103 short." << endl
			<<  "   6. Demo 5 - Xero machine." << endl
			<<  "   7. Demo 6 - Toilets from room 110." << endl
			<<  "   8. Demo 7 - Room 110." << endl << ">> ";
		cin >> choice;

		if(choice > 1 && choice <= 8)
		{
			fin.open(demoFiles[choice - 2].c_str(),ios::in);

			if( fin.is_open() )
			{
				fileExist = true;
				preChoice = defChoices[choice - 2];
				cameraCalibFile = "data\\kalibrdemo.dat";
			}
			else
			{
				cout << "Error: File "<< demoFiles[choice - 2] << " does not exist!" << endl;
				fileExist = false;
			}
		}
		else
			fileExist = true;

	} while(choice < 1 || choice > 8 || fileExist == false);

	init(choice);
	//glutInit(&argc, argv);
    arVideoCapStart();
	argMainLoop( NULL, keyEvent, mainLoop );
	return (0);
}

static void init( int choice )
	// author Kamil Neczaj
{
	gl = GLWindow::instance();
	// making graph connections ----------------
	list<Pattern*> *patterns = graph.init();
	if (!patterns)
	{
		cout << "Markers cannot be loaded.\n";
		delete patterns;
		exit(4);
	}
	// -----------------------------------------

	// video capture initialization -----
	int error = ar.init(videoConfFile[choice - 1], &imageData, patterns);
	if (error == 1)
	{
		cout << "Configure file cannot be opened: " << videoConfFile[choice - 1] << endl;
		exit(1);
	}
	else if (error==2) // 2
	{
		cout << "Image initialization error" << endl;
		exit(2);
	}
	//------------------------------------------

	// camera calibration -----------------------
	ARParam	rawCamParam;	// raw calibration from file
	ARParam	cameraParam;	// calibration taking into consideration image resolution
    if( arParamLoad(cameraCalibFile, 1, &rawCamParam) < 0 )
	{
		cout << "File cannot be opened: " << cameraCalibFile << endl;
		exit(3);
	}
	arParamChangeSize( &rawCamParam, ar.getSizeX(), ar.getSizeY(), &cameraParam );
	arInitCparam( &cameraParam );
	// ----------------------------------------

	guider = new Guider(&graph);

	if(preChoice != -1)
	{
		enterToMenu = false;
		guider->setAim(graph.searchFor(preChoice));	
		page = 0;
		constrErr = 0;
		confirmed = 0;

		preChoice = -1;
	}

	gl->init(&imageData, cameraParam, ar.getSizeX(), ar.getSizeY());
}

static void mainLoop(void)
	// author Kamil Neczaj
{
	GLWindow* gl = GLWindow::instance();
	list<Pattern*> scene;
    
	if(!ar.grabFrame())
		return; // fail, not all mainLoop excutions manage to grab image
	ar.findMarkers();
    arVideoCapNext();
	gl->drawVideo();
	scene = ar.getScene();

	if (enterToMenu)
		menuGL();
	else
	{
		// top information field
		string aim = guider->aimName();
		ostringstream os;
		if (!aim.empty())
		{
			gl->drawBackground(-0.97, 0.9, 1.93, 0.1);
			os << "Aim: " << aim << ". ";
			os	<< "Press Enter to change the aim";
		}
		else
			os	<< "Press Enter to set the aim";
		gl->printString(os.str(), -0.95, 1.0);

		if(!guider->aimName().empty())
		{
			if (!scene.empty())
			{
				guider->update(scene);
				if(!guider->atAim())
					gl->drawArrow(guider->getAngle());

				// get rid of unused pointer
				for (list<Pattern*>::iterator it=scene.begin() ; it != scene.end(); it++ )
					(*it)->markerInfo = NULL;
			}
			
			// bottom information field
			string hint = guider->getHint();
			if (!hint.empty())
			{
				gl->drawBackground(-0.97, -1.0, 1.93, 0.21);
				gl->printString(hint, -0.95, -0.85);
			}
		}
	}

    argSwapBuffers();
}

void keyEvent( unsigned char key, int x, int y)
	// author Kamil Neczaj
{
    // ESC - quit
	if(enterToMenu)
	{
		if (key >= '0' && key <= '9') // checks if the user typed a figure
			userText.append(1, key);
		else if (key == 8 && !userText.empty()) //backspace
			userText.erase(--userText.end());
		else if (key ==27) // Enter
		{
			enterToMenu = 0;
			userText.clear();
		}
		else if ( key == 13)
			confirmed = 1;
		// support of large node list divided to pages
		else if (key == GLUT_KEY_LEFT)
		{
			if (page>0) 
				page--;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			if (page<maxPage) 
				page++;
		}
	}
	else
	{
		if( key == 27 )
			exit(0);
		else if (key == 13)// Enter
			enterToMenu = 1; // new aim
	}
}

void menuGL()
	// author Kamil Neczaj
{
	int pointsNumber = graph.nodeList.size();
	int linesNumber = floor((float)ar.getSizeY()-14.0)/16.0 - 1; // ilosc punktow ktore sie zmieszcza
	maxPage = ceil((float)pointsNumber/(float)linesNumber);
	Node** nodeT = new Node*[pointsNumber];
	string str = "Available points: ";
	if (maxPage>1)
		str = str + "<- prev. page, next page ->";
	str += "\n";
	
	int t=0;
	for (list<Node*>::const_iterator i=graph.nodeList.begin(); i != graph.nodeList.end(); i++, t++)
		nodeT[t] = *i;

	for (int i = page*linesNumber; i<page*linesNumber+linesNumber && i < pointsNumber; i++)
	{
		ostringstream ss;
		ss << nodeT[i]->id << ". " << nodeT[i]->placeName << "\n";
		str = str + ss.str();
	}

	gl->drawBackground(-1.0, -1.0, 2.0, 2.0);
	gl->printString((char*)str.c_str(), -1.0, 1.0);
	
	ostringstream ss;
	ss << "Choose the aim";

	if (confirmed)
	{
		istringstream nr(userText);
		int nodeId;
		nr >> nodeId;
		if (nodeId < pointsNumber)
		{
			guider->setAim(graph.searchFor(nodeId));	
			enterToMenu = 0;
			page = 0;
			constrErr = 0;
			confirmed = 0;
			userText.clear();
		}
		else
		{
			constrErr = 1;
			confirmed = 0;
		}
	}

	if (constrErr)
		ss << "(Too large value!) ";

	ss << ": " << userText;

	gl->printString((char*)ss.str().c_str(), -1.0, -0.98 + 32.0/ar.getSizeY());
}