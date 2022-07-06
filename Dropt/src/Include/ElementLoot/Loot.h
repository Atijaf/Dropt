#pragma once
#include "../Core/Dispatch.h"
#include "../Core/CoreLoot.h"
#include "../LootContainers/Handler/AbstractHandler.h"

//debug
#include <iostream>

namespace Dropt {
	namespace impl
	{


		class AbstractElementLoot : public AbstractHandler{
		public:
			bool CanLootBeObtained() const override final { return true; }
			bool PrepareLootToBeObtained() override final { return true; }
		protected:
			AbstractElementLoot(AbstractLootDispatcher* _Sister) :
				AbstractHandler(_Sister) {};
		};

		template<typename LootType, Variance Variant>
		class CoreElementLoot : public AbstractElementLoot{
		public:
			CoreLootContainer<LootType, Variant>* GetSister() const {
				return static_cast<CoreLootContainer<LootType, Variant>*>(this->Sister);
			}
			bool GetLoot(std::list<LootType*>& OutLoot) {
				return GetSister()->GetLoot(OutLoot);
			}

			bool operator == (const CoreElementLoot<LootType, Variant>& Other) const {
				return (*this->Loot == *Other.Loot);
			}
		protected:
			CoreElementLoot(LootType* _Loot, AbstractLootDispatcher* _Sister) :
				AbstractElementLoot(_Sister),
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
				CoreLoot<LootType, Variant, Obtainability>(this),
				CoreElementLoot<LootType, Variant>(_Loot, this) {};

			bool FinalizeLoot() override final { return true; }

			CoreLoot<LootType, Variant, Obtainability>* GetSister() const {
				return static_cast<CoreLoot<LootType, Variant, Obtainability>*>(this->Sister);
			}

			using CoreLoot<LootType, Variant, Obtainability>::GetLoot;
		protected:
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				OutLoot.push_back(this->Loot);
				return true;
			}

			virtual bool IsFinalized() const override { return true; }
		};
	}
}