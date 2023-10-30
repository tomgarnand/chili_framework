#pragma once

#include <cmath>
#include "Mat3.h"
#include "Dice.h"

constexpr double PI_D = 3.141592653589793238462643383279;
constexpr float PI = (float)PI_D;

inline Vec2 GetVec2(const Mat3& ma)
{
	return Vec2(ma * Vec3(0, 0));
}

template<typename T>
auto sq(T x)
{
	return x * x;
}

template<typename T>
T DistancePointLine(const Vec2_<T>& p0, const Vec2_<T>& p1, const Vec2_<T>& q)
{
	const T a = p0.y - p1.y;
	const T b = p1.x - p0.x;
	const T c = p0.x * p1.y - p1.x * p0.y;

	return std::abs(a * q.x + b * q.y + c) / std::sqrt(sq(a) + sq(b));
}

 inline bool IsBetween(float a, float b, float c, float error)
{
	if (((b - a) < error) && (c - b) < error)
	{
		return true;
	}
	else if (((b - c) < error) && (a - b) < error)
	{
		return true;
	}
	return false;
}

 static Dice d20(1, 20);
 static Dice d99999(10000, 99999);

 

