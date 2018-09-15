/*****************************************************************************/
/*!
\file			Matrix4x4.h
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
#pragma once


typedef
class alignas(16) Matrix4x4
{
public:
	/*
	 elements numbers labels are row,columm. eg. r12 = row 1 col 2.
	 stored as column major
	*/
	Matrix4x4() :
		r00{ 1.f }, r01{ 0.f }, r02{ 0.f }, r03{ 0.f },
		r10{ 0.f }, r11{ 1.f }, r12{ 0.f }, r13{ 0.f },
		r20{ 0.f }, r21{ 0.f }, r22{ 1.f }, r23{ 0.f },
		r30{ 0.f }, r31{ 0.f }, r32{ 0.f }, r33{ 1.f }
	{}

	Matrix4x4(float r0_0, float r0_1, float r0_2, float r0_3,
		float r1_0, float r1_1, float r1_2, float r1_3,
		float r2_0, float r2_1, float r2_2, float r2_3,
		float r3_0, float r3_1, float r3_2, float r3_3) :
		r00{ r0_0 }, r01{ r0_1 }, r02{ r0_2 }, r03{ r0_3 },
		r10{ r1_0 }, r11{ r1_1 }, r12{ r1_2 }, r13{ r1_3 },
		r20{ r2_0 }, r21{ r2_1 }, r22{ r2_2 }, r23{ r2_3 },
		r30{ r3_0 }, r31{ r3_1 }, r32{ r3_2 }, r33{ r3_3 }
	{}

	//vectors are rows
	Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);

	//vectors are columns
	void InitViaColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3);
public:

	// matrix multiplication.
	Matrix4x4& operator *= (const Matrix4x4& m);
	Matrix4x4  operator  * (const Matrix4x4& m) const;

	// array access operators, to acess an element specify: [column][row]
	float* operator[]       (unsigned int i);
	const float* operator[] (unsigned int i) const;

	Vector3 operator*(const Vector3& v) const;
	Vector4 operator*(const Vector4& v) const;

	const Vector4& GetCol(unsigned int i) const;
	Vector4 GetRow(unsigned int i) const;
public:

	// -------------------------------------------------------------------
	/** @brief Construction from a 4x4 matrix. The remaining parts
	*  of the matrix are 0. bottom right is 1.
	*/
	explicit Matrix4x4(const Matrix3x3& other);

	// -------------------------------------------------------------------
	/** @brief Transpose the matrix
	*/
	Matrix4x4& Transpose();

	// -------------------------------------------------------------------
	/** @brief Invert the matrix.
	*  If the matrix is not invertible all elements are set to qnan.
	*  Beware, use (f != f) to check whether a float f is qnan.
	*/
	Matrix4x4& Inverse();
	float Determinant() const;

public:
	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around z
	*  @param a Rotation angle, in radians
	*  @param out Receives the output matrix
	*  @return Reference to the output matrix
	*/
	static Matrix4x4& RotationZ(float radian, Matrix4x4& outmat);
	static Matrix4x4& RotationX(float radian, Matrix4x4& outmat);
	static Matrix4x4& RotationY(float radian, Matrix4x4& outmat);

	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around
	*    an arbitrary axis.
	*
	*  @param a Rotation angle, in radians
	*  @param axis Axis to rotate around
	*  @param out To be filled
	*/
	static Matrix4x4& Rotate(float radian,
		const Vector3& axis, Matrix4x4& out);

	// -------------------------------------------------------------------
	/** @brief Returns a translation matrix
	*  @param v Translation vector
	*  @param out Receives the output matrix
	*  @return Reference to the output matrix
	*/
	static Matrix4x4& Translation(const Vector3& v, Matrix4x4& out);

	static Matrix4x4& Scale(const Vector3& v, Matrix4x4& out);

	//form a rotation matrix using euler angles about the principle axis
	static Matrix4x4& EulerRotation(float x, float y, float z, Matrix4x4& out);
	static Matrix4x4& EulerRotation(vec3 angles, Matrix4x4& out);

	Matrix4x4& SetTranslation(const Vector3& v);
	Matrix4x4& SetScale(const Vector3& v);
	Matrix4x4& SetRotationEuler(const Vector3& v);
	Matrix4x4& SetRotationAxisAngle(float radian, const Vector3& axis);

	Matrix4x4& SetSRT(const vec3& scale, const vec3& rot, const vec3& translate);

	Vector3 GetEulerAngles() const;
	Vector3 GetPosition() const;
	Vector3 GetScale() const;

public:
	union {
		struct
		{
			float r00, r10, r20, r30,
				r01, r11, r21, r31,
				r02, r12, r22, r32,
				r03, r13, r23, r33;
		};
		struct 
		{
			float a1, a2, a3, a4,
				b1, b2, b3, b4,
				c1, c2, c3, c4,
				d1, d2, d3, d4;
		};

		float mtx[16];
		float mtx4[4][4];
	};


} Mat4, mat4;