#include "../includes/Vector3.h"

#include <sstream>

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

std::string Vector3::ToString() const
{
	std::stringstream res;

	res << "X: " << this->x << " | Y: " << this->y << " | Z: " << this->z;
	return res.str();
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
	os << "X: " << vec.x << " | Y: " << vec.y << " | Z: " << vec.z;
	return os;
}