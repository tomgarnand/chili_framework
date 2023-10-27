#pragma once

#include "Entity.h"


class Player : public Entity
{
public:
	Player(std::string name, Surface& src, Attributes stats, std::pair<std::string, Vec2> starting_pos)
		:
		Entity(name, src, stats, starting_pos)
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
		if (action->GetMaxTicks() == -1)
		{
			tick = -1;
		}
		else
		{
			tick = 0;
		}
		ActionIsQueued = false;

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
	

private:
	Action* QueuedAction = nullptr;
	std::vector<Entity*> QueuedTargets;
	bool ActionIsQueued = false;
};