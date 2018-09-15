/*****************************************************************************/
/*!
\file			Vector3.h
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
3D vector class for math library.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#pragma once
#include <iostream>
class Vector4;

typedef
class Vector3
{
public:

	Vector3() : x(), y(), z() {}
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
	explicit Vector3(float f) : x(f), y(f), z(f) {}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(const Vector4& v);
	//explicit Vector3(const DirectX::XMFLOAT3&);
	void Set(float X, float Y, float Z);
	float SqLength() const;
	float Length() const;
	Vector3& Normalize();
	Vector3& NormalizeSafe();
	// combined operators
	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *= (float f);
	Vector3& operator /= (float f);
	Vector3& operator *=(const Vector3& v);
	Vector3& operator /=(const Vector3& v);

	// access a single element
	float operator[](unsigned int i) const;
	float& operator[](unsigned int i);

	// comparison
	bool operator== (const Vector3& v) const;
	bool operator!= (const Vector3& v) const;
	
	bool Equal(const Vector3& v, float epsilon = MATH_EPSILON) const;
	float Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v)  const;
	float Distance(const Vector3& v) const;

	float x, y, z;


  
} vec3, Vec3, Point3, point3;

Vector3 operator-(const Vector3& v);

Vector3 operator-(const Vector3& a, const Vector3& b);
Vector3 operator+(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& a, float b);
Vector3 operator/(const Vector3& a, float b);
Vector3 operator*(float b, const Vector3& a);

//component wise multiply
Vector3 operator*(const Vector3& a, const Vector3& b);
//component wise divide
Vector3 operator/(const Vector3& a, const Vector3& b);

Vector3 Slerp(const Vector3& from, const Vector3& to, const float step); // bad stuff

Vector3 lerp(const Vector3& from, const Vector3& to, const float step);


std::ostream& operator<<(std::ostream&, const Vector3&);
