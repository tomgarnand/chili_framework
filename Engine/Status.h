#pragma once
#include <vector>
#include <map>

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
	//void AddAngle(float angle_in) { angle = angle_in; }
	//float GetAngle() const { return angle; }
private:
	EffectCategory cat;
	EffectType type;
	int duration; //0 duration represents an instant effect
	float effectiveness;
	//float angle;
	Animation* splash_effect = nullptr;
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