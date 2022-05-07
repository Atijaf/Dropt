#include "../Include/LootBag.h"

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

	Armor TheActualLoot("Nested Armor");

	ElementLoot<Armor, Variance::Chance, Identifiers::Common> ArmorLoot(&TheActualLoot);
	LootBag<Armor, Variance::Chance, Identifiers::Common, Variance::Chance> NestedArmorBag;


	LootBag<Armor, Variance::Chance, Identifiers::Common, Variance::Chance> ArmorBag;
	NestedArmorBag.AddLoot<Identifiers::Common>(&ArmorLoot);
	ArmorBag.AddNestedLoot<Identifiers::Common>(&NestedArmorBag);


	std::list<Armor*> ObtainedArmor;
	ArmorBag.ObtainLoot(ObtainedArmor);
	ArmorBag.ObtainLoot(ObtainedArmor);

 
}