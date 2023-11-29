#pragma once
#include <cmath>

template <typename T> class Vec2_
{
public:
	Vec2_() = default;
	Vec2_(T x_in, T y_in)
		:
		x(x_in),
		y(y_in)
	{
	}
	template<typename S>
	Vec2_(const Vec2_<S>& src)
		:
		x((T)src.x),
		y((T)src.y)
	{}

	Vec2_ operator+(const Vec2_& rhs) const
	{
		return Vec2_(x + rhs.x, y + rhs.y);
	}
	Vec2_ operator+(const T& rhs) const
	{
		return Vec2_(x + rhs, y + rhs);
	}
	Vec2_& operator+=(const Vec2_& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2_ operator*(T rhs) const
	{
		return Vec2_(x * rhs, y * rhs);
	}
	Vec2_& operator*(const Vec2_& rhs)
	{
		return Vec2_(x * rhs.x, y * rhs.y);
	}
	Vec2_& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}
	Vec2_ operator-(const Vec2_& rhs) const
	{
		return Vec2_(x - rhs.x, y - rhs.y);
	}
	Vec2_ operator-(const T& rhs) const
	{
		return Vec2_(x - rhs, y - rhs);
	}
	Vec2_& operator-=(const Vec2_& rhs)
	{
		return *this = *this - rhs;
	}
	bool operator!=(const Vec2_& rhs) const
	{
		return (x != rhs.x || y != rhs.y);
	}
	T GetLength() const
	{
		return std::sqrt(GetLengthSq());
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if (len != 0.0f)
		{
			return *this * (1.0f / len);
		}
		return *this;
	}
	float GetDistanceSq(const Vec2_& pt) const
	{
		return ((pt.x - x) * (pt.x - x)) + ((pt.y - y) * (pt.y - y));
	}
	float GetDistance(const Vec2_& pt) const
	{
		return std::sqrt(((pt.x - x) * (pt.x - x)) + ((pt.y - y) * (pt.y - y)));
	}

public:
	T x;
	T y;
};

typedef Vec2_<int> Vei2;
typedef Vec2_<float> Vec2;