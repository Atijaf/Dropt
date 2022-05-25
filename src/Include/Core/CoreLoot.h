#pragma once
#include "LootDispatcherDefinition.h"
#include "LootObtainabilityController.h"

namespace impl
{	

	/// <summary>
	/// A type of base parent for storing arrays of elements containing both a LootType and Variant
	/// </summary>
	/// <typeparam name="LootType">: Type of Loot (Armor, Weapon, Potion)</typeparam>
	/// <typeparam name="Variant">: Variant of Loot (Chance, Weight, Constant)</typeparam>
	template<typename LootType, Variance Variant>
	class CoreLootContainer : public AbstractLootDispatcher, public LootDispatchVariance<Variant>
	{
	public:
		virtual bool GetLoot(std::list<LootType*>& OutLoot) = 0;

	protected:
	};



	/// <summary>
	/// A slightly more defined type of base parent for storing arrays of elements containing LootType, Variant, and Obtainability
	/// </summary>
	/// <typeparam name="LootType">: Type of Loot (Armor, Weapon, Potion)</typeparam>
	/// <typeparam name="Variant">: Variant of Loot (Chance, Weight, Constant)</typeparam>
	/// <typeparam name="Obtainability">: The Obtainability of the Loot (Unique, Variable, Common)</typeparam>
	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class CoreLoot : public CoreLootContainer<LootType, Variant>, public LootObtainabilityController<Obtainability>
	{
	public:
		bool GetLoot(std::list<LootType*>& OutLoot) override final {
			Observe_GetLoot();
			return GetLoot_Impl(OutLoot);
		}

	protected:

		// Required Overrides.  Derived classes should probably keep this protected or private
		virtual bool GetLoot_Impl(std::list<LootType*>& OutLoot) = 0;
		virtual bool FinalizeLoot() override = 0;
		virtual bool IsFinalized() const override = 0;
	private:
		bool ShouldRemoveFromContainer() const override final {
			return LootObtainabilityController::ShouldRemoveFromContainer();
		}
	};


}