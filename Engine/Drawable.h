#pragma once

#include "Graphics.h"
#include <vector>
#include "Vec2.h"
#include "Surface.h"
#include "SpriteEffect.h"

enum class VisualEffect
{
	Default,
	Red
};


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
		switch(effect)
		{
			case(VisualEffect::Default):
			if (hasRect)
			{

				gfx.DrawSprite(transformation, srcRect, *src, SpriteEffect::Chroma{ Colors::Magenta });
			}
			else
			{
				gfx.DrawSprite(transformation, *src, SpriteEffect::Chroma{ Colors::Magenta });
			}
			break;
		
			case(VisualEffect::Red):
			if (hasRect)
			{

				gfx.DrawSprite(transformation, srcRect, *src, SpriteEffect::Substitution{ Colors::Magenta, Colors::Red });
			}
			else
			{
				gfx.DrawSprite(transformation, *src, SpriteEffect::Substitution{ Colors::Magenta, Colors::Red });
			}
			break;
		}
		if (extra)
		{
			gfx.DrawSprite((*extra).transformation, (*extra).srcRect, *(*extra).src, SpriteEffect::Chroma{ Colors::Magenta });
		}
	}
	void AddSourceRect(const RectI& srcRect_in)
	{
		srcRect = srcRect_in;
		hasRect = true;

	}
	void AddVisualEffect(const VisualEffect& effect_in)
	{
		effect = effect_in;
	}
	bool FromSpriteSheet() const
	{
		return hasRect;
	}
	void AddExraDrawable(Drawable& extra_in)
	{
		extra = &extra_in;
	}
private:
	Drawable* extra = nullptr;
	VisualEffect effect = VisualEffect::Default;
	const Surface* src; 
	RectI srcRect = {};
	bool hasRect = false;
	Mat3 transformation = Mat3::Identity();
};

