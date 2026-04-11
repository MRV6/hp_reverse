#include "../includes/Vector3.h"

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
	os << "X: " << vec.x << " | Y: " << vec.y << " | Z: " << vec.z;
	return os;
}