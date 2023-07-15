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
	{}
	void ApplyTransformation( const Mat3& transformation_in )
	{
		transformation = transformation_in * transformation;
	}
	void Render( Graphics& gfx ) const
	{
		gfx.DrawSprite( transformation, *src, SpriteEffect::Chroma{ Colors::Magenta });
	}
private:
	const Surface* src; 
	Mat3 transformation = Mat3::Identity();
};