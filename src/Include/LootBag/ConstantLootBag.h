#pragma once

#include "AbstractLootBag.h"

namespace impl
{
	/// <summary>
	/// Specialized class for Constant Loot Bag
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType>
	class CoreLootBag<LootType, Variance::Constant> : public BaseLootBag<LootType, Variance::Constant>
	{
	protected:
		bool GrabLoot(std::list<LootType*>& OutLoot) override;
		bool FinalizeLoot_impl() override;
	};

	template<typename LootType>
	inline bool impl::CoreLootBag<LootType, Variance::Constant>::FinalizeLoot_impl()
	{
		return false;
	}

	template<typename LootType>
	inline bool CoreLootBag<LootType, Variance::Constant>::GrabLoot(std::list<LootType*>& OutLoot) {
		return false;
	}
}