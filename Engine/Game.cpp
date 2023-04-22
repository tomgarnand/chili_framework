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


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	gui(),
	Inventory(SelectionMenu(gui.GetSubMenuRect(), Items[0], 2)),
	Equipment(SelectionMenu(gui.GetSubMenuRect(), Items[1], 2)),
	Abilities(SelectionMenu(gui.GetSubMenuRect(), Ability, 2))
	

{
	//initialize inventory from load file? we could push in a vector<string>, besides that they arent needed anymore


	MenuState = std::make_pair(GUI_Boxes::Menu{}, gui.pGetMainMenu());

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
				Equipment.UpdateSelectionMenu("Sword of Cunning", gui.GetSubMenuRect());
				Inventory.UpdateSelectionMenu("Health Potion", gui.GetSubMenuRect());
				Inventory.UpdateSelectionMenu("Teleport Crystal", gui.GetSubMenuRect());
				Inventory.UpdateSelectionMenu("Scroll of Revival", gui.GetSubMenuRect());
				
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

			SelectionMenu::Entry* main = gui.GetMainMenu().ProcessMouse(e);
			SelectionMenu::Entry* sub = Inventory.ProcessMouse(e);
			SelectionMenu::Entry* tab = gui.GetInvTabsMenu().ProcessMouse(e);
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				if (main->GetStr() == "Game End")
				{
					wnd.Kill();
				}
				if (main->GetStr() == "Items")
				{
					MenuState = std::make_pair(GUI_Boxes::SubMenu{ gui.GetMainMenu(), gui.GetInvTabsMenu() }, &Inventory);
				}
				if (tab->GetStr() == "Equipment")
				{
					MenuState = std::make_pair(GUI_Boxes::SubMenu{ gui.GetMainMenu(), gui.GetInvTabsMenu() }, &Equipment);

				}
				if (sub != nullptr)
				{
					//use item
					//TODO: create a whole use item function
					Inventory.UpdateSelectionMenu(sub);
					
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
	
	link.Draw(gfx);
	if (state == State::Menu)
	{
		gui.DrawGUI(gfx, MenuState.first, *MenuState.second);
	}

}
