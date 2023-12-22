#pragma once
#include <vector>
#include <unordered_map>
#include "Rect.h"
#include "Circle.h"

class Entity; //forward declare for holding pointer

class WorldHolder
{
public:
	WorldHolder() = default;
	void InitFixtures(std::vector<RectF>* fixtures_in)
	{
		fixtures = fixtures_in;
	}
	void AddEntity(Entity* entity, CircF* circle)
	{
		m_entity[circle] = entity;
	}
	void RemoveEntity(Entity* entity, CircF* circle)
	{
		auto it = m_entity.find(circle);
		if (it != m_entity.end())
		{
			m_entity.erase(it);
		}
		else
		{
			//yikers
		}
	}
	std::vector < std::pair<Entity*, Vec2> > CheckCollision_Entities(const Vec2& origin, const Vec2& move) const
	{

		LineF movement = LineF(origin, move);
		std::pair<bool, Vec2> pair_return;
		std::vector < std::pair<Entity*, Vec2> > intercepts;

		//for (auto& entity : *entities)
		for (auto& circle : m_entity)
		{
			if (movement.GetNearbyLinesByDistance(circle.first->GetCenter(), circle.first->GetRadius())) //if line is within a reasonable dist to circle
			{
				std::pair<bool, Vec2> pair = movement.ClosestIntercept(*circle.first);
				if (pair.first)
				{
					intercepts.emplace_back(circle.second, pair.second);
				}
			}
		}

	}
	std::vector<Vec2> CheckCollision_Fixtures(const Vec2& origin, const Vec2& move) const
	{
		std::vector<Vec2> intercepts;
		std::pair<bool, Vec2> pair_return;
		LineF movement = LineF(origin, move);

		for (auto& rect : *fixtures)
		{
			if (rect.lBottom.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				pair_return = rect.lBottom.Intercept(movement);
				if (pair_return.first == true)
				{
					intercepts.emplace_back(pair_return.second);
				}
			}
			if (rect.lTop.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				pair_return = rect.lTop.Intercept(movement);
				if (pair_return.first == true)
				{
					intercepts.emplace_back(pair_return.second);
				}
			}
			if (rect.lLeft.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				pair_return = rect.lLeft.Intercept(movement);
				if (pair_return.first == true)
				{
					intercepts.emplace_back(pair_return.second);
				}
			}
			if (rect.lRight.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				pair_return = rect.lRight.Intercept(movement);
				if (pair_return.first == true)
				{
					intercepts.emplace_back(pair_return.second);
				}
			}
		}


	}
	std::pair<bool, Entity*> CheckCollision_And_ReturnEntity(const CircF& circle, const Vec2& move) const
	{
		Vec2 origin = circle.GetCenter();

		//calculate move + radius
		float radius = circle.GetRadius();
		LineF movement = LineF(origin, move);
		Vec2 Dir = { movement.Q.x - movement.P.x, movement.Q.y - movement.P.y };
		float length = movement.GetLength();
		Vec2 NormalizedDir = { Dir.x / length, Dir.y / length };
		Vec2 Q_star = { movement.Q.x + radius * NormalizedDir.x, movement.Q.y + radius * NormalizedDir.y };


		std::vector < std::pair<Entity*, Vec2> > intercepts_entities = CheckCollision_Entities(origin, Q_star);

		std::vector<Vec2> intercepts = CheckCollision_Fixtures(origin, Q_star);
		for (auto& vec : intercepts)
		{
			intercepts_entities.emplace_back(nullptr, vec);
		}
		if (intercepts.empty())
		{

			return { false, nullptr };
		}

		std::pair<Entity*, Vec2> first_collision;
		float first_collision_dist;

		first_collision = intercepts_entities[0]; //init
		first_collision_dist = origin.GetDistanceSq(first_collision.second);
		for (auto& pair : intercepts_entities)
		{
			if (origin.GetDistanceSq(pair.second) < first_collision_dist)
			{
				first_collision_dist = origin.GetDistanceSq(pair.second);
				first_collision = pair;
			}
		}
		return { true, first_collision.first };
	}


	Vec2 CheckAndAdjustMovement(const CircF& circle, const Vec2& move) const
	{
		Vec2 origin = circle.GetCenter();

		//calculate move + radius
		float radius = circle.GetRadius();
		LineF movement = LineF(origin, move);
		Vec2 Dir = { movement.Q.x - movement.P.x, movement.Q.y - movement.P.y };
		float length = movement.GetLength();
		Vec2 NormalizedDir = { Dir.x / length, Dir.y / length };
		Vec2 Q_star = { movement.Q.x + radius * NormalizedDir.x, movement.Q.y + radius * NormalizedDir.y };

		std::vector<Vec2> intercepts = CheckCollision_Fixtures(origin, Q_star);
		std::vector < std::pair<Entity*, Vec2> > intercepts_entities = CheckCollision_Entities(origin, Q_star);
		for (auto& pair : intercepts_entities)
		{
			intercepts.emplace_back(pair.second);
		}

		if (intercepts.empty())
		{
			return move;
		}

		Vec2 shortest_move;
		float shortest_move_dist;

		shortest_move = intercepts[0]; //init
		shortest_move_dist = origin.GetDistanceSq(shortest_move);
		for (auto& intercept : intercepts)
		{
			if (origin.GetDistanceSq(intercept) < shortest_move_dist)
			{
				shortest_move_dist = origin.GetDistanceSq(intercept);
				shortest_move = intercept;
			}
		}

		//taking away the radius we added to the collision checks earlier
		return { shortest_move.x - radius * NormalizedDir.x, shortest_move.y - radius * NormalizedDir.y };
	}
private:
	std::vector<RectF>* fixtures;
	//std::vector<Entity*>* entities;
	std::unordered_map<CircF*, Entity*> m_entity;

};