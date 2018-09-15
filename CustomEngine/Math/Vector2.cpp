/*****************************************************************************/
/*!
\file			Vector2.cpp
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
2D vector class for math library.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#include "MathInc.h"

void Vector2::Set(float X, float Y)
{
	x = X; y = Y;
}

float Vector2::SqLength() const
{
	return x*x + y*y;
}

float Vector2::Length() const
{
	return std::sqrt(SqLength());
}

Vector2 & Vector2::Normalize()
{

	*this /= Length();
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x; 
	y += v.y;
	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & v)
{

	x -= v.x; 
	y -= v.y;
	return *this;
}

Vector2 & Vector2::operator*=(float f)
{
	
	x *= f; 
	y *= f;
	return *this;
}

Vector2 & Vector2::operator*=(const Vector2 & v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

Vector2 & Vector2::operator/=(float f)
{
	x /= f;
	y /= f;
	return *this;
}

Vector2 & Vector2::operator/=(const Vector2 & v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}


Vector2 Vector2::operator*(const Vector2 & v) const
{
	return Vector2{ x*v.x, y*v.y };
}

float Vector2::operator[](unsigned int i) const
{
	return *(&x + i);
}

float & Vector2::operator[](unsigned int i)
{
	return *(&x + i);
}

bool Vector2::operator==(const Vector2 & other) const
{
	return ( (fabs(x - other.x) <= MATH_EPSILON) && (fabs(y - other.y) <= MATH_EPSILON) );
}

bool Vector2::operator!=(const Vector2 & other) const
{
	return !(*this == other);
}

bool Vector2::Equal(const Vector2 & other, float epsilon) const
{
	return ((fabs(x - other.x) <= epsilon) && (fabs(y - other.y) <= epsilon));
}

Vector2 & Vector2::operator=(float f)
{
	x = y = f;
	return *this;
}

float Vector2::Dot(const Vector2 & other) const
{
	return x*other.x + y*other.y;
}

Vector2 operator-(const Vector2 & v)
{
	return Vector2(-v.x,-v.y);
}

Vector2 operator-(const Vector2 & a, const Vector2 & b)
{
	return Vector2(a.x-b.x,a.y-b.y);
}

Vector2 operator+(const Vector2 & a, const Vector2 & b)
{
	return Vector2(a.x+b.x,a.y+b.y);
}

Vector2 operator*(const Vector2 & a, float b)
{
	return Vector2(a.x*b,a.y*b);
}

Vector2 operator/(const Vector2 & a, float b)
{
	return Vector2(a.x/b,a.y/b);
}

Vector2 operator*(float b, const Vector2 & a)
{
	return a * b;
}

Vector2 operator/(float b, const Vector2 & a)
{
	return Vector2();
}

Vector2 operator*(const Vector2 & a, const Vector2 & b)
{
	return Vector2(a.x*b.x,a.y*b.y);
}

Vector2 operator/(const Vector2 & a, const Vector2 & b)
{
	return Vector2(a.x/b.x,a.y/b.y);
}
