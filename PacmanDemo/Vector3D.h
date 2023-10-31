#ifndef _VECTOR_3D
#define _VECTOR_3D

class Vector3D {

public:

	float x, y, z;

	Vector3D();
	Vector3D(float p_x, float p_y, float p_z);

	const float* toArray();
};
#endif