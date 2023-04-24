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
	PossibleSelect.emplace_back(root);
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
			for (SelectionMenu* m : PossibleSelect)
			{
				select = m->ProcessMouse(e); 

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
					if (select->GetProcessFunc() != nullptr)
					{
						select->Process(select);
						break;
					}
					if (MenuStack.empty()) //if nothing has been added to the MenuStack, just straight add whateva
					{
						MenuStack.emplace_back(select);
						PossibleSelect.emplace_back(select->pGetNextMenu());
						if (select->pGetNextMenu()->GetOpenDefault() != nullptr) //if it has a defaulted child, emplace that too!
						{
							MenuStack.emplace_back(select->pGetNextMenu()->GetOpenDefault());
							PossibleSelect.emplace_back(select->pGetNextMenu()->GetOpenDefault()->pGetNextMenu());
						}
						break;
					}


					bool inMenu = false;
					bool inStack = false;
					int i = 0;
					for (auto s : MenuStack)
					{

						if (s == select) //check to see if it already is in stack.
						{
							inStack = true;
							if (s->pGetParentMenu()->GetOpenDefault() != nullptr) //If Select Entry, already on the stack, with with a Default open, ignore the click
							{
								break;
							}
							std::vector<SelectionMenu::Entry*>::iterator Iter = MenuStack.begin() + i;
							MenuStack.erase(Iter, MenuStack.end());
							while (PossibleSelect.size() > MenuStack.size() + 1) //this might not work in a case: Menu -> Menu & Menu(Default) -> Menu -> Menu & Menu(Default) . I think it would pop back 1 too many time. Use i?
							{
								PossibleSelect.pop_back();
							}
							//TODO: set opendefault to the last open tab as we back out. Possibly with a game-level toggle that will appear in a config file
							break;
						}
						i++;
					}
					if (!inStack)
					{
						while (!inMenu) //check to see if select is an alternative entry of a SelectionMenu that is already on the stack
						{
							int i = 0;
							for (auto s : MenuStack) //TODO: reverse iteration
							{
								for (auto sm : s->pGetParentMenu()->GetEntries())  //sm is all of the entries of the parent menu, i.e. The an Inventory Tab entry gets all the Inventory Tab Entries 
								{
									if (*select == sm) //highlighted members wont be equal so we cant check for equality the normal way (even tho my operator== doesnt check for it... weird) EDIT: fixed?
									{
										std::vector<SelectionMenu::Entry*>::iterator Iter = MenuStack.begin() + i;
										MenuStack.erase(Iter, MenuStack.end());
										while (PossibleSelect.size() > MenuStack.size() + 1)
										{
											PossibleSelect.pop_back();
										}
										//TODO: set opendefault to the last open tab as we back out. Possibly with a game-level toggle that will appear in a config file
										inMenu = true;
										MenuStack.emplace_back(select);
										PossibleSelect.emplace_back(select->pGetNextMenu());
										if (select->pGetNextMenu()->GetOpenDefault() != nullptr) //if it has a defaulted child, emplace that too!
										{
											MenuStack.emplace_back(select->pGetNextMenu()->GetOpenDefault());
											PossibleSelect.emplace_back(select->pGetNextMenu()->GetOpenDefault()->pGetNextMenu());
										}
										break;
									}
								}
								if (inMenu)
								{
									break;
								}
								i++;
							}
						}
					}
				
					
				}
			}
		}
	}
}

void Game::ComposeFrame()
{
	
	link.Draw(gfx);
	if (state == State::Menu)
	{
 		gui.DrawGUI(gfx, MenuStack);
	}

}
