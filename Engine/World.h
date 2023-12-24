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
	
	
private:
	TMXLoader* loader;
	Surface spritesheet;
	std::vector<RectI> tiles;
	std::vector<RectF> fixtures;
	std::vector<Entity*> entities;
	std::vector<RectF> entities_coll;

};

