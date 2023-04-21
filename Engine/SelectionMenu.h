#pragma once
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"
#include "Sound.h"
#include <string>
#include <map>
#include "Font.h"

class SelectionMenu
{
public:
	enum class Fill
	{
		Down,
		Right
	};
private:
	class Entry
	{
	public:

		Entry(std::string s, const RectI& rect)
			:
			s(s),
			rect(rect)
		{}
		void Draw(Graphics& gfx) const
		{
			if (highlighted)
			{
				gfx.DrawRect( rect,highlightColor );
			}
			font.DrawText(s, Vei2(rect.left, rect.top), gfx);
		}
		bool IsHit(const Vei2& pt) const
		{
			return rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y;
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
			return s;
		}

	private:
		static constexpr int highlightThickness = 6;
		static constexpr Color highlightColor = Colors::Yellow;
		bool highlighted = false;
		std::string s;
		RectI rect;
		Font& font = Font("Images//Fixedsys16x28.bmp", Colors::White);
		
	};
public:
	SelectionMenu() = default;
	SelectionMenu(const RectI rect, std::vector<std::string> input, Fill dir, int wrap) 
	{
		assert(wrap > 0);
		Vei2 topLeft = Vei2(rect.left, rect.top) + GetTopOffsetMenu();
		Vei2 botRight;
		int Col = 0;
		switch (dir) {
		case Fill::Down :
			botRight = Vei2(rect.right - GetTopOffsetMenu().x, rect.top + font_height + GetTopOffsetMenu().y);
			break;
		case Fill::Right :
			Col = (rect.right - rect.left - (GetTopOffsetMenu().x * 2))/wrap; //the avaiable space in each column
			botRight = Vei2(Col + rect.left + GetTopOffsetMenu().x, rect.top + font_height + GetTopOffsetMenu().y);
			break;
		}
		
		auto entryRect = RectI(topLeft,botRight); //top line of first rect, ready to be scaled down with font
		
		switch (dir)
		case Fill::Down:
		{
			for (auto s : input)
			{
				entries.emplace_back(s, entryRect);
				entryRect.top += font_height;
				entryRect.bottom += font_height;
			}
			break;
		case Fill::Right:
			int i = 0;
			RectI temp = entryRect;
			for (auto s : input)
			{
				if (i >= wrap)
				{
					entryRect.left = temp.left;
					entryRect.right = temp.right;
					entryRect.top += font_height;
					entryRect.bottom += font_height;
					i = 0;
				}
				entries.emplace_back(s, entryRect);
				entryRect.left += Col;
				entryRect.right += Col;
				i++;
			}
			break;
		}
	}
	
	void UpdateSelectionMenu(std::vector<std::string> input)
	{
		//TODO: add a method of expanding menus without reinitializing
	}
	// returns Size::Something when an entry is clicked, otherwise returns Size::Invalid
	std::string ProcessMouse(const Mouse::Event& e) //TODO: change this to returning a rect? or vector loc?
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
					return n.GetStr();
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
					return n.GetStr();
				}
			}
			break;
		}
		return ""; 
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

};

class SelectionRects
{
public:
	class Selection
	{
	public:
		Selection(int x0, int x1, int y0, int y1)
			:
			rect(RectI(x0,x1,y0,y1))
		{

		}
		RectI GetRect() const
		{
			return rect;
		}
	private:
		RectI rect;
	};
public:
	SelectionRects(RectI firstRect, int xOff, int yOff, int numSquares, int numRows)
	{
		int x0 = firstRect.left;
		int y0 = firstRect.top;
		int x1 = firstRect.right;
		int y1 = firstRect.bottom;

		int x = firstRect.right - firstRect.left;
		int y = firstRect.bottom - firstRect.top;

		int r = 0;
		for (int i = 0; i < numSquares; i++)
		{
			if (r > numRows)
			{
				x0 = firstRect.left;
				x1 = firstRect.right;
				y0 += y;
				y1 += y;
				r = 0;
			}
			grid[i] = Selection(x0 + xOff, y0 + yOff, x1 - xOff, y1 - yOff);
			x0 += x;
			x1 += x;
			
		}
	}
	RectI GetRect(int index)
	{
		return grid[index].GetRect();
	}

private:
	Selection grid[];
};