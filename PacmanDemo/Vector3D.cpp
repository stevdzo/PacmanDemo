#include "Vector3D.h"

Vector3D::Vector3D() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3D::Vector3D(float p_x, float p_y, float p_z) {
	x = p_x;
	y = p_y;
	z = p_z;
}

const float* Vector3D::toArray() {
	float arr[]{ x, y, z };
	return arr;
}