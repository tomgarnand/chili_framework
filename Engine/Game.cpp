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
	g.player.SetDir(3 * pi / 2);
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
		if (wnd.kbd.ReadKey().IsRelease())
		{
			state = (state == GameState::Menu) ? GameState::Moving : GameState::Menu; //toggle
		}
	}
	Vec2 dir = { 0,0 };
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::MOVE_RIGHT)))
	{
		if (state == GameState::Moving)
		{
			dir.x += 1;
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
			dir.y -= 1;
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
			dir.x -= 1;
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
			dir.y += 1;
		}
		else if (state == GameState::Menu)
		{
			//implement keyboard menu controls
		}
	}
	if (dir != Vec2(0,0))
	{
		g.player.QueueAction(g.Move, g.player.Self());
		g.player.SetDir(std::atan2(dir.y, dir.x));
	}

		


	
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::SELECT)))
	{
		g.player.QueueAction(Action::Idle, {});

	}

	
	

	
	
	
	
	
	if (TickLive)
	{
		g.player.SubTickUpdate(world, dt, StateStack); //update animations & do subtick
		for (Entity* unit : (*g.entities))
		{
			unit->SubTickUpdate(world, dt, StateStack);
		}

		//end tick
		if (TickTimer > maxTickDuration)
		{
			g.player.EndTick(world, dt, StateStack); 

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
		g.player.Update(world, dt);//update statuses
		for (Entity* unit : (*g.entities))
		{
			unit->Update(world, dt);
		}
		//-------
		// PLAYER
		//-------
		
		if (g.player.IsActionEnded()) 
		{
			if (g.player.IsActionQueued()) 
			{
				g.player.StartAction(g.player.GetQueuedAction(), g.player.GetQueuedTargets()); //tick = 0
			}
			else
			{
				g.player.StartAction(Action::Idle, {}); //tick = 0
			}
		}
		else
		{
			g.player.StartTick(StateStack); //criteria check for current_action & hitMethod init
			
		}
		

		//---------
		// ENTITIES
		//---------
		for (Entity* unit : (*g.entities))
		{
			
			if (unit->IsActionEnded() == true)
			{
				unit->UpdateFromScript(); //hmmm

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
	if (g.npc1.GetStatuses().CheckForEffect(EffectType::Burn))
	{
		int b = g.npc1.GetStatuses().getEffectByType(EffectType::Burn).GetDuration();
		std::stringstream ss;
		ss << "NPC is burning! Duration: " << b;
		std::string s = ss.str();

		
		g.dialogbox.AddText(s);
		g.dialogbox.Draw(gfx);
	}
	
	cam.Draw(g.player.GetDrawable(current_map));
	cam.Draw(g.npc1.GetDrawable(current_map));
	cam.Draw(g.npc2.GetDrawable(current_map));

	gfx.DrawRect(0, Graphics::ScreenWidth - 1, 0, 20, Colors::Gray);
	int percentfill = Graphics::ScreenWidth * (TickTimer/maxTickDuration);
	gfx.DrawRect(0, percentfill, 0, 20, Colors::Gray);
	
}
