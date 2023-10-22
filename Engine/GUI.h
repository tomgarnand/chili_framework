#pragma once





class GUI
{
public:
	

	SelectionMenu& GetMainMenu()
	{
		return MainMenu;
	}
	SelectionMenu* pGetMainMenu()
	{
		return &MainMenu;
	}
	SelectionMenu* pGetConfirmMenu()
	{
		return &ConfirmMenu;
	}
	Font* GetFont()
	{
		return &font;
	}

private:

public:
	
	
	
};