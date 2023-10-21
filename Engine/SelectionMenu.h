#pragma once
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"
#include "Sound.h"
#include <string>
#include "Font.h"
#include <vector>
#include <functional>
#include <cmath>
#include "BoxMenu.h"
#include "GameContainer.h"



class SelectionMenu
{
public:
	class SelectionItem
	{
	using Element = Collection::Element;
	public:
		SelectionItem(std::string s)
			:
			s(s)
		{}
		SelectionItem(Element* element)
			:
			element(element)
		{
			s = element->GetString();
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
		Element* pGetEle() 
		{
			if (this == nullptr)
			{
				return nullptr;
			}
			return element; 
		}
		void InitInnerMenu(SelectionMenu* inner)
		{
			NextMenu = inner;
		}
		void InitParentMenu(SelectionMenu* parent)
		{
			ParentMenu = parent;
		}
		SelectionMenu* pGetNextMenu() const { return NextMenu; }
		SelectionMenu* pGetParentMenu() const { return ParentMenu; }

	private:
		std::string s;
		
		SelectionMenu* NextMenu = nullptr; //SelectionMenu that *this item leads to
		SelectionMenu* ParentMenu = nullptr; //SelectionMenu that *this item is contained in

		Element* element = nullptr;
	};
public:
	SelectionMenu(BoxMenu box, Collection* collection)
		:
		boxmenu(box),
		collection(collection)
	{
		for (auto& e : collection->pGetpElements())
		{
			items.emplace_back(e);
		}


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
		for (auto& s : input)
		{
			items.emplace_back(s);
		}
	}
	SelectionMenu* pGetSelectionMenu()
	{
		return this;
	}
	Collection* pGetCollection()
	{
		return collection;
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
			else if (ScrollOffset > (int(items.size()) / boxmenu.GetNumRows()))
			{
				ScrollOffset = (int(items.size()) / boxmenu.GetNumRows()) - 1;
			}
			//update SelectedTab
			for (int i = 0; i < int(boxmenu.GetBoxItems().size()); i++)
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
	bool NeedsUpdate()
	{
		if (this == nullptr)
		{
			return false;
		}
		if (collection == nullptr)
		{
			return false;
		}
		else
		{
			return collection->IsQueueUpdate();
		}
	}
	void UpdateFromCollection()
	{

		auto ele = collection->pGetpElements();
		
		for (int i = int(ele.size()) - 1; i >=0 ; i--)
		{
			if (items.size() < ele.size())
			{
				items.emplace(items.begin(), ele[i]);
				items.front().InitParentMenu(this);
				items.front().InitInnerMenu(ele[i]->pGetMenu());
			}
			else if (ele.size() < items.size())
			{
				items.pop_back();
			}
			else if (items[i].pGetEle() != ele[i])
			{
				items[i] = SelectionItem(ele[i]);
				items[i].InitParentMenu(this);
				items[i].InitInnerMenu(ele[i]->pGetMenu());
			}
		}
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
	void ResetDefaultEntry()
	{
		openDefaultEntry = nullptr;
	}

private:
	std::vector<SelectionItem> items;
	BoxMenu boxmenu;
	Collection* collection = nullptr;

	SelectionMenu::SelectionItem* openDefaultEntry = nullptr; //For when a 'Tab' menu should be paired with another menu. Could be used in longer chains too

	int ScrollOffset = 0; //1 = 1 row down
	bool QueueUpdate = false;
};

