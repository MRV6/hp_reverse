#include "../includes/Vector3.h"

#include <sstream>

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator+(Vector3 vec)
{
	return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

Vector3 Vector3::operator-(Vector3 vec)
{
	return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

Vector3 Vector3::operator*(Vector3 vec)
{
	return Vector3(this->x * vec.x, this->y * vec.y, this->z * vec.z);
}

Vector3 Vector3::Normalize() const
{
	float length = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	return Vector3(this->x / length, this->y / length, this->z / length);
}

std::string Vector3::ToString() const
{
	std::stringstream res;

	res << "X: " << this->x << " | Y: " << this->y << " | Z: " << this->z;
	return res.str();
}