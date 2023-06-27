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
		loader->loadMap("testmap", "Assets\\testmap.tmx");
		loader->printMapData("testmap");

		int sheetWidth = spritesheet.GetWidth();
		int sheetHeight = spritesheet.GetHeight();
		int tileWidth = loader->getMap("testmap")->getTileWidth();
		int tileHeight = loader->getMap("testmap")->getTileHeight();

		InitCollTest();

		for (int row = 0; row < sheetHeight / tileHeight; row++) //TODO: create loader->getMap("testmap")->getTileSet("defaulttileset")->getNumTiles()
		{
			for (int col = 0; col < sheetWidth / tileWidth; col++)
			{
				tiles.emplace_back(row * tileHeight, (row * tileHeight) + tileHeight, col * tileWidth, (col * tileWidth) + tileWidth);
			}
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
				if (tileID == 9)
				{
					coll_test.emplace_back(RectI((int)i * tileWidth, tileWidth, (int)j * tileHeight, tileHeight));
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
					int left = (int)i * tileWidth;
					int top = (int)j * tileHeight;
					coll_test.emplace_back(RectI(left, tileWidth + left, top, tileHeight + top));
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


private:
	TMXLoader* loader;
	Surface spritesheet;
	std::vector<RectI> tiles;
	std::vector<RectI> coll_test;

};