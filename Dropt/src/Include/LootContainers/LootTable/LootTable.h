#pragma once
#include "AbstractLootTable.h"

namespace Dropt {
	namespace impl
	{
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class LootTable : public CoreLootTable<LootType, Variant>, public CoreLoot<LootType, Variant, Obtainability>
		{
		public:
			LootTable():
				CoreLootTable<LootType, Variant>(this)
			{

			}
			using CoreLoot<LootType, Variant, Obtainability>::GetLoot;	// Ambiguous solution
			bool FinalizeLoot() override final { 
				if (this->FinalizeLootTable())
					this->bIsFinalized;
				return this->bIsFinalized; 
			}
		protected:
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				return this->RollForLoot(OutLoot);
			}

			virtual bool IsFinalized() const override { return this->IsLootTableFinalized(); }
		};
	}
}