#pragma once

/**
* @file pattern.h
* @brief plik nag³ówkowy klasy Pattern
*
* @author Kamil Neczaj, Ernest Staszuk
*
* @date 9.12.2011
*/

#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <AR/ar.h>

#include "transmatrix.h"
#include "node.h"

class TransMatrix;
class Node;

/**
* @class GLWindow
*
* @brief Klasa zawieraj¹ca informacje o markerze
*
*/ 
class Pattern
{
public:
	/* @brief  konstruktor*/
	Pattern(void);

	/** @brief kostruktor
	 *
	 *  @param filename nazwa pliku z markerem
	 *  @param node wskaŸnik na wierzcho³ek
	 *  @param distanceFromNode odleg³oœæ od wierzcho³ka
	 *  @param directionAngle k¹t pomiêdzy wektorem Node->Marker, a wektorem N
	 *  @param orientacja markera - k¹t pomiêdzy wektorem wychodz¹cym prostopadle z jego powierzchni w stronê obserwatora, a wektorem N
	 */
	Pattern(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );

	/* @brief  destruktor */
	~Pattern(void) {};

	/** @brief funkcja inicjalizuj¹ca
	 *
	 *  @param filename nazwa pliku z markerem
	 *  @param node wskaŸnik na wierzcho³ek
	 *  @param distanceFromNode odleg³oœæ od wierzcho³ka
	 *  @param directionAngle k¹t pomiêdzy wektorem Node->Marker, a wektorem N
	 *  @param orientacja markera - k¹t pomiêdzy wektorem wychodz¹cym prostopadle z jego powierzchni w stronê obserwatora, a wektorem N
	 */
	bool init(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );

	int			id; /*< numer id*/
	double		width; /*< rozmiar*/
	double		center[2]; /*< punkt centralny 0,0 dla œrodka markera*/
	TransMatrix transformation; /*< macierz transformacji kamera->marker*/
	bool		initialized; /*< czy zainicjalizowany*/
	
	ARMarkerInfo* markerInfo; /*< wskaŸnik na informacje o znalezionym markerze wygenerowane prze ARToolkit*/
	double matchProbability; /*< prawdopodobienstwo dopasowania */

	float directionAngle;	/*< k¹t [st] pomiêdzy wektorem Node->Marker, a wektorem N*///
	float distanceFromNode; /*< odleg³oœæ od wierzcho³ka [m]*///
	Node* node;

	float faceAngle;	/*< orientacja markera [st] - k¹t pomiêdzy wektorem wychodz¹cym prostopadle z jego powierzchni w stronê obserwatora, a wektorem N */

private:
	/*< plik z markerem */
	std::string file;
	/* @brief automatyczny k¹t, bêdzie ok, gdy marker jest powieszony na œcianie w pomieszczeniu prostokatnym ze œcianami równolegle i prostopadle do kierunku N */
	void autoFaceAngle();
};

#endif //PATTERN_H