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

	ElementLoot<Armor, Variance::Chance, Obtainabilities::Common> ArmorLoot(&TheActualLoot);
	LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> NestedArmorBag;


	LootBag<Armor, Variance::Chance, Obtainabilities::Common, Variance::Chance> ArmorBag;

	NestedArmorBag.AddLoot(&ArmorLoot);
	ArmorBag.AddLoot(&NestedArmorBag);


	std::list<Armor*> ObtainedArmor;
	ArmorBag.ObtainLoot(ObtainedArmor);
	ArmorBag.ObtainLoot(ObtainedArmor);

 
}