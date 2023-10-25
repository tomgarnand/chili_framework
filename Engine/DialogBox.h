#pragma once
#include <string>
#include "Graphics.h"
#include "Font.h"


class DialogBox
{
public:
	static RectI staticBoxRect;
	static Font* staticFont;
	DialogBox() = default;
	DialogBox(std::string text) :
		text(text)
	{

	}
	void Draw(Graphics& gfx)
	{
		gfx.DrawRect(staticBoxRect, Colors::Blue);
		staticFont->DrawText(text, staticBoxRect.topLeft + 20, gfx);
	}
	void AddText(const std::string& text_in)
	{
		text = text_in;
	}

private:
	std::string text;

};

