#ifndef _VECTOR_2D_H
#define _VECTOR_2D_H

#include <math.h>

class Vector2D {

public:

	float x, y;

	Vector2D();
	Vector2D(float p_x, float p_y);

	const Vector2D add(const Vector2D& p_v);
	const Vector2D subtract(const Vector2D& p_v);
	const Vector2D multiply(const Vector2D& p_v);
	const Vector2D divide(const Vector2D& p_v);
	
	const Vector2D operator+(const Vector2D&);
	const Vector2D operator-(const Vector2D&);
	const Vector2D operator+(float);
	const Vector2D operator-(float);
	const Vector2D operator*(float);
	const Vector2D operator/(float);
	const Vector2D operator+=(const Vector2D&);
	const Vector2D operator-=(const Vector2D&);

	const bool operator==(const Vector2D&) const;
	const bool operator!=(const Vector2D&) const;

	const float magnitude();
	const float distanceTo(const Vector2D& p_v1);

	const float* toArray();
};
#endif