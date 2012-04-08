#pragma once

/**
* @file pattern.h
* @brief plik nag��wkowy klasy Pattern
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
* @brief Klasa zawieraj�ca informacje o markerze
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
	 *  @param node wska�nik na wierzcho�ek
	 *  @param distanceFromNode odleg�o�� od wierzcho�ka
	 *  @param directionAngle k�t pomi�dzy wektorem Node->Marker, a wektorem N
	 *  @param orientacja markera - k�t pomi�dzy wektorem wychodz�cym prostopadle z jego powierzchni w stron� obserwatora, a wektorem N
	 */
	Pattern(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );

	/* @brief  destruktor */
	~Pattern(void) {};

	/** @brief funkcja inicjalizuj�ca
	 *
	 *  @param filename nazwa pliku z markerem
	 *  @param node wska�nik na wierzcho�ek
	 *  @param distanceFromNode odleg�o�� od wierzcho�ka
	 *  @param directionAngle k�t pomi�dzy wektorem Node->Marker, a wektorem N
	 *  @param orientacja markera - k�t pomi�dzy wektorem wychodz�cym prostopadle z jego powierzchni w stron� obserwatora, a wektorem N
	 */
	bool init(std::string filename, Node* node, float distanceFromNode, float directionAngle, float faceAngle = -1.0 );

	int			id; /*< numer id*/
	double		width; /*< rozmiar*/
	double		center[2]; /*< punkt centralny 0,0 dla �rodka markera*/
	TransMatrix transformation; /*< macierz transformacji kamera->marker*/
	bool		initialized; /*< czy zainicjalizowany*/
	
	ARMarkerInfo* markerInfo; /*< wska�nik na informacje o znalezionym markerze wygenerowane prze ARToolkit*/
	double matchProbability; /*< prawdopodobienstwo dopasowania */

	float directionAngle;	/*< k�t [st] pomi�dzy wektorem Node->Marker, a wektorem N*///
	float distanceFromNode; /*< odleg�o�� od wierzcho�ka [m]*///
	Node* node;

	float faceAngle;	/*< orientacja markera [st] - k�t pomi�dzy wektorem wychodz�cym prostopadle z jego powierzchni w stron� obserwatora, a wektorem N */

private:
	/*< plik z markerem */
	std::string file;
	/* @brief automatyczny k�t, b�dzie ok, gdy marker jest powieszony na �cianie w pomieszczeniu prostokatnym ze �cianami r�wnolegle i prostopadle do kierunku N */
	void autoFaceAngle();
};

#endif //PATTERN_H