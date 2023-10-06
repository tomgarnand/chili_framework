#pragma once
#include "Surface.h"
#include "SpriteEffect.h"
#include <set>
#include "Line.h"
#include "TMXLoader/TMXLoader.h"

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
		coll_test.emplace_back(RectF({ 400,200 }, { 600, 300 }));
		coll_test.emplace_back(RectF({ 200,440 }, { 250, 460 }));


		for (int row = 0; row < sheetHeight / tileHeight; row++) //TODO: create loader->getMap("testmap")->getTileSet("defaulttileset")->getNumTiles()
		{
			for (int col = 0; col < sheetWidth / tileWidth; col++)
			{
				tiles.emplace_back(row * tileHeight, (row * tileHeight) + tileHeight, col * tileWidth, (col * tileWidth) + tileWidth);
			}
		}
	}
	void DrawRects(Graphics& gfx)
	{
		for (auto& rect : coll_test)
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
					gfx.DrawSprite((int)i * tileWidth, (int)j * tileHeight, tiles[tileID - 1], spritesheet, SpriteEffect::Chroma{ Colors::Magenta });
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
					coll_test.emplace_back(RectF(left, tileWidth + left, top, tileHeight + top));
				}
			}
		}
	}
	//bool GetNearbyLinesByBounds(Vec2 Opt, Vec2 Mpt, float radius)
	//{
	//
	//	bool y_bound = false;
	//	bool x_bound = false;
	//
	//		//if mpt is between line.a.y and line.b.y
	//		// and if mpt is between line.a.x and line.b.x
	//
	//		//or, same check but for opt (only necessary for diagonal movement)
	//
	//	if ((line.A.y < Mpt.y - radius && line.B.y > Mpt.y + radius) ||
	//		(line.A.y > Mpt.y + radius && line.B.y < Mpt.y - radius))
	//	{
	//		y_bound = true;
	//	}
	//	else if ((line.A.y < Opt.y - radius && line.B.y > Opt.y + radius) ||
	//			(line.A.y > Opt.y + radius && line.B.y < Opt.y - radius))
	//	{
	//		y_bound = true;
	//	}
	//	else
	//	{
	//		continue;
	//	}
	//	if ((line.A.x < Mpt.x - radius && line.B.x > Mpt.x + radius) ||
	//		(line.A.x > Mpt.x + radius && line.B.x < Mpt.x - radius))
	//	{
	//		x_bound = true;
	//	}
	//	else if ((line.A.x < Opt.x - radius && line.B.x > Opt.x + radius) ||
	//		(line.A.x > Opt.x + radius && line.B.x < Opt.x - radius))
	//	{
	//		x_bound = true;
	//	}
	//
	//
	//	if (x_bound && y_bound)
	//	{
	//		nearby.emplace(line);
	//	}
	//
	//}

	Vec2 CheckAndAdjustMovement(const Vec2& origin, const Vec2& move, float radius) const
	{
		std::vector<Vec2> intercepts;
		Vec2 shortest_move;
		std::pair<bool,Vec2> tempVec;
		float shortest_move_dist;
		LineF movement = LineF(origin, move);
		for (auto& rect : coll_test)
		{
			if (rect.lBottom.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				tempVec = rect.lBottom.Intercept(movement);
				if (tempVec.first == true)
				{
					intercepts.emplace_back(tempVec.second);
				}
			}
			if (rect.lTop.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				tempVec = rect.lTop.Intercept(movement);
				if (tempVec.first == true)
				{
					intercepts.emplace_back(tempVec.second);
				}
			}
			if (rect.lLeft.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				tempVec = rect.lLeft.Intercept(movement);
				if (tempVec.first == true)
				{
					intercepts.emplace_back(tempVec.second);
				}
			}
			if (rect.lRight.GetNearbyLinesByDistance(move, movement.lengthSq))
			{
				tempVec = rect.lRight.Intercept(movement);
				if (tempVec.first == true)
				{
					intercepts.emplace_back(tempVec.second);
				}
			}
		}

		if (intercepts.empty())
		{
			return move;
		}
		float temp;
		shortest_move = intercepts[0];
		shortest_move_dist = origin.GetDistanceSq(shortest_move);
		for (auto& in : intercepts)
		{
			temp = origin.GetDistanceSq(in);
			if (temp < shortest_move_dist)
			{
				shortest_move_dist = temp;
				shortest_move = in;
			}
		}

		//check direction
		Vec2 dir = { 0,0 };

		if (move.x - origin.x < 0)
		{
			dir.x = -1;
		}
		else if (move.x - origin.x > 0)
		{
			dir.x = 1;
		}
		if (move.y - origin.y < 0)
		{
			dir.y = -1;
		}
		else if (move.y - origin.y > 0)
		{
			dir.y = 1;
		}

		//return the shortest move possible, adjusted for the radius of the unit in the direction its moving
		return shortest_move  -(dir * 1.0f);
	}


private:
	TMXLoader* loader;
	Surface spritesheet;
	std::vector<RectI> tiles;
	std::vector<RectF> coll_test;

};