#pragma once
#include "SelectionMenu.h"
#include "MainWindow.h"
#include <cmath>
#include "Player.h"
#include "Entity.h"

class MenuProcessing
{
public:
	MenuProcessing(Player* player, std::vector<Entity*>* allEntities)
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
				//need to lock in when menus get updated so I can add a flag to run this
				sm->Update();
				DrawMenu(sm, gfx, font);
			}
		}
		for (auto sm : stack)
		{
			sm->Update();
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
			if (selectionMenu->DrawNames())
			{
				std::string s = selectionMenu->GetSelectionItems()[i].GetStr();
				centered ? font->DrawText(s, b.GetRect(), gfx) : font->DrawText(s, Vei2(b.GetRect().left, b.GetRect().top), gfx);
				
			}
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
				if (select->IsLeaf()) //if the selected SelectionItem contains an Element
				{
					if (select->pGetNextMenu() != nullptr)
					{
						//TODO
					}
					if (select->IsAction())
					{
						if (select->NeedsTarget() && targetingMenu == nullptr) //if the selected item needs a target and a targeting menu hasn't been made yet, make a targeting menu and store the action
						{
							SuspendProcess(Stack); //move the stack to storage, so it cant be interacted with (until we resolve the selected Element)
							Stack = {};
							hide_stored_stack = true;

							CreateTargetingMenu();
							leaf = select;
							select->InitNextMenu(targetingMenu);
							Stack.emplace_back(select->pGetNextMenu()); //new Stack is the nextMenu of SelectionItem select
							return Stack;
						}
					}
					if (select->IsEntity() && leaf != nullptr) //if the selected item is an entity and a leaf has been stored
					{
						player->QueueAction(leaf->pGetAction(), select->pGetEntity());

						//clean up
						delete targetingMenu;
						targetingMenu = nullptr;
						leaf = nullptr;

						ResetHighlights(Stack);
						Stack = ResolveProcess();

						SetForceClose();
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
							assert(true); //a selectionitem is clicked on that is not a leaf & has no nextmenu
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

					if (sm[iOffset].IsLeaf())
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
		for (Entity* entity : *allEntities)
		{
			//implement target filtering
			
			boxes.emplace_back((Rect<int>)entity->GetRect());
		}
		targetingMenu = new SelectionMenu(BoxMenu(boxes), allEntities);
		targetingMenu->DisableDrawNames();
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
	void ResetHighlights(std::vector<SelectionMenu*> Stack)
	{
		Stack[0]->pGetBoxMenu()->ResetSelectedTab();
		Stack[0]->ResetDefaultEntry();
		Stack[0]->pGetBoxMenu()->ResetHighlights();
		stored_stack[stored_stack.size() - 1]->pGetBoxMenu()->ResetSelectedTab();
		stored_stack[stored_stack.size() - 1]->ResetDefaultEntry();
		stored_stack[stored_stack.size() - 1]->pGetBoxMenu()->ResetHighlights();
		element_box->ResetSelectedTab();
		element_box->ResetHighlight();
		element_box = nullptr;
	}
private:
	Player* player;
	std::vector<Entity*>* allEntities; //make sure this is of entity type, or the static cast gon have a bad time
	Entity* target = nullptr;

	std::vector<SelectionMenu*> stored_stack;
	bool forceClose = false;
	bool hide_stored_stack = false;
	BoxMenu::BoxItem* element_box = nullptr;

	SelectionMenu::SelectionItem* leaf = nullptr;
	SelectionMenu* targetingMenu = nullptr;

	
};
