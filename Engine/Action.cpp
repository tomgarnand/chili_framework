#include "Action.h"


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

const std::vector<Effect> Status::getAllEffects() const
{
	std::vector<Effect> allEffects;

	for (const auto& pair : effects) { // Iterate over each key-value pair in the map
		const std::vector<Effect>& categoryEffects = pair.second;
		allEffects.insert(allEffects.end(), categoryEffects.begin(), categoryEffects.end());
	}

	return allEffects;
}

const std::vector<Effect> Status::getEffectsByType(const EffectCategory& cat) const
{
	static const std::vector<Effect> emptyVector;  // Static empty vector to return in case of error

	try {
		return effects.at(cat);
	}
	catch (const std::out_of_range& e) {
		assert(true);
		std::cerr << e.what() << "\nEffect type not found in the map!\n";
		return emptyVector;
	}
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
