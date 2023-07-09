#pragma once
#include "Vec2.h"
#include <assert.h>
#include "Util.h"


template<typename T>
class Line
{
public:
	Line() = default;
	Line(Vec2_<T> A, Vec2_<T> B)
		:
		P(A),
		Q(B)
	{
		//aproach is for a equation in the form ax + by + c = 0
		//method: https://bobobobo.wordpress.com/2008/01/07/solving-linear-equations-ax-by-c-0/

		a = Q.y - P.y;
		b = Q.x - P.x;
		c = (P.x * Q.y) - (Q.x * P.y);
		
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
		return float((P.x - Q.x) * (P.x - Q.x)) + ((P.y - Q.y) * (P.y - Q.y));
	}
	
	bool PointIsInLineSegment(const Vec2_<T>& pt) const
	{
		if (!(IsBetween(P.x, pt.x, Q.x, 0.1f))) 
		{
			return false;
		}
		else if (!(IsBetween(P.y, pt.y, Q.y, 0.1f)))
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
		if (P.GetDistanceSq(pos) <= (lengthSq / 2) + distSq || Q.GetDistanceSq(pos) <= (lengthSq / 2) + distSq)
		{
			return true;
		}
		return false;
	}

	//line AB is obj and line CD is arg
	//returns an empty Vec2 if no intercept
	std::pair<bool,Vec2> Intercept(const Line& that) const
	{
		//https://www.cuemath.com/algebra/simultaneous-linear-equations/
		// first, check for parallel lines, the only case with non-intersection
		//also catches the infinitely many solutions case
		if (that.a * this->b == that.b * this->a)
		{
			return { false, {} };
		}
		else
		{
			float x0;
			float y0;
			float d = (this->a * that.b - this->b * that.a);

			x0 = -(this->b * that.c - this->c * that.b) / d;
			y0 = (this->c * that.a - this->a * that.c) / d;
			if (this->PointIsInLineSegment({ x0,y0 }) && that.PointIsInLineSegment({ x0,y0 }))
			{
				return { true, {x0,y0} };
			}
			return { false, {} };

		}
	}


public:
	Vec2_<T> P;
	Vec2_<T> Q;
	T a;
	T b;
	T c;
	float lengthSq;
};

typedef Line<int> LineI;
typedef Line<float> LineF;