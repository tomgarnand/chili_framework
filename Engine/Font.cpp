#include "Font.h"
#include <cassert>
#include "SpriteEffect.h"


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
//draws text in the center (width) of a Rect
void Font::DrawText(const std::string& text, const RectI& rect, Graphics& gfx) const
{
	int sizeStr = text.length() * glyphWidth;
	int sizeRect = rect.right - rect.left;
	if (sizeStr > (rect.right - rect.left))
	{
		DrawText(text, Vec2(rect.left, rect.top), gfx);
	}
	else
	{
		int newPos = (sizeRect - sizeStr) / 2;
		DrawText(text, Vec2(rect.left + newPos, rect.top), gfx);
	}
}
void Font::DrawText(const std::string& text, const Vei2& pos, Graphics& gfx) const
{
	SpriteEffect::Substitution sub{ chroma, textColor };
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
			gfx.DrawSprite(pos.x + i, pos.y + j, MapGlyphRect(c), gfx.GetScreenRect(), surface, sub);
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
