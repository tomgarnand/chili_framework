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
	menu(&allEntities),
	cam(gfx),
	player("link", link, Attributes(), {current_map, {100,100}}),
	npc1("npc1", link, Attributes(), {current_map, {200,200}}),
	world()
{
	//initialize inventory from load file? we could push in a vector<string>, besides that they arent needed anymore
	Stack.emplace_back(gui.pGetMainMenu());

	Collection::eEntity* eNpc1 = new Collection::eEntity(&npc1);
	allEntities.AddElement(eNpc1);


	Application* Burn = new Application(Effect(EffectCategory::Active, EffectType::Burn, 10, 1.0f));
	Action* Fireball = new Action("Fireball", Burn);
	Collection::eAction* aFireball = new Collection::eAction(Fireball);
	gui.Collection_Spells.AddElement(aFireball);

	//WalkingLeft = 0
	//WalkingRight,
	//WalkingUp,
	//WalkingDown,
	//StandingLeft,
	//StandingRight,
	//StandingUp,
	//StandingDown = 7
	//Count
	for (int i = 0; i < 4; i++)
	{
		link_animations.emplace_back(Animation(90, 90 * i, 90, 90, 4, link, 0.16f));
	}
	for (int i = 4; i < 8; i++)
	{
		link_animations.emplace_back(Animation(0, 90 * (i - 4), 90, 90, 1, link, 0.16f));
	}

	player.AddAction(WalkingLeft, link_animations[0]);
	player.AddAction(WalkingRight, link_animations[1]);
	player.AddAction(WalkingUp, link_animations[2]);
	player.AddAction(WalkingDown, link_animations[3]);
	player.AddAction(StandingLeft, link_animations[4]);
	player.AddAction(StandingRight, link_animations[5]);
	player.AddAction(StandingUp, link_animations[6]);
	player.AddAction(StandingDown, link_animations[7]);
	player.AddAction(Entity::Idle, link_animations[7]);

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
			player.QueueAction(WalkingRight, player.Self());
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
			player.QueueAction(WalkingLeft, {});
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
			player.QueueAction(WalkingDown, {});
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
			player.QueueAction(WalkingUp, {});
		}
		else if (state == GameState::Menu)
		{
			//implement keyboard menu controls
		}
	}
	if (wnd.kbd.KeyIsPressed(config.GetKeycode(Command::SELECT)))
	{
		player.QueueAction(Entity::Idle, {});

	}

	
	

	
	
	
	
	
	if (TickLive)
	{
		//subtick
		player.SubTickUpdate(world, dt, StateStack);
		

		//end tick
		if (TickTimer > maxTickDuration)
		{
			player.EndTick(world, dt, StateStack); //don't check for idleness because idle is default next_action

			for (Entity* unit : entities)
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

		if (player.IsActionEnded() == true && player.IsActionQueued())
		{
			player.StartAction(player.GetQueuedAction(), player.GetQueuedTargets() ); 
		}


		player.StartTick(StateStack);

		for (Entity* unit : entities)
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
		}
	}

}
void Game::ComposeFrame()
{
	if (state == GameState::Menu)
	{
 		menu.DrawGUI(Stack, gfx, gui.GetFont());
	}
	
	cam.Draw(player.GetDrawable(current_map));
	cam.Draw(npc1.GetDrawable(current_map));

}
