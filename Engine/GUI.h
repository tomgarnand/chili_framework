#pragma once

#include "Graphics.h"
#include <map>
#include "SelectionMenu.h"
//#include "ProcessMenu.h"
#include "GameContainer.h"
#include "Action.h"

class GUI
{
public:
	GUI()
		:
		//BoxMenus
		BoxMenu_MainMenu(BoxMenu(GetMainMenuRect(), 1, font_height, false)),
		BoxMenu_SubMenuTabs2(BoxMenu(GetSubMenuTabsRect(), 2, font_height, true)),
		BoxMenu_SubMenuTabs3(BoxMenu(GetSubMenuTabsRect(), 3, font_height, true)),
		BoxMenu_SubMenu(BoxMenu(GetSubMenuRect(), 2, font_height, true)),
		BoxMenu_ConfirmMenu(BoxMenu(GetConfirmMenuRect(), 1, font_height, false)),
		//Collections
		Collection_Inventory(),
		Collection_Equipment(),
		Collection_Spells(),
		Collection_Worn(),
		Collection_Important(),

		Inventory(SelectionMenu(BoxMenu_SubMenu, &Collection_Inventory)),
		Equipment(SelectionMenu(BoxMenu_SubMenu, &Collection_Equipment)),
		Worn(SelectionMenu(BoxMenu_SubMenu, &Collection_Worn)),
		Spells(SelectionMenu(BoxMenu_SubMenu, &Collection_Spells)),
		Important(SelectionMenu(BoxMenu_SubMenu, &Collection_Important)),
		EquipmentTabs(SelectionMenu(BoxMenu_SubMenuTabs2, { "Worn", "Inventory" }, { &Worn, &Equipment })),
		InventoryTabs(SelectionMenu(BoxMenu_SubMenuTabs3, { "Items", "Equipment", "Important" }, { &Inventory, &Equipment, &Important })),
		AbilitiesTabs(SelectionMenu(BoxMenu_SubMenuTabs3, { "Skills", "Spells", "Passives" }, { &Spells, nullptr, nullptr })),
		//Menu
		MainMenu(SelectionMenu(BoxMenu_MainMenu, { "Inventory","Equipment","Abilities","Save","Load","Game End" },{ &InventoryTabs, &EquipmentTabs, &AbilitiesTabs })),

		ConfirmMenu(SelectionMenu(BoxMenu_ConfirmMenu, { "Yes", "No" }, { nullptr, nullptr }))

	{
		InventoryTabs.SetDefaultEntry(0);
		EquipmentTabs.SetDefaultEntry(1);
		AbilitiesTabs.SetDefaultEntry(0);
		Collection_Equipment.InitPairedCollection(&Collection_Worn);
		Collection_Worn.InitPairedCollection(&Collection_Equipment);


	}

	SelectionMenu& GetMainMenu()
	{
		return MainMenu;
	}
	SelectionMenu* pGetMainMenu()
	{
		return &MainMenu;
	}
	SelectionMenu* pGetConfirmMenu()
	{
		return &ConfirmMenu;
	}
	Font* GetFont()
	{
		return &font;
	}

private:

public:
	static constexpr Color BoxColor = Colors::Blue;
	Font fontV = Font("Images//Fixedsys16x28.bmp", Colors::White);
	Font& font = fontV;
	int font_height = fontV.GetGlyphHeight();

	//BoxMenus
	BoxMenu BoxMenu_MainMenu;
	BoxMenu BoxMenu_SubMenuTabs2;
	BoxMenu BoxMenu_SubMenuTabs3;
	BoxMenu BoxMenu_SubMenu;
	BoxMenu BoxMenu_ConfirmMenu;

	//Collections - get this from character in the future?
	Collection Collection_Inventory;
	Collection Collection_Equipment;
	Collection Collection_Spells;
	Collection Collection_Worn;
	Collection Collection_Important;

	//items - maybe a container container down the line
	Collection::Item item = Collection::Item("Health Potion", pGetConfirmMenu());
	Collection::Equipment armor = Collection::Equipment("Armour", pGetConfirmMenu());

	
	

	//SelectionMenus
	SelectionMenu Inventory;
	SelectionMenu Equipment;
	SelectionMenu Worn;
	SelectionMenu Spells;
	SelectionMenu Important;
	SelectionMenu EquipmentTabs;
	SelectionMenu InventoryTabs;
	SelectionMenu AbilitiesTabs;
	SelectionMenu MainMenu;
	SelectionMenu ConfirmMenu;
	static RectI GetMainMenuRect()
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
	static RectI GetConfirmMenuRect()
	{
		return { 400, 500, 600, 700 };
	}
	Sound hover = { L"Sounds//menu_boop.wav" };
	
};