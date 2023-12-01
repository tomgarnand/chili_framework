#pragma once
#include "Surface.h"
#include "Graphics.h"
#include <vector>
#include <map>


class Animation
{
public:
	Animation() = default;
	Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime);
	Animation(std::vector<Animation> animations, std::vector<float> angle);
	void Update(float dt);
	RectI GetSourceRect() const;
	const RectI& GetSourceRectRef() const;
	RectI GetSourceRect(float angle) const;

	bool IsDirectional() const { return Directional; }
	const Surface& GetSource() const { return sprite; }
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
	
};