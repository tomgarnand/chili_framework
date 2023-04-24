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
			menu.UpdateSelectionMenu(entry);
		}
	private:
		SelectionMenu& menu;
	};
	class Equip
	{

	};
}
