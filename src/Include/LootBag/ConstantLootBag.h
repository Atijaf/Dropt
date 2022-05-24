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
	public:
		CoreLootBag() :
			BaseLootBag([](CoreLootContainer<LootType, Variance::Constant>* A, CoreLootContainer<LootType, Variance::Constant>* B) {
			return(*A > *B);
				})
		{};
	protected:
		virtual bool GrabLoot(std::list<LootType*>& OutLoot) override final;
		virtual bool FinalizeLoot_impl() override final;
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