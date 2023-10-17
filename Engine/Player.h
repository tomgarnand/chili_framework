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
	void QueueAction(Action* action, Entity* target)
	{
		QueueAction(action, std::vector<Entity*>{ target });
	}
	void QueueAction(Action* action, std::vector<Entity*> targets)
	{
		//if action is subtick action, toggle subtick applier, disregard rest

		if (action->IsSubTickEvent())
		{
			FlagSubTickEvent(action, targets);
		}
		else
		{
			QueuedAction = action;
			QueuedTargets = targets;
			ActionIsQueued = true;
		}
	}
	
	void StartAction(Action* action, std::vector<Entity*> targets_in)
	{
		ActionIsQueued = false;
		tick = 0;
		current_action = action;
		current_targets = targets_in;

		QueuedAction = nullptr;
		QueuedTargets.clear();
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
	void SubTickUpdate(const World& world, float dt, std::vector<std::string>& stateStack)
	{
		Action* action = Idle;
		if (SubTickEvent)
		{
			//immediately apply the action effects to status, then resolve status
			EndTick(world, dt, stateStack);

			//then immediately remove them from statuses
			GetStatuses().RemoveSubTickEvents();
			

			action = current_subtick_action;

			//clear flags
			SubTickEvent = false;
			current_subtick_action = nullptr;
			
		}

		else
		{
			action = current_action;
		}
		
		actionToAnimate = action;
		//animation
		auto it = animation.find(actionToAnimate);
		if (it != animation.end()) {
			it->second.Update(dt);
		}
		
		//animation effects - currently not used
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