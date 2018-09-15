/*****************************************************************************/
/*!
\file			Quaternion.cpp
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
#include "MathInc.h"

Quaternion::Quaternion(const Matrix3x3 & pRotMatrix)
{
	float t = pRotMatrix.r00 + pRotMatrix.r11 + pRotMatrix.r22;

	if (t > 0.f)
	{
		float s = static_cast<float>(std::sqrt(1 + t) * 2.0);
		x = (pRotMatrix.r21 - pRotMatrix.r12) / s;
		y = (pRotMatrix.r02 - pRotMatrix.r20) / s;
		z = (pRotMatrix.r10 - pRotMatrix.r01) / s;
		w = 0.25f * s;
	} // check other cases
	else if (pRotMatrix.r00 > pRotMatrix.r11 && pRotMatrix.r00 > pRotMatrix.r22)
	{
		// Column 0
		float s = static_cast<float>(std::sqrt(1.0 + pRotMatrix.r00 - pRotMatrix.r11 - pRotMatrix.r22) * 2.0);
		x = static_cast<float>(0.25) * s;
		y = (pRotMatrix.r10 + pRotMatrix.r01) / s;
		z = (pRotMatrix.r02 + pRotMatrix.r20) / s;
		w = (pRotMatrix.r21 - pRotMatrix.r12) / s;
	}
	else if (pRotMatrix.r11 > pRotMatrix.r22)
	{
		// Column 1
		float s = static_cast<float>(std::sqrt(1.0 + pRotMatrix.r11 - pRotMatrix.r00 - pRotMatrix.r22) * 2.0);
		x = (pRotMatrix.r10 + pRotMatrix.r01) / s;
		y = static_cast<float>(0.25) * s;
		z = (pRotMatrix.r21 + pRotMatrix.r12) / s;
		w = (pRotMatrix.r02 - pRotMatrix.r20) / s;
	}
	else
	{
		// Column 2
		float s = static_cast<float>(std::sqrt(1.0 + pRotMatrix.r22 - pRotMatrix.r00 - pRotMatrix.r11) * 2.0);
		x = (pRotMatrix.r02 + pRotMatrix.r20) / s;
		y = (pRotMatrix.r21 + pRotMatrix.r12) / s;
		z = 0.25f * s;
		w = (pRotMatrix.r10 - pRotMatrix.r01) / s;
	}
}

Quaternion::Quaternion(const vec3& v)
  : Quaternion(v.x, v.y, v.z)
{

}

Quaternion::Quaternion(float rx, float ry, float rz)
{

	float SinX(std::sin(rx*0.5f));
	float CosX(std::cos(rx*0.5f));
	float SinY(std::sin(ry*0.5f));
	float CosY(std::cos(ry*0.5f));
	float SinZ(std::sin(rz*0.5f));
	float CosZ(std::cos(rz*0.5f));
	float CosXCosY{ CosX*CosY };
	float SinXSinY{ SinX*SinY };
#if 0
	x = SinZ * CosXCosY - CosZ * SinXSinY;
	y = CosZ * SinX * CosY + SinZ * CosX * SinY;
	z = CosZ * CosX * SinY - SinZ * SinX * CosY;
	w = CosZ * CosXCosY + SinZ * SinXSinY;

	w = CosZ * CosXCosY - SinZ * SinXSinY;
	x = CosY*CosZ*SinX + SinY*SinZ*CosX;
	y = SinY*CosZ*CosX + CosY*SinZ*SinX;
	z = CosY*SinZ*SinX - SinY*CosZ*SinX;
#endif

	Quaternion qx(Vec3(1.f, 0.f, 0.f), rx);
	Quaternion qy(Vec3(0.f, 1.f, 0.f), ry);
	Quaternion qz(Vec3(0.f, 0.f, 1.f), rz);

	(*this) = qy*qx*qz;

}

float & Quaternion::operator[](int index)
{
	// TODO: insert return statement here
	return *(&w + index);
}

Quaternion::Quaternion(Vector3 axis, float angle)
{
	axis.Normalize();

	float sin_a = std::sinf(angle / 2);
	float cos_a = std::cosf(angle / 2);
	x = axis.x * sin_a;
	y = axis.y * sin_a;
	z = axis.z * sin_a;
	w = cos_a;
}

Matrix3x3 Quaternion::GetMatrix() const
{
	Matrix3x3 res{};
	res.r00 = 1.f - 2.f * (y * y + z * z);
	res.r01 = 2.f * (x * y - z * w);
	res.r02 = 2.f * (x * z + y * w);
	res.r10 = 2.f * (x * y + z * w);
	res.r11 = 1.f - 2.f * (x * x + z * z);
	res.r12 = 2.f * (y * z - x * w);
	res.r20 = 2.f * (x * z - y * w);
	res.r21 = 2.f * (y * z + x * w);
	res.r22 = 1.f - 2.f * (x * x + y * y);

	return res;
	/*
	Matrix3x3 res;
	res.r00 = 1.f - 2.f * (y * y + z * z);
	res.r10 = 2.f * (x * y - z * w);
	res.r20 = 2.f * (x * z + y * w);
	res.r01 = 2.f * (x * y + z * w);
	res.r11 = 1.f - 2.f * (x * x + z * z);
	res.r21 = 2.f * (y * z - x * w);
	res.r02 = 2.f * (x * z - y * w);
	res.r12 = 2.f * (y * z + x * w);
	res.r22 = 1.f - 2.f * (x * x + y * y);
	*/
	//return res;
}

Matrix4x4 Quaternion::GetMatrix4() const
{
	return Matrix4x4(GetMatrix());
}

bool Quaternion::Equal(const Quaternion & o, float epsilon) const
{
	return( fabs(x - o.x) <= epsilon &&
		fabs(y - o.y) <= epsilon &&
		fabs(z - o.z) <= epsilon &&
		fabs(w - o.w) <= epsilon );
}

Quaternion & Quaternion::Normalize()
{
	float mag = std::sqrtf(x*x + y*y + z*z + w*w);
	if (mag)
	{
		float invMag = 1.f / mag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}
	return *this;
}

Quaternion Quaternion::GetConjugate()
{
	return Quaternion(w,-x,-y,-z);
}

Vector3 Quaternion::operator*(const Vector3 & v)
{
	Quaternion q2(0.f, v.x, v.y, v.z), q = *this;
	Quaternion qinv{ q.GetConjugate() };

	Quaternion result{ q*q2*qinv };
	return Vector3(result.x, result.y, result.z);
}

Quaternion Quaternion::operator-()
{
	return Quaternion(w,-x,-y,-z);
}

Quaternion Quaternion::operator*(const Quaternion & q) const
{
	return Quaternion(w*q.w - x*q.x - y*q.y - z*q.z,
		w*q.x + x*q.w + y*q.z - z*q.y,
		w*q.y + y*q.w + z*q.x - x*q.z,
		w*q.z + z*q.w + x*q.y - y*q.x);
}

void Quaternion::Slerp(Quaternion & out, const Quaternion & Start, const Quaternion & End, float t)
{
	// cosine theta
	float cosom = Start.x * End.x + Start.y * End.y + Start.z * End.z + Start.w * End.w;

	// adjust signs (if necessary)
	Quaternion end = End;
	if (cosom < static_cast<float>(0.0))
	{
		cosom = -cosom;
		end.x = -end.x;   // Reverse all signs
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}


	// Calculate coefficients
	float sclp, sclq;
	if ((static_cast<float>(1.0) - cosom) > static_cast<float>(0.0001)) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		float omega, sinom;
		omega = std::acos(cosom); // extract theta from dot product's cos theta
		sinom = std::sin(omega);
		sclp = std::sin((static_cast<float>(1.0) - t) * omega) / sinom;
		sclq = std::sin(t * omega) / sinom;
	}
	else
	{
		// if very close, do LERP
		sclp = static_cast<float>(1.0) - t;
		sclq = t;
	}

	out.x = sclp * Start.x + sclq * end.x;
	out.y = sclp * Start.y + sclq * end.y;
	out.z = sclp * Start.z + sclq * end.z;
	out.w = sclp * Start.w + sclq * end.w;
}

Quaternion Quaternion::FromToRotation(const Vec3 & From, const Vec3 & To)
{
	auto nfrom{ From };
	auto nto{ To };
	nfrom.Normalize();
	nto.Normalize();

	if (nto == -nfrom)
	{
		float rotAngle = PI;
		auto fAbs{ nfrom };
		fAbs.x = fAbs.x >= 0.f ? fAbs.x : -fAbs.x;
		fAbs.y = fAbs.y >= 0.f ? fAbs.y : -fAbs.y;
		fAbs.z = fAbs.z >= 0.f ? fAbs.z : -fAbs.z;

		auto RotAxis = fAbs.x < fAbs.y ? Vec3(1, 0, 0) : (fAbs.y < fAbs.z ? Vec3(0, 1, 0) : Vec3(0, 0, 1));
		return Quaternion(RotAxis, rotAngle);
	}
	float rotAngle = acosf(nfrom.Dot(nto));
	auto RotAxis = nfrom.Cross(nto).Normalize();
	return Quaternion(RotAxis, rotAngle);
}



