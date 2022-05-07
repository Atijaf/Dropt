#pragma once
#include "Loot.h"
#include "LootDispatcherDefinition.h"

namespace impl
{
	/// <summary>
	/// A base class to be used for arrays, as an array should not care if this is Common, Variable, or Unique.
	/// </summary>
	/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
	/// <param name="Bag Variance">:	Defines if this Loot Bag is obtained by Chance, Schedule, Interval, or constantly </param>
	template<typename LootType, Variance BagVariance>
	class CoreLootContainer : public CoreLoot<LootType, BagVariance>
	{
	public:

	};

	/// <summary>
	/// A base class for storing functionality between LootBags, which will both inherit from this
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType, Variance BagVariance, Identifiers Ident>
	class LootContainer : public CoreLootContainer<LootType, BagVariance>, public LootDispatcherIdentity<Ident>
	{
	public:

	};
}