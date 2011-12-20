#pragma once

/**
* @file guider.h
* @brief Plik nag��wkowy klasy Guider
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <string>

#include "pattern.h"
#include "graph.h"

using namespace std;

enum Direction
{
	TURNAROUND,
	STRLEFT,
	LEFT,
	SOFTLEFT,
	FORWARD,
	SOFTRIGHT,
	RIGHT,
	STRRIGHT
};

/**
* @class Guider
*
* @brief Klasa odpowiadaj�ca za nawigacj�
*
*/ 
class Guider
{
public:
	/** @brief Konstruktor
	 *
	 *  @param graph wska�nik na graf z kt�rego b�dzie korzysta� obiekt
	 */
	Guider(Graph* graph) : graph(graph), aim(NULL) {};

	/** @brief Destruktor*/
	~Guider(void) {};

	/** @brief uaktualnienie informacji o po�o�eniu
	 *
	 *  @param scene Lista widocznych marker�w z informacjami o nich
	 */
	bool update(list<Pattern*> scene);		// uaktualnia polozenie

	/** @brief generuje wskaz�wk� s�own�*/
	string getHint() { return hint; }						// wskazowka slowna

	/** @brief zwraca wierzcho�ek, w ktorym znajduje si� u�ytkownik*/
	Node* findUs() { return nearestNode; }	// znajduje wierzcho�ek najbli�szy lokalizacji

	/** @brief zwraca k�t wzgl�dem osi pionowej, wzgl�dem kt�rego u�ytkownik ma i��*/
	double getAngle() { return angle; }		// kat w strone ktorego masz isc

	/** @brief 
	 *
	 *  @param ustawia/zmienia cel
	 */
	void setAim(Node* aim) { this->aim = aim; _atAim=0;}
	
	/** @brief czy dotarto do celu*/
	bool atAim() { return _atAim; }

	/** @brief zwraca nazw� wierzcho�ka celu*/
	string aimName();

	void makeHint();

private:
	/** @brief zwraca kierunek celu*/
	Direction getDirection();	/**< kierunek do nastepnego wierzcholka */
	
	double nextNodeDistance;	/**< odleglosc do nastepnego wierzcholka */
	Direction direction;		/**< kierunek do nastepnego wierzcholka */
	Node* nearestNode;			/**< najblizszy wierzcholek */
	TransMatrix user2aim;		/**< macierz transformacji do nastepnego wierzcholka */
	Pattern* nearestPattern;	/**< najblizszy marker */

	string hint;

	bool _atAim;					/**< czy osiagnieto cel*/

	Graph *graph;

	list<gConn*> path;			/**< aktualna sciezka do celu */
	Node* aim;					/**< cel */
	double angle;				/**< kat do celu */
	double aimDistance;			/**< odlegosc do celu */

	int lastUpdate;				/**< nr klatki w kt�rej ostatnio by�y uaktualniane wyniki */
	
	static const char dictionary[8][32]; /**< slownik kierunkow */

	static const double goalDistance; /**< dystans odpowiadaj�cy osi�gni�ciu celu */
};
