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
		Inventory    (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, ProcessMenu::Item{ GetInv() })),
		Equipment    (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, ProcessMenu::Equip{ GetEqu(), GetWorn() })),
		Worn         (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, ProcessMenu::Equip{ GetWorn(), GetEqu() })),
		Skills       (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, ProcessMenu::Use{ GetSki() })),
		Important    (SelectionMenu(GetSubMenuRect()    , {}                                   , 2, false, ProcessMenu::Use{ GetImp() })),
		EquipmentTabs(SelectionMenu(GetSubMenuTabsRect(), { "Worn", "Inventory" }              , 3, true , { &Worn, &Equipment })),
		InventoryTabs(SelectionMenu(GetSubMenuTabsRect(), { "Items", "Equipment", "Important" }, 3, true , { &Inventory, &Equipment, &Important })),
		AbilitiesTabs(SelectionMenu(GetSubMenuTabsRect(), { "Skills", "Spells", "Passives" }   , 3, true , { &Skills, nullptr, nullptr })),
		//Menu
		MainMenu(SelectionMenu(GetMenuRect(), { "Inventory","Equipment","Abilities","Save","Load","Game End" }, 1, false, 
			{ &InventoryTabs, &EquipmentTabs, &AbilitiesTabs }
		))
		
	{
		InventoryTabs.CreateDefaultEntry(0);
		EquipmentTabs.CreateDefaultEntry(1);
		AbilitiesTabs.CreateDefaultEntry(0);

	}
	void DrawGUI(Graphics& gfx,  std::vector<SelectionMenu::Entry*> stack)
	{
		gfx.DrawRect(GUI::GetMenuRect(), GUI::BoxColor);
		MainMenu.Draw(gfx);
		for (auto& e : stack)
		{
			gfx.DrawRect(e->pGetNextMenu()->GetMenuRect(), BoxColor);
			e->pGetNextMenu()->Draw(gfx);
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
	SelectionMenu& GetInv()
	{
		return Inventory;
	}
	SelectionMenu* pGetInv()
	{
		return &Inventory;
	}
	SelectionMenu& GetEqu()
	{
		return Equipment;
	}
	SelectionMenu& GetSki()
	{
		return Skills;
	}
	SelectionMenu& GetImp()
	{
		return Important;
	}
	SelectionMenu& GetWorn()
	{
		return Worn;
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


