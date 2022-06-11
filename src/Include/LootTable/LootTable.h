#pragma once
#include "AbstractLootTable.h"

namespace Dropt {
	namespace impl
	{
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class LootTable : public CoreLootTable<LootType>, public CoreLoot<LootType, Variant, Obtainability>
		{
		public:
			bool FinalizeLoot() override { return FinalizeLootTable(); }
		protected:
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				return RollForLoot(OutLoot);
			}

			virtual bool IsFinalized() const override { return IsLootTableFinalized(); }
		};
	}
}