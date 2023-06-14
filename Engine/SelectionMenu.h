#pragma once
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"
#include "Sound.h"
#include <string>
#include "Font.h"
#include <vector>
#include <functional>
#include "BoxMenu.h"
#include "GameContainer.h"



class SelectionMenu
{
public:
	class SelectionItem
	{
	using Element = Collection::Element;
	public:
		SelectionItem(std::string s, int iter)
			:
			s(s),
			iter(iter)
		{}
		SelectionItem(Element element, int iter)
			:
			element(element),
			iter(iter)
		{
			s = element.GetString();
		}
		~SelectionItem() = default;
		SelectionItem operator=(const SelectionItem& other)
		{
			s = other.s;
			return *this;
		}
		bool operator==(const SelectionItem& other) const
		{
			return (s == other.s);
		}
		bool operator!=(const SelectionItem& other) const
		{
			return (s != other.s);
		}

		std::string GetStr() const
		{
			if (this == nullptr)
			{
				return "";
			}
			return s;
		}
		int GetIter() const { return iter; }

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
		void Process()
		{
			element.Use();
		}

	private:
		std::string s;
		int iter;
		
		SelectionMenu* NextMenu = nullptr; //SelectionMenu that *this item leads to
		SelectionMenu* ParentMenu = nullptr; //SelectionMenu that *this item is contained in

		Element element;
	};
public:
	SelectionMenu(BoxMenu box, Collection collection)
		:
		boxmenu(box)
	{
		int i = 0;
		for (auto& s : collection.GetElements())
		{
			items.emplace_back(s, i);
			i++;
		}
		!collection.GetElements().empty() ? pLast = &items.back() : pLast = nullptr;

	}
	SelectionMenu(BoxMenu box, std::vector<std::string> input, std::vector<SelectionMenu*> NextMenu)
		:
		SelectionMenu(box, input)
	{
		int i = 0;
		for (auto& e : items)
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
	SelectionMenu(BoxMenu box, std::vector<std::string> input)
		:
		boxmenu(box)
	{
		int i = 0;
		for (auto& s : input)
		{
			items.emplace_back(s, i);
			i++;
		}
		!input.empty() ? pLast = &items.back() : pLast = nullptr;
	}

	SelectionItem* GetOpenDefault() const 
	{
		if (this == nullptr)
		{
			return nullptr;
		}
		return openDefaultEntry; 
	}
	std::vector<SelectionItem> GetSelectionItems() { return items; }
	SelectionItem* pGetSelectionItems() { return items.data(); }
	BoxMenu GetBoxMenu() { return boxmenu; }
	BoxMenu* pGetBoxMenu() { return &boxmenu; }
	int GetScrollOffset() { return ScrollOffset; }


	void UpdateScrollOffset(int dir)
	{
		if (items.size() > boxmenu.GetBoxItemRects().size())
		{
			ScrollOffset += dir;
			if (ScrollOffset < 0)
			{
				ScrollOffset = 0;
			}
			else if (ScrollOffset > (items.size() / boxmenu.GetNumRows()))
			{
				ScrollOffset = (items.size() / boxmenu.GetNumRows()) - 1;
			}
			//update SelectedTab
			for (int i = 0; i < boxmenu.GetBoxItems().size(); i++)
			{
				if (boxmenu.GetBoxItems()[i].IsSelectedTab())
				{
					boxmenu.ResetSelectedTab();
					boxmenu.GetBoxItems()[i + (ScrollOffset * boxmenu.GetNumRows())].IsSelectedTab();
					break;
				}
			}
			
		}
	}
	//expand when I decide the update processing
	void UpdateSelectionMenu(std::string input)
	{
		if (pLast == nullptr)
		{
			items.emplace_back(input, 0);
		}
		else
		{
			items.emplace_back(input, pLast->GetIter() + 1);
		}
		pLast = &items.back();
	}
	void UpdateSelectionMenu(Collection::Element element)
	{
		if (pLast == nullptr)
		{
			items.emplace_back(element, 0);
		}
		else
		{
			items.emplace_back(element, pLast->GetIter() + 1);
		}
		pLast = &items.back();
	}



	void SetDefaultEntry(int i) 
	{
		openDefaultEntry = &items[i];
		boxmenu.GetBoxItems()[i + (ScrollOffset * boxmenu.GetNumRows())].SetSelectedTab();
	}

	void RememberDefaultEntry(SelectionItem* sel)
	{
		openDefaultEntry = sel;
	}

private:
	std::vector<SelectionItem> items;
	BoxMenu boxmenu;

	SelectionItem* pLast = nullptr; //Pointer to last item element, used to efficiently add on additional entries
	SelectionMenu::SelectionItem* openDefaultEntry = nullptr; //For when a 'Tab' menu should be paired with another menu. Could be used in longer chains too

	int ScrollOffset = 0; //1 = 1 row down
};

