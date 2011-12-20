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
	Guider(Graph* graph) : graph(graph), aim(NULL), aimAchivedDist(0.8) {};

	/** @brief Destruktor*/
	~Guider(void) {};

	/** @brief uaktualnienie informacji o po�o�eniu
	 *
	 *  @param scene Lista widocznych marker�w z informacjami o nich
	 */
	bool update(list<Pattern*> scene);		// uaktualnia polozenie

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

	/** @brief generuje wskaz�wk� s�own�*/
	void makeHint();

	/** @brief pobiera wskaz�wk� s�own�*/
	string getHint() { return hint; }

private:
	/** @brief zwraca kierunek do nastepnego wierzcholka*/
	Direction getDirection();
	
	double nextNodeDistance;	/**< odleglosc do nastepnego wierzcholka */
	Direction direction;		/**< kierunek do nastepnego wierzcholka */
	Node* nearestNode;			/**< najblizszy wierzcholek */
	TransMatrix user2aim;		/**< macierz transformacji do nastepnego wierzcholka */
	Pattern* nearestPattern;	/**< najblizszy marker */

	string hint;				/**< wygenerowana wskaz�wka s�owna */

	Graph *graph;				/**< graf po��cze�*/

	list<gConn*> path;			/**< aktualna sciezka do celu */
	Node* aim;					/**< cel */
	double angle;				/**< kat do celu */
	double aimDistance;			/**< odlegosc do celu */
	bool _atAim;				/**< czy osiagnieto cel*/
	double aimAchivedDist;	/**< poni�ej tej odleg�o�ci do celu system uzna, �e cel zosta� osi�gni�ty*/
	
	static const char dictionary[8][32];/**< slownik kierunkow */
};
