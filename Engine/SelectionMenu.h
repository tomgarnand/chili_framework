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
	{
		assert(rows > 0);
		RectI adjustedRect = RectI(Vei2(rect.left, rect.top) + GetTopOffsetMenu(), (Vei2(rect.right, rect.bottom) - GetTopOffsetMenu()));
		std::vector<RectI> MenuRects = SelectionRects(adjustedRect, font_height, int(input.size()), rows, 0, 0);
		int i = 0;
		for (auto s : input)
		{
			entries.emplace_back(s, MenuRects[i]);
			i++;
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
	std::vector<RectI> SelectionRects(RectI space, int height, int numSquares, int numRows, int xOff, int yOff)
	{
		std::vector<RectI> grid(numSquares);
		int x0 = space.left;
		int y0 = space.top;
		int x1 = space.left + ((space.right - space.left) / numRows);
		int y1 = space.top + height;

		int x = x1 - x0;
		int y = y1 - y0;

		int r = 0;
		for (int i = 0; i < numSquares; i++)
		{
			if (r >= numRows)
			{
				x0 = space.left;
				x1 = space.left + ((space.right - space.left) / numRows);
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

