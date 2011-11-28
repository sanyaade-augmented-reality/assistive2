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
Guider*				guider;
ARUint8*			imageData; // obraz
bool				enterToMenu = 1;

static void   init();
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop();
void menu();

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

	guider = new Guider(&graph);

	gl.init(&imageData, cameraParam);
}

static void keyEvent( unsigned char key, int x, int y)
{
    // ESC - wyjscie
    if( key == 0x1b ) {
		printf("*** %f (frame/sec)\n", (double)ar.getFrameNumber()/arUtilTimer());
        exit(0);
    }
	else if (key == 13) // Enter
		// nowy cel
		enterToMenu = 1;
}

static void mainLoop(void)
{
	if (enterToMenu)
		menu();
	list<Pattern*> scene;
    
	if(!ar.grabFrame())
		return; //nie udalo sie
	gl.drawVideo();
	ar.findMarkers();
    arVideoCapNext();
	scene = ar.getScene();

	// górne pole informacyjne
	gl.drawBackground(-0.97, 0.87, 0.8, 0.1);
	gl.printString("Nacisnij Enter aby zmienic cel", -0.95, 0.90);
	
	// nic nie ma
	if( scene.empty() )
	{
        argSwapBuffers();
        return;
    }

	// narysuj je
	////gl.draw3DScene(scene);

	guider->update(scene);

	//char str[100];
	//printf("kierunek celu: %f3.3, odl: %f3.2\n", guider.getAngle(), guider.getDistance());
	//printf("------------------------------------\n");
	//for (list<Pattern*>::iterator i=scene.begin(); i!=scene.end(); i++)
	//	printf("id: %d, prob: %3.2f, odl: %3.2f\n", (*i)->id, (*i)->matchProbability, (*i)->transformation.getDistance() );
	
	// dolne pole informacyjne
	gl.drawBackground(-0.97, -0.97, 1.9, 0.22);
	gl.printString(guider->getHint(), -0.95, -0.85);

	gl.drawArrow(guider->getAngle());

	// usuwa niepotrzebne wskazniki
	for (list<Pattern*>::iterator it=scene.begin() ; it != scene.end(); it++ )
		(*it)->markerInfo = NULL;

    argSwapBuffers();
}

void menu()
{
	int i;
	string temp;
	cout << "Dostepne punkty: \n";
	cout << graph << endl;
	cout << endl;
	cout << "Wybierz cel: ";
	//cin.ignore(2);
	cin >> i;
	while (!cin.good() || i >= graph.nodeList.size())
	{
		if (!cin.good())
		{
			cin.clear();
			cin.ignore(255, '\n');
			cout<<"Nie podales liczby";
		}
		else
			cout << "Liczba musi byc w zakresie od 0 do " << graph.nodeList.size()-1;
		cout<<", sprobuj jeszcze raz: ";
		cin >> i;
	}
	Node *aim = graph.searchFor(i);
	cout << "Nawigacja do " << aim->placeName << endl;
	guider->setAim(aim);
	enterToMenu = 0;
}