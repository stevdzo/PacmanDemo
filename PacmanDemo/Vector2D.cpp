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

const Vector2D Vector2D::operator+=(const Vector2D& p_v) {
	return Vector2D(x += p_v.x, y += p_v.y);
}

const Vector2D Vector2D::operator-=(const Vector2D& p_v) {
	return Vector2D(x -= p_v.x, y -= p_v.y);
}

const bool Vector2D::operator==(const Vector2D& p_v) const {
	return (x == p_v.x && y == p_v.y);
}

const bool Vector2D::operator!=(const Vector2D& p_v) const {
	return (x != p_v.x || y != p_v.y);;
}

const float Vector2D::magnitude() {
	return sqrt(x * x + y * y);
}

const float Vector2D::distanceTo(const Vector2D& p_v1) {

	float x = this->x - p_v1.x;
	float y = this->y - p_v1.y;

	return sqrt(x * x + y * y);
}

const float* Vector2D::toArray() {
	float arr[]{ x, y };
	return arr;
}