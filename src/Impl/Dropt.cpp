#include "../Include/LootBag/LootBag.h"
#include "../Include/LootTable/LootTable.h"
#include "../Include/Helper/MArray.h"

// Debug
#include <string>

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


	if(false){
		Armor ActualArmorLoot("Nested Armor");
		Weapon ActualWeaponLoot;

		ElementLoot<Armor, Variance::Chance, Obtainabilities::Unique> ArmorLoot(&ActualArmorLoot);
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

		
		ElementLoot<Armor, Variance::Constant, Obtainabilities::Unique> ChestPlateLoot(&Chestplate);
		ElementLoot<Armor, Variance::Constant, Obtainabilities::Unique> HelmetLoot(&Helmet);
		ElementLoot<Armor, Variance::Constant, Obtainabilities::Common> LegLoot(&LegArmor);
		ElementLoot<Armor, Variance::Constant, Obtainabilities::Unique> HandLoot(&Gloves);

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