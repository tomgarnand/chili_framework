#pragma once
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"
#include "Sound.h"
#include <string>
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
		~Entry() = default;
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

		void Draw(Graphics& gfx, Font& font) const;

		void SetSelectedTab() {SelectedTab = true;}
		void ResetSelectedTab() {SelectedTab = false;}
		bool IsSelectedTab() const {return SelectedTab;}

		bool IsHit(const Vei2& pt) const{return rect.left < pt.x&& rect.right > pt.x && rect.top < pt.y&& rect.bottom > pt.y;}

		void ResetHighlight(){highlighted = false;}
		void SetHighlight(){highlighted = true;}
		bool IsHighlighted() const {return highlighted;}

		std::string GetStr() const
		{
			if (this == nullptr)
			{
				return "";
			}
			return s;
		}

		RectI GetRect() const {return rect;}
		
		void SetCentered() {Centered = true;}

		RectI SwapRects(RectI temp);
		void InitInnerMenu(SelectionMenu* inner)
		{
			NextMenu = inner;
		}
		void InitParentMenu(SelectionMenu* parent)
		{
			ParentMenu = parent;
		}
		SelectionMenu* pGetNextMenu() const
		{
			return NextMenu;
		}
		SelectionMenu* pGetParentMenu() const
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

	private:
		static constexpr Color highlightColor = Colors::Yellow;

		RectI rect;
		bool Centered = false;
		bool highlighted = false;
		bool SelectedTab = false;

		std::string s;
		
		SelectionMenu* NextMenu = nullptr; //SelectionMenu that *this entry leads to
		SelectionMenu* ParentMenu = nullptr; //SelectionMenu that *this entry is contained in

		std::function<void(Entry* entry)> ProcessFunc = nullptr;
	};
public:
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
	{}
	
	void Draw(Graphics& gfx) const;

	RectI GetMenuRect() const { return MenuRect; }
	Entry* GetOpenDefault() const { return openDefaultEntry; }
	std::vector<Entry> GetEntries() { return entries; }

	void UpdateSelectionMenu(std::string input, RectI guiRect);
	void UpdateSelectionMenu(Entry* entry);

	Entry* ProcessMouse(const Mouse::Event& e);

	void SetDefaultEntry(int i); 
	void RememberDefaultEntry(Entry* sel);
	

private:

	bool MenuIsHit(const Vei2& pt) const
	{
		return MenuRect.left + GetTopOffsetMenu().x < pt.x&& MenuRect.right - GetTopOffsetMenu().x > pt.x && MenuRect.top + GetTopOffsetMenu().y < pt.y&& MenuRect.bottom - GetTopOffsetMenu().y > pt.y;
	}
	void ResetHighlights();
	std::vector<RectI> SelectionRects(RectI space, int numSquares);
	RectI GetNewRect();
private:
	//Spacing between top corner of GUI Rect and Text
	static Vei2 GetTopOffsetMenu() 
	{
		return { 20, 20 };
	}
	static constexpr int verticalSpacing = 4;
	Sound hover = { L"Sounds//menu_boop.wav" };
	Font fontV = Font("Images//Fixedsys16x28.bmp", Colors::White);
	Font& font = fontV;
	int font_height = fontV.GetGlyphHeight();

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

