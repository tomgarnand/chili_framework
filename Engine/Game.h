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
#include "Character.h"
#include "Font.h"
#include "Sound.h"
#include "GUI_Boxes.h"
#include <utility>
#include <functional>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	enum class State
	{
		Menu,
		World,
		Combat
	};
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
	FrameTimer ft;
	Font font = Font("Images//Fixedsys16x28.bmp", Colors::White);
	Character link = Character({ 100,100 });
	Sound hover = { L"Sounds//menu_boop.wav" };
	State state = State::World;
	GUI gui;
	std::vector<std::string> Items[4]; //not really needed anymore

	SelectionMenu Inventory;
	std::vector<std::string> Ability;
	SelectionMenu Abilities;
	SelectionMenu Equipment;
	
	std::pair<std::function<void(Graphics&, SelectionMenu&)>, SelectionMenu*> MenuState;

	
	/********************************/
};