#pragma once

/**
* @file guider.h
* @brief Plik nag³ówkowy klasy Guider
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
* @brief Klasa odpowiadaj¹ca za nawigacjê
*
*/ 
class Guider
{
public:
	/** @brief Konstruktor
	 *
	 *  @param graph wskaŸnik na graf z którego bêdzie korzysta³ obiekt
	 */
	Guider(Graph* graph) : graph(graph), aim(NULL), aimAchivedDist(0.8) {};

	/** @brief Destruktor*/
	~Guider(void) {};

	/** @brief uaktualnienie informacji o po³o¿eniu
	 *
	 *  @param scene Lista widocznych markerów z informacjami o nich
	 */
	bool update(list<Pattern*> scene);		// uaktualnia polozenie

	/** @brief zwraca wierzcho³ek, w ktorym znajduje siê u¿ytkownik*/
	Node* findUs() { return nearestNode; }	// znajduje wierzcho³ek najbli¿szy lokalizacji

	/** @brief zwraca k¹t wzglêdem osi pionowej, wzglêdem którego u¿ytkownik ma iœæ*/
	double getAngle() { return angle; }		// kat w strone ktorego masz isc

	/** @brief 
	 *
	 *  @param ustawia/zmienia cel
	 */
	void setAim(Node* aim) { this->aim = aim; _atAim=0;}
	
	/** @brief czy dotarto do celu*/
	bool atAim() { return _atAim; }

	/** @brief zwraca nazwê wierzcho³ka celu*/
	string aimName();

	/** @brief generuje wskazówkê s³own¹*/
	void makeHint();

	/** @brief pobiera wskazówkê s³own¹*/
	string getHint() { return hint; }

private:
	/** @brief zwraca kierunek do nastepnego wierzcholka*/
	Direction getDirection();
	
	double nextNodeDistance;	/**< odleglosc do nastepnego wierzcholka */
	Direction direction;		/**< kierunek do nastepnego wierzcholka */
	Node* nearestNode;			/**< najblizszy wierzcholek */
	TransMatrix user2aim;		/**< macierz transformacji do nastepnego wierzcholka */
	Pattern* nearestPattern;	/**< najblizszy marker */

	string hint;				/**< wygenerowana wskazówka s³owna */

	Graph *graph;				/**< graf po³¹czeñ*/

	list<gConn*> path;			/**< aktualna sciezka do celu */
	Node* aim;					/**< cel */
	double angle;				/**< kat do celu */
	double aimDistance;			/**< odlegosc do celu */
	bool _atAim;				/**< czy osiagnieto cel*/
	double aimAchivedDist;	/**< poni¿ej tej odleg³oœci do celu system uzna, ¿e cel zosta³ osi¹gniêty*/
	
	static const char dictionary[8][32];/**< slownik kierunkow */
};
