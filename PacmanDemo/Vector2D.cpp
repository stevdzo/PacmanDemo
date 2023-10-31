#include "Vector2D.h"

Vector2D::Vector2D() {
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float p_x, float p_y) {
	x = p_x;
	y = p_y;
}

const Vector2D Vector2D::operator+(const Vector2D& p_v) {
	return Vector2D(x + p_v.x, y + p_v.y);
}

const Vector2D Vector2D::operator-(const Vector2D& p_v) {
	return Vector2D(x - p_v.x, y - p_v.y);
}

const Vector2D Vector2D::operator+(float p_n) {
	return Vector2D(x + p_n, y + p_n);
}

const Vector2D Vector2D::operator-(float p_n) {
	return Vector2D(x - p_n, y - p_n);
}

const Vector2D Vector2D::operator*(float p_n) {
	return Vector2D(x * p_n, y * p_n);
}

const Vector2D Vector2D::operator/(float p_n) {
	return Vector2D(x / p_n, y / p_n);
}

const Vector2D Vector2D::operator+=(const Vector2D&) {
	return Vector2D();
}

const Vector2D Vector2D::operator-=(const Vector2D&) {
	return Vector2D();
}

const bool Vector2D::operator==(const Vector2D&) const {
	return false;
}

const bool Vector2D::operator!=(const Vector2D&) const {
	return false;
}

const float* Vector2D::toArray() {
	float arr[]{ x, y };
	return arr;
}