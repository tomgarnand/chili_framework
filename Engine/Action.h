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
#include "Surface.h"

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
	Heal,

	Move,
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
	SubTick,
	None
};

class Effect
{
public:
	Effect(const EffectCategory& cat, const EffectType& type, int duration, float effectiveness)
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
	void AddAngle(float angle_in) { angle = angle_in; }
	float GetAngle() const { return angle; }
private:
	EffectCategory cat;
	EffectType type;
	int duration; //0 duration represents an instant effect
	float effectiveness;
	float angle;
public:
	static Effect nulleff;
};



class Status //container for current effects
{
public:
	Status() = default;
	void AddEffect(const EffectCategory& cat, const Effect& effect);
	void RemoveEffect(const EffectType& check); //removes effect if it shares the same type
	void RemoveEffect(const EffectCategory& cat, const EffectType& check);

	//create get effects and return the map
	std::vector<Effect>& getEffectsByCategory(const EffectCategory& cat);
	Effect& getEffectByType(const EffectType& check);
	bool CheckForEffect(const EffectType& check) const;
	void RemoveSubTickEvents();

private:
	std::map<EffectCategory, std::vector<Effect>> effects;
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
		effect(effect),
		hitMethod(new Guaranteed(Outcome::Hit))
	{}
	Application(const Effect& effect, HitMethod* hitMethod)
		:
		effect(effect),
		hitMethod(hitMethod)
	{}


	Effect GetEffect() const { return effect; }
	HitMethod* GetHitMethod() const { return hitMethod; }
private:
	Effect effect;
	HitMethod* hitMethod; //if I ever wanted to store multiple different hitmethods in an application, it might have to be a vec of unique ptrs
public:
	static Application* nullapp;
	
};

class Action
{
public:
	Action() = default;
	Action(std::string name_in, std::pair<int, Application*> Applications)
	{
		if (Applications.first == -1)
		{
			ApplicationVector = { Applications.second };
			SubTick = true;
		}
		else
		{
			std::vector<Application*> newVec(Applications.first, Application::nullapp);
			newVec[Applications.first] = Applications.second;
			ApplicationVector = newVec;
		}
		name.clear();
		name = name_in;
	}
	Action(std::string name_in, std::vector<std::pair<int, Application*>> Applications)
	{
		int largest = 0;
		for (auto& pair : Applications)
		{
			if (pair.first > largest)
			{
				largest = pair.first;
			}
		}
		std::vector<Application*> newVec(largest + 1, Application::nullapp);
		for (auto& pair : Applications)
		{
			newVec[pair.first] =  pair.second;
		}
		ApplicationVector = newVec;

		name.clear();
		name = name_in;
	}
	//Starts on tick 0
	Action(std::string name_in, int maxTicks, std::vector<std::pair<int, Application*>> Applications, const Criteria& criteria, const float& range)
		:
		maxTicks(maxTicks),
		criteria(criteria),
		range(range)
	{
		int largest = 0;
		for (auto& pair : Applications)
		{
			if (pair.first > largest)
			{
				largest = pair.first;
			}
		}
		std::vector<Application*> newVec(maxTicks + 1, Application::nullapp);
		for (auto& pair : Applications)
		{
			newVec[pair.first] = pair.second;
		}
		ApplicationVector = newVec;

		name.clear();
		name = name_in;
	}
	int GetMaxTicks() const { return maxTicks; }

	Application* GetApplicationByTick(const int& tick) //could be multiple applications per tick
	{
		return ApplicationVector[tick];
	}
	Application* GetSubTickApplication()
	{
		return ApplicationVector[0];
	}
	bool CriteriaPassed(const Status& status); //returns true if criteria doesn't prevent action from occuring
	bool CheckRange(const float& dist) const { return (range > dist); }
	bool IsSubTickEvent() const { return SubTick; }
	std::string GetName() const { return name; }
	bool RequiresTargetSelection() { return false; }
	void AddSurface(Surface& src_in) { src = &src_in; }
	Surface& GetSurface() { return *src; }
private:
	int maxTicks = -1; //uninitialized value, aka lasts forever
	std::vector<Application*> ApplicationVector;
	Criteria criteria;
	float range = 0; //I wanted to put this in criteria, but criteria is only dealing with effects rn
	bool SubTick = false;
	static Criteria noCriteria;
	std::string name;
	Surface* src = nullptr;
public:
	static Action* Idle;
	//targets
	//self only
	//self or other(min_range, max_range)
	//other only (min_range, max_range)
	//AoE all (min_range, max_range)
	//AoE all less self (min_range, max_range)
	//AoE choice (min_range, max_range)
	//AoE area (X by Y) (min_range, max_range)
};


