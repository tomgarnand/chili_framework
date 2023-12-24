#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include "Vec2.h"
#include "Drawable.h"
#include "Action.h"
#include "Animation.h"
#include <cmath>
#include <numbers>

#include "World_Holder.h"


class Entity
{
public:
	Entity( std::string name, Surface& src ) //maybe it doesn't nec need a surface, if it can be initialized with tiled import
		:
		name(name),
		src( src ),
		current_action(Action::Idle)
	{
		//defaults
		ArmorClass = 10; 
		Health = 10;
		tick = -1;



	}
	Entity(std::string name, Surface& src, Attributes stats, std::pair<std::string, Vec2> starting_pos)
		:
		name(name),
		src(src),
		current_action(Action::Idle),
		stats(stats)
		
	{

		current_map = starting_pos.first;
		pos[current_map] = starting_pos.second;
		//defaults
		ArmorClass = 10;
		Health = 10;
		tick = -1;

		float radius = 45.0f;
		circle = CircF(starting_pos.second, radius);
	}
	const std::string& GetName() const { return name; }
	const Vec2& GetPos(const std::string& map) const{return pos.at(map);}
	void SetPos(const std::string& map, const Vec2& newPos ){pos[map] = newPos;}
	//void TranslateBy( const Vec2& offset ){pos += offset;}

	void SetScale( float s ){scale = s;}
	float GetScale() const{return scale;}
	void SetAngle( float a ){angle = a;}
	float GetAngle() const{return angle;}
	void SetDir(float angle) //for keyboard inputs
	{
		if (angle < 0)
		{
			angle += (2.0f*std::numbers::pi); //atan2 conversion from range -pi<theta<pi to 0<theta<2pi
		}
		dir = angle;
	}
	void SetDir(Vec2 vec) //for mouse input, relative to player pos
	{

	}

	CircF GetCircle() const { return circle; }
	RectF GetRect() const { return rect; }

	std::vector<Drawable> GetDrawables(const std::string& map) const;
	//std::vector<Projectile*>& GetProjectiles() { return ownedProjectiles; }

	void Update(float dt);
	void UpdateFromScript();
	void effectActivate();
	void AddAction(Action* action_in, Animation animation_in);

	int GetArmorClass() const { return ArmorClass; }
	const Attributes& GetStats() const { return stats; }
	Status& GetStatuses() { return statuses; }

	void EndTick(float dt, std::vector<std::string>& stateStack);
	void StartTick(std::vector<std::string>& stateStack);
	void StartAction(Action* action, std::vector<Entity*> targets_in);
	void AdvanceTick() 
	{ 
		if (tick != -1) 
		{ tick++; } 
	}

	bool IsActionEnded();

	void DoApplication(const Effect& effect, HitMethod& HitMethod, std::vector<Entity*> targets, std::vector<std::string>& stateStack);
	void DoApplication(Projectile* proj, std::vector<Entity*> targets, std::vector<std::string>& stateStack);
	void DoApplication(Application* app, std::vector<Entity*> targets, std::vector<std::string>& stateStack);
	void Apply(const Application* app, const Outcome& out);
	void Apply(Effect effect, const Outcome& out);
	void FlagSubTickEvent(Action* action, Entity* target);
	void FlagSubTickEvent(Action* action, std::vector<Entity*> targets);
	
	void Resolve(float dt);

	void SubTickUpdate(float dt, std::vector<std::string>& stateStack);
	Entity* Self()
	{
		return this; //is this dangerous?
	}

	//void Update(std::string current_map, const Entity& player);

protected:
	WorldHolder* world;

	std::string name;

	CircF circle;
	RectF rect;
	//bool collision = true;

	float angle = 0.0f;
	float scale = 1.0f;

	std::string current_map;
	std::unordered_map<std::string, Vec2> pos;

	Vec2 vel = { 0.0f, 0.0f };
	float dir; //dir angle
	float speed = 80.0f;

	Surface& src;
	std::unordered_map<Action*, Animation> animation; //player_animations[Slash] = Slash_Animation;
	Action* actionToAnimate = nullptr;
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;

	std::vector<Projectile*> ownedProjectiles;


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

	
};