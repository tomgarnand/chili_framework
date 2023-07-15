#pragma once


#include "Rect.h"
#include "Graphics.h"
#include "Util.h"
#include "Drawable.h"
#include "Mat3.h"

class Camera
{
public:
	Camera(Graphics& gfx)
		:
		gfx(gfx)
	{

	}
	Vec2 GetPos() const
	{
		return pos;
	}
	void MoveBy( const Vec2& offset )
	{
		pos += offset;
	}
	void MoveTo( const Vec2& pos_in )
	{
		pos = pos_in;
	}
	void Draw( Drawable drawable ) const
	{
		drawable.ApplyTransformation(
			Mat3::Rotation( angle ) *
			Mat3::Scale( scale ) *
			Mat3::Translation( -pos.x,-pos.y )
		);
		drawable.Render( gfx );
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
	RectF GetViewportRect() const
	{
		const float zoom = 1.0f / scale;
		// do a pessimistic viewport rect that works regardless of the viewport rotation
		const float diagonal = sqrt(
			sq( float( Graphics::ScreenWidth / 2 ) * zoom ) +
			sq( float( Graphics::ScreenHeight / 2 ) * zoom )
		);
		return RectF::FromCenter( pos,diagonal,diagonal	);
	}

private:
	Vec2 pos = {0.0f,0.0f};
	float scale = 1.0f;
	float angle = 0.0f;
	Graphics& gfx;
};