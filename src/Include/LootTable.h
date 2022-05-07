#pragma once
#include "LootBag.h"

namespace Core
{
	struct testobject {};
	using namespace impl;

	/// <summary>
	/// Core Interface for rolling for loot
	/// </summary>
	/// <typeparam name="LootType"> The type of loot being stored (Armor, Weapons, etc..)</typeparam>
	/// <typeparam name="Variant"> How the Loot Table is obtained (If stored within another table)</typeparam>
	/// <typeparam name="Obtainability"> How many times @ObtainLoot() can be called on this object</typeparam>
	template<typename LootType, Variance Variant = Variance::Chance, Identifiers Obtainability = Identifiers::Common>
	class LootTable : public LootContainer<LootType, Variant, Obtainability>
	{
	public:
		LootTable() {};
	protected:
		

		/// Array of LootBags that are obtained via Weight
		CoreLootContainer<LootType, Variance::Chance>** WeightedLootBags;
		/// Array of LootBags that are obtained at an Interval
		CoreLootContainer<LootType, Variance::Interval>** IntervalLootBags;
		/// Array of LootBags that are obtained on each call
		CoreLootContainer<LootType, Variance::Constant>** ConstantLootBags;

		void test() { WeightedLootBags[0]. }

	};
}