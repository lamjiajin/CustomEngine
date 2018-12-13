/*****************************************************************************/
/*!
\file			MathInc.h
\author(s)		Calven Koh Yao Lin (y.koh)
\par Course:	GAM350
\par Project:   GAM350
\brief
Math library includes and defines.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/*****************************************************************************/
#pragma once

#define MATH_EPSILON 0.0000000001f
#define PI 3.1415926535897932f
#define PI_OVER2 1.570796326794897f
#define PI_OVER4 0.7853981633974483f
#define TWO_PI 6.283185307179586f

#include <memory.h>  
#include <algorithm>
#include <limits>
#include <iostream>
#include <utility>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"