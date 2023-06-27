#pragma once
#include "Vec2.h"


template<typename T>
class Line_
{
public:
	Line_() = default;
	Line_(Vec2_<T> a, Vec2_<T> b)
		:
		A(a),
		B(b)
	{
		if (b.x - a.x != 0)
		{
			slope = ((b.y - a.y) / (b.x - a.x));
		}
		else
		{
			slope = 0.0f;
		}



		y_intercept = b.y - (slope * b.x);
		lengthSq = GetLengthSq();
		

	}
	Line_(T x1, T x2, T y1, T y2)
		:
		Line_(Vec2_(x1, y1) , Vec2_(x2, y2))
	{

	}
	//potentially performance bad if used beyond intializing (changing line lengths)
	float GetLengthSq()
	{
		return float((A.x - B.x) * (A.x - B.x)) + ((A.y - B.y) * (A.y - B.y));
	}
	//strict inequality for bounds check
	bool PointIsInLineSegment(Vec2_<T> pt, Line_ L)
	{
		if (!((L.A.x < pt.x && pt.x < L.B.x) || (L.A.x > pt.x && pt.x > L.B.x))) //either A < x < B OR B < x < A
		{
			return false;
		}
		else if (!((L.A.y < pt.y && pt.y < L.B.y) || (L.A.y > pt.y && pt.y > L.B.y)))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool GetNearbyLinesByDistance(Vec2 pos, float distSq)
	{
		if (A.GetDistanceSq(pos) <= (lengthSq / 2) + distSq || B.GetDistanceSq(pos) <= (lengthSq / 2) + distSq)
		{
			return true;
		}

	}


	//returns an empty Vec2 if no intercept
	Vec2_<T> Intercept(Line_ AB, Line_ CD)
	{
		Vec2_<T> intercept;
		T x;
		T y;
		T b_diff = CD.y_intercept - AB.y_intercept;
		T m_diff = AB.slope - CD.slope;
		if (b_diff == 0 && m_diff == 0)
		{
			//infinite intercepts
			return { 10000.0f, 10000.0f }; //returning something crazy to notify me that this case needs to be addressed
			//probably look at which intercept I need for my use case
		}
		if (m_diff == 0) //parallel
		{
			return {};
		}
		if (b_diff == 0)
		{
			x = 0;
			y = CD.y_intercept;
			intercept = { x,y };
			if (!PointIsInLineSegment(intercept, AB))
			{
				return {};
			}
			if (!PointIsInLineSegment(intercept, CD))
			{
				return {};
			}
			else
			{
				return intercept;
			}
		}
		else
		{
			x = b_diff / m_diff;
			y = (AB.slope * x) + AB.y_intercept;
			intercept = { x,y };
			if (!PointIsInLineSegment(intercept, AB))
			{
				return {};
			}
			if (!PointIsInLineSegment(intercept, CD))
			{
				return {};
			}
			else
			{
				return intercept;
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

typedef Line_<int> LineI;
typedef Line_<float> Line;