#pragma once
#include "SelectionMenu.h"
#include "GameContainer.h"
#include "GUI.h"
#include "MainWindow.h"
#include <cmath>

class MenuProcessing
{
public:
	MenuProcessing(Player* player, Collection* allEntities)
		:
		player(player),
		allEntities(allEntities)
	{}

	void DrawGUI(std::vector<SelectionMenu*> stack, Graphics& gfx, Font* font)
	{
		if (!stored_stack.empty() && !hide_stored_stack) //if processing is suspended for a stack of menus
		{
			for (auto sm : stored_stack)
			{
				//everytime we draw a menu, we should make sure none of the SelectionItems need to be updated
				//pretty lightweight, just looks at a boolean in the selectionMenu's associated collection
				if (sm->NeedsUpdate())
				{
					sm->UpdateFromCollection();
				}
				DrawMenu(sm, gfx, font);
			}
		}
		for (auto sm : stack)
		{
			if (sm->NeedsUpdate()) //as above, so below
			{
				sm->UpdateFromCollection();
			}
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
				if (select->pGetEle() != nullptr) //if the selected SelectionItem contains an Element
				{
					if (targetingMenu != nullptr) //Collection::eEntity* entity = dynamic_cast<Collection::eEntity*>(select->pGetEle()))
					{
						Collection::eEntity* e = static_cast<Collection::eEntity*>(select->pGetEle());
						target = e->pGetEntity();
						element->Use(player, target);

						delete targetingMenu;
						targetingMenu = nullptr;
						collection->RemoveElement(element); //only if removeafteruse is true
						element = nullptr;
						collection = nullptr;
						Stack[0]->pGetBoxMenu()->ResetSelectedTab();
						Stack[0]->ResetDefaultEntry();
						Stack[0]->pGetBoxMenu()->ResetHighlights();
						Stack = ResolveProcess();
						element_box->ResetSelectedTab();
						element_box->ResetHighlight();
						element_box = nullptr;
						SetForceClose();
						return Stack;
					}
					else
					{
						SuspendProcess(Stack); //move the stack to storage, so it cant be interacted with (until we resolve the selected Element)

						//design choice to make menu untargetable when an item is clicked. Could be changed to avoid (hitting Escape?) to cancel
						//if I wanted to make the menu still interactable, I would just need to remove the Element from storage in *this when the 'confirmation menu' is unselected below
						//of course, it would be tough (atm) to implement suspending for select menus/elements

						Stack = {};
						element = select->pGetEle();
						collection = select->pGetParentMenu()->pGetCollection(); //game container for SelectionItem select's element
						if (element->NeedsTarget() && element->pGetMenu() == nullptr) //no functionality if the element has a specific menu rn, not even sure they should be holding a menu in the first place
						{
							hide_stored_stack = true;
							CreateTargetingMenu();
							select->InitInnerMenu(targetingMenu);
						}

						Stack.emplace_back(select->pGetNextMenu()); //new Stack is the nextMenu of SelectionItem select (could be nullptr)

						return Stack;
					}
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

							delete targetingMenu;
							collection->RemoveElement(element); //only if removeafteruse is true
							element = nullptr;
							collection = nullptr;
							Stack[0]->pGetBoxMenu()->ResetSelectedTab();
							Stack[0]->ResetDefaultEntry();
							Stack[0]->pGetBoxMenu()->ResetHighlights();
							Stack = ResolveProcess();
							element_box->ResetSelectedTab();
							element_box->ResetHighlight();
							element_box = nullptr;
							SetForceClose();
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

					return &check->GetSelectionItems()[((int)check->GetScrollOffset() * check->GetBoxMenu().GetNumRows()) + i];
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
	void CreateTargetingMenu() //current map? world? player?
	{
		std::vector<RectI> boxes;
		for (Collection::Element* elemPtr : *allEntities) 
		{
			//implement target filtering
			Collection::eEntity* e = static_cast<Collection::eEntity*>(elemPtr);
			boxes.emplace_back(e->pGetEntity()->GetHitBox());
		}
		targetingMenu = new SelectionMenu(BoxMenu(boxes), allEntities);
		//element->GiveTargetingMenu(targetingMenu);
	}
	bool ForceClose()
	{
		bool temp = forceClose;
		forceClose = false;
		return temp;
	}
	void SetForceClose()
	{
		forceClose = true;
	}

private:
	Player* player;

	Collection* allEntities; //make sure this is of entity type, or the static cast gon have a bad time
	Entity* target = nullptr;
	std::vector<SelectionMenu*> stored_stack;
	bool hide_stored_stack = false;
	SelectionMenu* targetingMenu = nullptr;

	Collection::Element* element = nullptr;
	BoxMenu::BoxItem* element_box = nullptr;
	Collection* collection = nullptr;

	bool forceClose = false;
};
