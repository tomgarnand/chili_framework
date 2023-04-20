#pragma once
#include "Graphics.h"
#include <map>

class GUI_Boxes
{
public:

	enum class BoxType
	{
		Menu,
		Text,
		Dialog,
		CombatMoves
	};



	GUI_Boxes()
	{
		//initialization of default boxes
		types.emplace(BoxType::Menu, RectI ({ 100,100 }, { 200,200 }));
	}

	void DrawGUI(RectI rect, Graphics& gfx);
	RectI GetDefaultLoc(BoxType);

private:
	std::map<BoxType, RectI> types;
	Color BoxColor = Colors::Blue;
};
