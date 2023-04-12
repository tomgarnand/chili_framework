#include "Font.h"
#include <cassert>


Font::Font(const std::string& filename, Color chroma)
	:
	surface(filename),
	chroma(chroma),
	glyphWidth(surface.GetWidth()/ nColumns),
	glyphHeight(surface.GetHeight()/nRows)
{
	alphabetSrcRect.reserve(int(' ') + (nRows * nColumns));
	for (int i = 0; i < 32; i++)
	{
		alphabetSrcRect.emplace_back();
	}
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nColumns; j++)
		{
			// index = 1 + (int(' ') + (i * nColumns) + j);
			alphabetSrcRect.emplace_back(RectI(0 + j * glyphWidth, (j + 1) * glyphWidth, 1 + i * glyphHeight, (i + 1) * glyphHeight));
			
		}
	}
}
void Font::DrawText(const std::string& text, const Vei2& pos, Graphics& gfx) const
{
	int i = 0;
	int j = 0;
	for (const char c : text)
	{
		if (c == 10)
		{
			j += glyphHeight;
			i = 0;
			continue;
		}
		else if (c >= firstChar && c <= lastChar)
		{
			gfx.DrawSpriteSubstitute(pos.x + i, pos.y + j, textColor, MapGlyphRect(c), gfx.GetScreenRect(), surface, chroma);
		}
		i += glyphWidth;
	}
}
RectI Font::MapGlyphRect(const char c) const
{
	assert(c >= firstChar);
	assert(c <= lastChar);
	return alphabetSrcRect[c];
}
