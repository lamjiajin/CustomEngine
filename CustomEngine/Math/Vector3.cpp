/*****************************************************************************/
/*!
\file			Vector3.cpp
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
#include "MathInc.h"
#define clamp(v, _min, _max) (std::max(std::min(v, _max), _min))
Vector3::Vector3(const Vector4 & v) : x(v.x), y(v.y), z(v.z)
{
}

//Vector3::Vector3(const DirectX::XMFLOAT3 &f) : x(f.x),y(f.y),z(f.z)
//{
//}

void Vector3::Set(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}

float Vector3::SqLength() const
{
	return x*x + y*y + z*z;
}

float Vector3::Length() const
{
	return std::sqrt(SqLength());
}

Vector3 & Vector3::Normalize()
{
	*this /= Length();
	return *this;
}

Vector3 & Vector3::NormalizeSafe()
{
	float len = Length();
	if (len > 0.f)
		*this /= len;

	return *this;
}

Vector3& Vector3::operator+=(const Vector3 & v)
{
	x += v.x; 
	y += v.y; 
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 & v)
{
	x -= v.x; 
	y -= v.y; 
	z -= v.z; 
	return *this;
}

Vector3& Vector3::operator*=(float f)
{
	x *= f; 
	y *= f; 
	z *= f; 
	return *this;
}

Vector3& Vector3::operator/=(float f)
{
	x /= f; 
	y /= f; 
	z /= f; 
	return *this;
}

Vector3 & Vector3::operator*=(const Vector3 & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3 & Vector3::operator/=(const Vector3 & v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}


float Vector3::operator[](unsigned int i) const
{
	return *(&x + i);
}

float & Vector3::operator[](unsigned int i)
{
	return *(&x + i);
}

bool Vector3::operator==(const Vector3 & v) const
{
	return ( (fabs(x - v.x) <= MATH_EPSILON) &&
			(fabs(y - v.y) <= MATH_EPSILON) &&
		(fabs(z - v.z) <= MATH_EPSILON) );
}

bool Vector3::operator!=(const Vector3 & v) const
{
	return !(*this == v);
}

bool Vector3::Equal(const Vector3 & v, float epsilon) const
{
	return ((fabs(x - v.x) <= epsilon) &&
		(fabs(y - v.y) <= epsilon) &&
		(fabs(z - v.z) <= epsilon));
}

float Vector3::Dot(const Vector3 & v) const
{
	return x*v.x + y*v.y + z*v.z;
}

Vector3 Vector3::Cross(const Vector3 & v) const
{
	return Vector3(y*v.z - z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
}

float Vector3::Distance(const Vector3 & v) const
{
	Vector3 temp{ *this - v };
	return temp.Length();
}

Vector3 operator-(const Vector3 & v)
{
	return Vector3(-v.x,-v.y,-v.z);
}

Vector3 operator-(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.x-b.x,a.y-b.y,a.z-b.z);
}

Vector3 operator+(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.x+b.x,a.y+b.y,a.z+b.z);
}

Vector3 operator*(const Vector3 & a, float b)
{
	return Vector3(a.x*b,a.y*b,a.z*b);
}

Vector3 operator/(const Vector3 & a, float b)
{
	return Vector3(a.x/b,a.y/b,a.z/b);
}

Vector3 operator*(float b, const Vector3 & a)
{
	return a*b;
}

Vector3 operator*(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.x*b.x,a.y*b.y,a.z*b.z);
}

Vector3 operator/(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.x/b.x,a.y/b.y,a.z/b.z);
}


Vector3 lerp(const Vector3 & from, const Vector3 & to, const float step)
{
	return from + step*(to - from);
}

std::ostream & operator<<(std::ostream &os, const Vector3 &v)
{
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}



Vector3 Slerp(const Vector3 & from, const Vector3 & to, const float step)
{
	//if (step == 0) return from;
	//if (from == to || step == 1.0f) return to;

	//float theta = acos(from.Dot(to) / (from.Length() * to.Length()));
	////float thetaDegree = theta * RadianToDegree;

	//if (theta == 0) return to;

	//float sinTheta = sin(theta);
	//auto res = (from * (sin((1 - step) * theta) / sinTheta)) + (to * (sin(step * theta) / sinTheta));
	//if (std::isnan(res.x))
	//{
	//	int x = 1;
	//	++x;
	//}
	//return res;


	// Dot product - the cosine of the angle between 2 vectors.
	float dot = from.Dot(to);
	// Clamp it to be in the range of Acos()
	// This may be unnecessary, but floating point
	// precision can be a fickle mistress.
	dot = clamp(dot, -1.0f, 1.0f);
	// Acos(dot) returns the angle between start and end,
	// And multiplying that by percent returns the angle between
	// start and the final result.
	float theta = acosf(dot)*step;
	Vector3 RelativeVec = to - from*dot;
	RelativeVec.Normalize();     // Orthonormal basis
								 // The final result.
	return ((from*cosf(theta)) + (RelativeVec*sinf(theta)));
}
