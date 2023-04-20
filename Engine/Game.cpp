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
		hover.Play();
		link.effectActivate();
	}
	link.SetDirection(dir);
	link.Update(ft.Mark());


	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (state != State::Menu)
		{
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				//state = State::World;
			}
		}
		else
		{
			const std::string s = gui.GetMainMenu().ProcessMouse(e);
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
	std::vector<std::string> s = { "hi" };
	gui.DrawGUI(gfx, GUI_Boxes::Menu{}, s, gui.GetMainMenu());

}
