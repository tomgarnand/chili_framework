#pragma once

#include "Entity.h"


class Player : public Entity
{
public:
	Player() = default;

	void QueueAction(const Action& action, const std::vector<Entity&> targets)
	{
		QueuedAction = action;
		QueuedTargets = targets;
	}
	Action& GetQueuedAction() const
	{
		return QueuedAction;
	}


private:
	Action& QueuedAction;
	std::vector<Entity&> QueuedTargets;
};