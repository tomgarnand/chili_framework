#pragma once
#include "Surface.h"
#include "Graphics.h"
#include <vector>
#include "Drawable.h"
#include <map>


class Animation
{
public:
	Animation() = default;
	Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime);
	Animation(std::vector<Animation> animations, std::vector<float> angle);
	void Update(float dt);
	RectI GetSourceRect() const;
	RectI GetSourceRect(float angle) const;
	void AddExtraAnimation(Animation* extra_in) { extra = extra_in; }
	bool ContainsExtraAnimation() const;
	Animation* GetExtraAnimation() const { return extra; }
	bool IsDirectional() const { return Directional; }
private:
	void Advance();
private:

	bool Directional = false;


	const Surface& sprite;
	std::vector<RectI> frames;
	std::map<float, std::vector<RectI>> directional_frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
	
	Animation* extra = nullptr;
};