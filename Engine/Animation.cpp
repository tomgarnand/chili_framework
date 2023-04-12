#include "Animation.h"

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
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], sprite, chroma);
}

void Animation::Draw(const Vei2& pos, Graphics& gfx, const RectI& clipRect) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clipRect, sprite, chroma);
}

void Animation::DrawColor(const Vei2& pos, Color sub, Graphics& gfx) const
{
	gfx.DrawSpriteSubstitute(pos.x, pos.y, Colors::Red, frames[iCurFrame], sprite, chroma);
}

void Animation::DrawColor(const Vei2& pos, Color sub, Graphics& gfx, const RectI& clipRect) const
{
	gfx.DrawSpriteSubstitute(pos.x, pos.y, Colors::Red, frames[iCurFrame], clipRect, sprite, chroma);
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
