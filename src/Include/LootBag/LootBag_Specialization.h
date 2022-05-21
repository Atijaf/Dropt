#pragma once

#include "AbstractLootBag.h"

namespace impl
{
	/// <summary>
	/// Specialized class for Weighted Loot Bag
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType>
	class CoreLootBag<LootType, Variance::Chance> : public BaseLootBag<LootType, Variance::Chance>
	{
		template<typename>
		friend class CoreLootTable;
	protected:
		bool GrabLoot(std::list<LootType*>& OutLoot) override {
			return false;
		};
	};
	


	/// <summary>
	/// Specialized class for Interval Loot Bag
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType>
	class CoreLootBag<LootType, Variance::Interval> : public BaseLootBag<LootType, Variance::Interval>
	{
		template<typename>
		friend class CoreLootTable;
	protected:
		bool GrabLoot(std::list<LootType*>& OutLoot) override {
			return false;
		}
	};



	/// <summary>
	/// Specialized class for Constant Loot Bag
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType>
	class CoreLootBag<LootType, Variance::Constant> : public BaseLootBag<LootType, Variance::Constant>
	{
		template<typename>
		friend class CoreLootTable;
	protected:
		bool GrabLoot(std::list<LootType*>& OutLoot) override { return false; };
	};
}