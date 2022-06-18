#pragma once
#include "../Core/Dispatch.h"
#include "../Core/CoreLoot.h"

namespace Dropt {
	namespace impl
	{

		template<typename LootType>
		class CoreElementLoot {
		protected:
			LootType* Loot;
		};

		/// <summary>
		/// Loot Class Wrapper for creating obtainable loot (The Final Product)
		/// </summary>
		/// <typeparam name="LootType">:		The Type of Loot being stored</typeparam>
		/// <typeparam name="Variant">:			If loot is obtained via Chance, Interval, or is constant</typeparam>
		/// <typeparam name="Obtainability">:	If the loot is Unique, Variable, or Common</typeparam>
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class ElementLoot : public CoreElementLoot<LootType>, public CoreLoot<LootType, Variant, Obtainability>
		{
		public:
			ElementLoot(LootType* _Loot) : Loot(_Loot) {};
			bool FinalizeLoot() override final { return true; }

			//using CoreLoot::GetLoot;

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