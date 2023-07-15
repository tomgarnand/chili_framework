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
	menu(),
	world(),
	cam(gfx)

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
			state == GameState::Menu ? state = GameState::Moving : state = GameState::Menu;
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
				Collection::Item* new_item = new Collection::Item(gui.item);
				gui.Collection_Inventory.AddElement(new_item);
				
				//gui.Inventory.pGetSelectionItems()[0].InitInnerMenu(gui.pGetConfirmMenu());
				//gui.Inventory.pGetSelectionItems()[0].InitParentMenu(gui.Inventory.pGetSelectionMenu());

				cam.MoveTo({ 50,50 });
				gui.Collection_Equipment.AddElement(&gui.armor);
			
				//gui.Equipment.pGetSelectionItems()[0].InitInnerMenu(gui.pGetConfirmMenu());
				//gui.Equipment.pGetSelectionItems()[0].InitParentMenu(gui.Equipment.pGetSelectionMenu());
				// 
				//gui.Inventory.UpdateSelectionMenu("Teleport Crystal");
				//gui.Inventory.UpdateSelectionMenu("Scroll of Revival");
				//
				//gui.Equipment.UpdateSelectionMenu("Sword of Cunning");
				//gui.Spells.UpdateSelectionMenu("Attack");
				//gui.Spells.UpdateSelectionMenu("Heal");
				//
				//gui.Important.UpdateSelectionMenu("Notebook");
			}
		}
	}
	
	link.SetDirection(dir);
	link.Update(world, ft.Mark());

	
	

	
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (state == GameState::Menu)
		{
			Stack = menu.ProcessMenu(e, Stack, wnd);
		}
	}

	// line test
	//if (!wnd.mouse.IsEmpty())
	//{
	//	line = LineI(Vei2(200, 200), wnd.mouse.GetPos());
	//}
}

void Game::ComposeFrame()
{

	world.DrawRects(gfx);
	//world.DrawWorld(gfx);
	link.Draw(gfx);


	if (state == GameState::Menu)
	{
 		menu.DrawGUI(Stack, gfx, gui.GetFont());
	}
	//gfx.DrawLine(line);
	gfx.DrawCircle(25, link.GetPos().x, link.GetPos().y, Colors::Blue);

	cam.Draw(guy.GetDrawable());
	

}
