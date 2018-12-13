/*****************************************************************************/
/*!
\file			Quaternion.h
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
quaternion class for math library.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#pragma once


class Quaternion
{
public:
	Quaternion() : w(1.0), x(0.f), y(0.f), z(0.f) {}
	Quaternion(float pw, float px, float py, float pz)
		: w(pw), x(px), y(py), z(pz) {}

	//Construct from rotation matrix. must be orthonormal
	explicit Quaternion(const Matrix3x3& pRotMatrix);

	// Construct from euler angles
  Quaternion(const vec3&);
	Quaternion(float rx, float ry, float rz);

	float& operator[](int index);

	//Construct from an axis-angle
	Quaternion(Vector3 axis, float angle);

	// get rotation matrix representation
	Matrix3x3 GetMatrix() const;

	Matrix4x4 GetMatrix4() const;

	//test equal
	bool Equal(const Quaternion& o, float epsilon = MATH_EPSILON) const;

	Quaternion& Normalize();

	// get quaternion conjugate
	Quaternion GetConjugate();

	// Rotate a vector by this quaternion
	Vector3 operator*(const Vector3& v);

	Quaternion operator-();

	//Multiply two quaternions
	Quaternion operator* (const Quaternion& other) const;

	// spherical interpolation between two quaternions and writes the result into out
	//t must be between 0 to 1
	static void Slerp(Quaternion& out, const Quaternion& Start,
		const Quaternion& End, float t);

	//constructs a quarternion rotation that can rotate a vector from From to To
	static Quaternion FromToRotation(const Vec3& From, const Vec3& To);

	//! w,x,y,z components of the quaternion
	float w, x, y, z;
};
