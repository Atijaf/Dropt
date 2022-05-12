#pragma once
#include "../Core/CoreLoot.h"
#include <list>

namespace Core
{
	using namespace impl;

	/// <summary>
	/// Controller for Obtaining loot from the Loot Table
	/// </summary>

	/// <summary>
	/// Forces Variant to remain the same amongst an array of these objects
	/// </summary>
	/// <typeparam name="Variant"> How the Loot Table is obtained (If stored within another Loot Table or Loot Bag)</typeparam>
	/// 
	
	class AbstractLootTable {
	protected:
		VariantLoot<Variance::Chance>** WeightedLootArray;
		VariantLoot<Variance::Interval>** IntervalLootArray;
		VariantLoot<Variance::Constant>** ConstantLootArray;
		bool RollForLoot(std::list<void*>& OutLoot) {
			return false;
		}
	};

	template<typename LootType>
	class CoreLootTable : public AbstractLootTable
	{
	public:
		void AddWeightedLoot(CoreLootContainer<LootType, Variance::Chance>* Loot) {
			WeightedLootArray[0] = Loot;
		}

		void AddWeightedLoot(CoreLootContainer<LootType, Variance::Interval>* Loot) {
			IntervalLootArray[0] = Loot;
		}

		void AddWeightedLoot(CoreLootContainer<LootType, Variance::Constant>* Loot) {
			ConstantLootArray[0] = Loot;
		}
	};


	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class LootTable : public CoreLootTable<LootType>, public CoreLoot<LootType, Variant, Obtainability>
	{
	protected:
		bool GetLoot_Impl(std::list<void*>& OutLoot) override {
			return RollForLoot(OutLoot);
		}
	};
}