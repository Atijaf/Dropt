#pragma once
#include "AbstractLootTable.h"

namespace Dropt {
	namespace impl
	{
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class LootTable : public CoreVariantLootTable<LootType, Variant>, public CoreLoot<LootType, Variant, Obtainability>
		{
		public:
			LootTable() :
				CoreVariantLootTable<LootType,Variant>(*this)
			{

			}
			using CoreLoot::GetLoot;	// Ambiguous solution
			bool FinalizeLoot() override { return FinalizeLootTable(); }
		protected:
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				return RollForLoot(OutLoot);
			}

			virtual bool IsFinalized() const override { return IsLootTableFinalized(); }
		};
	}
}