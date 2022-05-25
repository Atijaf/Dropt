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
	public:
		CoreLootBag(uint32_t InitialSize) :
			BaseLootBag([](CoreLootContainer<LootType, Variance::Interval>* A, CoreLootContainer<LootType, Variance::Interval>* B) {
				return(*A > *B); },
				InitialSize	
			)
			
		{};

	protected:
		virtual bool GrabLoot(std::list<LootType*>& OutLoot) override final;
		virtual bool FinalizeLootBag_impl() override final;
	};

	template<typename LootType>
	inline bool impl::CoreLootBag<LootType, Variance::Interval>::FinalizeLootBag_impl()
	{
		return false;
	}

	template<typename LootType>
	inline bool CoreLootBag<LootType, Variance::Interval>::GrabLoot(std::list<LootType*>& OutLoot) {
		return false;
	}
}