#include "Animation.h"
#include "SpriteEffect.h"

Animation::Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime, Color chroma)
	:
	sprite(sprite),
	holdTime(holdTime),
	chroma(chroma)
{
	for (int i = 0; i < count; i++)
	{
		frames.emplace_back(x + i * width, x + (i + 1) * width, y + 1, y + height); //why do I need a + 1?
	}
}

void Animation::Draw(const Vei2& pos, Graphics& gfx) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], sprite, SpriteEffect::Chroma{ chroma });
}

void Animation::Draw(const Vei2& pos, Graphics& gfx, const RectI& clipRect) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clipRect, sprite, SpriteEffect::Chroma{ chroma });
}

void Animation::DrawInverse(const Vei2& pos, Graphics& gfx) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], sprite, SpriteEffect::Inverse{ chroma });
}

void Animation::DrawInverse(const Vei2& pos, Graphics& gfx, const RectI& clipRect) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clipRect, sprite, SpriteEffect::Inverse{ chroma });
}

void Animation::DrawColor(const Vei2& pos, Color sub, Graphics& gfx) const
{
	gfx.DrawSprite(pos.x, pos.y,  frames[iCurFrame], sprite, SpriteEffect::Substitution{ chroma, sub });
}

void Animation::DrawColor(const Vei2& pos, Color sub, Graphics& gfx, const RectI& clipRect) const
{
	SpriteEffect::Substitution e{ chroma, sub };
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clipRect, sprite, SpriteEffect::Substitution{ chroma, Colors::Red });
}

void Animation::Update(float dt)
{
	curFrameTime += dt;
	while (curFrameTime >= holdTime)
	{
		curFrameTime -= holdTime;
		Advance();
	}
	
}

void Animation::Advance()
{
	++iCurFrame;
	if (iCurFrame == frames.size())
	{
		iCurFrame = 0;
	}
}
