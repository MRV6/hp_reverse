#include "../includes/Vector3.h"

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
	os << "x: " << vec.x << " | y: " << vec.y << " | z: " << vec.z;
	return os;
}