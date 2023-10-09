#pragma once

#include <cmath>
#include "Mat3.h"
#include <random>

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

 class Dice
 {
 public:
	 Dice()
		 :
		 rng(rd)
	 {}

	 int Roll20() const
	 {
		 std::uniform_int_distribution<int> distribution(1, 20);
		 return distribution(rng);
	 }
	 int Roll20()
	 {
		 std::uniform_int_distribution<int> distribution(1, 20);
		 return distribution(rng);
	 }
	 int Roll99999()
	 {
		 std::uniform_int_distribution<int> distribution(10000, 99999);
		 return distribution(rng);
	 }

 private:
	 std::random_device rd;
	 std::mt19937 rng;

 };

 static Dice dice;

 enum class Outcome : int
 {
	 CriticalHit = 2,
	 CriticalMiss = 0,
	 Hit = 1,
	 Miss = 0,
	 NotApplicable = 0
 };