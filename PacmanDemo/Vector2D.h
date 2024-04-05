#ifndef _VECTOR_2D_H
#define _VECTOR_2D_H

#include <math.h>

class Vector2D {

public:

	float x, y; // x i y parametri vektora

	Vector2D(); // prazan konstruktor
	Vector2D(float p_x, float p_y); // konstruktor sa parametrima
	const Vector2D operator+(const Vector2D&); // preklopljen operator za sabiranje dva vektora
	const Vector2D operator-(const Vector2D&); // preklopljen operator za oduzimanje dva vektora
	const Vector2D operator+(float); // preklopljen operator za sabiranje vektora sa skalarom
	const Vector2D operator-(float); // preklopljen operator za oduzimanje skalara od vektora 
	const Vector2D operator*(float); // preklopljen operator za množenje vektora sa skalarom 
	const Vector2D operator/(float); // preklopljen operator za deljenje vektora sa skalarom
	const Vector2D operator+=(const Vector2D&); // preklopljen operator sabiranja i dodele
	const Vector2D operator-=(const Vector2D&); // preklopljen operator oduzimanja i dodele
	const bool operator==(const Vector2D&) const; // preklopljen operator jednakosti
	const bool operator!=(const Vector2D&) const; // preklopljen operator nejednakosti
	const float magnitude(); // funkcija za intenzitet vektora
	const float distanceTo(const Vector2D& p_v1); // funkcija za distancu između dva vektora
	const float distanceToSq(const Vector2D& p_v1); // fnkcija za distancu između dva vektora bez korena
};
#endif