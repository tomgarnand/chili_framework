#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include "Vec2.h"
#include "Drawable.h"
#include "Action.h"
#include "Animation.h"
#include "Action.h"

class Entity
{
public:
	Entity( Surface& src ,const Vec2& pos = { 0.0f,0.0f })
		:
		pos( pos ),
		src( src ),
		current_action(Idle)
	{
		 
	}
	const Vec2& GetPos() const
	{
		return pos;
	}
	void SetPos( const Vec2& newPos )
	{
		pos = newPos;
	}
	void TranslateBy( const Vec2& offset )
	{
		pos += offset;
	}
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

	Drawable GetDrawable() const
	{
		Drawable d( src );
		d.ApplyTransformation(
			Mat3::Translation( pos.x,pos.y ) *
			Mat3::Scale( scale ) *
			Mat3::Rotation( angle )
		);
		return d;
	}

	void EndTick();
	void StartTick();
	void StartAction(const Action& action, const std::vector<Entity&> targets_in);
	bool IsActionEnded();
	void Apply(ApplicationType type, int value);

	void Update(const Map& current_map, const Entity& player);

private:
	float angle = 0.0f;
	float scale = 1.0f;
	Vec2 pos = {0.0f,0.0f}; //convert to a map point vector
	Surface& src;


	//Behaviour& script;
	Action& current_action;
	std::vector<Entity&> targets;
	std::vector<Action&> past_actions;
	std::vector<Action&> action_pool;
	int tick;



	std::unordered_map<Action&, Animation&> animations; //player_animations[Slash] = Slash_Animation;
	float Health;

	//Trigger triggerType
	bool trigger_check;
	std::string scenario_keyword;
};