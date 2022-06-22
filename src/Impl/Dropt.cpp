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
		Armor Chestplate("Chest Armor");
		Armor Helmet("Head Gear");
		Armor LegArmor("Leg Armor");
		Armor Gloves("Hand Armor");

		
		ElementLoot<Armor, Variance::Chance, Obtainabilities::Unique> ChestPlateLoot(&Chestplate);
		ElementLoot<Armor, Variance::Chance, Obtainabilities::Unique> HelmetLoot(&Helmet);
		ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> LegLoot(&LegArmor);
		ElementLoot<Armor, Variance::Chance, Obtainabilities::Unique> HandLoot(&Gloves);

		ChestPlateLoot.SetWeight(2);
		HelmetLoot.SetWeight(5);
		LegLoot.SetWeight(20);
		HandLoot.SetWeight(8);

		auto Table = Inter.CreateLootTable_Weighted<Armor>("PoopyTable", 50);
		auto Bag = Inter.CreateBag_Weighted<Armor, Variance::Chance>("PoopyBag", 50);

		Table->AddLoot(&ChestPlateLoot);

		Bag->AddLoot(&HelmetLoot);


		std::list<Armor*> OutLoot;
		Table->GetLoot(OutLoot);
	}
}