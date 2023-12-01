#pragma once

#include "Graphics.h"
#include "Rect.h"
#include "Mat3.h"
#include "Vec2.h"
#include "Surface.h"
#include "Animation.h"
#include "SpriteEffect.h"
#include <optional>
#include <functional> // For std::reference_wrapper


enum class VisualEffect
{
	Default,
	Red
};


class Drawable
{
public:

	Drawable(const Surface& src)
		:
		src( src )
	{
	}
	Drawable(const Animation& ani)
		:
		src(ani.GetSource()),
		hasRect(true),
		animation(ani)
	{

	}
	void ApplyTransformation( const Mat3& transformation_in )
	{
		transformation = transformation_in * transformation;
	}
	void Render( Graphics& gfx ) 
	{
		if (animation.has_value())
		{
			srcRect = animation.value().get().GetSourceRectRef();
		}
		
		switch(effect)
		{
			case(VisualEffect::Default):
			if (hasRect)
			{

				gfx.DrawSprite(transformation, srcRect, src, SpriteEffect::Chroma{ Colors::Magenta });
			}
			else
			{
				gfx.DrawSprite(transformation, src, SpriteEffect::Chroma{ Colors::Magenta });
			}
			break;
		
			case(VisualEffect::Red):
			if (hasRect)
			{

				gfx.DrawSprite(transformation, srcRect, src, SpriteEffect::Substitution{ Colors::Magenta, Colors::Red });
			}
			else
			{
				gfx.DrawSprite(transformation, src, SpriteEffect::Substitution{ Colors::Magenta, Colors::Red });
			}
			break;
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
	const Surface& GetSurface() const
	{
		return src;
	}
private:
	VisualEffect effect = VisualEffect::Default;

	const Surface& src; 
	std::optional<std::reference_wrapper<const Animation>> animation;

	RectI srcRect = {};
	bool hasRect = false;
	Mat3 transformation = Mat3::Identity();
};

