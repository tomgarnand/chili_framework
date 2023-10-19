#pragma once
#include "Keyboard.h"
#include"Mouse.h"
#include "MainWindow.h"
#include <map>

enum class Command
{
	SELECT,
	MAIN_MENU,
	COMBAT_MENU,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	ACTION,
	_Blank_
};

class ControlConfig
{
public:
	ControlConfig()
	{
		config[Command::SELECT]       = VK_SELECT ;
		config[Command::MAIN_MENU]    = VK_ESCAPE ;
		config[Command::COMBAT_MENU]  = VK_SPACE  ;
		config[Command::MOVE_LEFT]    = VK_LEFT   ;
		config[Command::MOVE_RIGHT]   = VK_RIGHT  ;
		config[Command::MOVE_UP]      = VK_UP     ;
		config[Command::MOVE_DOWN]    = VK_DOWN   ;
		config[Command::ACTION]       = VK_LSHIFT ;
	}
	unsigned char GetKeycode(Command command) //given a keycode, return a Command
	{
		//TODO: work through error handling (as implemented below) or use map[value] method (where a poorly written function could assign unmapped key presses)
		try 
		{
			return config.at(command);
		}
		catch (const std::out_of_range& e)
		{
			assert(true);
			std::cerr << e.what() << "\nKey not bound\n";
			return '\0'; //shouldn't ever happen but avoids complier warning
		}

	}
	void ChangeBind(Command command, unsigned char keycode) //fine tune this when an assoicated menu is created, or loading is added
	{
		//remove command from currently bound key
		for (auto& pair : config)
		{
			if (pair.second == keycode)
			{
				pair.second = '\0';
			}
		}
		//assign command to new key
		config[command] = keycode;
	}

private:
	std::map<Command, unsigned char> config;
};