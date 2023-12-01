#pragma once
#include "Vec2.h"
#include "Status.h"
#include "Circle.h"

class Entity; //forward declaration
class Application; //forward declaration

class Projectile
{
public:
	Projectile(Effect effect, const Animation& animation, float range, float speed, float radius)
		:
		effect(effect),
		animation(new Animation(animation)), //hopefully this default copy just copies the src reference
		speed(speed),
		range(range),
		circle({ {}, radius }) //uninitiallized pos
	{}
	~Projectile()
	{
		// Since the class has ownership of animation and extraHitMethod only
		// when they are created using 'new' inside the constructors, we need
		// to check if the class owns these resources before deleting them.

		// Delete the animation only if it was created with new inside the constructor.
		if (animation != nullptr)
		{
			delete animation;
			animation = nullptr; // Set to nullptr to avoid dangling pointer
		}
	}
	void InitParent(Application* app)
	{
		parent = app;
	}
	void FireProjectile(CircF firer, Vec2 target_pos) //there is a way to do this without squareroots, but does it matter?
	{
		float dist = circle.pos.GetDistance(target_pos);

		//normalized direction from firer to target, could have used angles
		dir = { (target_pos.x - firer.pos.x) / dist ,(target_pos.y - firer.pos.y) / dist };
		circle.pos = firer.pos + (dir * (circle.radius + firer.radius));
	}
	CircF GetCircle() const { return circle; }
	float GetRadius() const { return circle.radius; }
	Vec2 AttemptMoveProjectile(float dt)
	{
		//need direction from target pos
		if (circle.pos.GetDistanceSq(final_pos) < (dt * speed * speed))
		{
			return circle.pos + (dir * dt * speed);
		}
		return final_pos;
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


	Animation& GetAnimation() const
	{
		return *animation;
	}
private:
	Application* parent = nullptr;

	CircF circle;
	Vec2 final_pos;
	Vec2 dir;
	float speed;
	float range; //0 means instant land on target location

	Animation* animation;
	Effect effect;

	bool expired = false;
	Entity* target_hit = nullptr;


};