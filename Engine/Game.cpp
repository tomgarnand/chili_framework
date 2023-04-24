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
	gui()
	

{
	//initialize inventory from load file? we could push in a vector<string>, besides that they arent needed anymore

	root = gui.pGetMainMenu();
	Stack.emplace_back(root);
	select = nullptr;
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
			int it = 1;
			for (SelectionMenu* sm : Stack)
			{
				select = sm->ProcessMouse(e); 

				if (e.GetType() == Mouse::Event::Type::LPress && select != nullptr) 
				{
					if (select->GetStr() == "Game End")
					{
						wnd.Kill();
						break;
					}
					if (select->GetStr() == "Save" || select->GetStr() == "Load")
					{
						break;
					}

					bool inMenu = false;
					int i = 0;
					for (auto e : sm->GetEntries()) //for every Entry in a SelectionMenu of Stack
					{
						//check to see if Entry is in the Selection Menu
						if (*select == e) //if an Entry of a SelectionMenu layer is what was Selected
						{
							//Remove any SelectionMenu layers above the currently iterating SelectionMenu (sm)
							//If the current SelectionMenu layer (sm) is the top one, this does nothing
							int size = Stack.size();
							for (int i = 0; i < size - it; i++)
							{
								Stack.pop_back();
							}

							//Emplace the NextMenu the Selected Entry points to
							auto e_temp = e.pGetNextMenu();
							if (e.pGetNextMenu() != nullptr)
							{
								Stack.emplace_back(e_temp);
								while (e_temp->GetOpenDefault() != nullptr) //Add a chain of defaults, however long (c++23 replace this with a Appened_range vector funtion)
								{
									e_temp = e_temp->GetOpenDefault()->pGetNextMenu();
									Stack.emplace_back(e_temp);
									
								}
							}
							else
							{
								select->Process(select);
							}
						}
					}
					
				}
				it++;
			}
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
