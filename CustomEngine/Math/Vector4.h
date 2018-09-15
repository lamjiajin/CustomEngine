/*****************************************************************************/
/*!
\file			Vector4.h
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
4D homogenous vector class for math library.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#pragma once

typedef
class Vector4
{
public:

	Vector4() : x(), y(), z(),w() {}
	Vector4(float a, float b, float c,float d) : x(a), y(b), z(c),w(d) {}
	explicit Vector4(float f) : x(f), y(f), z(f),w(f) {}
	Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z),w(v.w) {}
	Vector4(const Vector3& v,float W = 0.f) : x(v.x), y(v.y), z(v.z), w(W) {}
	void Set(float X, float Y, float Z,float W);
	float SqLength() const;
	float Length() const;
	Vector4& Normalize();
	Vector4& NormalizeSafe();
	// combined operators
	Vector4& operator += (const Vector4& v);
	Vector4& operator -= (const Vector4& v);
	Vector4& operator *= (float f);
	Vector4& operator /= (float f);
	//component wise multiply
	Vector4& operator *=(const Vector4& v);
	//component wise divide
	Vector4& operator /=(const Vector4& v);
	

	const Vector3& xyz() const;//swizzle
	Vector4 operator*(const Vector4& v) const;

	// access a single element
	float operator[](unsigned int i) const;
	float& operator[](unsigned int i);

	// comparison
	bool operator== (const Vector4& v) const;
	bool operator!= (const Vector4& v) const;

	bool Equal(const Vector4& v, float epsilon = MATH_EPSILON) const;
	float Dot(const Vector4& v) const;
	float Distance(const Vector4& v);

	float x, y, z, w;
}vec4, Vec4, point4, Point4;

Vector4 operator-(const Vector4& v);

Vector4 operator-(const Vector4& a, const Vector4& b);
Vector4 operator+(const Vector4& a, const Vector4& b);
Vector4 operator*(const Vector4& a, float b);
Vector4 operator/(const Vector4& a, float b);
Vector4 operator*(float b, const Vector4& a);

//component wise multiply
Vector4 operator*(const Vector4& a, const Vector4& b);
//component wise divide
Vector4 operator/(const Vector4& a, const Vector4& b);