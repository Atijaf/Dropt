#pragma once
#include "AbstractLootTable.h"


namespace Core
{
	using namespace impl;


	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class LootTable : public CoreLootTable<LootType>, public CoreLoot<LootType, Variant, Obtainability>
	{
	public:
		bool FinalizeLoot() override final { return FinalizeLoot_impl(); }
	protected:
		bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
			return RollForLoot(OutLoot);
		}
	};
}