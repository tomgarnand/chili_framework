#pragma once
#include "Vec2.h"
#include <assert.h>


template<typename T>
class Line
{
public:
	Line() = default;
	Line(Vec2_<T> a, Vec2_<T> b)
		:
		A(a),
		B(b)
	{
		if (b.x - a.x != 0)
		{
			slope = (((float)b.y - (float)a.y) / ((float)b.x - (float)a.x));
		}
		else
		{
			slope = 0.0f;
		}

		y_intercept = (float)b.y - (slope * (float)b.x);
		lengthSq = GetLengthSq();
		

	}
	Line(T x1, T x2, T y1, T y2)
		:
		Line(Vec2_<T>(x1, y1) , Vec2_<T>(x2, y2))
	{

	}
	//potentially performance bad if used beyond intializing (changing line lengths)
	float GetLengthSq() const
	{
		return float((A.x - B.x) * (A.x - B.x)) + ((A.y - B.y) * (A.y - B.y));
	}
	//strict inequality for bounds check
	bool PointIsInLineSegment(const Vec2_<T>& pt) const
	{
		if (!((A.x < pt.x && pt.x < B.x) || (A.x > pt.x && pt.x > B.x))) //either A < x < B OR B < x < A
		{
			return false;
		}
		else if (!((A.y < pt.y && pt.y < B.y) || (A.y > pt.y && pt.y > B.y)))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool GetNearbyLinesByDistance(const Vec2_<T>& pos, const float distSq) const
	{
		if (A.GetDistanceSq(pos) <= (lengthSq / 2) + distSq || B.GetDistanceSq(pos) <= (lengthSq / 2) + distSq)
		{
			return true;
		}
		return false;
	}

	//line AB is obj and line CD is arg
	//returns an empty Vec2 if no intercept
	std::pair<bool,Vec2> Intercept(const Line& CD) const
	{
		Vec2 intercept;
		float x;
		float y;
		float b_diff = CD.y_intercept - y_intercept;
		float m_diff = slope - CD.slope;
		if (b_diff == 0 && m_diff == 0)
		{
			//infinite intercepts
			assert(true);
			return { true, {} };
			//probably look at which intercept I need for my use case
		}
		if (m_diff == 0) //parallel
		{
			return { false, {} };
		}
		if (b_diff == 0)
		{
			x = 0;
			y = CD.y_intercept;
			intercept = { x,y };
			if (!PointIsInLineSegment(intercept)) //called on this
			{
				return { false, {} };
			}
			if (!CD.PointIsInLineSegment(intercept))
			{
				return { false, {} };
			}
			else
			{
				return { true, intercept };
			}
		}
		else
		{
			x = b_diff / m_diff;
			y = (slope * x) + y_intercept;
			intercept = { x,y };
			if (!PointIsInLineSegment(intercept))
			{
				return { false, {} };
			}
			if (!CD.PointIsInLineSegment(intercept))
			{
				return { false, {} };
			}
			else
			{
				return { true, intercept };
			}
		}
	}


public:
	Vec2_<T> A;
	Vec2_<T> B;
	float slope;
	float y_intercept;
	float lengthSq;
};

typedef Line<int> LineI;
typedef Line<float> LineF;