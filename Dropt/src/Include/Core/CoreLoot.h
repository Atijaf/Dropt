#pragma once
#include "LootDispatcherDefinition.h"
#include "LootObtainabilityController.h"
#include "Dispatch.h"
#include <list>

namespace Dropt {
	namespace impl
	{

		//Forward Declaration
		template <typename LootType, Variance Variant>
		class BaseNode;


		template<typename LootType>
		class AbstractCoreLoot : public AbstractLootDispatcher {
		public:
			virtual bool GetLoot(std::list<LootType*>& OutLoot) = 0;
		protected:
			AbstractCoreLoot(AbstractHandler* _Brother) :
				AbstractLootDispatcher(_Brother) {}
		};

		/// <summary>
		/// A type of base parent for storing arrays of elements containing both a LootType and Variant
		/// </summary>
		/// <typeparam name="LootType">: Type of Loot (Armor, Weapon, Potion)</typeparam>
		/// <typeparam name="Variant">: Variant of Instance (Chance, Weight, Constant)</typeparam>
		template<typename LootType, Variance Variant>
		class CoreLootContainer : public AbstractCoreLoot<LootType>, public LootDispatchVariance<Variant>
		{
		public:
			template<typename LootType, Variance Variant>
			friend class BaseNode;
			constexpr Variance GetVariant() const override { return Variant; }

		protected:
			CoreLootContainer(AbstractHandler* _Brother) :
				AbstractCoreLoot<LootType>(_Brother) {}
			//virtual bool ShouldRemoveFromContainer() const = 0;
			//virtual bool GetLoot(std::list<LootType*>& OutLoot) = 0;
			//virtual bool FinalizeLoot() = 0;
			//virtual bool IsFinalized() const = 0;
		};



		/// <summary>
		/// A slightly more defined type of base parent for storing arrays of elements containing LootType, Variant, and Obtainability
		/// </summary>
		/// <typeparam name="LootType">: Type of Loot (Armor, Weapon, Potion)</typeparam>
		/// <typeparam name="Variant">: Variant of Instance (Chance, Weight, Constant)</typeparam>
		/// <typeparam name="Obtainability">: The Obtainability of this instance (Unique, Variable, Common)</typeparam>
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class CoreLoot : public CoreLootContainer<LootType, Variant>, public LootObtainabilityController<Obtainability>
		{
		public:
			bool GetLoot(std::list<LootType*>& OutLoot) override final {
				this->Observe_GetLoot();
				return GetLoot_Impl(OutLoot);
			}
			constexpr Obtainabilities GetObtainability() const override { return Obtainability; }

		protected:
			CoreLoot(AbstractHandler* _Brother) :
				CoreLootContainer<LootType, Variant>(_Brother) {}
		private:

			virtual bool GetLoot_Impl(std::list<LootType*>& OutLoot) = 0;
			bool ShouldRemoveFromContainer() const override {
				return LootObtainabilityController<Obtainability>::ShouldRemoveFromContainer();
			}

		};
	}
}