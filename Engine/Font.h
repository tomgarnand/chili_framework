#pragma once
#include <vector>
#include "Graphics.h"
#include "Surface.h"
#include "Vec2.h"

class Font
{
public:
	Font(const std::string& filename, Color chroma = Colors::White);
	void DrawText(const std::string& text, const Vei2& pos, Graphics& gfx) const;
private:
	RectI MapGlyphRect(char c) const;
private:
	Surface surface; //holds the font sheet bitmap data
	int glyphWidth;
	int glyphHeight; //dimensions of a glyph on the font sheet
	static constexpr int nColumns = 32;
	static constexpr int nRows = 3; //fixed num of cols/rows from the predetermined font sheet
	Color chroma; //transparancy color
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
	std::vector<RectI> alphabetSrcRect;
	Color textColor = Colors::White;
	
};