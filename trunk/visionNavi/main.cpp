#ifdef _WIN32
#include <windows.h>
#endif
//#include <stdio.h>
//#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

#include <iostream>
#include <vector>
#include <list>

#include "graph.h"
#include "transmatrix.h"

using namespace std;

//
// Camera configuration.
//
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

int             xsize, ysize;
int             thresh = 100;
int             count = 0;

char           *cparam_name    = "kalibracja.dat";
ARParam         cparam;
string          patt_name[2];

Graph graph;	//graf

class SceneItem
{
public:
	SceneItem(Pattern* pattern) : pattern(pattern) {};
	Pattern *pattern;
	TransMatrix camTrans;
};


std::list<Pattern*> patternList;

static void   init();
static void   cleanup();
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop();
static void	  draw( list<SceneItem> &scene );

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	init();

    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
	return (0);
}

static void init( void )
{
    ARParam  wparam;

	// tworzenie grafu po³¹czeñ ------------

	// zestaw testowy:
	//   N - w strone obserwatora, hiro po lewej od kanji
	//     
	//   kanji(B) ------------>  hiro(A)
	//   dirA=0,  dirAB = 90 (bo oœ Z do obserwatora),   dirB = 0

	Node* aNode = graph.addNode();
	Pattern *hiro = new Pattern("Data/patt.hiro\0", aNode, 0.5, 0);
	patternList.push_back(hiro);
	aNode->patterns.push_back(hiro);
	
	Node* bNode = graph.addNode();
	Pattern *kanji = new Pattern("Data/patt.kanji\0", bNode, 0.5, 0);	
	patternList.push_back(kanji);
	bNode->patterns.push_back(kanji);
	
	graph.addConnection(aNode, bNode, 0.5, 90);
	// -------------------------------------
	
    // open the video path
    if( arVideoOpen( vconf ) < 0 ) exit(0);

    // find the size of the window
    if( arVideoInqSize(&xsize, &ysize) < 0 )
		exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    // set the initial camera parameters
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }

    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

	for (list<Node*>::iterator node = graph.nodeList.begin() ; node != graph.nodeList.end(); node++)
		for (list<Pattern*>::iterator pat = (*node)->patterns.begin() ; pat != (*node)->patterns.end(); pat++ )
			if (!(*pat)->initialized) 
			{
				printf("pattern load error !!\n");
				exit(0);
			}

    // open the graphics window
    argInit( &cparam, 1.0, 0, 0, 0, 0 );
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

static void keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }
}

static void mainLoop(void)
{
    ARUint8         *dataPtr; // obraz
    ARMarkerInfo    *markerInfo;
//	ARMarkerInfo    *markerInfo_t[10];
    int             marker_num;

    // grab a vide frame
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    // detect the markers in the video frame
    if( arDetectMarker(dataPtr, thresh, &markerInfo, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }

    arVideoCapNext();

	std::list<SceneItem> scene;
    // check for object visibility
	for (list<Node*>::iterator node = graph.nodeList.begin() ; node != graph.nodeList.end(); node++ )
	{
		for (list<Pattern*>::iterator pat = (*node)->patterns.begin() ; pat != (*node)->patterns.end(); pat++ )
		{
			for(int j = 0; j < marker_num; j++ )
				// sprawdza czy marker w polu j to ten, który ma sprawdzane teraz id
				if( (*pat)->id == markerInfo[j].id )
					if( (*pat)->markerInfo == NULL 
						// jeœli wczesniej znaleziono taki sam to sprawdza czy to dopasowanie nie jest lepsze
						|| markerInfo[j].cf > (*pat)->markerInfo->cf)
						(*pat)->markerInfo = &markerInfo[j];
		
			// znaleziono wzor, wiec dodaj do sceny - do narysowania
			if ((*pat)->markerInfo)
			{
				// dodaje wzor
				scene.push_back(SceneItem(*pat));
				// liczy transformacje pomiedzy markerem a kamera
				arGetTransMat(scene.back().pattern->markerInfo, scene.back().pattern->center, scene.back().pattern->width, scene.back().pattern->transformation);
				arUtilMatInv(scene.back().pattern->transformation, scene.back().camTrans.m);
			}
		}
	}
	
	// nic nie ma
	if( scene.empty() )
	{
        argSwapBuffers();
        return;
    }

//	cout << scene.front()->node->connections.front().cost << "m " << scene.front()->node->connections.front().info << endl;

	// narysuj je
    draw(scene);

	// znalezc najblizszy marker
	Pattern* closestPat = scene.begin()->pattern;
	Node* currentNode = closestPat->node;
	// jesli widac ich kilka to ewentualnie zweryfikowac pozycje

	TransMatrix node2marker = TransMatrix(*closestPat);
	TransMatrix marker2user = scene.begin()->camTrans;
	TransMatrix node2user = node2marker*marker2user;

	TransMatrix node2aim = TransMatrix(currentNode->connections.front());

	TransMatrix user2aim = node2user.inverse()*node2aim;
	
	// wyci¹gn¹æ k¹ty
	printf("kierunek celu: %f3.3, odl: %f3.2\n", user2aim.getYAngle(), user2aim.getDistance());
	//printf("kierunek: %f3.3, odl: %f3.2\n", node2aim.getYAngle(), node2aim.getDistance());

	// usuwa niepotrzebne wskazniki
	for (list<SceneItem>::iterator it=scene.begin() ; it != scene.end(); it++ )
		(*it).pattern->markerInfo = NULL;

    argSwapBuffers();
}

static void draw( list<SceneItem> &scene )
{
    double    gl_para[16];

    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
	for (list<SceneItem>::iterator it=scene.begin() ; it != scene.end(); it++ )
	{
		GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
		GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
		GLfloat   mat_flash_shiny[] = {50.0};
		GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
		GLfloat   ambi[]            = {0.1, 0.5, 0.1, 0.1};
		GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
		
		//glColor3f( ((*it)->id%3)/3.0, (((*it)->id+1)%3)/3.0, (((*it)->id+2)%3)/3.0);
		/* load the camera transformation matrix */
		argConvGlpara((*it).pattern->transformation, gl_para);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd( gl_para );

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		// kolor, oswietlenie, material
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		// -----------

		//glMatrixMode(GL_MODELVIEW);
		glTranslatef( 0.0, 0.0, 25.0 );
		glutSolidCube(50.0);
		glDisable( GL_LIGHTING );
		glTranslatef( 0.0, 0.0, -25.0 );

		glDisable( GL_DEPTH_TEST );
	}
}
