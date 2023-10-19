/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Surface.h"
#include "FrameTimer.h"

#include "Font.h"
#include "Sound.h"
#include "GUI.h"
#include <utility>
#include <functional>
#include <map>
#include <unordered_map>
#include "World.h"
#include "ProcessMenu.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "ControlConfig.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();

	enum GameState
	{
		Menu,
		Moving,
		Combat
	};

private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	ControlConfig config;
	FrameTimer ft;
	bool TickLive = false;
	float TickTimer = 0.0f;
	float maxTickDuration = 2.0f;

	Font font = Font("Images//Fixedsys16x28.bmp", Colors::White);
	Surface link = Surface("Images//link90x90.bmp");
	Sound hover = { L"Sounds//menu_boop.wav" };

	World world;
	std::string current_map = "testmap";
	std::vector<std::string> StateStack;


	GameState state = GameState::Moving;
	GUI gui;

	MenuProcessing menu;
	std::vector<SelectionMenu*> Stack;

	Camera cam;


	Player player;
	Entity npc1;

	std::vector<Entity*> entities;

	std::vector<Animation> link_animations;

	

	Application* moveRight = new Application(Effect(EffectCategory::SubTick, EffectType::MoveRight, 0, 1.0f));
	Action* WalkingRight = new Action(moveRight);

	Application* moveLeft = new Application(Effect(EffectCategory::SubTick, EffectType::MoveLeft, 0, 1.0f));
	Action* WalkingLeft = new Action(moveLeft);

	Application* moveUp = new Application(Effect(EffectCategory::SubTick, EffectType::MoveUp, 0, 1.0f));
	Action* WalkingUp = new Action(moveUp);

	Application* moveDown = new Application(Effect(EffectCategory::SubTick, EffectType::MoveDown, 0, 1.0f));
	Action* WalkingDown = new Action(moveDown);

	Application* standRight = new Application(Effect(EffectCategory::SubTick, EffectType::StandRight, 0, 1.0f));
	Action* StandingRight = new Action(standRight);

	Application* standLeft = new Application(Effect(EffectCategory::SubTick, EffectType::StandLeft, 0, 1.0f));
	Action* StandingLeft = new Action(standLeft);

	Application* standUp = new Application(Effect(EffectCategory::SubTick, EffectType::StandUp, 0, 1.0f));
	Action* StandingUp = new Action(standUp);

	Application* standDown = new Application(Effect(EffectCategory::SubTick, EffectType::StandDown, 0, 1.0f));
	Action* StandingDown = new Action(standDown);

	std::vector<unsigned char> PressedKeys;
	unsigned char key = 0;
	/********************************/
};