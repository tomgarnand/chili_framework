#pragma once
#include "SelectionMenu.h"
#include "BoxMenu.h"
#include "Entity.h"
#include "Player.h"
#include "Action.h"
#include "Equipment.h"
#include "Font.h"
#include "Sound.h"
#include "Animation.h"

#include "DialogBox.h"



class GameAssets
{
public:
	GameAssets()
		
	{
		DialogBox::staticBoxRect = RectI(0, Graphics::ScreenWidth - 400, 500, Graphics::ScreenHeight - 1);
		DialogBox::staticFont = &fontV;

		mInventoryTabs.SetDefaultEntry(0);
		mEquipmentTabs.SetDefaultEntry(1);
		mAbilitiesTabs.SetDefaultEntry(0);
		//Collection_Equipment.InitPairedCollection(&Collection_Worn);
		//Collection_Worn.InitPairedCollection(&Collection_Equipment);

		entities->emplace_back(&npc1);
		entities->emplace_back(&npc2);
		Spells->emplace_back(Fireball);
		Inventory->emplace_back(Health_Potion);
		mInventory.AddNextMenuToActionMenu(Health_Potion, &ConfirmMenu);

		for (int i = 0; i < 4; i++)
		{
			link_animations.emplace_back(Animation(90, 90 * i, 90, 90, 4, link, 0.16f));
		}
		for (int i = 4; i < 8; i++)
		{
			link_animations.emplace_back(Animation(0, 90 * (i - 4), 90, 90, 1, link, 0.16f));
		}
		casting = new Animation(link_animations[7]);
		Fireball->AddSurface(ball);
		casting->AddExtraAnimation(fireball_animation);
		

		player.AddAction(WalkingLeft, link_animations[0]);
		player.AddAction(WalkingRight, link_animations[1]);
		player.AddAction(WalkingUp, link_animations[2]);
		player.AddAction(WalkingDown, link_animations[3]);
		player.AddAction(StandingLeft, link_animations[4]);
		player.AddAction(StandingRight, link_animations[5]);
		player.AddAction(StandingUp, link_animations[6]);
		player.AddAction(StandingDown, link_animations[7]);
		player.AddAction(Action::Idle, link_animations[7]);

		player.AddAction(Fireball, *casting);

		npc1.AddAction(Action::Idle, link_animations[7]);
		npc2.AddAction(Action::Idle, link_animations[7]);

	}

private:
public:
	std::string current_map = "testmap"; //need to fix


	std::vector<Entity*>* entities = new std::vector<Entity*>;
	std::vector<Action*>* Spells = new std::vector<Action*>;
	std::vector<Action*>* Inventory = new std::vector<Action*>;
	std::vector<Action*>* Important = new std::vector<Action*>;
	std::vector<Equipment*>* Gear = new std::vector<Equipment*>;
	std::vector<Equipment*>* Worn = new std::vector<Equipment*>;


	static constexpr Color BoxColor = Colors::Blue;
	Font fontV = Font("Images//Fixedsys16x28.bmp", Colors::White);
	Font* font = &fontV;
	int font_height = fontV.GetGlyphHeight();

	DialogBox dialogbox = DialogBox("Testing testing 123");

#pragma region Menus

	//BoxMenus
	BoxMenu BoxMenu_MainMenu = BoxMenu(GetMainMenuRect(), 1, font_height, false);
	BoxMenu BoxMenu_SubMenuTabs2 = BoxMenu(GetSubMenuTabsRect(), 2, font_height, true);
	BoxMenu BoxMenu_SubMenuTabs3 = BoxMenu(GetSubMenuTabsRect(), 3, font_height, true);
	BoxMenu BoxMenu_SubMenu = BoxMenu(GetSubMenuRect(), 2, font_height, true);
	BoxMenu BoxMenu_ConfirmMenu = BoxMenu(GetConfirmMenuRect(), 1, font_height, false);


	//SelectionMenus
	SelectionMenu mInventory = SelectionMenu(BoxMenu_SubMenu, Inventory);
	SelectionMenu mGear = SelectionMenu(BoxMenu_SubMenu, Gear);
	SelectionMenu mWorn = SelectionMenu(BoxMenu_SubMenu, Worn);
	SelectionMenu mSpells = SelectionMenu(BoxMenu_SubMenu, Spells);
	SelectionMenu mImportant = SelectionMenu(BoxMenu_SubMenu, Important);
	SelectionMenu mEquipmentTabs = SelectionMenu(BoxMenu_SubMenuTabs2, { "Worn", "Inventory" }, { &mWorn, &mGear });
	SelectionMenu mInventoryTabs = SelectionMenu(BoxMenu_SubMenuTabs3, { "Items", "Equipment", "Important" }, { &mInventory, &mGear, &mImportant });
	SelectionMenu mAbilitiesTabs = SelectionMenu(BoxMenu_SubMenuTabs3, { "Skills", "Spells", "Passives" }, { &mSpells, nullptr, nullptr });
	SelectionMenu mMainMenu = SelectionMenu(BoxMenu_MainMenu, { "Inventory","Equipment","Abilities","Save","Load","Game End" }, { &mInventoryTabs, &mEquipmentTabs, &mAbilitiesTabs });


	SelectionMenu ConfirmMenu = SelectionMenu(BoxMenu_ConfirmMenu, { "Yes", "No" }, { nullptr, nullptr });

	SelectionMenu* pMainMenu = &mMainMenu;

#pragma endregion

#pragma region Entities
	Surface link = Surface("Images//link90x90.bmp");
	std::vector<Animation> link_animations;
	Player player = Player("link", link, Attributes(), { current_map, {100,100} });
	Player* pPlayer = &player;

	Entity npc1 = Entity("npc1", link, Attributes(), { current_map, {200,200} });
	Entity npc2 = Entity("npc2", link, Attributes(), { current_map, {400,200} });


	Surface ball = Surface("Assets//ARWFlame.bmp");
	Animation* fireball_animation = new Animation(0, 0, 24, 24, 7, ball, 0.16f);
	Animation* casting;

#pragma endregion
#pragma region Moves
	Application* moveRight = new Application(Effect(EffectCategory::SubTick, EffectType::MoveRight, 0, 1.0f));
	Action* WalkingRight = new Action("WalkingRight", { -1,moveRight });

	Application* moveLeft = new Application(Effect(EffectCategory::SubTick, EffectType::MoveLeft, 0, 1.0f));
	Action* WalkingLeft = new Action("WalkingLeft", { -1,moveLeft });

	Application* moveUp = new Application(Effect(EffectCategory::SubTick, EffectType::MoveUp, 0, 1.0f));
	Action* WalkingUp = new Action("WalkingUp", { -1,moveUp });

	Application* moveDown = new Application(Effect(EffectCategory::SubTick, EffectType::MoveDown, 0, 1.0f));
	Action* WalkingDown = new Action("WalkingDown", { -1,moveDown });

	Application* standRight = new Application(Effect(EffectCategory::SubTick, EffectType::StandRight, 0, 1.0f));
	Action* StandingRight = new Action("StandingRight", { -1,standRight });

	Application* standLeft = new Application(Effect(EffectCategory::SubTick, EffectType::StandLeft, 0, 1.0f));
	Action* StandingLeft = new Action("StandingLeft", { -1,standLeft });

	Application* standUp = new Application(Effect(EffectCategory::SubTick, EffectType::StandUp, 0, 1.0f));
	Action* StandingUp = new Action("StandingUp", { -1,standUp });

	Application* standDown = new Application(Effect(EffectCategory::SubTick, EffectType::StandDown, 0, 1.0f));
	Action* StandingDown = new Action("StandingDown", { -1, standDown });

	Application* Burn = new Application(Effect(EffectCategory::Active, EffectType::Burn, 10, 1.0f));
	Action* Fireball = new Action("Fireball", { { 1,Burn } });

	Application* RestoreHealth = new Application(Effect(EffectCategory::Active, EffectType::Heal, 0, 5.0f));
	Action* Health_Potion = new Action("Health Potion", { {1,RestoreHealth} });




#pragma endregion

	


	static RectI GetMainMenuRect()
	{
		return { 40,240,40,380 };
	}
	static RectI GetSubMenuRect()
	{
		return { 244,800,124,380 };
	}
	static RectI GetSubMenuTabsRect()
	{
		return { 244,800,40,120 };
	}
	static RectI GetConfirmMenuRect()
	{
		return { 400, 500, 600, 700 };
	}
	//Sound hover = { L"Sounds//menu_boop.wav" };



}; 
