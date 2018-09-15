/*****************************************************************************/
/*!
\file			Matrix3x3.cpp
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
Matrix 3x3 class for math library.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
//#include "SoulEngine.h"
#include "MathInc.h"
#include <utility>

Matrix3x3::Matrix3x3(const Vector3& r0, const Vector3& r1, const Vector3& r2)
{
	memcpy_s(&r00, sizeof(Vector3), &r0, sizeof(Vector3));
	memcpy_s(&r10, sizeof(Vector3), &r1, sizeof(Vector3));
	memcpy_s(&r20, sizeof(Vector3), &r2, sizeof(Vector3));
}

Matrix3x3 & Matrix3x3::operator*=(const Matrix3x3 & m)
{
	*this = Matrix3x3(m.r00 * r00 + m.r10 * r01 + m.r20 * r02,
		m.r01 * r00 + m.r11 * r01 + m.r21 * r02,
		m.r02 * r00 + m.r12 * r01 + m.r22 * r02,
		m.r00 * r10 + m.r10 * r11 + m.r20 * r12,
		m.r01 * r10 + m.r11 * r11 + m.r21 * r12,
		m.r02 * r10 + m.r12 * r11 + m.r22 * r12,
		m.r00 * r20 + m.r10 * r21 + m.r20 * r22,
		m.r01 * r20 + m.r11 * r21 + m.r21 * r22,
		m.r02 * r20 + m.r12 * r21 + m.r22 * r22);

	return *this;
}

inline Matrix3x3 Matrix3x3::operator*(const Matrix3x3 & m) const
{
	Matrix3x3 temp(*this);
	temp *= m;
	return temp;
}

float * Matrix3x3::operator[](unsigned int i)
{
	return &r00 + i * 3;
}

const float * Matrix3x3::operator[](unsigned int i) const
{
	return &r00 + i * 3;
}

const Vector3 & Matrix3x3::GetCol(unsigned int i) const
{
	return *reinterpret_cast<const Vector3*>(&r00 + i*3);
}

Vector3 Matrix3x3::GetRow(unsigned int i) const
{
	return Vector3((*this)[0][i], (*this)[1][i], (*this)[2][i]);
}

Vector3 Matrix3x3::operator*(const Vector3 & v) const
{
	return Vector3
	(
		r00*v.x+r01*v.y+r02*v.z,
		r10*v.x + r11*v.y + r12*v.z,
		r20*v.x + r21*v.y + r22*v.z
	);
}

Vector4 Matrix3x3::operator*(const Vector4 & v) const
{
	Vector3 temp = *this*Vector3(v);
	return Vector4(temp,v.w);
}

Vector2 Matrix3x3::operator*(const Vector2 & v) const
{
	Vector3 temp(v.x,v.y,0.f);
	Vector3 res{ *this * temp };
	return Vector2(res.x,res.y);
}

Matrix3x3::Matrix3x3(const Matrix4x4 & m)
{
	r00 = m.r00; r01 = m.r01; r02 = m.r02;
	r10 = m.r10; r11 = m.r11; r12 = m.r12;
	r20 = m.r20; r21 = m.r21; r22 = m.r22;
}

inline Matrix3x3 & Matrix3x3::Transpose()
{
	std::swap(r01, r10);
	std::swap(r02, r20);
	std::swap(r12, r21);
	return *this;
}

Matrix3x3 & Matrix3x3::Inverse()
{
	// Compute the reciprocal determinant
	float det = Determinant();
	if (det == 0.0)
	{
		// Matrix not invertible. 
		return *this;
	}

	float invdet = 1.0f / det;

	Matrix3x3 res;
	res.r00 = invdet  * (r11 * r22 - r12 * r21);
	res.r01 = -invdet * (r01 * r22 - r02 * r21);
	res.r02 = invdet  * (r01 * r12 - r02 * r11);
	res.r10 = -invdet * (r10 * r22 - r12 * r20);
	res.r11 = invdet  * (r00 * r22 - r02 * r20);
	res.r12 = -invdet * (r00 * r12 - r02 * r10);
	res.r20 = invdet  * (r10 * r21 - r11 * r20);
	res.r21 = -invdet * (r00 * r21 - r01 * r20);
	res.r22 = invdet  * (r00 * r11 - r01 * r10);
	*this = res;

	return *this;
}

inline float Matrix3x3::Determinant() const
{
	return r00*r11*r22 - r00*r12*r21 + r02*r12*r20 - r02*r10*r22 + r02*r10*r21 - r02*r11*r20;
}

Matrix3x3& Matrix3x3::RotateZ(float a, Matrix3x3& outmat)
{
	outmat.r00 = outmat.r11 = std::cos(a);
	outmat.r10 = std::sin(a);
	outmat.r01 = -outmat.r10;

	outmat.r02 = outmat.r12 = outmat.r20 = outmat.r21 = 0.f;
	outmat.r22 = 1.f;

	return outmat;
	
}

 Matrix3x3 & Matrix3x3::RotateX(float a, Matrix3x3 & outmat)
{
	outmat.r11 = outmat.r22 = std::cos(a);
	outmat.r21 = std::sin(a);
	outmat.r12 = -outmat.r21;

	outmat.r00 = 1.f;
	outmat.r01 = outmat.r02 = outmat.r10 = outmat.r20 = 0.f;

	return outmat;
}

Matrix3x3 & Matrix3x3::RotateY(float a, Matrix3x3 & outmat)
{
	outmat.r00 = outmat.r22 = std::cos(a);
	outmat.r02 = std::sin(a);
	outmat.r20 = -outmat.r02;

	outmat.r11 = 1.f;
	outmat.r01 = outmat.r10 = outmat.r12 = outmat.r21 = 0.f;

	return outmat;
}

Matrix3x3 & Matrix3x3::Rotate(float a, const Vector3 & axis, Matrix3x3 & out)
{
	float c = std::cos(a), s = std::sin(a), t = 1 - c;
	float x = axis.x, y = axis.y, z = axis.z;

	out.r00 = t*x*x + c;   out.r01 = t*x*y - s*z; out.r02 = t*x*z + s*y;
	out.r10 = t*x*y + s*z; out.r11 = t*y*y + c;   out.r12 = t*y*z - s*x;
	out.r20 = t*x*z - s*y; out.r21 = t*y*z + s*x; out.r22 = t*z*z + c;

	return out;
}

Matrix3x3 & Matrix3x3::Translation(const Vector2 & v, Matrix3x3 & out)
{
	out = Matrix3x3();
	out.r02 = v.x;
	out.r12 = v.y;
	return out;
}

Matrix3x3 & Matrix3x3::Scale(const Vector3 & v, Matrix3x3 & out)
{
	out = Matrix3x3();
	out.r00 = v.x;
	out.r11 = v.y;
	out.r22 = v.z;
	return out;
}

