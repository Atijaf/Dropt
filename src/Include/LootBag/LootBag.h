#pragma once
#include "WeightedLootBag.h"
#include "IntervalLootBag.h"
#include "ConstantLootBag.h"

namespace Core
{
	using namespace impl;

	/// <summary>
	/// </summary>
	/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
	/// <param name="Variant">:			Is this LootBag obtained by Chance, Interval, or constantly </param>
	/// <param name="Obtainability">:	Defines if this bag is Unique, Variable, or Common</param>
	/// <param name="ContentVariant">:	Defines if the contents of this bag are obtained by Chance, Interval, or Constantly</param>
	template<	typename LootType,
		Variance Variant,
		Obtainabilities Obtainability,
		Variance ContentVariant>
	class LootBag : public CoreLootBag<LootType, ContentVariant>, public CoreLoot<LootType, Variant, Obtainability>
	{
	public:
		bool FinalizeLoot() override {
			return FinalizeLoot_impl();
		}
	protected:
		bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
			return GrabLoot(OutLoot);
		}

	};

}