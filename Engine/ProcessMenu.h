#pragma once
#include "SelectionMenu.h"
#include "GameContainer.h"
#include "GUI.h"
#include "MainWindow.h"
#include <cmath>

class MenuProcessing
{
public:
	MenuProcessing() = default;

	void DrawGUI(std::vector<SelectionMenu*> stack, Graphics& gfx, Font* font)
	{
		if (!stored_stack.empty()) //if processing is suspended for a stack of menus
		{
			for (auto sm : stored_stack)
			{
				DrawMenu(sm, gfx, font);
			}
		}
		for (auto sm : stack)
		{
			DrawMenu(sm, gfx, font);
		}
	}

	//draw a single selection menu and all it's items
	void DrawMenu(SelectionMenu* selectionMenu, Graphics& gfx, Font* font)
	{
		BoxMenu boxMenu = selectionMenu->GetBoxMenu();
		gfx.DrawRect(boxMenu.GetMenuRect(), boxMenu.GetColor());
		int i = selectionMenu->GetScrollOffset() * boxMenu.GetNumRows();
		bool centered = boxMenu.IsCentered();
		int r = 0;
		for (i; i < std::min(selectionMenu->GetBoxMenu().GetBoxItems().size(), selectionMenu->GetSelectionItems().size()); i++)
		{
			BoxMenu::BoxItem b = boxMenu.GetBoxItems()[r];
			if (b.IsHighlighted() || b.IsSelectedTab())
			{
				gfx.DrawRect(b.GetRect(), BoxMenu::BoxItem::highlightColor);
			}
			std::string s = selectionMenu->GetSelectionItems()[i].GetStr();
			centered ? font->DrawText(s, b.GetRect(), gfx) : font->DrawText(s, Vei2(b.GetRect().left, b.GetRect().top), gfx);
			r++;
		}
	}



	//process the MenuStack
	std::vector<SelectionMenu*> ProcessMenu(const Mouse::Event& e, std::vector<SelectionMenu*> Stack, MainWindow& wnd)
	{
		int it = 1;

		for (SelectionMenu* sm : Stack)
		{
			SelectionMenu::SelectionItem* select = ProcessMouse(e, sm);

			

			if (e.GetType() == Mouse::Event::Type::LPress && select != nullptr)
			{
				if (select->pGetEle() != nullptr)
				{
					SuspendProcess(Stack); //design choice to make menu untargetable when an item is clicked. Could be changed to avoid (hitting Escape?) to cancel
					//if I wanted to make the menu still interactable, I would just need to remove the Element from storage in *this when the 'confirmation menu' is unselected below
					//of course, it would be tough (atm) to implement suspending for select menus/elements
					Stack = {};
					Stack.emplace_back(select->pGetNextMenu());
					element = select->pGetEle();
					return Stack;
				}
				

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
				for (auto e : sm->GetSelectionItems()) //for every Entry in a SelectionMenu of Stack
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
						else //this occurs when the stack has been suspended, but now a menu has been selected that doesnt lead anywhere
						{
							
							//element->Use(target);
							//delete element;
							element = nullptr;
							Stack[0]->pGetBoxMenu()->ResetSelectedTab();
							Stack[0]->ResetDefaultEntry();
							Stack[0]->pGetBoxMenu()->ResetHighlights();
							Stack = ResolveProcess();
							element_box->ResetSelectedTab();
							element_box->ResetHighlight();
							element_box = nullptr;
							return Stack;
						}
					}
				}

			}
			it++;
		}
		return Stack;
	}
	SelectionMenu::SelectionItem* ProcessMouse(const Mouse::Event& e, SelectionMenu* check)
	{
		switch (e.GetType())
		{
		case Mouse::Event::Type::Move:
		{
			bool oneHit = false;
			for (int i = 0; i < check->GetSelectionItems().size(); i++)
			{
				BoxMenu::BoxItem* n = check->pGetBoxMenu()->pGetBoxItems();
				if (n[i].IsHit(e.GetPos()))
				{
					oneHit = true;
					if (!n[i].IsHighlighted())
					{
						check->pGetBoxMenu()->ResetHighlights();
						n[i].SetHighlight();
						//hover.Play();
					}

					return &check->GetSelectionItems()[(check->GetScrollOffset() * check->GetBoxMenu().GetNumRows()) + i];
				}
			}
			if (!oneHit && !check->GetBoxMenu().MenuIsHit(e.GetPos()))
			{
				//for (auto& n : check->GetBoxMenu().GetBoxItems())
				for (int i = 0; i < check->GetSelectionItems().size(); i++)
				{
					BoxMenu::BoxItem* n = check->pGetBoxMenu()->pGetBoxItems();
					if (!n[i].IsSelectedTab())
					{
						n[i].ResetHighlight();
					}
					else
					{
						n[i].SetHighlight();
					}
				}
			}
			break;
		}
		case Mouse::Event::Type::LPress:
		{
			BoxMenu::BoxItem* bm = check->pGetBoxMenu()->pGetBoxItems();
			SelectionMenu::SelectionItem* sm = check->pGetSelectionItems();
			for (int i = 0; i < check->GetSelectionItems().size(); i++)
			{
				//Iteration:
				//i iterates the BoxItems in BoxMenu bm
				//iOffset iterates the SelectionItems in SelectionMenu sm
				

				if (bm[i].IsHit(e.GetPos()))
				{
					int iOffset = (check->GetScrollOffset() * check->GetBoxMenu().GetNumRows()) + i;
					check->pGetBoxMenu()->ResetSelectedTab();
					bm[i].SetSelectedTab();

					if (sm[iOffset].pGetEle() != nullptr)
					{
						element_box = &bm[i];
					}

					SelectionMenu::SelectionItem* NextOpenDefault = sm[iOffset].pGetNextMenu()->GetOpenDefault();
					if (NextOpenDefault != nullptr)
					{
						SelectionMenu::SelectionItem* NextOpenDefault = sm[iOffset].pGetNextMenu()->GetOpenDefault();
						SelectionMenu* NextMenu = NextOpenDefault->pGetParentMenu();
						int m = 0;
						for (auto& e : NextMenu->GetSelectionItems())
						{
							if (e == *NextOpenDefault) //or e == *opendef
							{
								NextMenu->pGetBoxMenu()->ResetSelectedTab();
								break;
							}
							m++;
						}
						NextMenu->pGetBoxMenu()->pGetBoxItems()[m].SetSelectedTab();
					}
					
					return &sm[iOffset];

				}
			}
			break;
		}
		case Mouse::Event::Type::WheelUp:
		{
			check->UpdateScrollOffset(-1);
			break;
		}
		case Mouse::Event::Type::WheelDown:
		{
			check->UpdateScrollOffset(1);
			break;
		}

		}

		return nullptr;
	}
	void SuspendProcess(std::vector<SelectionMenu*> Stack)
	{
		stored_stack = Stack;
	}
	std::vector<SelectionMenu*> ResolveProcess()
	{
		auto temp = stored_stack;
		
		stored_stack.clear();
		return temp;
	}

private:
	std::vector<SelectionMenu*> stored_stack;

	Collection::Element* element = nullptr;
	BoxMenu::BoxItem* element_box = nullptr;

};
