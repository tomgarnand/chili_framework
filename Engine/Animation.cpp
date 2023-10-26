#include "Animation.h"
#include "SpriteEffect.h"

Animation::Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime)
	:
	sprite(sprite),
	holdTime(holdTime)
{
	for (int i = 0; i < count; i++)
	{
		frames.emplace_back(x + i * width, x + (i + 1) * width, y + 1, y + height); //why do I need a + 1?
	}
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
RectI Animation::GetSourceRect() const
{
	return frames[iCurFrame];
}

bool Animation::ContainsExtraAnimation() const
{
	if (extra == nullptr)
	{
		return false;
	}
	return true;
}

void Animation::Advance()
{
	++iCurFrame;
	if (iCurFrame == frames.size())
	{
		iCurFrame = 0;
	}
}
