#include "matrix4.h"

void SetZero(Matrix4& mat)
{
	mat.m[0][0] = 0.0f;
	mat.m[0][1] = 0.0f;
	mat.m[0][2] = 0.0f;
	mat.m[0][3] = 0.0f;

	mat.m[1][0] = 0.0f;
	mat.m[1][1] = 0.0f;
	mat.m[1][2] = 0.0f;
	mat.m[1][3] = 0.0f;

	mat.m[2][0] = 0.0f;
	mat.m[2][1] = 0.0f;
	mat.m[2][2] = 0.0f;
	mat.m[2][3] = 0.0f;

	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 0.0f;
}

void SetIdentity(Matrix4& mat)
{
	SetZero(mat);

	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
}

void CreateOrthoProjection(Matrix4& mat, float width, float height)
{
	SetZero(mat);

	mat.m[0][0] = 2.0f / width;
	mat.m[1][1] = 2.0f / -height; mat.m[2][2] = -2.0f / (1.0f - -1.0f);

	mat.m[3][0] = -1.0f;
	mat.m[3][1] = 1.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;
}