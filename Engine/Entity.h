#pragma once

#include <vector>
#include "Vec2.h"
#include "Drawable.h"

class Entity
{
public:
	Entity( Surface& src ,const Vec2& pos = { 0.0f,0.0f })
		:
		pos( pos ),
		src( src )
	{}
	const Vec2& GetPos() const
	{
		return pos;
	}
	void SetPos( const Vec2& newPos )
	{
		pos = newPos;
	}
	void TranslateBy( const Vec2& offset )
	{
		pos += offset;
	}
	void SetScale( float s )
	{
		scale = s;
	}
	float GetScale() const
	{
		return scale;
	}
	void SetAngle( float a )
	{
		angle = a;
	}
	float GetAngle() const
	{
		return angle;
	}

	Drawable GetDrawable() const
	{
		Drawable d( src );
		d.ApplyTransformation(
			Mat3::Translation( pos.x,pos.y ) *
			Mat3::Scale( scale ) *
			Mat3::Rotation( angle )
		);
		return d;
	}
private:
	float angle = 0.0f;
	float scale = 1.0f;
	Vec2 pos = {0.0f,0.0f};
	Surface& src;
};