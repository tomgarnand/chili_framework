#include "Font.h"


Font::Font(const std::string& filename, Color chroma)
	:
	surface(filename),
	glyphWidth(),
	glyphHeight()
{

}
void Font::DrawText(const std::string& text, const Vei2& pos, Graphics& gfx) const
{
}
RectI Font::MapGlyphRect(char c) const
{
	return RectI();
}
