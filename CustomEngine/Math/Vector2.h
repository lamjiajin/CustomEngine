/*****************************************************************************/
/*!
\file			Vector2.h
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
#pragma once

#include <cmath>

typedef
class Vector2
{
public:

	Vector2() : x(), y() {}
	Vector2(float a, float b) : x(a), y(b) {}
	explicit Vector2(float f) : x(f), y(f) {}
	Vector2(const Vector2& o) : x(o.x), y(o.y) {}

	void Set(float pX, float pY);
	float SqLength() const;
	float Length() const;
	Vector2& Normalize();

	Vector2& operator += (const Vector2& v);
	Vector2& operator -= (const Vector2& v);
	Vector2& operator *= (float f);
	Vector2& operator *=(const Vector2& v);
	Vector2& operator /= (float f);
	Vector2& operator /=(const Vector2& v);
	
	//component wise multiplication
	Vector2 operator*(const Vector2& v) const; 

	float operator[](unsigned int i) const;
	float& operator[](unsigned int i);

	bool operator== (const Vector2& other) const;
	bool operator!= (const Vector2& other) const;

	bool Equal(const Vector2& other, float epsilon = MATH_EPSILON) const;

	Vector2& operator= (float f);
	float Dot(const Vector2& other) const;
	

	float x, y;
} vec2, Vec2, Point2, point2;

Vector2 operator-(const Vector2& v);

Vector2 operator-(const Vector2& a, const Vector2& b);
Vector2 operator+(const Vector2& a, const Vector2& b);
Vector2 operator*(const Vector2& a, float b);
Vector2 operator/(const Vector2& a, float b);
Vector2 operator*(float b, const Vector2& a);

//component wise multiply
Vector2 operator*(const Vector2& a, const Vector2& b);
//component wise divide
Vector2 operator/(const Vector2& a, const Vector2& b);

