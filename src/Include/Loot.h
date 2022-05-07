#pragma once
#include "Dispatch.h"
#include "CoreLoot.h"
namespace impl
{


	/// <summary>
	/// Loot Class Wrapper for creating obtainable loot (The Final Product)
	/// </summary>
	/// <typeparam name="LootType">:		The Type of Loot being stored</typeparam>
	/// <typeparam name="Variant">:			If loot is obtained via Chance, Interval, or is constant</typeparam>
	/// <typeparam name="Obtainability">:	If the loot is Unique, Variable, or Common</typeparam>
	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class ElementLoot : public CoreLoot<LootType, Variant, Obtainability>
	{
	public:
		ElementLoot(LootType* _Loot) : Loot(_Loot) {};

		bool GetLoot(std::list<void*>& OutLoot) override{
			OutLoot.push_back(Loot);
			return true;
		}
	protected:
		LootType* Loot;
	};
}