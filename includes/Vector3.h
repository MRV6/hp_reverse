#pragma once

#include <iostream>

class Vector3
{
public:
	Vector3(float x, float y, float z);

	float x;
	float y;
	float z;

	std::string ToString() const;

	Vector3 Normalize() const;

	Vector3 operator+(Vector3 vec);
	Vector3 operator-(Vector3 vec);
	Vector3 operator*(Vector3 vec);
};