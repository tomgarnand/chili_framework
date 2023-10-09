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
		config[VK_SELECT] = Command::SELECT      ;
		config[VK_ESCAPE] = Command::MAIN_MENU   ;
		config[VK_SPACE ] = Command::COMBAT_MENU ;
		config[VK_LEFT  ] = Command::MOVE_LEFT   ;
		config[VK_RIGHT ] = Command::MOVE_RIGHT  ;
		config[VK_UP    ] = Command::MOVE_UP	 ;
		config[VK_DOWN  ] = Command::MOVE_DOWN   ;
		config[VK_LSHIFT] = Command::ACTION	     ;
	}
	Command GetCommand(unsigned char keycode)
	{
		//work through error handling or use map[value] method
		try 
		{
			return config.at(keycode);
		}
		catch (const std::out_of_range& e)
		{
			assert(true);
			std::cerr << e.what() << "\nKey not bound\n";
			return Command::_Blank_; //shouldn't ever happen but avoids complier warning
		}

	}
	void ChangeBind(Command command, unsigned char keycode) //fine tune this when an assoicated menu is created, or loading is added
	{
		//remove command from currently bound key
		for (auto& pair : config)
		{
			if (pair.second == command)
			{
				pair.second = Command::_Blank_;
			}
		}
		//assign command to new key
		config[keycode] = command;
	}

private:
	std::map<unsigned char, Command> config;
};