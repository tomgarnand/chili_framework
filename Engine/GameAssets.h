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

#include <numbers>


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
			link_moving_animations.emplace_back(Animation(90, 90 * i, 90, 90, 4, link, 0.16f));
		}
		for (int i = 4; i < 8; i++)
		{
			link_standing_animations.emplace_back(Animation(0, 90 * (i - 4), 90, 90, 1, link, 0.16f));
		}
		casting = new Animation(link_standing_animations[3]);
		Fireball->AddSurface(ball);
		casting->AddExtraAnimation(fireball_animation);
		
		float pi = std::numbers::pi;
		player.AddAction(Move, Animation(link_moving_animations, { pi, 0.0f, ((3.0f * pi) / 2.0f), (pi / 2.0f) })); //left right up down - via spritesheet
		player.AddAction(Action::Idle, Animation(link_standing_animations, { pi, 0.0f, ((3.0f * pi) / 2.0f), (pi / 2.0f) }));

		

		player.AddAction(Fireball, *casting);

		npc1.AddAction(Action::Idle, link_standing_animations[3]);
		npc2.AddAction(Action::Idle, link_standing_animations[3]);

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
	std::vector<Animation> link_moving_animations;
	std::vector<Animation> link_standing_animations;
	Player player = Player("link", link, Attributes(), { current_map, {100,100} });
	Player* pPlayer = &player;

	Entity npc1 = Entity("npc1", link, Attributes(), { current_map, {200,200} });
	Entity npc2 = Entity("npc2", link, Attributes(), { current_map, {400,200} });


	Surface ball = Surface("Assets//ARWFlame.bmp");
	Animation* fireball_animation = new Animation(0, 0, 24, 24, 7, ball, 0.16f);
	Animation* casting;

#pragma endregion
#pragma region Moves

	Application* move = new Application(Effect(EffectCategory::SubTick, EffectType::Move, 0, 1.0f));
	Action* Move = new Action("Move", { -1,move });


	Application* Burn = new Application(Effect(EffectCategory::Active, EffectType::Burn, 10, 1.0f));
	Action* Fireball = new Action("Fireball", 10, { { 0,Burn } }, {}, 100.0f );

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
