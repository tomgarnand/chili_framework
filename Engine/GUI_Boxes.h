#pragma once
#include "Graphics.h"
#include <map>
#include "SelectionMenu.h"


class GUI
{
public:
	GUI()
		:
		//Menu
		input({ "Items","Equipment","Abilities","Save","Load","Game End" }),
		MainMenu(SelectionMenu(GetMenuRect(), input, 1)),
		//Inv
		InventoryTabs(SelectionMenu(GetSubMenuTabsRect(), { "Items", "Equipment", "Important" }, 3, true))

	{
		
	}
	template<typename E> void DrawGUI(Graphics& gfx, E which, SelectionMenu& Menu)
	{
		which(gfx, Menu);
	}
	SelectionMenu& GetMainMenu()
	{
		return MainMenu;
	}
	SelectionMenu& GetInvTabsMenu()
	{
		return InventoryTabs;
	}
	SelectionMenu GetSubMenu(std::vector<std::string> input)
	{
		return SelectionMenu(GetSubMenuRect(), input,  3);
	}
private:

public:
	static constexpr Color BoxColor = Colors::Blue;
	std::vector<std::string> input;
	SelectionMenu MainMenu;
	SelectionMenu InventoryTabs;
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
namespace GUI_Boxes
{
	class Menu
	{
	public:
		Menu() = default;
		void operator()(Graphics& gfx, SelectionMenu& menu)
		{
			gfx.DrawRect(GUI::GetMenuRect(), GUI::BoxColor);
			menu.Draw(gfx);
		}

	private:
		
	};
	class SubMenu
	{
	public:
		SubMenu(SelectionMenu& MainMenu, SelectionMenu& InventoryTabs)
			:
			MainMenu(MainMenu),
			InventoryTabs(InventoryTabs)
		{
			
		}
		void operator()(Graphics& gfx, SelectionMenu& menu)
		{
			gfx.DrawRect(GUI::GetMenuRect(), GUI::BoxColor);
			MainMenu.Draw(gfx);
			gfx.DrawRect(GUI::GetSubMenuTabsRect(), GUI::BoxColor);
			InventoryTabs.Draw(gfx);
			gfx.DrawRect(GUI::GetSubMenuRect(), GUI::BoxColor);
			menu.Draw(gfx);
		}
	private:
		SelectionMenu& MainMenu;
		SelectionMenu& InventoryTabs;
	};
}