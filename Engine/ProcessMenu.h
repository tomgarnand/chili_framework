#pragma once
#include "SelectionMenu.h"

namespace ProcessMenu
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
