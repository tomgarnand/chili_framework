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


#include "Attributes.h"
#include "Projectile.h"
#include "Status.h"
#include <optional>
#include <variant>

//after pretty much finishing this, a question popped in my mind: What if I had just used maps instead of a million vector iterating sub functions?
//but I'm not knowledgeable enough (yet) to know if what would be the better HitMethod


enum class Method
{
	None,
	DiceThrow,
	Guaranteed,
	QTE,
	HitBox
};

class HitMethod
{
public:		
	HitMethod() = default;
 
	virtual Outcome CheckSuccess(const int& roll20, const int& bonus, const int& AC, const int& other) const { return Outcome::NotApplicable; }
	virtual Outcome CheckSuccess() const { return Outcome::NotApplicable; }
	virtual Outcome CheckSuccess(const std::vector<std::string>& stateStack) const { return Outcome::NotApplicable; }
	virtual Outcome CheckSuccess(const Vec2& entity_pos) { return Outcome::NotApplicable; }

	virtual std::unique_ptr<HitMethod> clone() const { return std::make_unique<HitMethod>(*this); };

	virtual void InitiateCheck(std::vector<std::string>& stateStack) const {}

	bool returnAtTickEnd() const { return returnAtTickEndFlag; }
	const Method& GetMethod() { return method; }
	void AddExtraHitMethod(const HitMethod& extra)
	{
		
	}
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
	std::unique_ptr<HitMethod> clone() const override 
	{
		return std::make_unique<DiceThrow>(*this);
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
	std::unique_ptr<HitMethod> clone() const override {
		return std::make_unique<Guaranteed>(*this);
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
	std::unique_ptr<HitMethod> clone() const override 
	{
		return std::make_unique<QTE>(*this);
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
class HitBox : public HitMethod
{
public:
	HitBox()
	{
		method = Method::HitBox;
	}
	// Custom copy constructor - commented out for variant solution
	//HitBox(const HitBox& other) {
	//	if (other.postHitMethod) {
	//		postHitMethod = other.postHitMethod->clone();
	//	}
	//}
	Outcome CheckSuccess(const Vec2& entity_pos)
	{
		return {};
	}
	std::unique_ptr<HitMethod> clone() const override //shouldnt be used
	{
		return std::make_unique<HitBox>(*this);
	}
	void AddExtraHitMethod(const HitMethod& extra)
	{
		postHitMethod = extra.clone();
	}
private:
	std::unique_ptr<HitMethod> postHitMethod = nullptr;
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
		hitMethod(Guaranteed(Outcome::Hit))
	{}
	Application(const Effect& effect, const HitMethod& hitMethod)
		:
		effect(effect),
		hitMethod(hitMethod)
	{}
#pragma region projectile
	Application(const Effect& effect, Projectile* proj, bool homing)
		:
		effect(Effect::nulleff),
		projectile(*proj)
	{
		//its either homing or hitbox
		//nothing fancy
		if (homing)
		{
			hitMethod = Guaranteed(Outcome::Hit);
		}
		else
		{
			hitMethod = HitBox();
		}
		projectile.value().InitParent(this);
	}
	Application(const Effect& effect, Projectile* proj, bool homing, const HitMethod& hitMethod_in)
		:
		effect(Effect::nulleff),
		projectile(*proj)
	{
		//its either homing with a hitmethod or hotbox with a extra hitmethod
		if (homing)
		{
			hitMethod = hitMethod_in;
		}
		else
		{
			hitMethod = HitBox();
			//hitMethod.AddExtraHitMethod(hitMethod_in);
			std::visit([&hitMethod_in](auto& obj) { obj.AddExtraHitMethod(hitMethod_in); }, hitMethod);
		}
		projectile.value().InitParent(this);
	}
public:
	//not sure how i can access this without making it public... haha be safe out there!!
	std::optional<Projectile> projectile; //1 proj per app cuz you can have multiple apps on the same tick
#pragma endregion
	Effect GetEffect() const { return effect; }
	HitMethod& GetHitMethod() { return hitMethod; }
private:
	Effect effect;
	std::variant<HitMethod, DiceThrow, Guaranteed, QTE, HitBox> hitMethod; //if I ever wanted to store multiple different hitmethods in an application, it might have to be a vec of unique ptrs
	//instead of an effect, a application can store a proj with an effect in it

public:
	static Application* nullapp;
};



class Action
{
public:
	Action() = default;
	Action(std::string name_in, std::pair<int, Application*> Applications) //single app
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
	float GetRange() const { return range; }
private:
	int maxTicks = -1; //uninitialized value, aka lasts forever
	std::vector<Application*> ApplicationVector;
	Criteria criteria;
	//targeting range
	float range = 0; //I wanted to put this in criteria, but criteria is only dealing with effects rn
	bool SubTick = false;
	static Criteria noCriteria;
	std::string name;
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


