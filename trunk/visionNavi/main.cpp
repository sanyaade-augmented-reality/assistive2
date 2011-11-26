#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>

#include <AR/gsub.h>
#include <AR/param.h>

#include <iostream>
#include <list>

#include "graph.h"
#include "transmatrix.h"
#include "artoolkitwrapper.h"
#include "glwindow.h"
#include "guider.h"
#include "pattern.h"

using namespace std;

string	videoConfFile = "Data\\WDM_camera_flipV.xml";
char*	cameraCalibFile = "kalibracja.dat";
string	patt_name[2];

Graph				graph;
ARToolKitWrapper	ar;
GLWindow			gl;
Guider				guider;
ARUint8*			imageData; // obraz

static void   init();
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop();

int main(int argc, char **argv)
{
	init();
	glutInit(&argc, argv);
    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
	return (0);
}

static void init( void )
{
	// tworzenie grafu po³¹czeñ ----------------
	list<Pattern*> *patterns = graph.init();
	if (!patterns)
	{
		cout << "Nie udalo sie wczytac markerow.\n";
		delete patterns;
		exit(4);
	}
	// -----------------------------------------

	// inicjalizacja przechwytywania video -----
	int error = ar.init(videoConfFile, &imageData, patterns);
	if (error == 1)
	{
		cout << "Nie mozna otworzyc pliku: " << videoConfFile << endl;
		exit(1);
	}
	else if (error==2) // 2
	{
		cout << "Blad inicjalizacji obrazu" << endl;
		exit(2);
	}
	//------------------------------------------

	// kalibracja kamery -----------------------
	ARParam	rawCamParam;	// kalibracja wczytana z pliku
	ARParam	cameraParam;	// kalibracja z uwzgledniona wielkoscia obrazu
    if( arParamLoad(cameraCalibFile, 1, &rawCamParam) < 0 )
	{
		cout << "Nie mozna otworzyc pliku: " << cameraCalibFile << endl;
		exit(3);
	}
	arParamChangeSize( &rawCamParam, ar.getSizeX(), ar.getSizeY(), &cameraParam );
	arInitCparam( &cameraParam );
	// ----------------------------------------

	printf("Image size (x,y) = (%d,%d)\n", ar.getSizeX(), ar.getSizeY());

	gl.init(&imageData, cameraParam);
}

static void keyEvent( unsigned char key, int x, int y)
{
    // ESC - wyjscie
    if( key == 0x1b ) {
		printf("*** %f (frame/sec)\n", (double)ar.getFrameNumber()/arUtilTimer());
        exit(0);
    }
}

static void mainLoop(void)
{
	list<Pattern*> scene;
    
	if(!ar.grabFrame())
		return; //nie udalo sie
	gl.drawVideo();
	ar.findMarkers();
    arVideoCapNext();
	scene = ar.getScene();
	
	// nic nie ma
	if( scene.empty() )
	{
        argSwapBuffers();
        return;
    }

	// narysuj je
	//gl.draw3DScene(scene);

	guider.update(scene);

	//char str[100];
	//printf("kierunek celu: %f3.3, odl: %f3.2\n", guider.getAngle(), guider.getDistance());
	//printf("------------------------------------\n");
	//for (list<Pattern*>::iterator i=scene.begin(); i!=scene.end(); i++)
	//	printf("id: %d, prob: %3.2f, odl: %3.2f\n", (*i)->id, (*i)->matchProbability, (*i)->transformation.getDistance() );
	
	
	gl.printString(guider.getHint());
	gl.drawArrow(guider.getAngle());

	// usuwa niepotrzebne wskazniki
	for (list<Pattern*>::iterator it=scene.begin() ; it != scene.end(); it++ )
		(*it)->markerInfo = NULL;

    argSwapBuffers();
}

