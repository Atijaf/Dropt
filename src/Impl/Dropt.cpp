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


	if(false){
		Armor ActualArmorLoot("Nested Armor");
		Weapon ActualWeaponLoot;

		ElementLoot<Armor, Variance::Chance, Obtainabilities::Unique> ArmorLoot(&ActualArmorLoot);
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
		Armor NestedChestplate("Nested Chest Plate");
		Armor Helmet("Head Gear");
		Armor NestedHelmet("Nested Head Gear");

		
		ElementLoot<Armor, Variance::Interval, Obtainabilities::Common> ChestPlateLoot(&Chestplate);
		ChestPlateLoot.SetInterval(2);

		ElementLoot<Armor, Variance::Interval, Obtainabilities::Common> HelmetLoot(&Helmet);
		HelmetLoot.SetInterval(5);
			
		
		LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Interval> LootBaga;
		LootBaga.SetWeight(100);
		LootBaga.AddLoot(&ChestPlateLoot);
		LootBaga.AddLoot(&HelmetLoot);

		LootTable<Armor, Variance::Chance, Obtainabilities::Common> DropTable;
		DropTable.AddIntervalLoot(&ChestPlateLoot);
		DropTable.AddIntervalLoot(&HelmetLoot);

		DropTable.FinalizeLoot();
		std::list<Armor*> OutLoot;
		for(uint32_t i = 0; i < 100000000; ++i)
			DropTable.GetLoot(OutLoot);
	}
}