#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include "node.h"

/**
* @file graph.h
* @brief plik nag��wkowy klasy Graph
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

	/** @brief szuka w grafie wierzcho�ka o danym id
	 *
	 *  @param id numer id
	 */
    Node* searchFor(int id);    // szuka elementu a o danym id
	
	/** @brief Znajduje najszybsz� drog� przez graf
	 *
	 *  @param start wska�nik na element pocz�tkowy
	 *  @param stop wska�nik na element ko�cowy
	 *  @param referencja do zmiennej, w kt�rej zostanie zapisany wynik
	 */
	void getPath(Node* start, Node* stop, list<gConn*>& path);

	list<Node*> nodeList;		/**< lista wierzcho�k�w*/
    int connectionNumber;       /**< ilo�� po��cze� w grafie*/
	
	friend ostream& operator<<(ostream &out, const Graph &g);
private:

	/** @brief dodaje po��czenie mi�dzy dwoma elementami
	 *
	 *  @param a pierwszy element
	 *  @param b drugi element
	 *  @param distance odleg�o�� mi�dzy nimi
	 *  @param directionAB k�t w stopniach mi�dzy wektorem AB, a wektorem bazowym
	 */
	bool addConnection(Node* a, Node* b, float distance, int directionAB);

	/** @brief dodaje wierzcho�ek do grafu
	 *
	 *  @param placeName nazwa wierzcho�ka
	 */
	Node* addNode(string placeName);
};

#endif // GRAPH_H
