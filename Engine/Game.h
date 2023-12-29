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
#include "GameAssets.h"
#include <sstream>
#include <numbers>
#include <cmath>

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

	std::string current_map = "testmap";

	double pi = std::numbers::pi;
	WorldHolder wh;
	World world;

	
	std::vector<std::string> StateStack;


	GameState state = GameState::Moving;
	GameAssets g;
	

	MenuProcessing menu;
	std::vector<SelectionMenu*> Stack;

	Camera cam;
	/********************************/
};