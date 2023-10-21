#pragma once
#include <assert.h>
#include <unordered_map>
#include <string>
#include <map>
#include <random>
#include "Util.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <algorithm>

//after pretty much finishing this, a question popped in my mind: What if I had just used maps instead of a million vector iterating sub functions?
//but I'm not knowledgeable enough (yet) to know if what would be the better HitMethod

enum class Stat
{
	Charisma,     //social ability
	Constitution, //special resistance, hitpoints, mental & physical fortitude, endurance
	Strength,	  //physical strength
	Dexterity,	  //quickness, sleight of hand, agility. Ability to complete complicated tasks and avoid 
	Intelligence, //knowledge, aptitude
	Wisdom	  //environmental awareness
};

class Attributes
{
public:
	Attributes()
	{
		//default values
		stats[Stat::Charisma] = 10;
		stats[Stat::Constitution] = 10;
		stats[Stat::Strength] = 10;
		stats[Stat::Dexterity] = 10;
		stats[Stat::Intelligence] = 10;
		stats[Stat::Wisdom] = 10;
	}
	Attributes(int cha, int con, int str, int dex, int intel, int wis)
	{
		stats[Stat::Charisma] = cha;
		stats[Stat::Constitution] = con;
		stats[Stat::Strength] = str;
		stats[Stat::Dexterity] = dex;
		stats[Stat::Intelligence] = intel;
		stats[Stat::Wisdom] = wis;
	}

	int getStat(Stat statName) const {
		auto it = stats.find(statName);
		if (it != stats.end()) {
			return it->second;
		}
		// Return a default value (you can customize this)
		return 0;
	}

	void setStat(Stat statName, int value) {
		stats[statName] = value;
	}
public:
	std::unordered_map<Stat, int> stats;

};

enum class EffectType
{
	None, //place holder for a extra roll/check
	Physical,
	Burn,
	Stun,
	Silence,
	Disorient, //effects Concentration


	MoveRight,
	MoveLeft,
	MoveUp,
	MoveDown,
	StandLeft,
	StandRight,
	StandUp,
	StandDown,
	Idle

	//types
	//Burn, Stun, Poison - DoT, concentration effected (accuracy, ?), increased effect of other moves
	//Blind - accuracy effected, visual effect onscreen
	//Knockdown, Disabled - movement and/or actions restricted
	//Anchor - movement tied to location
};

enum class EffectCategory //Active/Passive/Subtick
{
	Active,
	Passive,
	SubTick
};

class Effect
{
public:
	Effect(const EffectCategory& cat, const EffectType& type, const int& duration, const float& effectiveness)
		:
		cat(cat),
		type(type),
		duration(duration),
		effectiveness(effectiveness)
	{}
	EffectType GetType() const { return type; }
	EffectCategory GetCategory() const { return cat; }
	int GetDuration() const { return duration; }
	float GetEffectiveness() const { return effectiveness; }
	void SetDuration(const int& newDuration) { duration = newDuration; }
	void IncreaseEffectiveness(const float& effectivenessIncrease) { effectiveness += effectivenessIncrease; }
private:
	EffectCategory cat;
	EffectType type;
	int duration; //0 duration represents an instant effect
	float effectiveness;
};



class Status //container for current effects
{
public:
	Status() = default;
	void AddEffect(const EffectCategory& cat, const Effect& effect);
	void RemoveEffect(const EffectType& check); //removes effect if it shares the same type

	const std::vector<Effect> getAllEffects() const;
	const std::vector<Effect> getEffectsByType(const EffectCategory& cat) const;

	bool CheckForEffect(const EffectType& check) const;
	void RemoveSubTickEvents();

private:
	std::map< EffectCategory, std::vector<Effect>> effects;
};

enum class Method
{
	None,
	DiceThrow,
	Guaranteed,
	QTE
};

	class HitMethod
	{
	public:
		HitMethod() = default;
	 
	 virtual Outcome CheckSuccess(const int& roll20, const int& bonus, const int& AC, const int& other) const { return Outcome::NotApplicable; }
	 virtual Outcome CheckSuccess() const { return Outcome::NotApplicable; }
	 virtual Outcome CheckSuccess(const std::vector<std::string>& stateStack) const { return Outcome::NotApplicable; }

	 virtual void InitiateCheck(std::vector<std::string>& stateStack) const {}

	 bool returnAtTickEnd() const { return returnAtTickEndFlag; }
	 const Method& GetMethod() { return method; }
protected:
	bool returnAtTickEndFlag = false;
	Method method = Method::None;
};
class DiceThrow : public HitMethod
{
public:
	DiceThrow(const Stat& bonus)
		:
		bonus(bonus)
	{
		method = Method::DiceThrow;
	}

	Outcome CheckSuccess(const int& roll20, const int& bonus, const int& AC, const int& others) const override //others is armor + shield + dex + other effects
	{
		if (roll20 == 20)
		{
			return Outcome::CriticalHit;
		}
		else if (roll20 == 1)
		{
			return Outcome::CriticalMiss;
		}
		else if ((roll20 + bonus) > (AC + others))
		{
			return Outcome::Hit;
		}
		else
		{
			return Outcome::Miss;
		}
	}
	const Stat& GetBonusStat() const { return bonus; }
private:
	Stat bonus;
};
class Guaranteed : public HitMethod
{
public:
	Guaranteed (const Outcome& outcome)
		:
		outcome(outcome)
	{
		method = Method::Guaranteed;
	}
	Outcome CheckSuccess() const
	{
		return outcome;
	}
private:
	Outcome outcome;
};
class QTE : public HitMethod
{
public:
	QTE(const int& difficulty, const std::string& action_name)
		:
		action_name(action_name),
		difficulty(difficulty)
	{
		method = Method::QTE;
		code = d99999.roll();
	}

	Outcome CheckSuccess(const std::vector<std::string>& stateStack) const override
	{
		//checks for a unique string in the statestack, if its not there it adds one to be replaced later and this returned to
		std::string commonStart = "QTE_" + std::to_string(difficulty) + "_" + std::to_string(code);

		for (const std::string& str : stateStack) {
			if (str.compare(0, commonStart.length(), commonStart) == 0) {
				
				if (str == commonStart + "_Hit") {
					return Outcome::Hit;
				}
				else if (str == commonStart + "_Miss") {
					return Outcome::Miss;
				}
				else if (str == commonStart + "_CriticalHit") {
					return Outcome::CriticalHit;
				}
				else if (str == commonStart + "_CriticalMiss") {
					return Outcome::CriticalMiss;
				}
				else {
					assert(true);
					return Outcome::NotApplicable;
				}
			}
		}
		
	}
	void InitiateCheck(std::vector<std::string>& stateStack) const override
	{
		stateStack.emplace_back("QTE" + std::to_string(difficulty));
	}
private:
	std::string action_name; //should get automatically written down the line
	int code; //code to determine uniqueness for stateStack strings. An unlikely disaster (hopefully minor) waiting to happen
	bool returnAtTickEndFlag = true;
	int difficulty; //1-10
};

class Criteria
{
public:
	Criteria() = default;
	Criteria(const std::vector<EffectType>& required, const std::vector<EffectType>& prohibited)
		:
		Required(required),
		Prohibited(prohibited)
	{
		if (!Required.empty())
		{
			requirements = true;
		}
		if (!Prohibited.empty())
		{
			prohibitions = true;
		}
	}

	bool HasRequirements() { return requirements; }
	bool HasProhibitions() { return prohibitions; }

	std::vector<EffectType>& GetRequired() { return Required; }
	std::vector<EffectType>& GetProhibited() { return Prohibited; }

private:
	bool requirements = false;
	bool prohibitions = false;
	std::vector<EffectType> Required = {};
	std::vector<EffectType> Prohibited = {};
	//for now, 
	//if a requirement is missing, hide the action in selection
	//if a prohibition is missing, cross out the action in selection and make it unselectable
};

class Application
{
public:
	
	Application(const Effect& effect)
		:
		hitMethod(new Guaranteed(Outcome::Hit))
	{
		effects.emplace_back(effect);
	}
	Application(const int& tick, const Effect& effect)
		:
		tick(tick),
		hitMethod(new Guaranteed(Outcome::Hit))
	{
		effects.emplace_back(effect);
	}
	Application(const int& tick, const std::vector<Effect>& effects)
		:
		tick(tick),
		effects(effects),
		hitMethod(new Guaranteed(Outcome::Hit))
	{}
	Application(const int& tick, const std::vector<Effect>& effects, HitMethod* hitMethod)
		:
		tick(tick),
		effects(effects),
		hitMethod(hitMethod)
	{}
	int GetTick() const { return tick; }
	std::vector<Effect> GetEffects() const { return effects; }
	HitMethod* GetHitMethod() const { return hitMethod; }
private:
	int tick = -1;
	std::vector<Effect> effects;
	HitMethod* hitMethod; //if I ever wanted to store multiple different hitmethods in an application, it might have to be a vec of unique ptrs
	
	
};

class Action
{
public:
	Action() = default;
	Action(std::string name_in)
	{
		name.clear();
		name = name_in;
	}
	Action(Application* application) //right now this is the only initilizer that checks for subtick, could expand
		:
		criteria(noCriteria)
	{
		ApplicationVector.clear();
		ApplicationVector.emplace_back(application);
		bool subTick_in = true;
		for (auto& effect : application->GetEffects())
		{
			if (effect.GetCategory() != EffectCategory::SubTick)
			{
				subTick_in = false;
				break;
			}
		}
		SubTick = subTick_in;

	}
	Action(std::string name_in, Application* application)
		:
		Action(application)
	{
		name.clear();
		name = name_in;
	}
	Action(const int& maxTicks, std::vector<Application*> ApplicationVector, const Criteria& criteria, const float& range)
		:
		ApplicationVector(ApplicationVector),
		maxTicks(maxTicks),
		criteria(criteria),
		range(range)
	{}
	Action(std::string name_in, const int& maxTicks, std::vector<Application*> ApplicationVector, const Criteria& criteria, const float& range)
		:
		Action(maxTicks, ApplicationVector, criteria, range)
	{
		name.clear();
		name = name_in;
	}
	int GetMaxTicks() const { return maxTicks; }

	Application* GetApplicationByTick(const int& tick) //could be multiple applications per tick
	{	return ApplicationVector[tick];	}
	bool CriteriaPassed(const Status& status); //returns true if criteria doesn't prevent action from occuring
	bool CheckRange(const float& dist) const { return (range > dist); }
	bool IsSubTickEvent() const { return SubTick; }
	std::string GetName() const { return name; }
	bool RequiresTargetSelection() { return false; }

private:
	int maxTicks = -1; //uninitialized value, aka lasts forever
	std::vector<Application*> ApplicationVector = { nullptr };
	Criteria criteria;
	float range = 0; //I wanted to put this in criteria, but criteria is only dealing with effects rn
	bool SubTick = false;
	static Criteria noCriteria;
	std::string name = "";

	//targets
	//self only
	//self or other(min_range, max_range)
	//other only (min_range, max_range)
	//AoE all (min_range, max_range)
	//AoE all less self (min_range, max_range)
	//AoE choice (min_range, max_range)
	//AoE area (X by Y) (min_range, max_range)
};


