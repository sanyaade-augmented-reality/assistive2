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
ARUint8*			imageData; // obraz
GLWindow*			gl;

// zmienne menu
int				page = 0;
string			userText;
bool			enterToMenu = 1;
bool			confirmed = 0;
int				maxPage;
bool			constrErr = 0; // b³¹d wartoœci z poza zakresu wierzcho³ków

static void   init( int choice );
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop();
void menu();
void menuGL();

int main(int argc, char **argv)
{
	// Do sprawdzenia czy potrzebne pliki istniej¹:
	fstream fin;
	bool fileExist = false;
	// Menu:
	int choice = 0;
	do {
		cout << "==============================" << endl
			<<  "Witamy w programie Assistive 2" << endl
			<<  "==============================" << endl << endl
			<<  "Prosze wybrac tryb pracy:" << endl
			<<  "   1. Przechwytywanie obrazu z kamery." << endl
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
				cout << "Blad: Plik "<< demoFiles[choice - 2] << " nie istnieje!" << endl;
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
{
	gl = GLWindow::instance();
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
	//cout << videoConfFile[choice - 1] << endl;
	int error = ar.init(videoConfFile[choice - 1], &imageData, patterns);
	if (error == 1)
	{
		cout << "Nie mozna otworzyc pliku konfiguracyjnego: " << videoConfFile[choice - 1] << endl;
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
{
	GLWindow* gl = GLWindow::instance();
	list<Pattern*> scene;
    
	if(!ar.grabFrame())
		return; //nie udalo sie
	ar.findMarkers();
    arVideoCapNext();
	gl->drawVideo();
	scene = ar.getScene();

	if (enterToMenu)
		menuGL();
	else
	{
		// górne pole informacyjne

		string aim = guider->aimName();
		ostringstream os;
		if (!aim.empty())
		{
			gl->drawBackground(-0.97, 0.90, 1.5, 0.1);
			os << "Nawigacja do " << aim << ". ";
			os	<< "Nacisnij Enter aby zmienic cel";
		}
		else
		{
			gl->drawBackground(-0.97, 0.9, 0.8, 0.1);
			os	<< "Nacisnij Enter aby ustalic cel";
		}
		gl->printString(os.str(), -0.95, 1.0);
	}
	

	// nic nie ma
	if(!guider->aimName().empty())
	{
		// dolne pole informacyjne
		if (!scene.empty())
		{
			gl->draw3DScene(scene);
			guider->update(scene);
			gl->drawArrow(guider->getAngle());
		}
		gl->drawBackground(-0.97, -1.0, 1.9, 0.22);
		gl->printString(guider->getHint(), -0.95, -0.85);

		// usuwa niepotrzebne wskazniki
		for (list<Pattern*>::iterator it=scene.begin() ; it != scene.end(); it++ )
			(*it)->markerInfo = NULL;
	}

    argSwapBuffers();
}

void keyEvent( unsigned char key, int x, int y)
{
    // ESC - wyjscie
	if(enterToMenu)
	{
		printf("%d\n", key);
		if (key >= '0' && key <= '9') // sprawdza czy u¿ytkownik wpisa³ cyfrê
			userText.append(1, key);
		else if (key == 8 && !userText.empty()) //backspace
			userText.erase(--userText.end());
		else if (key ==27)
		{
			enterToMenu = 0;
			userText.clear();
		}
		else if ( key == 13)
			confirmed = 1;
		// obsluga duzej liczby wierzcholkow
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
			enterToMenu = 1; // nowy cel
	}
}

void menu()
{
	unsigned int i;
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

void menuGL()
{
	int pointsNumber = graph.nodeList.size();
	int linesNumber = floor((float)ar.getSizeY()-14.0)/16.0 - 1; // ilosc punktow ktore sie zmieszcza
	maxPage = ceil((float)pointsNumber/(float)linesNumber);
	Node** nodeT = new Node*[pointsNumber];
	string str = "Dostepne punkty: ";
	if (maxPage>1)
		str = str + "<- poprz. str, nast. strona ->";
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
	ss << "Wybierz punkt docelowy";

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
		ss << "(Za duza wartosc!) ";

	ss << ": " << userText;

	gl->printString((char*)ss.str().c_str(), -1.0, -0.98 + 32.0/ar.getSizeY());
}