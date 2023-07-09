#pragma once

#include "Animation.h"
#include "Vec2.h"
#include "World.h"

class Character
{
private:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};
public:
	Character(const Vec2& pos);
	void Draw(Graphics& gfx) const;
	Vec2 GetPos() const { return pos; }
	void SetDirection(const Vec2& dir);
	void Update(const World& world, float dt);
	void effectActivate();
	Surface GetSurface() const;
private:
	Surface sprite;
	Vec2 pos;
	Vec2 vel = { 0.0f, 0.0f };
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingDown;
	float speed = 80.0f;

	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;

	float radius = 25.0f;
};
