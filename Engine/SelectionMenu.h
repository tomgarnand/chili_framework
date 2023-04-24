#pragma once
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"
#include "Sound.h"
#include <string>
#include <map>
#include "Font.h"
#include <vector>
#include <functional>



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
			if (!Centered)
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
		void InitInnerMenu(SelectionMenu* inner)
		{
			NextMenu = inner;
		}
		void InitParentMenu(SelectionMenu* parent)
		{
			ParentMenu = parent;
		}
		SelectionMenu* pGetNextMenu()
		{
			return NextMenu;
		}
		SelectionMenu* pGetParentMenu()
		{
			return ParentMenu;
		}
		void SetProcess(std::function<void(Entry* entry)> Func)
		{
			ProcessFunc = Func;
		}
		void Process(Entry* entry)
		{
			ProcessFunc(entry);
		}
		std::function<void(Entry* entry)> GetProcessFunc()
		{
			return  ProcessFunc;
		}
	private:
		Font& font = Font("Images//Fixedsys16x28.bmp", Colors::White);
		static constexpr int highlightThickness = 6;
		static constexpr Color highlightColor = Colors::Yellow;

		RectI rect;
		bool highlighted = false;
		bool Centered = false;

		std::string s;
		
		SelectionMenu* NextMenu = nullptr; //SelectionMenu that *this entry leads to
		SelectionMenu* ParentMenu = nullptr; //SelectionMenu that *this entry is contained in

		std::function<void(Entry* entry)> ProcessFunc = nullptr;
	};
public:
	SelectionMenu() = default;
	SelectionMenu(const RectI MenuRect, std::vector<std::string> input, int rows, bool center, std::function<void(Entry* entry)> Func)
		:
		SelectionMenu(MenuRect, input, rows, center)
	{
		ProcessFunc = Func;
		for (auto& e : entries)
		{
			e.SetProcess(Func);
		}
	}
	SelectionMenu(const RectI MenuRect, std::vector<std::string> input, int rows, bool center, std::vector<SelectionMenu*> NextMenu)
		:
		SelectionMenu(MenuRect, input, rows, center)
	{
		int i = 0;
		for (auto& e : entries)
		{
			if (i >= NextMenu.size())
			{
				break;
			}
			if (NextMenu[i] != nullptr)
			{
				e.InitInnerMenu(NextMenu[i]);
				e.InitParentMenu(this);
			}
			i++;
		}
		//we only ever need to init one layer of selectionMenu's, because they are reverse-initialized, starting with a empty (front level, intended to be modified in game) selectionmenu
	}
	SelectionMenu(const RectI MenuRect, std::vector<std::string> input, int rows, bool center)
		:
		SelectionMenu(MenuRect, input, rows)
	{
		if (center)
		{
			for (auto& e : entries)
			{
				e.SetCentered();
			}
		}
	}
	SelectionMenu(const RectI MenuRect, std::vector<std::string> input, int rows)
		:
		rows(rows),
		MenuRect(MenuRect)
	{
		assert(rows > 0);
		RectI adjustedRect = RectI(Vei2(MenuRect.left, MenuRect.top) + GetTopOffsetMenu(), (Vei2(MenuRect.right, MenuRect.bottom) - GetTopOffsetMenu()));
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
	SelectionMenu(SelectionMenu& other)
	{
		for (auto e : other.entries)
		{
			
			entries.emplace_back(e.GetStr(), e.GetRect());
			entries.back().InitParentMenu(e.pGetParentMenu());
			entries.back().InitInnerMenu(e.pGetNextMenu());
			if (e.IsHighlighted())
			{
				entries.back().SetHighlight();
			}
			if (other.Centered)
			{
				entries.back().SetCentered();
			}
		}
		Centered = other.Centered;
		rows = other.rows;
		pLast = other.pLast;
		MenuRect = other.MenuRect;
	}
	void UpdateSelectionMenu(std::string input, RectI guiRect) //for some reason this is really slow
	{
		if (pLast != nullptr)
		{
			entries.emplace_back(input, GetNewRect());
			entries.back().SetProcess(ProcessFunc);
			pLast = &entries.back();
		}
		else
		{
			entries.emplace_back(input, SelectionRects(RectI(Vei2(guiRect.left, guiRect.top) + GetTopOffsetMenu(), Vei2(guiRect.right, guiRect.bottom) - GetTopOffsetMenu()), 1).front());
			entries.back().SetProcess(ProcessFunc);
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
	Entry* ProcessMouse(const Mouse::Event& e) 
	{
		switch (e.GetType())
		{
		case Mouse::Event::Type::Move:
			for (auto& n : entries)
			{
				if (n.IsHit(e.GetPos()))
				{
					if (!n.IsHighlighted())
					{
						ResetHighlights();
						n.SetHighlight();
						hover.Play();
					}

					return &n;
				}
			}
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
	std::vector<Entry> GetEntries()
	{
		return entries;
	}
	void CreateDefaultEntry(int i)
	{
		openDefaultEntry = &entries[i];
	}
	RectI GetMenuRect()
	{
		return MenuRect;
	}
	Entry* GetOpenDefault()
	{
		return openDefaultEntry;
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
	Font font = Font("Images//Fixedsys16x28.bmp", Colors::White);
	int font_height = font.GetGlyphHeight();
	
	std::vector<Entry> entries; 
	Entry* pLast = nullptr; //Pointer to last entry element, used to efficiently add on additional entries
	
	RectI MenuRect;
	int rows;
	int xOff = 0;
	int yOff = 0;
	bool Centered = false;
	
	SelectionMenu::Entry* openDefaultEntry = nullptr; //For when a 'Tab' menu should be paired with another menu. Could be used in longer chains too
	std::function<void(Entry* entry)> ProcessFunc = nullptr;
};

