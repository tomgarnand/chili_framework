#include "Font.h"


Font::Font(const std::string& filename, Color chroma)
	:
	surface(filename),
	chroma(chroma),
	glyphWidth(16),
	glyphHeight(28)
{
	int index;
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
	for (const char c : text)
	{
		gfx.DrawSprite(pos.x + i, pos.y, MapGlyphRect(c), surface, chroma);
		i += glyphWidth;
	}
}
RectI Font::MapGlyphRect(char c) const
{
	return alphabetSrcRect[c];
}
