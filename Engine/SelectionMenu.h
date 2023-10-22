#pragma once
#include <string>
#include <vector>
#include "BoxMenu.h"
#include "Entity.h"
#include "Action.h"
#include "Equipment.h"




class SelectionMenu
{
public:
	class SelectionItem
	{
	public:
		SelectionItem(std::string s)
			:
			s(s)
		{
			leaf = false;
		}
		SelectionItem(Entity* entity)
			:
			entity(entity)
		{
			s = entity->GetName();
			leaf = true;
			isEntity = true;
		}
		SelectionItem(Action* action)
			:
			action(action)
		{
			s = action->GetName();
			leaf = true;
			isAction = true;
			needsTarget = true;
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
		Action* pGetAction() 
		{
			if (this == nullptr) { return nullptr; }
			return action; 
		}
		Entity* pGetEntity() 
		{
			if (this == nullptr) { return nullptr; }
			return entity;
		}
		bool IsLeaf() const { return leaf; }
		bool IsAction() const { return isAction; }
		bool IsEntity() const { return isEntity; }
		void InitNextMenu(SelectionMenu* inner)
		{
			NextMenu = inner;
		}
		void InitParentMenu(SelectionMenu* parent)
		{
			ParentMenu = parent;
		}
		SelectionMenu* pGetNextMenu() const { return NextMenu; }
		SelectionMenu* pGetParentMenu() const { return ParentMenu; }
		bool NeedsTarget() const { return needsTarget; }

	private:
		std::string s;
		
		SelectionMenu* NextMenu = nullptr; //SelectionMenu that *this item leads to
		SelectionMenu* ParentMenu = nullptr; //SelectionMenu that *this item is contained in

		bool leaf = false;
		bool needsTarget = false;
		Action* action = nullptr;
		bool isAction = false;
		Entity* entity = nullptr;
		bool isEntity = false;
		//Equipment* equip = nullptr;
		//bool isEquipment = false;
	};
public:
	SelectionMenu(BoxMenu box, std::vector<Entity*>* entities) //bottom level menu
		:
		boxmenu(box),
		entities(entities)
	{
		for (auto* e : *entities)
		{
			items.emplace_back(e);
		}
		isEntityMenu = true;

	}
	SelectionMenu(BoxMenu box, std::vector<Action*>* actions) //bottom level menu
		:
		boxmenu(box),
		actions(actions)
	{
		for (auto* e : *actions)
		{
			items.emplace_back(e);
		}
		isActionMenu = true;

	}
	SelectionMenu(BoxMenu box, std::vector<Equipment*>* equip)
		:
		boxmenu(box)
	{

	}
	SelectionMenu(BoxMenu box, std::vector<std::string> input, std::vector<SelectionMenu*> NextMenu) //top level menu
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
				e.InitNextMenu(NextMenu[i]);
				e.InitParentMenu(this);
			}
			i++;
		}
	}
	SelectionMenu(BoxMenu box, std::vector<std::string> input) //diaglog selection, standalone menu, or testing dead end
		:
		boxmenu(box)
	{
		for (auto& s : input)
		{
			items.emplace_back(s);
		}
	}
	void AddNextMenuToActionMenu(Action* action, SelectionMenu* nextMenu)
	{
		Update();
		for (SelectionItem& item : items)
		{
			if (item.IsAction())
			{
				if (action = item.pGetAction())
				{
					item.InitNextMenu(nextMenu);
				}
			}
		}
	}
	SelectionItem* GetOpenDefault() const 
	{
		if (this == nullptr){return nullptr;}
		return openDefaultEntry; 
	}
	std::vector<SelectionItem>& GetSelectionItems() { return items; }
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
	
	void Update()
	{
		if (IsActionMenu() && items.size() != actions->size()) //it would be real bad if a zero sum operation happened on a menu...
		{
			for (int i = 0; i < int(actions->size()); i++)
			{
				if (items.empty() || items[i].pGetAction() == nullptr || items[i].pGetAction() != (*actions)[i])
				{
					items.insert(items.begin() + i, SelectionItem((*actions)[i]));
					items[i].InitParentMenu(this);
				}
			}
		}
		else if (IsEntityMenu())
		{
			for (int i = 0; i < int(entities->size()); i++)
			{
				if (items.empty() || items[i].pGetEntity() == nullptr || items[i].pGetEntity() != (*entities)[i])
				{
					items.insert(items.begin() + i, SelectionItem((*entities)[i]));
					items[i].InitParentMenu(this);
				}
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
	bool IsActionMenu() const  { return isActionMenu;}
	bool IsEntityMenu() const { return isEntityMenu; }
private:
	std::vector<SelectionItem> items;
	BoxMenu boxmenu;
	std::vector<Entity*>* entities = nullptr;
	std::vector<Action*>* actions = nullptr;
	//std::vector<Equipment*>* actions = nullptr;

	bool isEntityMenu = false;
	bool isActionMenu = false;

	SelectionMenu::SelectionItem* openDefaultEntry = nullptr; //For when a 'Tab' menu should be paired with another menu. Could be used in longer chains too

	int ScrollOffset = 0; //1 = 1 row down
};

