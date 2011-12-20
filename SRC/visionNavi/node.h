#ifndef NODE_H
#define NODE_H

/**
* @file node.h
* @brief plik nag��wkowy klasy Node
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#include <AR/ar.h>
#include <list>
#include <string>

#include "pattern.h"

class Pattern;
class Node;

using namespace std;

/**
* @class gConn
*
* @brief Po��czenie
*
*/ 
class gConn
{
public:
	/** @brief Konstruktor
	 *
	 *  @param dest wierzcho�ek docelowy
	 *  @param cost koszt
	 *  @param direction k�t pomi�dzy wektorem po��czenia, a kierunkiem bazowym (0-360st)
	 */
	gConn(Node* dest, float cost, int direction) : dest(dest), cost(cost), direction(direction) {}

    Node* dest;		/**< w�ze�� docelowy */
    float cost;		/**< koszt (droga w metrach) */
	int direction;	/**< kierunek połączenia - kat 0-360st wzgl kierunku N */
};

class Node
{
public:
	/** @brief Konstruktor
	 *
	 *  @param id numer id
	 *  @param placeName nazwa wierzcholka
	 */
	Node(int id, string placeName) : id(id), placeName(placeName) {}

	/** @brief Destruktor */
	~Node() {};

	/** @brief dodaje marker do wierzcho�ka
	 *
	 *  @param filename �cie�ka do pliku z markerem
	 *  @param distance odleg�o��
	 *  @param directionAngle k�t pomi�dzy wektorem Wierzcho�ek->Marker, a kierunkiem bazowym (0-360st)
	 *  @param faceAngle orientacja markera
	 */
	Pattern* addPattern(string filename, double distance, double directionAngle, double faceAngle = -1.0);

	int id;							/**< nr wierzcholka */
	string placeName;				/**< nazwa miejsca */
	std::list<gConn> connections;	/**< polaczenia z danego wierzcholka */
	std::list<Pattern*> patterns;	/**< markery przyporzadkowane do danego wierzcholka */

	/** @brief operator<<
	 *
	 *  @param out strumien wyjsciowy
	 *  @param n wierzcholek
	 */
	friend ostream& operator<<(ostream &out, const Node &n);

	/** @brief operator<
	 *
	 *  @param rhs wierzcholek
	 */
	//bool operator<(Node & rhs);

	gConn * prevConn;				/**< polaczenie do poprzedniego wierzcholka na sciezce (do Dijkstry) */
	Node * prevNode;				/**< poprzedni wierzcholek na sciezce (do Dijkstry) */
	double pathCost;				/**< koszt scie�ki (do Dijkstry) */
	bool explored;					/**< do Dijkstry */
};

bool relacja(Node * lhs, Node * rhs);

#endif // NODE_H
