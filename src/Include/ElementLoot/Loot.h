#pragma once
#include "../Core/Dispatch.h"
#include "../Core/CoreLoot.h"

namespace Dropt {
	namespace impl
	{


		class AbstractElementLoot {
		public:
			AbstractLootDispatcher* GetSibling() const { return Sibling; }
		protected:
			AbstractElementLoot(AbstractLootDispatcher* _Sibling) :
				Sibling(_Sibling) {};
			AbstractLootDispatcher* Sibling;
		};

		template<typename LootType, Variance Variant>
		class CoreElementLoot : public AbstractElementLoot{
		public:
			CoreLootContainer<LootType, Variant>* GetSibling() const {
				return static_cast<CoreLootContainer<LootType, Variant>*>(this->Sibling);
			}
			bool GetLoot(std::list<LootType*>& OutLoot) {
				return GetSibling()->GetLoot(OutLoot);
			}
		protected:
			CoreElementLoot(LootType* _Loot, AbstractLootDispatcher* _Sibling) :
				AbstractElementLoot(_Sibling),
				Loot(_Loot) {};
			LootType* Loot;
		};

		/// <summary>
		/// Loot Class Wrapper for creating obtainable loot (The Final Product)
		/// </summary>
		/// <typeparam name="LootType">:		The Type of Loot being stored</typeparam>
		/// <typeparam name="Variant">:			If loot is obtained via Chance, Interval, or is constant</typeparam>
		/// <typeparam name="Obtainability">:	If the loot is Unique, Variable, or Common</typeparam>
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class ElementLoot : public CoreElementLoot<LootType, Variant>, public CoreLoot<LootType, Variant, Obtainability>
		{
		public:
			ElementLoot(LootType* _Loot) : 
				CoreElementLoot<LootType, Variant>(_Loot, this) {};
			bool FinalizeLoot() override final { return true; }

			CoreLoot<LootType, Variant, Obtainability>* GetSibling() const {
				return static_cast<CoreLoot<LootType, Variant, Obtainability>*>(this->Sibling);
			}

			using CoreLoot<LootType, Variant, Obtainability>::GetLoot;
		protected:
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				OutLoot.push_back(Loot);
				return true;
			}

			virtual bool IsFinalized() const override { return true; }

			LootType* Loot;
		};
	}
}