#pragma once
#include "Surface.h"
#include "Graphics.h"
#include <vector>
#include "Drawable.h"


class Animation
{
public:
	Animation() = default;
	Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime, Color chroma = Colors::Magenta);
	void Draw(const Vei2& pos, Graphics& gfx) const;
	void Draw(const Vei2& pos, Graphics& gfx, const RectI& clipRect) const;
	void DrawColor(const Vei2& pos, Color sub, Graphics& gfx) const;
	void DrawColor(const Vei2& pos, Color sub, Graphics& gfx, const RectI& clipRect) const;
	void Update(float dt);
	RectI GetSourceRect() const;
	
private:
	void Advance();
private:
	Color chroma;
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
	

};