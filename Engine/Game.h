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

	Font font = Font("Images//Fixedsys16x28.bmp", Colors::White);
	Surface link = Surface("Images//link90x90.bmp");
	Sound hover = { L"Sounds//menu_boop.wav" };

	World world;
	std::string current_map = "testmap";

	GameState state = GameState::Moving;
	GUI gui;

	MenuProcessing menu;
	std::vector<SelectionMenu*> Stack;

	Camera cam;


	Player player;
	Entity npc1;

	std::vector<Animation> link_animations;

	Action* WalkingLeft   ;
	Action* WalkingRight  ;
	Action* WalkingUp     ;
	Action* WalkingDown   ;
	Action* StandingLeft  ;
	Action* StandingRight ;
	Action* StandingUp    ;
	Action* StandingDown  ;


	/********************************/
};