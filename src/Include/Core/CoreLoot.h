#pragma once
#include "LootDispatcherDefinition.h"
#include "LootObtainabilityController.h"

namespace impl
{	

	/// <summary>
	/// Constricts Array of this type to LootType and Variant
	/// </summary>
	/// <typeparam name="LootType"> The Type of Loot stored (Armor, Weapons, potions, etc..)</typeparam>
	template<Variance Variant>
	class VariantLoot : public AbstractLootDispatcher, public LootDispatchVariance<Variant>
	{
	public:
	protected:
		
	};

	template<typename LootType, Variance Variant>
	class CoreLootContainer : public VariantLoot<Variant>
	{
	public:
		virtual bool GetLoot(std::list<LootType*>& OutLoot) = 0;
	protected:
	};

	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class CoreLoot : public CoreLootContainer<LootType, Variant>, public LootObtainabilityController<Obtainability>
	{
	public:
		bool GetLoot(std::list<LootType*>& OutLoot) override final {
			Observe_GetLoot();
			return GetLoot_Impl(OutLoot);
		}

	protected:
		bool ShouldRemoveFromContainer() const override final {
			return LootObtainabilityController::ShouldRemoveFromContainer();
		}

		virtual bool GetLoot_Impl(std::list<LootType*>& OutLoot) = 0;
	};


}