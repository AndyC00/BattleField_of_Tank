// This include:
#include "vector2.h"

// Local includes:
#include "renderer.h"

// Library includes:
#include <cassert> 
#include <cmath>

Vector2::Vector2()
	: x(0.0f)
	, y(0.0f)
{

}

Vector2::~Vector2()
{

}

Vector2::Vector2(float inx, float iny): 
	x(inx),
	y(iny)
{

}
void Vector2::Set(float inx, float iny)
{
	x = inx; 
	y = iny;
}

float Vector2::LengthSquared() const
{
	return ((x * x) + (y * y));
}

float Vector2::Length() const
{
	return (sqrtf(LengthSquared()));
}

void Vector2::Normalise()
{
	float length = Length();

	x = x / length; 
	y = y / length;
}

float
Vector2::DotProduct(const Vector2& veca, const Vector2& vecb)
{
	return (veca.x * vecb.x) + (veca.y * vecb.y);
}

Vector2
Vector2::Lerp(const Vector2& veca, const Vector2& vecb, float time)
{
	return (Vector2(veca + time * (vecb - veca)));
}

Vector2
Vector2::Reflect(const Vector2& vec, const Vector2& normal)
{
	return (vec - 2.0f * Vector2::DotProduct(vec, normal) * normal);
}
