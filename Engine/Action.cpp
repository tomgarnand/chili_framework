#include "Action.h"

Effect Effect::nulleff = Effect(EffectCategory::None, EffectType::None, 0, 0.0f);
Application* Application::nullapp = new Application(Effect::nulleff);
Action* Action::Idle = new Action("Idle", { -1, Application::nullapp });

void Status::AddEffect(const EffectCategory& cat, const Effect& effect)
{
	bool preexisting = false;
	for (auto& categoryPair : effects) {
		auto& effectVector = categoryPair.second;
		for (auto& e : effectVector)
		{
			if (e.GetType() == effect.GetType())
			{
				if (e.GetDuration() < effect.GetDuration())
				{
					e.SetDuration(effect.GetDuration());
					e.IncreaseEffectiveness(effect.GetEffectiveness()); //design choice... maybe not for the best
				}
				preexisting = true;
			}
		}
	}
	if (!preexisting)
	{
		effects[cat].emplace_back(effect);
	}
}

void Status::RemoveEffect(const EffectType& check)
{
	for (auto& categoryPair : effects) {
		auto& effectVector = categoryPair.second;
		effectVector.erase(std::remove_if(effectVector.begin(), effectVector.end(),
			[check](const Effect& effect)
			{
				return check == effect.GetType();
			}),
			effectVector.end());
	}
}

void Status::RemoveEffect(const EffectCategory& cat, const EffectType& check)
{
	std::vector<Effect> & effectVector = getEffectsByCategory(cat);
	{
		effectVector.erase(std::remove_if(effectVector.begin(), effectVector.end(),
			[check](const Effect& effect)
			{
				return check == effect.GetType();
			}),
			effectVector.end());
	}
}



std::vector<Effect>& Status::getEffectsByCategory(const EffectCategory& cat)
{
	// Find the effects for the given category
	auto it = effects.find(cat);
	// If the category is not present in the map, throw an exception or handle it in some other way
	if (it == effects.end()) 
	{
		static std::vector<Effect> emptyVector;
		return emptyVector;
	}
	// Return a reference to the vector of effects
	return it->second;
}

bool Status::CheckForEffect(const EffectType& check) const
{
	for (auto& categoryPair : effects) {
		auto& effectVector = categoryPair.second;
		for (auto& e : effectVector)
		{
			if (e.GetType() == check)
			{
				return true;
			}
		}
	}
	return false;
}

void Status::RemoveSubTickEvents()
{
	
	for (auto& categoryPair : effects) {
		auto& effectVector = categoryPair.second;
		effectVector.erase(std::remove_if(effectVector.begin(), effectVector.end(),
			[](const Effect& effect)
			{
				return EffectCategory::SubTick == effect.GetCategory();
			}),
			effectVector.end());
	}
}

Effect& Status::getEffectByType(const EffectType& cat)
{
	for (auto& categoryPair : effects) {
		auto& effectVector = categoryPair.second;
		for (auto& e : effectVector)
		{
			if (e.GetType() == cat)
			{
				return e;
			}
		}
	}
	return Effect::nulleff;
}



Criteria Action::noCriteria = Criteria();

bool Action::CriteriaPassed(const Status& status)
{
	bool failed = false;
	//looks through all prohibtions, failed = true if there is a effect that be present
	if (criteria.HasProhibitions())
	{
		for (auto& e_c : criteria.GetProhibited())
		{
			failed = status.CheckForEffect(e_c);
			if (failed)
			{
				break;
			}
		}
	}
	if (criteria.HasRequirements() && !failed)
	{
		for (auto& e_c : criteria.GetRequired())
		{
			failed = !status.CheckForEffect(e_c);
			if (failed)
			{
				break;
			}
		}
	}
	return !failed; //return true if nothing triggered a change in the bool
}
