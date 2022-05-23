#pragma once
#include "../LootBag/LootBag.h"
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
		}

		void AddIntervalLoot(CoreLootContainer<LootType, Variance::Interval>* Loot) {
		}

		void AddConstantLoot(CoreLootContainer<LootType, Variance::Constant>* Loot) {

		}

		uint64_t GetTotalLoot() { 
			return	(uint64_t)(WeightedLootBag.GetNumOfLoot()) +
					(uint64_t)(IntervalLootBag.GetNumOfLoot()) +
					(uint64_t)(ConstantLootBag.GetNumOfLoot());
			return 0;
		}

	protected:
		bool RollForLoot(std::list<LootType*>& OutLoot) {
			if (GetTotalLoot() == 0)
				return false;
			return (WeightedLootBag.GetLoot(OutLoot) ||
					IntervalLootBag.GetLoot(OutLoot) ||
					ConstantLootBag.GetLoot(OutLoot));
		}

		bool FinalizeLoot_impl() {
			return (WeightedLootBag.FinalizeLoot() &&
					IntervalLootBag.FinalizeLoot() &&
					ConstantLootBag.FinalizeLoot());
		}

		//bool FinalizeLoot_impl();

		// LootBag with Weighted Drops
		Core::LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Chance> WeightedLootBag;
		// LootBag with Interval Drops
		Core::LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Interval> IntervalLootBag;
		// LootBag with Constant Drops
		Core::LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Constant> ConstantLootBag;

	};
} 