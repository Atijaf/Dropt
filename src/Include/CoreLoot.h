#pragma once
#include "LootDispatcherDefinition.h"

namespace impl
{
	/// <summary>
	/// Forces LootType to be the same within all containers
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType, Variance Variant>
	class CoreLoot : public AbstractLootDispatcher, public LootDispatchVariance<Variant>
	{
	public:
		bool GetLoot(std::list<void*>& OutLoot) { return false; };
	};
}