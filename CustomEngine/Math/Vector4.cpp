/*****************************************************************************/
/*!
\file			Vector4.cpp
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
#include "MathInc.h"

void Vector4::Set(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

float Vector4::SqLength() const
{
	return x*x + y*y + z*z + w*w;
}

float Vector4::Length() const
{
	return std::sqrt(SqLength());
}

Vector4& Vector4::Normalize()
{
	*this /= Length();
	return *this;
}

Vector4 & Vector4::NormalizeSafe()
{
	float len = Length();
	if (len > 0.f)
		*this /= len;

	return *this;
}

Vector4 & Vector4::operator+=(const Vector4 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4 & Vector4::operator-=(const Vector4 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;

}

Vector4 & Vector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

 Vector4 & Vector4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

 Vector4 & Vector4::operator*=(const Vector4 & v)
 {
	 x *= v.x;
	 y *= v.y;
	 z *= v.z;
	 w *= v.w;
	 return *this;
 }

 Vector4 & Vector4::operator/=(const Vector4 & v)
 {
	 x /= v.x;
	 y /= v.y;
	 z /= v.z;
	 w /= v.w;
	 return *this;
 }


 const Vector3 & Vector4::xyz() const
 {
	 return *reinterpret_cast<const Vector3*>(&x);
 }

 Vector4 Vector4::operator*(const Vector4 & v) const
{
	return Vector4(x*v.x,y*v.y,z*v.z,w*v.w);
}

float Vector4::operator[](unsigned int i) const
{
	return *(&x + i);
}

float & Vector4::operator[](unsigned int i)
{
	return *(&x + i);
}

bool Vector4::operator==(const Vector4 & v) const
{
	return ((fabs(x - v.x) <= MATH_EPSILON) &&
		(fabs(y - v.y) <= MATH_EPSILON) &&
		(fabs(z - v.z) <= MATH_EPSILON) &&
		(fabs(w - v.w) <= MATH_EPSILON));
}

bool Vector4::operator!=(const Vector4 & v) const
{
	return !(*this == v);
}

bool Vector4::Equal(const Vector4 & v, float epsilon) const
{
	return ((fabs(x - v.x) <= epsilon) &&
		(fabs(y - v.y) <= epsilon) &&
		(fabs(z - v.z) <= epsilon) &&
		(fabs(w - v.w) <= epsilon));
}

float Vector4::Dot(const Vector4 & v) const
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

float Vector4::Distance(const Vector4 & v)
{
	Vector4 temp{ *this - v };
	return temp.Length();
}

Vector4 operator-(const Vector4 & v)
{
	return Vector4(-v.x,-v.y,-v.z,-v.w);
}

Vector4 operator-(const Vector4 & a, const Vector4 & b)
{
	return Vector4(a.x-b.x,a.y-b.y,a.z-b.z,a.w-b.w);
}

Vector4 operator+(const Vector4 & a, const Vector4 & b)
{
	return Vector4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
}

Vector4 operator*(const Vector4 & a, float b)
{
	return Vector4(a.x*b,a.y*b,a.z*b,a.w*b);
}

Vector4 operator/(const Vector4 & a, float b)
{
	return Vector4(a.x/b,a.y/b,a.z/b,a.w/b);
}

Vector4 operator*(float b, const Vector4 & a)
{
	return a*b;
}

Vector4 operator*(const Vector4 & a, const Vector4 & b)
{
	return Vector4(a.x*b.x,a.y*b.y,a.z*b.z,a.w*b.w);
}

Vector4 operator/(const Vector4 & a, const Vector4 & b)
{
	return Vector4(a.x/b.x,a.y/b.y,a.z/b.z,a.w/b.w);
}
