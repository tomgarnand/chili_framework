#pragma once
#include "Character.h"
#include "GUI.h"
#include <unordered_map>


class Player : 
{
public:
	Player(Surface& src, Attributes stats, std::pair<std::string, Vec2> starting_pos)
		:
		Entity(src, stats, starting_pos)
	{
		Storage.emplace("Inventory", gui.Inventory);
	}
	void func()
	{
		
	}

private:
	std::string name;

	

	std::unordered_map<std::string, SelectionMenu*> Storage;

	SelectionMenu* Inventory;
	SelectionMenu* Equipment;
	SelectionMenu* Abilities;
	SelectionMenu* Worn;
};