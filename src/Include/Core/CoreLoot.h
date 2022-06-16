#pragma once
#include "LootDispatcherDefinition.h"
#include "LootObtainabilityController.h"

namespace Dropt {
	namespace impl
	{

		template<typename LootType>
		class AbstractCoreLoot : public AbstractLootDispatcher {
		public:
			virtual bool GetLoot(std::list<LootType*>& OutLoot) = 0;
		};

		/// <summary>
		/// A type of base parent for storing arrays of elements containing both a LootType and Variant
		/// </summary>
		/// <typeparam name="LootType">: Type of Loot (Armor, Weapon, Potion)</typeparam>
		/// <typeparam name="Variant">: Variant of Loot (Chance, Weight, Constant)</typeparam>
		template<typename LootType, Variance Variant>
		class CoreLootContainer : public AbstractCoreLoot<LootType>, public LootDispatchVariance<Variant>
		{
		public:

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

			virtual bool FinalizeLoot() override = 0;
			virtual bool IsFinalized() const override = 0;
		protected:
		private:

			virtual bool GetLoot_Impl(std::list<LootType*>& OutLoot) = 0;
			bool ShouldRemoveFromContainer() const override {
				return LootObtainabilityController::ShouldRemoveFromContainer();
			}
		};


	}
}