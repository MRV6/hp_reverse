#include "../includes/Matrix.h"

void Matrix::Multiply(struct Matrix4x4& mat, float multiplier)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat.m[i][j] *= multiplier;
		}
	}
}