#include "Vector.h"


Vector::Vector() : x(1.0), y(1.0), z(1.0) { normalize(); }
Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}
float Vector::length() { return sqrt(x * x + y * y + z * z); }
void Vector::normalize() {
	float lenght = length();
	x = x / lenght;
	y = y / lenght;
	z = z / lenght;
}