/*****************************************************************************/
/*!
\file			Matrix4x4.cpp
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
Matrix 4x4 class for math library.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#include "MathInc.h"

Matrix4x4::Matrix4x4(const Vector4 & r0, const Vector4 & r1, const Vector4 & r2, const Vector4 & r3)
{
	auto sz = sizeof(Vector4);
	memcpy_s(&r00, sz, &r0, sz);
	memcpy_s(&r01, sz, &r1, sz);
	memcpy_s(&r02, sz, &r2, sz);
	memcpy_s(&r03, sz, &r3, sz);
}

void Matrix4x4::InitViaColumns(const Vector4 & col0, const Vector4 & col1, const Vector4 & col2, const Vector4 & col3)
{
	*this = Matrix4x4(col0, col1, col2, col3);
	Transpose();
}


Matrix4x4 & Matrix4x4::operator*=(const Matrix4x4 & m)
{
	Matrix4x4 temp = m;
	temp.Transpose();

	const Vec4 thisr0 = GetRow(0);
	const Vec4 thisr1 = GetRow(1);
	const Vec4 thisr2 = GetRow(2);
	const Vec4 thisr3 = GetRow(3);

	const Vec4 otherr0 = temp.GetRow(0);
	const Vec4 otherr1 = temp.GetRow(1);
	const Vec4 otherr2 = temp.GetRow(2);
	const Vec4 otherr3 = temp.GetRow(3);
	*this = Matrix4x4
	{
		thisr0.Dot(otherr0),thisr0.Dot(otherr1),thisr0.Dot(otherr2),thisr0.Dot(otherr3),
		thisr1.Dot(otherr0),thisr1.Dot(otherr1),thisr1.Dot(otherr2),thisr1.Dot(otherr3),
		thisr2.Dot(otherr0),thisr2.Dot(otherr1),thisr2.Dot(otherr2),thisr2.Dot(otherr3),
		thisr3.Dot(otherr0),thisr3.Dot(otherr1),thisr3.Dot(otherr2),thisr3.Dot(otherr3)
	};

	return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & m) const
{
	Matrix4x4 res(*this);
	res *= m;
	return res;
}

float * Matrix4x4::operator[](unsigned int i)
{
	return &r00 + i * 4;
}

const float * Matrix4x4::operator[](unsigned int i) const
{
	return &r00 + i * 4;
}

Vector3 Matrix4x4::operator*(const Vector3 & v) const
{
	Vector4 temp{ v };
	return Vector3(GetRow(0).Dot(temp), GetRow(1).Dot(temp), GetRow(2).Dot(temp));
}

Vector4 Matrix4x4::operator*(const Vector4 & v) const
{
	return Vector4(GetRow(0).Dot(v), GetRow(1).Dot(v), GetRow(2).Dot(v), GetRow(3).Dot(v));
}

const Vector4 & Matrix4x4::GetCol(unsigned int i) const
{
	return *reinterpret_cast<const Vector4*>(&r00 + i * 4);
}

Vector4 Matrix4x4::GetRow(unsigned int i) const
{
	return Vector4((*this)[0][i], (*this)[1][i], (*this)[2][i], (*this)[3][i]);
}

Matrix4x4::Matrix4x4(const Matrix3x3 & other)
{
	auto sz = sizeof(float) * 3;
	memcpy_s(&r00, sz, &other.r00, sz);
	memcpy_s(&r01, sz, &other.r01, sz);
	memcpy_s(&r02, sz, &other.r02, sz);

	r03 = r13 = r23 = r30 = r31 = r32 = 0.f;
	r33 = 1.f;

}

Matrix4x4 & Matrix4x4::Transpose()
{
	std::swap(r01, r10);
	std::swap(r02, r20);
	std::swap(r03, r30);
	std::swap(r12, r21);
	std::swap(r13, r31);
	std::swap(r23, r32);
	return *this;
}

Matrix4x4 & Matrix4x4::Inverse()
{
	// Compute the reciprocal determinant
	const float det = Determinant();
	if (det == static_cast<float>(0.0))
	{
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense but it is easy to debug for the
		// programmer.
		const float nan = std::numeric_limits<float>::quiet_NaN();
		*this = Matrix4x4(
			nan, nan, nan, nan,
			nan, nan, nan, nan,
			nan, nan, nan, nan,
			nan, nan, nan, nan);

		return *this;
	}

	const float invdet = static_cast<float>(1.0) / det;

	Matrix4x4 res;
	res.a1 = invdet  * (b2 * (c3 * d4 - c4 * d3) + b3 * (c4 * d2 - c2 * d4) + b4 * (c2 * d3 - c3 * d2));
	res.a2 = -invdet * (a2 * (c3 * d4 - c4 * d3) + a3 * (c4 * d2 - c2 * d4) + a4 * (c2 * d3 - c3 * d2));
	res.a3 = invdet  * (a2 * (b3 * d4 - b4 * d3) + a3 * (b4 * d2 - b2 * d4) + a4 * (b2 * d3 - b3 * d2));
	res.a4 = -invdet * (a2 * (b3 * c4 - b4 * c3) + a3 * (b4 * c2 - b2 * c4) + a4 * (b2 * c3 - b3 * c2));
	res.b1 = -invdet * (b1 * (c3 * d4 - c4 * d3) + b3 * (c4 * d1 - c1 * d4) + b4 * (c1 * d3 - c3 * d1));
	res.b2 = invdet  * (a1 * (c3 * d4 - c4 * d3) + a3 * (c4 * d1 - c1 * d4) + a4 * (c1 * d3 - c3 * d1));
	res.b3 = -invdet * (a1 * (b3 * d4 - b4 * d3) + a3 * (b4 * d1 - b1 * d4) + a4 * (b1 * d3 - b3 * d1));
	res.b4 = invdet  * (a1 * (b3 * c4 - b4 * c3) + a3 * (b4 * c1 - b1 * c4) + a4 * (b1 * c3 - b3 * c1));
	res.c1 = invdet  * (b1 * (c2 * d4 - c4 * d2) + b2 * (c4 * d1 - c1 * d4) + b4 * (c1 * d2 - c2 * d1));
	res.c2 = -invdet * (a1 * (c2 * d4 - c4 * d2) + a2 * (c4 * d1 - c1 * d4) + a4 * (c1 * d2 - c2 * d1));
	res.c3 = invdet  * (a1 * (b2 * d4 - b4 * d2) + a2 * (b4 * d1 - b1 * d4) + a4 * (b1 * d2 - b2 * d1));
	res.c4 = -invdet * (a1 * (b2 * c4 - b4 * c2) + a2 * (b4 * c1 - b1 * c4) + a4 * (b1 * c2 - b2 * c1));
	res.d1 = -invdet * (b1 * (c2 * d3 - c3 * d2) + b2 * (c3 * d1 - c1 * d3) + b3 * (c1 * d2 - c2 * d1));
	res.d2 = invdet  * (a1 * (c2 * d3 - c3 * d2) + a2 * (c3 * d1 - c1 * d3) + a3 * (c1 * d2 - c2 * d1));
	res.d3 = -invdet * (a1 * (b2 * d3 - b3 * d2) + a2 * (b3 * d1 - b1 * d3) + a3 * (b1 * d2 - b2 * d1));
	res.d4 = invdet  * (a1 * (b2 * c3 - b3 * c2) + a2 * (b3 * c1 - b1 * c3) + a3 * (b1 * c2 - b2 * c1));
	*this = res;

	return *this;
}

float Matrix4x4::Determinant() const
{
	return a1*b2*c3*d4 - a1*b2*c4*d3 + a1*b3*c4*d2 - a1*b3*c2*d4
		+ a1*b4*c2*d3 - a1*b4*c3*d2 - a2*b3*c4*d1 + a2*b3*c1*d4
		- a2*b4*c1*d3 + a2*b4*c3*d1 - a2*b1*c3*d4 + a2*b1*c4*d3
		+ a3*b4*c1*d2 - a3*b4*c2*d1 + a3*b1*c2*d4 - a3*b1*c4*d2
		+ a3*b2*c4*d1 - a3*b2*c1*d4 - a4*b1*c2*d3 + a4*b1*c3*d2
		- a4*b2*c3*d1 + a4*b2*c1*d3 - a4*b3*c1*d2 + a4*b3*c2*d1;
}

Matrix4x4 & Matrix4x4::RotationZ(float radian, Matrix4x4 & outmat)
{
	Matrix3x3 temp{};
	Matrix4x4 res{ Matrix3x3::RotateZ(radian, temp) };
	outmat = res;
	return outmat;
}

Matrix4x4 & Matrix4x4::RotationX(float radian, Matrix4x4 & outmat)
{
	Matrix3x3 temp{};
	Matrix4x4 res{ Matrix3x3::RotateX(radian, temp) };
	outmat = res;
	return outmat;
}

Matrix4x4 & Matrix4x4::RotationY(float radian, Matrix4x4 & outmat)
{
	Matrix3x3 temp{};
	Matrix4x4 res{ Matrix3x3::RotateY(radian, temp) };
	outmat = res;
	return outmat;
}

Matrix4x4 & Matrix4x4::Rotate(float radian, const Vector3 & axis, Matrix4x4 & out)
{
	Matrix3x3 temp{};
	Matrix4x4 res{ Matrix3x3::Rotate(radian,axis, temp) };
	out = res;
	return out;
}

Matrix4x4 & Matrix4x4::Translation(const Vector3 & v, Matrix4x4 & out)
{
	out = Matrix4x4();
	out.r03 = v.x;
	out.r13 = v.y;
	out.r23 = v.z;
	return out;
}

Matrix4x4 & Matrix4x4::Scale(const Vector3 & v, Matrix4x4 & out)
{
	out = Matrix4x4();
	out.r00 = v.x;
	out.r11 = v.y;
	out.r22 = v.z;
	return out;
}

Matrix4x4 & Matrix4x4::EulerRotation(float x, float y, float z, Matrix4x4& out)
{
	out = Matrix4x4();

	float sx, cx;
	float sy, cy;
	float sz, cz;

	sx = sinf(x); cx = cosf(x);
	sy = sinf(y); cy = cosf(y);
	sz = sinf(z); cz = cosf(z);

	// Fill out 3x3 rotations.
	float sxsz = sx * sz;
	float sxcz = sx * cz;

	out[0][0] = cy*cz + sy*sxsz;   out[0][1] = cx*sz;   out[0][2] = cy*sxsz - sy*cz;
	out[1][0] = sy*sxcz - sz*cy;   out[1][1] = cx*cz;   out[1][2] = sy*sz + sxcz*cy;
	out[2][0] = cx*sy;             out[2][1] = -sx;     out[2][2] = cx*cy;

	return out;
}

Matrix4x4 & Matrix4x4::EulerRotation(vec3 angles, Matrix4x4 & out)
{
	return EulerRotation(angles.x, angles.y, angles.z, out);
}

Matrix4x4 & Matrix4x4::SetTranslation(const Vector3 & v)
{
	r03 = v.x;
	r13 = v.y;
	r23 = v.z;
	return *this;
}

Matrix4x4 & Matrix4x4::SetScale(const Vector3 & v)
{
	Matrix4x4::Scale(v, *this);
	return *this;
}

Matrix4x4 & Matrix4x4::SetRotationEuler(const Vector3 & v)
{
	Matrix4x4::EulerRotation(v.x, v.y, v.z, *this);
	return *this;
}

Matrix4x4 & Matrix4x4::SetRotationAxisAngle(float radian, const Vector3 & axis)
{
	Matrix4x4::Rotate(radian, axis, *this);
	return *this;
}

Matrix4x4 & Matrix4x4::SetSRT(const vec3 & scale, const vec3 & rot, const vec3 & translate)
{
	EulerRotation(rot.x, rot.y, rot.z, *this);
	/*
	Matrix4x4 x, y, z;
	RotationZ(rot.z, z);

	RotationX(rot.x, x);

	RotationY(rot.y, y);

	*this = z * x * y;*/
	SetTranslation(translate);
	
	mtx4[0][0] *= scale.x;	mtx4[0][1] *= scale.x;	mtx4[0][2] *= scale.x;
	mtx4[1][0] *= scale.y;	mtx4[1][1] *= scale.y;	mtx4[1][2] *= scale.y;
	mtx4[2][0] *= scale.z;	mtx4[2][1] *= scale.z;	mtx4[2][2] *= scale.z;

	return *this;
}

Vector3 Matrix4x4::GetEulerAngles() const
{

	Matrix4x4 temp{ *this };

	float length0 = temp.GetCol(0).Length();
	float length1 = temp.GetCol(1).Length();
	float length2 = temp.GetCol(2).Length();

	//remove scale from matrix
	const_cast<Vector4&>(temp.GetCol(0)) /= length0;
	const_cast<Vector4&>(temp.GetCol(1)) /= length1;
	const_cast<Vector4&>(temp.GetCol(2)) /= length2;

	
	float R, P, Y; //roll pitch yaw

	if (temp[2][1] < 1.0f)
	{
		if (temp[2][1] > -1.0f)
		{
			R = atan2f(temp[0][1], temp[1][1]);
			P = -asinf(temp[2][1]);
			Y = atan2f(temp[2][0], temp[2][2]);
		}
		else
		{
			R = atan2f(temp[0][2], temp[0][0]);
			P = PI_OVER2;
			Y = 0.f;
		}
	}
	else
	{
		R = -atan2f(temp[0][2], temp[0][0]);
		P = -PI_OVER2;
		Y = 0.f;
	}

	return vec3(P, Y, R);
}

Vector3 Matrix4x4::GetPosition() const
{
	return Vector3(r03, r13, r23);
}

Vector3 Matrix4x4::GetScale() const
{
	Matrix4x4 temp{ *this };
	temp.Transpose();

	float length0 = temp.GetRow(0).Length();
	float length1 = temp.GetRow(1).Length();
	float length2 = temp.GetRow(2).Length();

	return Vector3(length0, length1, length2);
}


