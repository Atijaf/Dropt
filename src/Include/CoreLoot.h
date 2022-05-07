#pragma once
#include "LootDispatcherDefinition.h"
#include "LootObtainabilityController.h"

namespace impl
{
	/// <summary>
	/// Forces LootType to be the same within all containers
	/// </summary>
	/// <typeparam name="LootType">The type of loot</typeparam>
	/// <typeparam name="Variant">Chance, Interval, or Constant</typeparam>
	template<typename LootType, Variance Variant>
	class CoreLootContainer : public AbstractLootDispatcher, public LootDispatchVariance<Variant>
	{
	public:
		bool GetLoot(std::list<void*>& OutLoot) { return false; };// Hides Parent GetLoot function (Makes public)
	};



	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class CoreLoot : public CoreLootContainer<LootType, Variant>, public LootObtainabilityController<Obtainability>
	{
	public:
		bool ShouldRemoveFromContainer() const override final{ 
			return LootObtainabilityController::ShouldRemoveFromContainer(); 
		}
	};


}