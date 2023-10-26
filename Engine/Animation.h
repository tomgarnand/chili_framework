#pragma once
#include "Surface.h"
#include "Graphics.h"
#include <vector>
#include "Drawable.h"


class Animation
{
public:
	Animation() = default;
	Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime);
	void Update(float dt);
	RectI GetSourceRect() const;
	void AddExtraAnimation(Animation* extra_in) { extra = extra_in; }
	bool ContainsExtraAnimation() const;
	Animation* GetExtraAnimation() const { return extra; }
private:
	void Advance();
private:
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
	
	Animation* extra = nullptr;
};