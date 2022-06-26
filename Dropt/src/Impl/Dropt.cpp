#include "../Include/LootContainers/LootBag/LootBag.h"
#include "../Include/LootContainers/LootTable/LootTable.h"
#include "../Include/Helper/MArray.h"
#include "../Include/Interface.h"


// Debug
#include <string>
#include <iostream>

int main()
{
	using namespace Dropt::impl;

	struct Armor
	{
		Armor(std::string _Name) :Name(_Name) {};
		std::string Name;
	};
	struct Weapon
	{

	};

//#ifdef __INTELLISENSE__
//#pragma diag_suppress 265
//#endif

	Dropt::Interface Inter;
	//Dropt::Interface::LootTypeFactory<Armor> Factory(Inter);

	



	{
		Armor* ChestPlate = new Armor("Chest Armor");
		Armor* Helmet = new Armor("Head Gear");
		Armor* LegArmor = new Armor("Leg Armor");
		Armor* Gloves = new Armor("Hand Armor");

		auto GloveLoot = Inter.CreateElementLoot_Weighted<Armor>("Hand Armor", Gloves, 121, 10);
		auto ChestPlateLoot = Inter.CreateElementLoot_Weighted<Armor>("Chestplate", ChestPlate, 50, 2);
		auto HelmetLoot = Inter.CreateElementLoot_Weighted<Armor>("Helmet", Helmet, 50, 6);
		auto LegLoot = Inter.CreateElementLoot_Weighted<Armor>("Leg", LegArmor, 2, 20);

		auto Table = Inter.CreateLootTable_Weighted<Armor>("PoopyTable", 50);
		auto Bag = Inter.CreateBag_Weighted<Armor, Variance::Chance>("PoopyBag", 50);

		Table->AddLoot(ChestPlateLoot);
		Table->AddLoot(HelmetLoot);
		Table->AddLoot(LegLoot);
		Table->AddLoot(GloveLoot);
		Table->GetSibling()->FinalizeLoot();

		std::list<Armor*> OutLoot;
		for (int i = 0; i < 40; ++i) {
			Table->GetLoot(OutLoot);
			std::cout << "Loot Obtained: ";
			std::cout << OutLoot.back()->Name << "\n";
		}

		std::cout << "\nTotal Loot Obtained: " << OutLoot.size();
	}
}