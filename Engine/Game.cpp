/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	gui()
{
	//initialize inventory from load file?
	
	
	Inventory = SelectionMenu(gui.GetSubMenuRect(), Items[0], SelectionMenu::Fill::Right, 2);
	Abilities = SelectionMenu(gui.GetSubMenuRect(), Ability, SelectionMenu::Fill::Right, 2);
	Equipment = SelectionMenu(gui.GetSubMenuRect(), Items[1], SelectionMenu::Fill::Right, 2); //TODO add items sub section... >_<
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	Vec2 dir = { 0.0f, 0.0f };

	const auto e = wnd.kbd.ReadKey();
	if (e.IsPress())
	{
		switch(e.GetCode())
		case (27) : //escape
		{
			state == State::Menu ? state = State::World : state = State::Menu;
			break;
		}
		
	}
	if (true) //state == State::World)
	{
		if (wnd.kbd.KeyIsPressed(VK_RIGHT))
		{
			dir.x += 1.0f;
		}
		else if (wnd.kbd.KeyIsPressed(VK_LEFT))
		{
			dir.x -= 1.0f;
		}
		else if (wnd.kbd.KeyIsPressed(VK_DOWN))
		{
			dir.y += 1.0f;
		}
		else if (wnd.kbd.KeyIsPressed(VK_UP))
		{
			dir.y -= 1.0f;
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			link.effectActivate();
			if (Items[0].size() < 2)
			{
				Items[0].emplace_back("Health Potion");
				Items[0].emplace_back("Teleport Crystal");
				Items[0].emplace_back("Scroll of Revival");
				Inventory = SelectionMenu(gui.GetSubMenuRect(), Items[0], SelectionMenu::Fill::Right, 2);
				
			}
		}
	}
	link.SetDirection(dir);
	link.Update(ft.Mark());


	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (state == State::Menu)
		{
			const std::string s = gui.GetMainMenu().ProcessMouse(e);
			const std::string ss = Inventory.ProcessMouse(e);
			const std::string t = gui.GetInvTabsMenu().ProcessMouse(e);
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				if (s != "")
				{
					auto p = GUI_Boxes::SubMenu{ gui.GetMainMenu(), gui.GetInvTabsMenu() };
				}
				if (ss != "")
				{
					//TODO: create a whole use item function
					const auto new_end = std::remove_if(Items[0].begin(), Items[0].end(), [ss](auto item)
						{
							return ss == item;
						});
					Items[0].erase(new_end, Items[0].end());
					Inventory = SelectionMenu(gui.GetSubMenuRect(), Items[0], SelectionMenu::Fill::Right, 2);
				}
			}
		}
		else
		{
			
			if (false)
			{
				//state = State::World;
			}
		}
	}

}

void Game::ComposeFrame()
{
	font.DrawText("hello there!! \n beeblebum", { 400,300 }, gfx);
	link.Draw(gfx);
	if (state == State::Menu)
	{

		//gui.DrawGUI(gfx, GUI_Boxes::Menu{}, gui.GetMainMenu());

		gui.DrawGUI(gfx, GUI_Boxes::SubMenu{ gui.GetMainMenu(), gui.GetInvTabsMenu() }, Inventory);
	}

}
