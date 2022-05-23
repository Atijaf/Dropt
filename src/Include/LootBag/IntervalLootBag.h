#pragma once
#include "AbstractLootBag.h"


namespace impl
{

	/// <summary>
	/// Specialized class for Interval Loot Bag
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType>
	class CoreLootBag<LootType, Variance::Interval> : public BaseLootBag<LootType, Variance::Interval>
	{
	protected:
		bool GrabLoot(std::list<LootType*>& OutLoot) override;
		bool FinalizeLoot_impl() override;
	};

	template<typename LootType>
	inline bool impl::CoreLootBag<LootType, Variance::Interval>::FinalizeLoot_impl()
	{
		return false;
	}

	template<typename LootType>
	inline bool CoreLootBag<LootType, Variance::Interval>::GrabLoot(std::list<LootType*>& OutLoot) {
		return false;
	}
}