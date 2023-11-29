#pragma once
#include "Surface.h"
#include "SpriteEffect.h"
#include <set>
#include "Line.h"
#include "TMXLoader/TMXLoader.h"
#include "Entity.h"

class World
{
public:
	class Level
	{
	public:
		class Fixtures
		{};
		Level() = default;
	private:
		std::vector<Fixtures> fixtures;

	};


public:
	World()
		:
		spritesheet("Assets\\spritesheet.bmp")
	{
		loader = new TMXLoader();
		loader->loadMap("testmap", "Assets\\testingformats.tmx");
		loader->printMapData("testmap");

		int sheetWidth = spritesheet.GetWidth();
		int sheetHeight = spritesheet.GetHeight();
		int tileWidth = loader->getMap("testmap")->getTileWidth();
		int tileHeight = loader->getMap("testmap")->getTileHeight();

		//InitCollTest();
		//replacing this with a test rects
		//fixtures.emplace_back(RectF({ 400,200 }, { 600, 300 }));
		//fixtures.emplace_back(RectF({ 200,440 }, { 250, 460 }));


		for (int row = 0; row < sheetHeight / tileHeight; row++) //TODO: create loader->getMap("testmap")->getTileSet("defaulttileset")->getNumTiles()
		{
			for (int col = 0; col < sheetWidth / tileWidth; col++)
			{
				tiles.emplace_back(row * tileHeight, (row * tileHeight) + tileHeight, col * tileWidth, (col * tileWidth) + tileWidth);
			}
		}
	}
	void InitEntities(Entity* entities)
	{

	}
	void DrawRects(Graphics& gfx)
	{
		for (auto& rect : fixtures)
		{
			gfx.DrawRect(rect, Colors::Blue);
		}
	}

	void DrawWorld(Graphics& gfx)
	{
		char tileID = 0;

		int tileWidth = loader->getMap("testmap")->getTileWidth();
		int tileHeight = loader->getMap("testmap")->getTileHeight();

		for (unsigned int j = 0; j < loader->getMap("testmap")->getHeight(); ++j)
		{

			for (unsigned int i = 0; i < loader->getMap("testmap")->getWidth(); ++i)
			{
				// get the tile at current position
				tileID = loader->getMap("testmap")->getLayer("layer2")->getTiles()[j][i];

				// only render if it is an actual tile (tileID = 0 means no tile / don't render anything here)
				if (tileID > 0)
				{

					//SDL_Rect srcrect = { ((tileID - 1) % 3) * tileWidth, ((tileID - 1) / 3) * tileHeight, tileWidth, tileHeight };
					//SDL_Rect dstrect = { (int)i * 25, (int)j * 25, 25, 25 };
					//SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
					gfx.DrawSprite((int)i * tileWidth, (int)j * tileHeight, tiles[(int)tileID - 1], spritesheet, SpriteEffect::Chroma{ Colors::Magenta });
				}
			}
		}
	}
	void InitCollTest()
	{
		char tileID = 0;

		int tileWidth = loader->getMap("testmap")->getTileWidth();
		int tileHeight = loader->getMap("testmap")->getTileHeight();

		for (unsigned int j = 0; j < loader->getMap("testmap")->getHeight(); ++j)
		{

			for (unsigned int i = 0; i < loader->getMap("testmap")->getWidth(); ++i)
			{
				// get the tile at current position
				tileID = loader->getMap("testmap")->getLayer("layer2")->getTiles()[j][i];
				if (tileID == 9)
				{
					float left = i * (float)tileWidth;
					float top = j * (float)tileHeight;
					fixtures.emplace_back(RectF(left, tileWidth + left, top, tileHeight + top));
				}
			}
		}
	}
	
	std::vector < std::pair<Entity*, Vec2> > CheckCollision_Entities(const Vec2& origin, const Vec2& move) const
	{

		LineF movement = LineF(origin, move);
		std::pair<bool, Vec2> pair_return;
		std::vector < std::pair<Entity*, Vec2> > intercepts;

		for (auto& entity : entities)
		{
			if (movement.GetNearbyLinesByDistance(entity->GetCircle().GetCenter(), entity->GetCircle().GetRadius())) //if line is within a reasonable dist to circle
			{
				std::pair<bool, Vec2> pair = movement.ClosestIntercept(entity->GetCircle());
				if (pair.first)
				{
					intercepts.emplace_back(entity, pair.second);
				}
			}
		}
		
	}
	std::vector<Vec2> CheckCollision_Fixtures(const Vec2& origin, const Vec2& move) const
	{
		std::vector<Vec2> intercepts;
		std::pair<bool, Vec2> pair_return;
		LineF movement = LineF(origin, move);

		for (auto& rect : fixtures)
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
	TMXLoader* loader;
	Surface spritesheet;
	std::vector<RectI> tiles;
	std::vector<RectF> fixtures;
	std::vector<Entity*> entities;
	std::vector<RectF> entities_coll;

};