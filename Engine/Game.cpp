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
	g(),
	menu(g.pPlayer, g.entities),
	cam(gfx),
	world()
{
	//initialize inventory from load file? we could push in a vector<string>, besides that they arent needed anymore
	Stack.emplace_back(g.pMainMenu);

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
	
	float dt = ft.Mark();
	TickTimer += dt;

	
	
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::MAIN_MENU)))
	{
		state = (state == GameState::Menu) ? GameState::Moving : GameState::Menu; //toggle
	}
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::MOVE_RIGHT)))
	{
		if (state == GameState::Moving)
		{
			g.player.QueueAction(g.WalkingRight, g.player.Self());
		}
		else if (state == GameState::Menu)
		{
			//implement keyboard menu controls
		}
	}
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::MOVE_LEFT)))
	{
		if (state == GameState::Moving)
		{
			g.player.QueueAction(g.WalkingLeft, {});
		}
		else if (state == GameState::Menu)
		{
			//implement keyboard menu controls
		}
	}
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::MOVE_DOWN)))
	{
		if (state == GameState::Moving)
		{
			g.player.QueueAction(g.WalkingDown, {});
		}
		else if (state == GameState::Menu)
		{
			//implement keyboard menu controls
		}
	}
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::MOVE_UP)))
	{
		if (state == GameState::Moving)
		{
			g.player.QueueAction(g.WalkingUp, {});
		}
		else if (state == GameState::Menu)
		{
			//implement keyboard menu controls
		}
	}
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::SELECT)))
	{
		g.player.QueueAction(Entity::Idle, {});

	}

	
	

	
	
	
	
	
	if (TickLive)
	{
		//subtick
		g.player.SubTickUpdate(world, dt, StateStack);
		for (Entity* unit : (*g.entities))
		{
			unit->SubTickUpdate(world, dt, StateStack);
		}

		//end tick
		if (TickTimer > maxTickDuration)
		{
			g.player.EndTick(world, dt, StateStack); //don't check for idleness because idle is default next_action

			for (Entity* unit : (*g.entities))
			{
				unit->EndTick(world, dt, StateStack);
			}

			TickLive = false;
			TickTimer = TickTimer - maxTickDuration; // ~0.0f
		}
	}
	//start tick
	if (!TickLive)
	{

		if (g.player.IsActionEnded())
		{
			if (g.player.IsActionQueued())
			{
				g.player.StartAction(g.player.GetQueuedAction(), g.player.GetQueuedTargets());
			}
			else
			{
				g.player.StartAction(Entity::Idle, {});
			}
		}
		g.player.StartTick(StateStack);


		for (Entity* unit : (*g.entities))
		{
			if (unit->IsActionEnded() == true)
			{
				unit->Update(world, dt); //hmmm

			}
			unit->StartTick(StateStack);
		}
		TickLive = true;
	}
	
	

	
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (state == GameState::Menu)
		{
			Stack = menu.ProcessMenu(e, Stack, wnd);
			if (menu.ForceClose())
			{
				state = GameState::Moving; //obviously this should be changed later, just a way to close menu from within it
			}
		}
	}

}
void Game::ComposeFrame()
{
	if (state == GameState::Menu)
	{
 		menu.DrawGUI(Stack, gfx, g.font);
	}
	
	cam.Draw(g.player.GetDrawable(current_map));
	cam.Draw(g.npc1.GetDrawable(current_map));
	cam.Draw(g.npc2.GetDrawable(current_map));

}
