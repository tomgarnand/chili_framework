#pragma once
#include "SelectionMenu.h"

namespace MenuElements
{
	class Item
	{
	public:
		Item(SelectionMenu& menu)
			:
			menu(menu)
		{}
		void operator()(SelectionMenu::Entry* entry)
		{
			//use
			menu.UpdateSelectionMenu(entry);
		}
	private:
		SelectionMenu& menu;
	};
	class Equip
	{
	public:
		Equip(SelectionMenu& menu, SelectionMenu& dest)
			:
			menu(menu),
			dest(dest)
		{}
		void operator()(SelectionMenu::Entry* entry)
		{
			dest.UpdateSelectionMenu(entry->GetStr(), dest.GetMenuRect());
			menu.UpdateSelectionMenu(entry);
		}
	private:
		SelectionMenu& menu;
		SelectionMenu& dest;
	};
	class Use
	{
	public:
		Use(SelectionMenu& menu)
			:
			menu(menu)
		{}
		void operator()(SelectionMenu::Entry* entry)
		{
			//use
		}
	private:
		SelectionMenu& menu;
	};
}

class MenuProcessing
{
public:
	MenuProcessing() = default;
	std::vector<SelectionMenu*> ProcessMenu(const Mouse::Event& e, std::vector<SelectionMenu*> Stack, MainWindow& wnd)
	{
		int it = 1;
		for (SelectionMenu* sm : Stack)
		{
			SelectionMenu::Entry* select = sm->ProcessMouse(e);

			if (e.GetType() == Mouse::Event::Type::LPress && select != nullptr)
			{
				if (select->GetStr() == "Game End")
				{
					wnd.Kill();
					break;
				}
				if (select->GetStr() == "Save" || select->GetStr() == "Load")
				{
					break;
				}

				bool inMenu = false;
				int i = 0;
				for (auto e : sm->GetEntries()) //for every Entry in a SelectionMenu of Stack
				{
					//check to see if Entry is in the Selection Menu
					if (*select == e) //if an Entry of a SelectionMenu layer is what was Selected
					{
						//Remove any SelectionMenu layers above the currently iterating SelectionMenu (sm)
						//If the current SelectionMenu layer (sm) is the top one, this does nothing
						int size = int(Stack.size());
						for (int i = 0; i < size - it; i++)
						{
							Stack.pop_back();
						}
						//Emplace the NextMenu the Selected Entry points to
						auto e_temp = e.pGetNextMenu();
						if (e.pGetNextMenu() != nullptr)
						{
							Stack.emplace_back(e_temp);
							while (e_temp->GetOpenDefault() != nullptr) //Add a chain of defaults, however long (c++23 replace this with a Appened_range vector funtion)
							{
								e_temp = e_temp->GetOpenDefault()->pGetNextMenu();
								Stack.emplace_back(e_temp);
							}
							sm->RememberDefaultEntry(select);
						}
						else //Use items, etc. If there's no NextMenu, that means it should be Processed
						{
							select->Process(select);
						}
					}
				}

			}
			it++;
		}
		return Stack;
	}
	
};
