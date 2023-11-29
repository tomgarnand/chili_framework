#include "Entity.h"



std::vector<Drawable> Entity::GetDrawable(const std::string& map) const
{
	std::vector<Drawable> Drawables;
	Drawable d(src);

	Vec2 currentPos{};

	auto it = pos.find(current_map);
	if (it != pos.end())
	{
		currentPos = it->second;
	}

	RectI currentFrame;
	auto it2 = animation.find(actionToAnimate);
	if (it2 != animation.end())
	{
		if (it2->second.IsDirectional())
		{
			currentFrame = it2->second.GetSourceRect(dir);
		}
		else
		{
			currentFrame = it2->second.GetSourceRect();
		}
		if (it2->second.ContainsExtraAnimation())
		{
			Drawable e(actionToAnimate->GetSurface());
			e.AddSourceRect(it2->second.GetExtraAnimation()->GetSourceRect());
			e.ApplyTransformation(
				Mat3::Translation(currentPos.x - radius, currentPos.y - radius) *
				Mat3::Scale(scale) * //entity's scale
				Mat3::Rotation(angle)
			);
			d.AddExraDrawable(e);

		}
		d.AddSourceRect(currentFrame);
	}
	if (effectActive)
	{
		d.AddVisualEffect(VisualEffect::Red);
	}

	d.ApplyTransformation(
		Mat3::Translation(currentPos.x - radius, currentPos.y - radius) *
		Mat3::Scale(scale) *
		Mat3::Rotation(angle)
	);
	return d;
}

void Entity::Update(const World& world, float dt)
{
	//update active effects
	for (Effect& effect : statuses.getEffectsByCategory(EffectCategory::Active))
	{
		effect.SetDuration(effect.GetDuration() - 1);
		if (effect.GetDuration() == 0)
		{
			//there shouldn't ever be two of the same effecttypes
			statuses.RemoveEffect(EffectCategory::Active, effect.GetType());
		}
	}
}

void Entity::UpdateFromScript()
{
	StartAction(Action::Idle, {});
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
	if (SubTickEvent)
	{
		//for ea action in current_subtick_actionS
		
		DoApplication(current_action->GetApplicationByTick(tick), current_subtick_targets, stateStack);
		Resolve(world, dt);
	}
	else if (tick == -1)
	{
		
	}

	//if this is called at the end of a standard tick cycle
	else if (!SubTickEvent)
	{
		if (tick == current_action->GetMaxTicks())
		{
			//rethink this
			tick = -1;
			past_actions.emplace_back(current_action);
			current_action = Action::Idle;
		}
		else
		{
			//Resolve projectives. could be handled every subtick too. Might be more fluid
			for (auto& proj : ownedProjectiles)
			{
				if (proj->isExpired() && proj->GetTargetHit() != nullptr)
				{
					DoApplication(proj, { proj->GetTargetHit() }, stateStack);
				}
				ownedProjectiles.erase(std::remove_if(ownedProjectiles.begin(), ownedProjectiles.end(), [](const Projectile& proj) { return proj.isExpired(); }), ownedProjectiles.end());
			}
			DoApplication(current_action->GetApplicationByTick(tick), current_targets, stateStack);
			Resolve(world, dt);
			AdvanceTick();
		}

	}
}

void Entity::StartTick(std::vector<std::string>& stateStack)
{
	//re check criteria
	//if you are still able to take the action
	if (tick == -1)
	{

	}
	else if (current_action->CriteriaPassed(statuses)) // && current_action.CheckRange())  and range check?? 
	{
		
		if (current_action->GetApplicationByTick(tick) != Application::nullapp)
		{
			//Queue hit methods that require input before hit can be determined
			if (current_action->GetApplicationByTick(tick)->GetHitMethod()->returnAtTickEnd())
			{
				current_action->GetApplicationByTick(tick)->GetHitMethod()->InitiateCheck(stateStack);
			}
			
			//give projectile
			//we handle it next in a subtick update
			if (current_action->GetApplicationByTick(tick)->projectile.has_value())
			{
				//the menu call to this action determines the number of targets, so we dont worry about it here
				for (auto& tar : current_targets)
				{
					ownedProjectiles.emplace_back(current_action->GetApplicationByTick(tick)->projectile.value());
					ownedProjectiles.back()->FireProjectile(circle, tar->GetPos("testmap"));
				}
			}
		}
	}
	else
	{
		//cancel current move
		current_action = Action::Idle; //rethink this later, dont want to be applying idle when I want to be applying stun, if stun is applied as an action...
	}
}

void Entity::StartAction(Action* action, const std::vector<Entity*> targets_in)
{
	if (action->GetMaxTicks() == -1)
	{
		tick = -1;
	}
	else
	{
		tick = 0;
	}

	current_action = action;
	current_targets = targets_in;
}

bool Entity::IsActionEnded() 
{
	if (tick == current_action->GetMaxTicks())
	{
		return true;
	}
	return false;
}

void Entity::DoApplication(Effect effect, HitMethod* HitMethod, std::vector<Entity*> targets, std::vector<std::string>& stateStack)
{
	std::vector<Outcome> out; //(targets.size()); didnt work with emplace back
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
			Apply(effect, Outcome::Hit); //design choice, self apply on critical miss
			goto skip_apply;
		}
	}
	for (Entity*& target : targets)
	{
		target->Apply(effect, out[i]);
		i++;
	}

skip_apply: {} //because crit miss self applies
}

void Entity::DoApplication(Projectile* proj, std::vector<Entity*> targets, std::vector<std::string>& stateStack)
{
	DoApplication(proj->GetEffect(), proj->GetHitMethod(), targets, stateStack);
}

void Entity::DoApplication(Application* app, std::vector<Entity*> targets, std::vector<std::string>& stateStack)
{
	if (app != nullptr)
	{
		DoApplication(app->GetEffect(), app->GetHitMethod(), targets, stateStack);
	}
}

void Entity::Apply(const Application* app, const Outcome& out)
{
	Apply(app->GetEffect(), out);
}

void Entity::Apply(Effect effect, const Outcome& out)
{
	if (out == Outcome::CriticalHit)
	{
		effect.IncreaseEffectiveness(effect.GetEffectiveness() * static_cast<int>(out)); //*2
	}
	statuses.AddEffect(effect.GetCategory(), effect);
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
		tick = -1;
	}

}

void Entity::Resolve(const World& world, float dt)
{
	//shaping up to be a huge bloaty function that has all the functionality for how statuses effect an entity
	if (statuses.CheckForEffect(EffectType::Move))
	{
		vel = {
			(float)std::cos(dir) * statuses.getEffectByType(EffectType::Move).GetEffectiveness(),
			(float)std::sin(dir) * statuses.getEffectByType(EffectType::Move).GetEffectiveness()
		};
	}

	if (vel != Vec2(0.0f, 0.0f))
	{
		pos[current_map] = world.CheckAndAdjustMovement(pos[current_map], pos[current_map] + (vel * dt * speed), radius);
	}


	if (statuses.CheckForEffect(EffectType::Burn))
	{
		//deal damage
		effectActivate();
	}


	vel = Vec2(0.0f, 0.0f);
}

void Entity::SubTickUpdate(const World& world, float dt, std::vector<std::string>& stateStack)
{
	Action* action = Action::Idle;
	if (SubTickEvent)
	{
		tick = 0;
		//immediately apply the action effects to status, then resolve status
		EndTick(world, dt, stateStack);

		//then immediately remove them from statuses
		GetStatuses().RemoveSubTickEvents();


		action = current_subtick_action;

		//clear flags
		tick = -1;
		SubTickEvent = false;
		current_subtick_action = nullptr;

	}

	else
	{
		action = current_action;
	}

	//projectiles
	//this is where projectiles will either land or miss, or get updated
	for (auto& proj : ownedProjectiles)
	{
		Vec2 move = proj->AttemptMoveProjectile(dt);
		std::pair < bool, Entity* > hit = world.CheckCollision_And_ReturnEntity(proj->GetCircle(), move);
		//need to also return who is hit, need to make a new world function, address how we add Rects to coll_test
		if (hit.first)
		{ 
			proj->setExpired();
			proj->setTargetHit(hit.second);
			if (proj->hasSpecialHitMethod() && proj->GetHitMethod()->returnAtTickEnd());
			{
				proj->GetHitMethod()->InitiateCheck(stateStack);
			}
		}
		else
		{
			proj->MoveProjectile(move);
		}
	}

	actionToAnimate = action;
	//animation
	auto it = animation.find(actionToAnimate);
	if (it != animation.end()) {
		it->second.Update(dt);
		if (it->second.ContainsExtraAnimation())
		{
			it->second.GetExtraAnimation()->Update(dt);
		}
	}
	else //case for animation not found, so that whole spritesheet isnt drawn
	{
		actionToAnimate = Action::Idle;
		auto it = animation.find(actionToAnimate);
		if (it != animation.end()) {
			it->second.Update(dt);
		}
	}

	//animation effects
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

