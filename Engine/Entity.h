#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include "Vec2.h"
#include "Drawable.h"
#include "Action.h"
#include "Animation.h"
#include "World.h"



class Entity
{
public:
	Entity( std::string name, Surface& src ) //maybe it doesn't nec need a surface, if it can be initialized with tiled import
		:
		name(name),
		src( src ),
		current_action(Idle)
	{
		//defaults
		ArmorClass = 10; 
		Health = 10;
		tick = 0;

	}
	Entity(std::string name, Surface& src, Attributes stats, std::pair<std::string, Vec2> starting_pos)
		:
		name(name),
		src(src),
		current_action(Idle),
		stats(stats)
		
	{

		current_map = starting_pos.first;
		pos[current_map] = starting_pos.second;
		//defaults
		ArmorClass = 10;
		Health = 10;
		tick = 0;
	}
	const std::string& GetName() const { return name; }
	const Vec2& GetPos(const std::string& map) const{return pos.at(map);}
	void SetPos(const std::string& map, const Vec2& newPos ){pos[map] = newPos;}
	//void TranslateBy( const Vec2& offset ){pos += offset;}
	void SetScale( float s ){scale = s;}
	float GetScale() const{return scale;}
	void SetAngle( float a ){angle = a;}
	float GetAngle() const{return angle;}

	RectI GetHitBox() const
	{
		return RectI(Vec2(GetPos(current_map) - radius), Vec2(GetPos(current_map) + radius));
	}

	Drawable GetDrawable(const std::string& map) const;
	

	void Update(const World& world, float dt);
	void effectActivate();
	void AddAction(Action* action_in, Animation animation_in);
	

	int GetArmorClass() const { return ArmorClass; }
	const Attributes& GetStats() const { return stats; }
	Status& GetStatuses() { return statuses; }

	void EndTick(const World& world, float dt, std::vector<std::string>& stateStack);
	void StartTick(std::vector<std::string>& stateStack);
	void StartAction(Action* action, std::vector<Entity*> targets_in);
	void AdvanceTick() 
	{ 
		if (tick != -1 && tick != 0) 
		{ tick++; } 
	}
	bool IsActionEnded();

	void DoAction(Action* action, std::vector<Entity*> targets, std::vector<std::string>& stateStack);
	void Apply(const Application* app, const Outcome& out);
	void FlagSubTickEvent(Action* action, Entity* target);
	void FlagSubTickEvent(Action* action, std::vector<Entity*> targets);
	
	void Resolve(const World& world, float dt);

	Entity* Self()
	{
		return this; //is this dangerous?
	}

	//void Update(std::string current_map, const Entity& player);

protected:
	std::string name;

	float radius = 25.0f;

	float angle = 0.0f;
	float scale = 1.0f;

	std::string current_map;
	std::unordered_map<std::string, Vec2> pos;

	Vec2 vel = { 0.0f, 0.0f };
	float speed = 80.0f;

	Surface& src;
	std::unordered_map<Action*, Animation> animation; //player_animations[Slash] = Slash_Animation;
	Action* actionToAnimate = nullptr;
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;


	//Behaviour& script;
	Action* current_action = nullptr;
	Action* current_subtick_action = {};
	std::vector<Entity*> current_subtick_targets = {};
	std::vector<Entity*> current_targets = {};
	std::vector<Action*> past_actions = {};
	std::vector<Action*> action_pool = {};
	int tick = -1;
	bool SubTickEvent = false;

	Attributes stats;
	Status statuses;
	float Health;
	int ArmorClass;

	//Trigger triggerType
	bool trigger_check = false;
	std::string scenario_keyword;

public:
	static Action* Idle;
};