#pragma once
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"
#include "Sound.h"
#include <string>
#include <map>
#include "Font.h"
#include <vector>



class SelectionMenu
{
public:
	class Entry
	{
	public:

		Entry(std::string s, const RectI& rect)
			:
			s(s),
			rect(rect)
		{}
		Entry operator=(const Entry& other)
		{
			rect = other.rect;
			s = other.s;
			Centered = other.Centered;
			return *this;
		}
		bool operator==(const Entry& other) const
		{
			return (s == other.s && rect == other.rect);
		}
		bool operator!=(const Entry& other) const
		{
			return (s != other.s && rect != other.rect);
		}
		void Draw(Graphics& gfx) const
		{
			if (highlighted)
			{
				gfx.DrawRect(rect, highlightColor);
			}
			if (Centered)
			{
				font.DrawText(s, rect, gfx);
			}
			else
			{
				font.DrawText(s, Vei2(rect.left, rect.top), gfx);
			}
		}
		bool IsHit(const Vei2& pt) const
		{
			return rect.left < pt.x&& rect.right > pt.x && rect.top < pt.y&& rect.bottom > pt.y;
		}
		void ResetHighlight()
		{
			highlighted = false;
		}
		void SetHighlight()
		{
			highlighted = true;
		}
		bool IsHighlighted() const
		{
			return highlighted;
		}
		std::string GetStr() const
		{
			if (this == nullptr)
			{
				return "";
			}
			return s;
		}
		RectI GetRect() const
		{
			return rect;
		}
		RectI SwapRects(RectI temp)
		{
			std::swap(temp, rect);
			return temp;
		}
		void SetCentered()
		{
			Centered = true;
		}
	private:
		static constexpr int highlightThickness = 6;
		static constexpr Color highlightColor = Colors::Yellow;
		bool highlighted = false;
		std::string s;
		RectI rect;
		Font& font = Font("Images//Fixedsys16x28.bmp", Colors::White);
		bool Centered = false;

	};
public:
	SelectionMenu() = default;

	SelectionMenu(const RectI rect, std::vector<std::string> input, int rows, bool center)
		:
		SelectionMenu(rect, input, rows)
	{
		if (center)
		{
			for (auto& e : entries)
			{
				e.SetCentered();
			}
		}
	}
	SelectionMenu(const RectI rect, std::vector<std::string> input, int rows)
		:
		rows(rows)
	{
		assert(rows > 0);
		RectI adjustedRect = RectI(Vei2(rect.left, rect.top) + GetTopOffsetMenu(), (Vei2(rect.right, rect.bottom) - GetTopOffsetMenu()));
		std::vector<RectI> MenuRects = SelectionRects(adjustedRect, int(input.size()));
		int i = 0;
		for (auto& s : input)
		{
			entries.emplace_back(s, MenuRects[i]);
			i++;
		}
		!input.empty() ? pLast = &entries.back() : pLast = nullptr;
	}
	SelectionMenu(const RectI rect, std::vector<std::string> input)
		:
		SelectionMenu(rect, input, 1)
	{

	}
	SelectionMenu& operator=(const SelectionMenu& other)
	{
		for (auto& e : other.entries)
		{
			entries.emplace_back(e.GetStr(), e.GetRect());
		}
		pLast = other.pLast;
		return *this;
	}
	SelectionMenu(SelectionMenu& other)
	{
		for (auto& e : other.entries)
		{
			entries.emplace_back(e.GetStr(), e.GetRect());
		}
		rows = other.rows;
		pLast = other.pLast;
	}
	void UpdateSelectionMenu(std::string input, RectI guiRect)
	{
		if (pLast != nullptr)
		{
			entries.emplace_back(input, GetNewRect());
			pLast = &entries.back();
		}
		else
		{
			entries.emplace_back(input, SelectionRects(RectI(Vei2(guiRect.left, guiRect.top) + GetTopOffsetMenu(), Vei2(guiRect.right, guiRect.bottom) - GetTopOffsetMenu()), 1).front());
			pLast = &entries.back();
		}
	}
	//erase entry and shift all rects over
	void UpdateSelectionMenu(Entry* entry)
	{
		//const auto Iter2del = std::remove_if(entries.begin(), entries.end(), 
		//	[entry](auto item)
		//	{
		//		return entry == item;
		//	});
		//auto new_Iter = entries.erase(Iter2del, entries.end());
		RectI temp = entry->GetRect();
		auto iter = entries.begin() + (entry - entries.data());
		for (auto i = iter, e = entries.end(); i != e; i++)
		{
			temp = i->SwapRects(temp);
		}
		entries.erase(iter);
		!entries.empty() ? pLast = &entries.back() : pLast = nullptr;

	}
	// returns Size::Something when an entry is clicked, otherwise returns Size::Invalid
	Entry* ProcessMouse(const Mouse::Event& e) //TODO: change this to returning a rect? or vector loc?
	{
		switch (e.GetType())
		{
		case Mouse::Event::Type::Move:
			for (auto& n : entries)
			{
				if (n.IsHit(e.GetPos()))
				{
					// need to test here to prevent sfx from firing
					// on every mouse move event
					if (!n.IsHighlighted())
					{
						ResetHighlights();
						n.SetHighlight();
						hover.Play();
					}
					// immediately exit if found a hit
					// (if we don't, highlight will be reset below)
					return &n;
				}
			}

			// if no entry was hit, reset all highlights
			ResetHighlights();
			break;
		case Mouse::Event::Type::LPress:
			for (auto& n : entries)
			{
				if (n.IsHit(e.GetPos()))
				{
					return &n;
				}
			}
			break;
		}
		return nullptr;
	}
	void Draw(Graphics& gfx) const
	{
		for (const auto n : entries)
		{
			n.Draw(gfx);
		}
	}
private:
	void ResetHighlights()
	{
		for (auto& n : entries)
		{
			n.ResetHighlight();
		}
	}
	std::vector<RectI> SelectionRects(RectI space, int numSquares)
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
	RectI GetNewRect()
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
private:
	//Spacing between top corner of GUI Rect and Text
	static Vei2 GetTopOffsetMenu() 
	{
		return { 20, 20 };
	}
	static constexpr int verticalSpacing = 4;
	Sound hover = { L"Sounds//menu_boop.wav" };
	std::vector<Entry> entries;
	Font font = Font("Images//Fixedsys16x28.bmp", Colors::White);
	int font_height = font.GetGlyphHeight();
	Entry* pLast = nullptr;
	int rows;
	int xOff = 0;
	int yOff = 0;
};

