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

	void QueueAction(Action* action, const std::vector<Entity*> targets)
	{
		QueuedAction = action;
		QueuedTargets = targets;
		ActionIsQueued = true;
	}
	void StartAction(Action* action, const std::vector<Entity*> targets_in)
	{
		ActionIsQueued = false;
		tick = 0;
		current_action = action;
		targets = targets_in;
	}
	bool IsActionQueued() const
	{
		return ActionIsQueued;
	}
	Action* GetQueuedAction() const
	{
		return QueuedAction;
	}
	std::vector<Entity*> GetQueuedTargets() const
	{
		return QueuedTargets;
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
	Action* QueuedAction = nullptr;
	std::vector<Entity*> QueuedTargets;
	bool ActionIsQueued = false;
};