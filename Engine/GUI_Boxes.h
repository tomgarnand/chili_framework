#pragma once
#include "Graphics.h"
#include <map>
#include "SelectionMenu.h"


class GUI
{
public:
	GUI()
		:
		Inventory(SelectionMenu(GetSubMenuRect(), {}, 2)),
		Equipment(SelectionMenu(GetSubMenuRect(), {}, 2)),
		Abilities(SelectionMenu(GetSubMenuRect(), {}, 2)),
		Important(SelectionMenu(GetSubMenuRect(), {}, 2)),
		EquipmentTabs(SelectionMenu(GetSubMenuTabsRect(), { "Worn", "Held" }, 3, true, { nullptr, &Equipment })),
		InventoryTabs(SelectionMenu(GetSubMenuTabsRect(), { "Items", "Equipment", "Important" }, 3, true, { &Inventory, &Equipment, &Important })),
		AbilitiesTabs(SelectionMenu(GetSubMenuTabsRect(), { "Skills", "Spells", "Passives" }, 3, true)),
		//Menu
		MainMenu(SelectionMenu(GetMenuRect(), { "Items","Equipment","Abilities","Save","Load","Game End" }, 1, false, 
			{ &InventoryTabs, &EquipmentTabs, &AbilitiesTabs }
		))
		
	{
		InventoryTabs.CreateDefaultEntry(0);
		EquipmentTabs.CreateDefaultEntry(1);

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
	SelectionMenu* GetSubMenu(std::vector<std::string> input)
	{
		return &SelectionMenu(GetSubMenuRect(), input);
	}
private:

public:
	static constexpr Color BoxColor = Colors::Blue;

	//Static Menus
	SelectionMenu Inventory;
	SelectionMenu Equipment;
	SelectionMenu Abilities;
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
