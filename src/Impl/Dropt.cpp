#include "../Include/LootBag/LootBag.h"
#include "../Include/LootTable/LootTable.h"
#include "../Include/Helper/MArray.h"

// Debug
#include <string>

int main()
{
	using namespace impl;
	using namespace Core;

	struct Armor
	{
		Armor(std::string _Name) :Name(_Name) {};
		std::string Name;
	};
	struct Weapon
	{

	};

	{
	Armor ActualArmorLoot("Nested Armor");
	Weapon ActualWeaponLoot;

	ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> ArmorLoot(&ActualArmorLoot);
	ElementLoot<Weapon, Variance::Chance, Obtainabilities::Common> WeaponLoot(&ActualWeaponLoot);

	LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> NestedArmorBag;
	LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> ArmorBag;

	Core::LootTable<Armor, Variance::Chance, Obtainabilities::Common> DropTable;

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

		ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> ChestPlateLoot(&Chestplate);
		ChestPlateLoot.SetWeight(10);

		ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> HelmetLoot(&Helmet);
		HelmetLoot.SetWeight(150);

		LootTable<Armor, Variance::Chance, Obtainabilities::Common> DropTable;
		DropTable.AddWeightedLoot(&ChestPlateLoot);
		DropTable.AddWeightedLoot(&HelmetLoot);

		DropTable.FinalizeLoot();
	}
}