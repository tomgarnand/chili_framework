#pragma once

#include "Entity.h"


class Player : public Entity
{
public:
	Player(Surface& src, Attributes stats, std::pair<std::string, Vec2> starting_pos)
		:
		Entity(src, stats, starting_pos)
	{
	
	}

	void QueueAction(const Action* action, const std::vector<Entity*> targets)
	{
		QueuedAction = action;
		QueuedTargets = targets;
	}
	const Action* GetQueuedAction() const
	{
		return QueuedAction;
	}
	void Update(const World& world, float dt)
	{
		if (vel != Vec2(0.0f, 0.0f))
		{
			pos[current_map] = world.CheckAndAdjustMovement(pos[current_map], pos[current_map] + (vel * dt), radius);		
		}
		auto it = animation.find(current_action);
		if (it != animation.end()) {
			it->second.Update(dt);
		}

		if (effectActive)
		{
			effectTime += dt;
			if (effectTime >= effectDuration)
			{
				effectTime = 0.0f;
				effectActive = false;
			}
		}
	}
	

private:
	const Action* QueuedAction = nullptr;
	std::vector<Entity*> QueuedTargets;
};