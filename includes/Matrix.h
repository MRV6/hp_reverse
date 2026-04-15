#pragma once

struct Matrix4x4
{
	float m[4][4];
};

class Matrix
{
public:
	static void Multiply(struct Matrix4x4& mat, float multiplier);
};