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

Animation::Animation(std::vector<Animation> animations, std::vector<float> angle)
	:
	sprite(animations.front().sprite),
	holdTime(animations.front().holdTime),
	Directional(true)
{
	int i = 0;
	for (Animation& a : animations)
	{
		directional_frames.emplace(angle[i], a.frames);
		i++;
	}
	//NOTE: we use the same frametimer for this type of animation
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

RectI Animation::GetSourceRect(float angle) const
{
	auto it = directional_frames.lower_bound(angle);

	// If it's the first element, then it's the closest
	if (it == directional_frames.begin()) {
		return it->second[iCurFrame];
	}

	// If it's past the last element, the last element is the closest
	if (it == directional_frames.end()) {
		return std::prev(it)->second[iCurFrame];
	}

	// Check the previous key to see if it's closer
	double prevKey = std::prev(it)->first;
	double currentKey = it->first;

	if (std::abs(angle - prevKey) < std::abs(angle - currentKey)) {
		return std::prev(it)->second[iCurFrame];
	}
	else {
		return it->second[iCurFrame];
	}
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
