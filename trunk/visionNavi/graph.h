#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include "node.h"

/**
* @file graph.h
* @brief plik nag³ówkowy klasy Graph
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

using namespace std;

/**
* @class Graph
*
* @brief Klasa obiektu grafu
*
*/ 
class Graph
{
public:
	/** @brief Konstruktor 	 */
	Graph(): connectionNumber(0) {}
	/** @brief Inicjalizacja - wczytanie grafu */
	std::list<Pattern*>* init();

	/** @brief szuka w grafie wierzcho³ka o danym id
	 *
	 *  @param id numer id
	 */
    Node* searchFor(int id);    // szuka elementu a o danym id
	
	/** @brief Znajduje najszybsz¹ drogê przez graf
	 *
	 *  @param start wskaŸnik na element pocz¹tkowy
	 *  @param stop wskaŸnik na element koñcowy
	 *  @param referencja do zmiennej, w której zostanie zapisany wynik
	 */
	void getPath(Node* start, Node* stop, list<gConn*>& path);

	list<Node*> nodeList;		/**< lista wierzcho³ków*/
    int connectionNumber;       /**< iloœæ po³¹czeñ w grafie*/
	
	friend ostream& operator<<(ostream &out, const Graph &g);
private:

	/** @brief dodaje po³¹czenie miêdzy dwoma elementami
	 *
	 *  @param a pierwszy element
	 *  @param b drugi element
	 *  @param distance odleg³oœæ miêdzy nimi
	 *  @param directionAB k¹t w stopniach miêdzy wektorem AB, a wektorem bazowym
	 */
	bool addConnection(Node* a, Node* b, float distance, int directionAB);

	/** @brief dodaje wierzcho³ek do grafu
	 *
	 *  @param placeName nazwa wierzcho³ka
	 */
	Node* addNode(string placeName);
};

#endif // GRAPH_H
