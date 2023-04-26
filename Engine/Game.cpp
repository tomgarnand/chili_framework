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
	menu()
	

{
	//initialize inventory from load file? we could push in a vector<string>, besides that they arent needed anymore
	Stack.emplace_back(gui.pGetMainMenu());
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
			if (true) //(Items[0].size() < 2)
			{
				gui.GetEqu().UpdateSelectionMenu("Sword of Cunning", gui.GetSubMenuRect());

				gui.GetInv().UpdateSelectionMenu("Health Potion", gui.GetSubMenuRect());
				gui.GetInv().UpdateSelectionMenu("Teleport Crystal", gui.GetSubMenuRect());
				gui.GetInv().UpdateSelectionMenu("Scroll of Revival", gui.GetSubMenuRect());

				gui.GetSki().UpdateSelectionMenu("Attack", gui.GetSubMenuRect());
				gui.GetSki().UpdateSelectionMenu("Heal", gui.GetSubMenuRect());

				gui.GetImp().UpdateSelectionMenu("Notebook", gui.GetSubMenuRect());
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
			Stack = menu.ProcessMenu(e, Stack, wnd);
		}
	}
}

void Game::ComposeFrame()
{
	
	link.Draw(gfx);
	if (state == State::Menu)
	{
 		gui.DrawGUI(gfx, Stack);
	}

}
