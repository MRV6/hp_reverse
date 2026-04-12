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
};

std::ostream& operator<<(std::ostream& os, const Vector3& vec);