#include "../Include/LootBag/LootBag.h"
#include "../Include/LootTable/LootTable.h"
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

	auto Test = Inter.CreateLootTable_Weighted<Armor>("PoopyTable", 50, 10);
	std::list<Armor*> OutLoot;
	Test->GetLoot(OutLoot);

	if(false){
		Armor ActualArmorLoot("Nested Armor");
		Weapon ActualWeaponLoot;

		ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> ArmorLoot(&ActualArmorLoot);
		ElementLoot<Weapon, Variance::Chance, Obtainabilities::Common> WeaponLoot(&ActualWeaponLoot);

		LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> NestedArmorBag;
		LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> ArmorBag;

		LootTable<Armor, Variance::Chance, Obtainabilities::Common> DropTable;

		NestedArmorBag.AddLoot(&ArmorLoot);

		ArmorBag.AddLoot(&NestedArmorBag);

		std::list<Armor*> ObtainedArmor;
		DropTable.GetLoot(ObtainedArmor);
		ArmorBag.GetLoot(ObtainedArmor);
		ArmorBag.GetLoot(ObtainedArmor);
	}

	{
		Armor Chestplate("Chest Armor");
		Armor Helmet("Head Gear");
		Armor LegArmor("Leg Armor");
		Armor Gloves("Hand Armor");

		
		ElementLoot<Armor, Variance::Interval, Obtainabilities::Unique> ChestPlateLoot(&Chestplate);
		ElementLoot<Armor, Variance::Interval, Obtainabilities::Unique> HelmetLoot(&Helmet);
		ElementLoot<Armor, Variance::Interval, Obtainabilities::Common> LegLoot(&LegArmor);
		ElementLoot<Armor, Variance::Interval, Obtainabilities::Unique> HandLoot(&Gloves);

		ChestPlateLoot.SetInterval(2);
		HelmetLoot.SetInterval(5);
		LegLoot.SetInterval(20);
		HandLoot.SetInterval(8);

		LootTable<Armor, Variance::Chance, Obtainabilities::Common> DropTable;
		DropTable.AddLoot(&ChestPlateLoot);

		DropTable.AddLoot(&HelmetLoot);
		DropTable.AddLoot(&LegLoot);
		DropTable.AddLoot(&HandLoot);

		DropTable.FinalizeLoot();
		std::list<Armor*> OutLoot;
		DropTable.GetLoot(OutLoot);
	}
}