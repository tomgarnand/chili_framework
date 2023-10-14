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
	Entity( Surface& src ) //maybe it doesn't nec need a surface, if it can be initialized with tiled import
		:
		src( src ),
		current_action(Idle)
	{
		//defaults
		ArmorClass = 10; 
		Health = 10;
		tick = 0;

	}
	Entity(Surface& src, Attributes stats, std::pair<std::string, Vec2> starting_pos)
		:
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
	const Vec2& GetPos(const std::string& map) const
	{
		return pos.at(map);
	}
	void SetPos(const std::string& map, const Vec2& newPos )
	{
		pos[map] = newPos;
	}
	//void TranslateBy( const Vec2& offset ){pos += offset;}
	void SetScale( float s )
	{
		scale = s;
	}
	float GetScale() const
	{
		return scale;
	}
	void SetAngle( float a )
	{
		angle = a;
	}
	float GetAngle() const
	{
		return angle;
	}

	Drawable GetDrawable(const std::string& map) const
	{
		Drawable d( src );
		d.ApplyTransformation(
			Mat3::Translation( pos.at(map).x,pos.at(map).y ) *
			Mat3::Scale( scale ) *
			Mat3::Rotation( angle )
		);
		return d;
	}

	void Draw(Graphics& gfx) const;
	void Update(const World& world, float dt);
	void effectActivate();
	void AddAction(Action* action_in, Animation animation_in);

	int GetArmorClass() const { return ArmorClass; }
	const Attributes& GetStats() const { return stats; }

	void EndTick(std::vector<std::string>& stateStack);
	void StartTick(std::vector<std::string>& stateStack);
	void StartAction(Action* action, std::vector<Entity*> targets_in);
	bool IsActionEnded();
	void Apply(const Application& app, const Outcome& out);

	//void Update(std::string current_map, const Entity& player);

protected:
	float radius = 25.0f;

	float angle = 0.0f;
	float scale = 1.0f;

	std::string current_map;
	std::unordered_map<std::string, Vec2> pos;

	Vec2 vel = { 0.0f, 0.0f };
	float speed = 80.0f;

	Surface& src;
	std::unordered_map<Action*, Animation> animation; //player_animations[Slash] = Slash_Animation;
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;


	//Behaviour& script;
	Action* current_action = nullptr;
	std::vector<Entity*> targets = {};
	std::vector<Action*> past_actions = {};
	std::vector<Action*> action_pool = {};
	int tick;


	Attributes stats;
	Status statuses;
	float Health;
	int ArmorClass;

	//Trigger triggerType
	bool trigger_check = false;
	std::string scenario_keyword;
};