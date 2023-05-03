#include "SelectionMenu.h"

void SelectionMenu::Entry::Draw(Graphics& gfx, Font& font) const
{
	if (highlighted)
	{
		gfx.DrawRect(rect, highlightColor);
	}
	Centered ? font.DrawText(s, rect, gfx) : font.DrawText(s, Vei2(rect.left, rect.top), gfx);
}

RectI SelectionMenu::Entry::SwapRects(RectI temp)
{
	std::swap(temp, rect);
	return temp;
}

//for some reason this is really slow
void SelectionMenu::UpdateSelectionMenu(std::string input, RectI guiRect)
{
	if (pLast != nullptr)
	{
		entries.emplace_back(input, GetNewRect());
		entries.back().SetProcess(ProcessFunc);
		pLast = &entries.back();
	}
	else
	{
		RectI newRect = SelectionRects(RectI(Vei2(guiRect.left, guiRect.top) + GetTopOffsetMenu(), Vei2(guiRect.right, guiRect.bottom) - GetTopOffsetMenu()), 1).front();
		entries.emplace_back(input, newRect);
		entries.back().SetProcess(ProcessFunc);
		pLast = &entries.back();
	}
}

//erase entry and shift all rects over
void SelectionMenu::UpdateSelectionMenu(Entry* entry)
{
	RectI temp = entry->GetRect();
	auto iter = entries.begin() + (entry - entries.data());
	for (auto i = iter, e = entries.end(); i != e; i++)
	{
		temp = i->SwapRects(temp);
	}
	entries.erase(iter);
	!entries.empty() ? pLast = &entries.back() : pLast = nullptr;
}

SelectionMenu::Entry* SelectionMenu::ProcessMouse(const Mouse::Event& e)
{
	switch (e.GetType())
	{
	case Mouse::Event::Type::Move:
	{
		bool oneHit = false;
		for (auto& n : entries)
		{
			if (n.IsHit(e.GetPos()))
			{
				oneHit = true;
				if (!n.IsHighlighted())
				{

					ResetHighlights();
					n.SetHighlight();
					hover.Play();
				}
				return &n;
			}
		}
		if (!oneHit && !MenuIsHit(e.GetPos()))
		{
			for (auto& n : entries)
			{
				if (!n.IsSelectedTab())
				{
					n.ResetHighlight();
				}
				else
				{
					n.SetHighlight();
				}
			}
		}
		break;
	}
	case Mouse::Event::Type::LPress:
		for (auto& n : entries)
		{
			if (n.IsHit(e.GetPos()))
			{
				n.pGetNextMenu()->GetOpenDefault()->SetHighlight();
				return &n;
				
			}
		}
		break;
	}
	return nullptr;
}

void SelectionMenu::Draw(Graphics& gfx) const
{
	
	for (const auto n : entries)
	{
		n.Draw(gfx, font);
	}
}

//probably dont need this if I could figure out how to grab a entry from GUI
void SelectionMenu::SetDefaultEntry(int i)
{
	openDefaultEntry = &entries[i];
	entries[i].SetSelectedTab();
}

void SelectionMenu::RememberDefaultEntry(Entry* sel)
{
	if (openDefaultEntry != nullptr)
	{
		openDefaultEntry->ResetSelectedTab();
	}
	openDefaultEntry = sel;
	sel->SetSelectedTab();
}

void SelectionMenu::ResetHighlights()
{
	for (auto& n : entries)
	{
		n.ResetHighlight();
	}
}

std::vector<RectI> SelectionMenu::SelectionRects(RectI space, int numSquares)
{
	int height = font_height;
	std::vector<RectI> grid(numSquares);
	int x0 = space.left;
	int y0 = space.top;
	int x1 = space.left + ((space.right - space.left) / rows);
	int y1 = space.top + height;

	int x = x1 - x0;
	int y = y1 - y0;

	int r = 0;
	for (int i = 0; i < numSquares; i++)
	{
		if (r >= rows)
		{
			x0 = space.left;
			x1 = space.left + ((space.right - space.left) / rows);
			y0 += y;
			y1 += y;
			r = 0;
		}
		grid[i] = (RectI(x0 + xOff, x1 - xOff, y0 + yOff, y1 - yOff));
		x0 += x;
		x1 += x;
		r++;
	}
	return grid;
}

RectI SelectionMenu::GetNewRect()
{
	RectI LastRect = pLast->GetRect();
	RectI NewRect;
	int ColWidth = (LastRect.right - LastRect.left);
	if (entries.size() % rows == 0) //if end of row, go down
	{
		NewRect = RectI(LastRect.left - (ColWidth * (rows - 1)), LastRect.right - (ColWidth * (rows - 1)), LastRect.top + font_height, LastRect.bottom + font_height);
	}
	//else if in a row, go right
	else
	{
		NewRect = RectI(LastRect.right, LastRect.right + ColWidth, LastRect.top, LastRect.bottom);
	}
	return NewRect;
}
