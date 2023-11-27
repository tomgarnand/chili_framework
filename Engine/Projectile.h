#pragma once
#include "Vec2.h"
#include "Animation.h"
#include "Status.h"

class Projectile
{
public:
	Projectile(Effect effect, Animation& animation, float speed)
		:
		effect(effect),
		animation(&animation)
	{
	}
	void InitProjectile(float range);
	void FireProjectile(Vec2 firing_pos, Vec2 target_pos);
	float GetRadius() const { return radius; }
	std::pair<Vec2, Vec2> AttemptMoveProjectile()
	{
		if (pos.GetDistanceSq(final_pos) < (speed * speed))
		{
			return { pos, pos + speed };
		}
		return { pos, final_pos };
	}
	void MoveProjectile(Vec2 result)
	{
		pos = result;
	}
private:
	Vec2 pos;
	Vec2 final_pos;
	float speed;
	float range;
	float radius;

	Animation* animation;
	Effect effect;
};