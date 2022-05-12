#include "../Include/LootBag/LootBag.h"
#include "../Include/LootTable/LootTable.h"

// Debug
#include <string>

int main()
{
	using namespace impl;

	struct Armor
	{
		Armor(std::string _Name) :Name(_Name) {};
		std::string Name;
	};
	struct Weapon
	{

	};

	Armor ActualArmorLoot("Nested Armor");
	Weapon ActualWeaponLoot;

	ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> ArmorLoot(&ActualArmorLoot);
	ElementLoot<Weapon, Variance::Chance, Obtainabilities::Common> WeaponLoot(&ActualWeaponLoot);

	LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> NestedArmorBag;
	LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> ArmorBag;

	Core::LootTable<Armor,Variance::Chance,Obtainabilities::Common> DropTable;

	NestedArmorBag.AddLoot(&ArmorLoot);

	ArmorBag.AddLoot(&NestedArmorBag);


	std::list<Armor*> ObtainedArmor;
	DropTable.ObtainLoot(ObtainedArmor);
	ArmorBag.ObtainLoot(ObtainedArmor);
	ArmorBag.ObtainLoot(ObtainedArmor);

 
}