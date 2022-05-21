#pragma once
#include "../Core/CoreLoot.h"
#include <list>

namespace impl
{
	/// <summary>
	/// Controller for Obtaining loot from the Loot Table
	/// </summary>

	/// <summary>
	/// Forces Variant to remain the same amongst an array of these objects
	/// </summary>
	/// <typeparam name="Variant"> How the Loot Table is obtained (If stored within another Loot Table or Loot Bag)</typeparam>
	/// 

	template<typename LootType>
	class CoreLootTable
	{
	public:
		void AddWeightedLoot(CoreLootContainer<LootType, Variance::Chance>* Loot) {
			WeightedLootArray[0] = Loot;
		}

		void AddIntervalLoot(CoreLootContainer<LootType, Variance::Interval>* Loot) {
			IntervalLootArray[0] = Loot;
		}

		void AddConstantLoot(CoreLootContainer<LootType, Variance::Constant>* Loot) {
			ConstantLootArray[0] = Loot;
		}


	protected:
		bool RollForLoot(std::list<LootType*>& OutLoot) {
			return (WeightedLootBag.GrabLoot(OutLoot) ||
					IntervalLootBag.GrabLoot(OutLoot) ||
					ConstantLootBag.GrabLoot(OutLoot));
		}

		CoreLootBag<LootType, Variance::Chance> WeightedLootBag;
		CoreLootBag<LootType, Variance::Interval> IntervalLootBag;
		CoreLootBag<LootType, Variance::Constant> ConstantLootBag;
	};
}