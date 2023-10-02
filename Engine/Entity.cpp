#include "Entity.h"

void Entity::EndTick()
{
	if (tick == current_action.GetMaxTicks())
	{
		tick = -1;
		past_actions.emplace_back(current_action);
		current_action = Idle;
	}
	else
	{
		for (auto& apply : current_action.GetApplicationByTick(tick))
		{
			for (Entity& target : targets)
			{
				target.Apply(apply.first(), apply.second());
			}
		}
	}
}

void Entity::StartTick()
{
	tick++;
}

void Entity::StartAction(const Action& action, const std::vector<Entity&> targets_in)
{
	tick = 0;
	current_action = action;
	targets = targets_in;
}

bool Entity::IsActionEnded()
{
	if (tick == -1)
	{
		return true;
	}
	return false;
}
