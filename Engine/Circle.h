#pragma once

#include "Vec2.h"

template<typename T> class Circle
{
public:
	Circle() = default;
	//center at (h,k) with radius r
	Circle(T h, T k, T, r)
		:
		h(h),
		k(k),
		r(r)
	{

	}
	Circle(const Vec2_<T> center, T radius)
	{
		Circle(center.x, center,y, radius)
	}
	Vec2_<T> GetCenter() const
	{
		return { h,k };
	}
	T GetRadius() const
	{
		return r;
	}

public:
	T h;
	T k;
	T r;
};

typedef Circle<float> CircF;
typedef Circle<int> CircI;