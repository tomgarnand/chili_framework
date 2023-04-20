#pragma once
#include "Graphics.h"
#include <map>
#include "SelectionMenu.h"


class GUI
{
public:
	GUI()
		:
		//test input for now, will have to come up with a method for holding static menus and dynamic like items
		//Menu
		input({ "Items","Equipment","Abilities","Save","Load","Game End" }),
		MainMenu(SelectionMenu(GetMenuRect(), input))
	{
		
	}
	template<typename E> void DrawGUI(Graphics& gfx, E which, std::vector<std::string> input, SelectionMenu& Menu)
	{
		which(gfx, input, Menu);
	}
	SelectionMenu& GetMainMenu()
	{
		return MainMenu;
	}
private:

public:
	static constexpr Color BoxColor = Colors::Blue;
	std::vector<std::string> input;
	SelectionMenu MainMenu;
	static RectI GetMenuRect()
	{
		return { 40,240,40,380 };
	}
};
namespace GUI_Boxes
{
	class Menu
	{
	public:
		Menu() = default;
		void operator()(Graphics& gfx, std::vector<std::string> input, SelectionMenu& menu)
		{
			gfx.DrawRect(GUI::GetMenuRect(), GUI::BoxColor);
			menu.Draw(gfx);
		}

	private:
		
	};
}