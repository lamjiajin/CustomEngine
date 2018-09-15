/*****************************************************************************/
/*!
\file			Matrix3x3.h
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
#pragma once

class Matrix4x4;

typedef
class Matrix3x3
{
public:
	/*
	elements numbers labels are row,columm. eg. r12 = row 1 col 2.
	Column Major Matrix
	*/
	Matrix3x3() :
		r00{ 1.f }, r01{ 0.f }, r02{0.f},
		r10{ 0.f }, r11{ 1.f }, r12{0.f},
		r20{ 0.f }, r21{ 0.f }, r22{1.f}
	{}

	Matrix3x3(float r0_0, float r0_1, float r0_2,
		float r1_0, float r1_1, float r1_2,
		float r2_0, float r2_1, float r2_2) :
		r00{ r0_0 }, r01{ r0_1 }, r02{ r0_2 },
		r10{ r1_0 }, r11{ r1_1 }, r12{ r1_2 },
		r20{ r2_0 }, r21{ r2_1 }, r22{ r2_2 }
	{}

	//vectors are row wise
	Matrix3x3(const Vector3& r0, const Vector3& r1, const Vector3& r2);

public:

	// matrix multiplication.
	Matrix3x3& operator *= (const Matrix3x3& m);
	Matrix3x3  operator  * (const Matrix3x3& m) const;

	// array access operators
	float* operator[]       (unsigned int i);
	const float* operator[] (unsigned int i) const;

	const Vector3& GetCol(unsigned int i) const;
	Vector3 GetRow(unsigned int i) const;

	Vector3 operator*(const Vector3& v) const;
	Vector4 operator*(const Vector4& v) const;
	Vector2 operator*(const Vector2& v) const;
public:

	// -------------------------------------------------------------------
	/** @brief Construction from a 4x4 matrix. The remaining parts
	*  of the matrix are ignored.
	*/
	explicit Matrix3x3(const Matrix4x4& other);

	// -------------------------------------------------------------------
	/** @brief Transpose the matrix
	*/
	Matrix3x3& Transpose();

	// -------------------------------------------------------------------
	/** @brief Invert the matrix.
	*  If the matrix is not invertible all elements are set to qnan.
	*  Beware, use (f != f) to check whether a float f is qnan.
	*/
	Matrix3x3& Inverse();
	float Determinant() const;

public:
	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around z
	*  @param a Rotation angle, in radians
	*  @param out Receives the output matrix
	*  @return Reference to the output matrix
	*/
	static Matrix3x3& RotateZ(float radian, Matrix3x3& outmat);
	static Matrix3x3& RotateX(float radian, Matrix3x3& outmat);
	static Matrix3x3& RotateY(float radian, Matrix3x3& outmat);

	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around
	*    an arbitrary axis.
	*
	*  @param a Rotation angle, in radians
	*  @param axis Axis to rotate around
	*  @param out To be filled
	*/
	static Matrix3x3& Rotate(float radian,
		const Vector3& axis, Matrix3x3& out);

	// -------------------------------------------------------------------
	/** @brief Returns a translation matrix
	*  @param v Translation vector
	*  @param out Receives the output matrix
	*  @return Reference to the output matrix
	*/
	static Matrix3x3& Translation(const Vector2& v, Matrix3x3& out);

	//input vector containing the scale along the 3 principle axis
	static Matrix3x3& Scale(const Vector3& v, Matrix3x3& out);
	

public:
	float r00,r10,r20,
		r01,r11,r21,
		r02,r12,r22;

} Mat3,mat3;
