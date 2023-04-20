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
	SelectionMenu(const RectI rect, std::vector<std::string> input)
	{
		Vei2 topLeft = Vei2(rect.left, rect.top) + GetTopOffsetMenu();
		Vei2 botRight = Vei2(rect.right - GetTopOffsetMenu().x, rect.top + GetTopOffsetMenu().y) ;
		auto entryRect = RectI(topLeft,botRight); //top line of first rect, ready to be scaled down with font
		
		
		for (auto s : input)
		{	
			entryRect.bottom += verticalSpacing + font_height;
			entries.emplace_back(s, entryRect);
			//iterate rect after
			entryRect.top += verticalSpacing + font_height;
			
		}
	}
	// returns Size::Something when an entry is clicked, otherwise returns Size::Invalid
	std::string ProcessMouse(const Mouse::Event& e)
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