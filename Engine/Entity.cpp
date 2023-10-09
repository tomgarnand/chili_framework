#include "Entity.h"

void Entity::EndTick(std::vector<std::string>& stateStack)
{

	if (tick == current_action.GetMaxTicks())
	{
		tick = -1;
		past_actions.emplace_back(current_action);
		current_action = Idle;
	}
	
	else
	{
		HitMethod& HitMethod = current_action.GetApplicationByTick(tick).GetHitMethod();
		std::vector<Outcome> out(targets.size());
		Outcome i_out;
		switch (HitMethod.GetMethod())
		{
			case(Method::DiceThrow):
			{
				int roll = dice.Roll20();
				if (roll == 1) { out.emplace_back(Outcome::CriticalMiss); break; }
				for (Entity& target : targets)
				{

					i_out = dynamic_cast<DiceThrow&>(HitMethod).CheckSuccess(roll,
						stats.getStat(dynamic_cast<DiceThrow&>(HitMethod).GetBonusStat()),
						target.GetArmorClass(),
						target.GetStats().getStat(Stat::Dexterity)
					);
					out.emplace_back(i_out);
				}
			}
			case(Method::QTE):
			{
				for (Entity& target : targets)
				{

					i_out = dynamic_cast<QTE&>(HitMethod).CheckSuccess(stateStack);
					out.emplace_back(i_out);
				}
			}
		}
		int i = 0;
		
		for (auto& o : out)
		{
			if (o == Outcome::CriticalMiss)
			{
				Apply(current_action.GetApplicationByTick(tick), Outcome::Hit);
				goto skip_apply;
			}
		}
		for (Entity& target : targets)
		{
			target.Apply(current_action.GetApplicationByTick(tick), out[i]);
			i++;
		}
		skip_apply:
	}
}

void Entity::StartTick(std::vector<std::string>& stateStack)
{
	//re check criteria
	//if you are still able to take the action
	if (current_action.CheckCriteria(statuses) ) // && current_action.CheckRange())  and range check?? 
	{
		tick++;
		if (current_action.GetApplicationByTick(tick).GetHitMethod().returnAtTickEnd())
		{
			current_action.GetApplicationByTick(tick).GetHitMethod().InitiateCheck(stateStack);
		}
	}
	else
	{
		//cancel current move
		current_action = Idle; //rethink this later, dont want to be applying idle when I want to be applying stun, if stun is applied as an action...
	}
	
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

void Entity::Apply(const Application& app, const Outcome& out)
{
	for (auto& effect : app.GetEffects())
	{
		if (out == Outcome::CriticalHit)
		{
			effect.IncreaseEffectiveness(effect.GetEffectiveness() * static_cast<int>(out));
		}
		statuses.AddActiveEffect(effect);

		//queue visual/sound effects
	}
}

