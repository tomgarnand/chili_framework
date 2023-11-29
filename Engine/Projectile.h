#pragma once
#include "Vec2.h"
#include "Animation.h"
#include "Status.h"
#include "Circle.h"



class Entity; //forward declaration
class HitMethod;


class Projectile
{
public:
	Projectile(Effect effect, Animation& animation, float speed)
		:
		effect(effect),
		animation(&animation),
		extraHitMethod(new Guaranteed(Outcome::Hit))
	{
	}
	void InitProjectile(float range_in)
	{
		range = range_in;
	}
	void FireProjectile(CircF firer, Vec2 target_pos) //there is a way to do this without squareroots, but does it matter?
	{
		float dist = circle.pos.GetDistance(target_pos);

		//direction from firer to target
		dir = { (target_pos.x - firer.pos.x) / dist ,(target_pos.y - firer.pos.y) / dist };
		circle.pos = firer.pos + (dir * (circle.radius + firer.radius));

	}
	CircF GetCircle() const { return circle; }
	float GetRadius() const { return circle.radius; }
	Vec2 AttemptMoveProjectile(float dt)
	{
		//need direction from target pos
		if (circle.pos.GetDistanceSq(final_pos) < (speed * speed))
		{
			return circle.pos + (dir * dt * speed);
		}
		return final_pos ;
	}
	void MoveProjectile(Vec2 result)
	{
		circle.pos = result;
	}
	bool isExpired() const { return expired; }
	void setExpired() { expired = true; }
	void setTargetHit(Entity* hit) { target_hit = hit; }
	Entity* GetTargetHit() const { return target_hit; }
	Effect GetEffect() const { return effect; }
	bool hasSpecialHitMethod() const { return has_special_hit_method; }
	HitMethod* GetHitMethod() const { return extraHitMethod; }
private:

	CircF circle;
	Vec2 final_pos;
	Vec2 dir;
	float speed;
	float range;


	Animation* animation;
	Effect effect;

	HitMethod* extraHitMethod; //if proj hits then its guaranteed

	bool expired = false;
	Entity* target_hit = nullptr;

	bool has_special_hit_method = false;
};