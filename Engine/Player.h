#pragma once
#include "Character.h"
#include "GUI.h"
#include <unordered_map>

class Attributes //move to character?
{
public:
	Attributes()
	{

	}
private:
	int Strength;
	int Dexterity;
	int Constitution;
	int Intelligence;
	int Intuition;
	int Charisma;
};

class Player
{
public:
	Player(Character character, Attributes attributes, GUI gui)
		:
		character(character)
	{
		Storage.emplace("Inventory", gui.Inventory);
	}
	void func()
	{
		
	}

private:
	std::string name;
	Character character;
	Attributes attributes;

	std::unordered_map<std::string, SelectionMenu*> Storage;

	SelectionMenu* Inventory;
	SelectionMenu* Equipment;
	SelectionMenu* Abilities;
	SelectionMenu* Worn;
};