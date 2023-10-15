#pragma once

#include "Graphics.h"
#include <vector>
#include "Vec2.h"
#include "Surface.h"
#include "SpriteEffect.h"

class Drawable
{
public:
	Drawable( const Surface& src)
		:
		src( &src )
	{
	}
	void ApplyTransformation( const Mat3& transformation_in )
	{
		transformation = transformation_in * transformation;
	}
	void Render( Graphics& gfx ) const 
	{
		if (hasRect)
		{
			
			gfx.DrawSprite(transformation, srcRect, *src, SpriteEffect::Chroma{ Colors::Magenta });
		}
		else
		{
			gfx.DrawSprite(transformation, *src, SpriteEffect::Chroma{ Colors::Magenta });
		}
	}
	void AddSourceRect(const RectI& srcRect_in)
	{
		srcRect = srcRect_in;
		hasRect = true;

	}
	bool FromSpriteSheet() const
	{
		return hasRect;
	}
private:
	
	const Surface* src; 
	RectI srcRect = {};
	bool hasRect = false;
	Mat3 transformation = Mat3::Identity();
};