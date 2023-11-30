#pragma once

#include "Vec2.h"

template<typename T> class Circle
{
public:
	Circle() = default;
	//center at (h,k) with radius r
	Circle(T h, T k, T r)
		:
		pos({h,k}),
		radius(r)
	{

	}
	Circle(const Vec2_<T> center, T radius)
	{
		Circle(Vec2(center.x, center.y), radius);
	}

#pragma region operators
	//Circle operator+(const Circle& rhs) const
	//{
	//	return Circle(x + rhs.x, y + rhs.y);
	//}
	//Circle operator+(const T& rhs) const
	//{
	//	return Circle(x + rhs, y + rhs);
	//}
	//Circle& operator+=(const Circle& rhs)
	//{
	//	return *this = *this + rhs;
	//}
	//Circle operator*(T rhs) const
	//{
	//	return Circle(x * rhs, y * rhs);
	//}
	//Circle& operator*(const Circle& rhs)
	//{
	//	return Circle(x * rhs.x, y * rhs.y);
	//}
	//Circle& operator*=(T rhs)
	//{
	//	return *this = *this * rhs;
	//}
	//Circle operator-(const Circle& rhs) const
	//{
	//	return Circle(x - rhs.x, y - rhs.y);
	//}
	//Circle operator-(const T& rhs) const
	//{
	//	return Circle(x - rhs, y - rhs);
	//}
	//Circle& operator-=(const Circle& rhs)
	//{
	//	return *this = *this - rhs;
	//}
	//bool operator!=(const Circle& rhs) const
	//{
	//	return (x != rhs.x || y != rhs.y);
	//}
#pragma endregion

	Vec2_<T> GetCenter() const
	{
		return { pos.x,pos.y };
	}
	T GetRadius() const
	{
		return radius;
	}

public:
	Vec2_<T> pos;
	T radius;
};

typedef Circle<float> CircF;
typedef Circle<int> CircI;