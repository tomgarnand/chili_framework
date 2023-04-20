#include "GUI_Boxes.h"

void GUI_Boxes::DrawGUI(RectI rect, Graphics& gfx)
{
	gfx.DrawRect(rect.left, rect.top, rect.right, rect.bottom, BoxColor);
}

RectI GUI_Boxes::GetDefaultLoc(BoxType which)
{
	return types[which];
}
