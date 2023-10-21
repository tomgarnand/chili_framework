#include "Entity.h"

Action* Entity::Idle = new Action("Idle");

Drawable Entity::GetDrawable(const std::string& map) const
{
	{
		Drawable d(src);

		Vec2 currentPos;
		auto it = pos.find(current_map);
		if (it != pos.end())
		{
			currentPos = it->second;
		}
		RectI currentFrame;
		
		auto it2 = animation.find(actionToAnimate);
		if (it2 != animation.end())
		{
			currentFrame = it2->second.GetSourceRect();
			d.AddSourceRect(currentFrame);
		}

		d.ApplyTransformation(
			Mat3::Translation(currentPos.x, currentPos.y) *
			Mat3::Scale(scale) *
			Mat3::Rotation(angle)
		);
		return d;
	}
}

void Entity::Update(const World& world, float dt)
{
	//run behaviour script
}



void Entity::effectActivate()
{
	effectActive = true;
	effectTime = 0.0f;
}

void Entity::AddAction(Action* action_in, Animation animation_in)
{
	action_pool.emplace_back(action_in);
	animation.emplace(action_in, animation_in);

}

void Entity::EndTick(const World& world, float dt, std::vector<std::string>& stateStack)
{
	//if this is a subtick
	if (SubTickEvent)
	{
		//for ea action in current_subtick_actionS
		DoAction(current_subtick_action, current_subtick_targets, stateStack);
		Resolve(world, dt);
	}

	//if this is called at the end of a standard tick cycle
	else if (!SubTickEvent)
	{
		if (tick == current_action->GetMaxTicks())
		{
			//rethink this
			tick = -1;
			past_actions.emplace_back(current_action);
			current_action = Idle;
		}
		else
		{
			DoAction(current_action, current_targets, stateStack);
			Resolve(world, dt);
			
		}
	}
	
	

}

void Entity::StartTick(std::vector<std::string>& stateStack)
{
	//re check criteria
	//if you are still able to take the action
	if (current_action->CriteriaPassed(statuses)) // && current_action.CheckRange())  and range check?? 
	{
		AdvanceTick();
		if (current_action->GetApplicationByTick(tick) != nullptr)
		{
			//Queue hit methods that require input before hit can be determined
			if (current_action->GetApplicationByTick(tick)->GetHitMethod()->returnAtTickEnd())
			{
				current_action->GetApplicationByTick(tick)->GetHitMethod()->InitiateCheck(stateStack);
			}
		}
	}
	else
	{
		//cancel current move
		current_action = Idle; //rethink this later, dont want to be applying idle when I want to be applying stun, if stun is applied as an action...
	}
}

void Entity::StartAction(Action* action, const std::vector<Entity*> targets_in)
{
	tick = 0;
	current_action = action;
	current_targets = targets_in;
}

bool Entity::IsActionEnded()
{
	if (tick == 0 || tick == -1)
	{
		return true;
	}
	return false;
}

void Entity::DoAction(Action* action, std::vector<Entity*> targets, std::vector<std::string>& stateStack)
{
	//need behavior if there is no hitmethod or even application
	if (action->GetApplicationByTick(tick) != nullptr)
	{
		HitMethod* HitMethod = action->GetApplicationByTick(tick)->GetHitMethod();
		std::vector<Outcome> out(targets.size());
		Outcome i_out;
		switch (HitMethod->GetMethod())
		{
		case(Method::DiceThrow):
		{

			int roll = d20.roll();
			if (roll == 1)
			{
				out.emplace_back(Outcome::CriticalMiss);
				break;
			}
			else
			{
				for (Entity*& target : targets)
				{
					DiceThrow* DiceThrowPtr = dynamic_cast<DiceThrow*>(HitMethod);
					if (DiceThrowPtr) {
						i_out = DiceThrowPtr->CheckSuccess(roll,
							stats.getStat(DiceThrowPtr->GetBonusStat()),
							target->GetArmorClass(),
							target->GetStats().getStat(Stat::Dexterity));
						out.emplace_back(i_out);
					}
					else {
						// Handle the case where the cast fails (not a Guaranteed object)
					}

					out.emplace_back(i_out);
				}
			}
			break;
		}
		case(Method::QTE):
		{
			for (Entity*& target : targets)
			{
				QTE* QTEPtr = dynamic_cast<QTE*>(HitMethod);
				if (QTEPtr) {
					i_out = QTEPtr->CheckSuccess(stateStack);
					out.emplace_back(i_out);
				}
				else {
					// Handle the case where the cast fails (not a Guaranteed object)
				}
			}
			break;
		}
		case(Method::Guaranteed):
			for (Entity*& target : targets)
			{
				Guaranteed* guaranteedPtr = dynamic_cast<Guaranteed*>(HitMethod);
				if (guaranteedPtr) {
					i_out = guaranteedPtr->CheckSuccess();
					out.emplace_back(i_out);
				}
				else {
					// Handle the case where the cast fails (not a Guaranteed object)
				}
			}
			break;
		}
		int i = 0;

		for (auto& o : out)
		{
			if (o == Outcome::CriticalMiss)
			{
				Apply(action->GetApplicationByTick(tick), Outcome::Hit); //design choice, self apply on critical miss
				goto skip_apply;
			}
		}
		for (Entity*& target : targets)
		{
			target->Apply(action->GetApplicationByTick(tick), out[i]);
			i++;
		}
	}
skip_apply: {} //because crit miss self applies

}

void Entity::Apply(const Application* app, const Outcome& out)
{
	for (auto& effect : app->GetEffects())
	{
		if (out == Outcome::CriticalHit)
		{
			effect.IncreaseEffectiveness(effect.GetEffectiveness() * static_cast<int>(out)); //*2
		}
		statuses.AddEffect(effect.GetCategory(), effect);

		//queue visual/sound effects
	}
}

void Entity::FlagSubTickEvent(Action* action, Entity* target)
{
	FlagSubTickEvent(action, std::vector<Entity*>{ target });
}

void Entity::FlagSubTickEvent(Action* action, std::vector<Entity*> targets)
{
	SubTickEvent = true;
	if (current_subtick_action != nullptr)
	{
		assert(true);
		//need to add something for if multiple actions get queued into the same subtick
	}
	else
	{
		current_subtick_action = action;
		current_subtick_targets = targets;
	}

}

void Entity::Resolve(const World& world, float dt)
{
	//shaping up to be a huge bloaty function that has all the functionality for how statuses effect an entity


	if (statuses.CheckForEffect(EffectType::MoveRight))
	{
		vel = Vec2(1.0f, 0.0f);
	}
	if (statuses.CheckForEffect(EffectType::MoveLeft))
	{
		vel = Vec2(-1.0f, 0.0f);
	}
	if (statuses.CheckForEffect(EffectType::MoveUp))
	{
		vel = Vec2(0.0f, -1.0f);
	}
	if (statuses.CheckForEffect(EffectType::MoveDown))
	{
		vel = Vec2(0.0f, 1.0f);
	}
	if (vel != Vec2(0.0f, 0.0f))
	{
		pos[current_map] = world.CheckAndAdjustMovement(pos[current_map], pos[current_map] + (vel * dt * speed), radius);
	}


	vel = Vec2(0.0f, 0.0f);
}

