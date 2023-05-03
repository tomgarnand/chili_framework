#pragma once
#include "Graphics.h"
#include <map>
#include "SelectionMenu.h"
#include "ProcessMenu.h"

class GUI
{
public:
	GUI()
		:
		Inventory    (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, MenuElements::Item{ Inventory })),
		Equipment    (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, MenuElements::Equip{ Equipment, Worn })),
		Worn         (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, MenuElements::Equip{ Worn, Equipment })),
		Skills       (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, MenuElements::Use{ Skills })),
		Important    (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, MenuElements::Use{ Important })),
		EquipmentTabs(SelectionMenu(GetSubMenuTabsRect(), { "Worn", "Inventory" }              , 3, true , { &Worn, &Equipment })),
		InventoryTabs(SelectionMenu(GetSubMenuTabsRect(), { "Items", "Equipment", "Important" }, 3, true , { &Inventory, &Equipment, &Important })),
		AbilitiesTabs(SelectionMenu(GetSubMenuTabsRect(), { "Skills", "Spells", "Passives" }   , 3, true , { &Skills, nullptr, nullptr })),
		//Menu
		MainMenu(SelectionMenu(GetMenuRect(), { "Inventory","Equipment","Abilities","Save","Load","Game End" }, 1, false, 
			{ &InventoryTabs, &EquipmentTabs, &AbilitiesTabs }))

		//Combat Menu
		//CombatMenu(SelectionMenu(GetCombatMenuRect(), { "Abilities", "Items", "Move" }, 1, false, {&AbilitiesCmbtTabs, &Inventory}))

	{
		InventoryTabs.SetDefaultEntry(0);
		EquipmentTabs.SetDefaultEntry(1);
		AbilitiesTabs.SetDefaultEntry(0);

	}
	void DrawGUI(Graphics& gfx,  std::vector<SelectionMenu*> stack)
	{
		for (auto& e : stack)
		{
			gfx.DrawRect(e->GetMenuRect(), BoxColor);
			e->Draw(gfx);
		}
	}
	SelectionMenu& GetMainMenu()
	{
		return MainMenu;
	}
	SelectionMenu* pGetMainMenu()
	{
		return &MainMenu;
	}
	
private:

public:
	static constexpr Color BoxColor = Colors::Blue;

	//Static Menus
	SelectionMenu Inventory;
	SelectionMenu Equipment;
	SelectionMenu Worn;
	SelectionMenu Skills;
	SelectionMenu Important;
	SelectionMenu EquipmentTabs;
	SelectionMenu InventoryTabs;
	SelectionMenu AbilitiesTabs;
	SelectionMenu MainMenu;
	static RectI GetMenuRect()
	{
		return { 40,240,40,380 };
	}
	static RectI GetSubMenuRect()
	{
		return { 244,800,124,380 };
	}
	static RectI GetSubMenuTabsRect()
	{
		return { 244,800,40,120 };
	}
	
};

class Combat_GUI
{
	Combat_GUI()
	{

	}

};


