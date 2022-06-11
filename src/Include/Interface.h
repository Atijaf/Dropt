#pragma once
#include "LootBag/LootBag.h"
#include "LootTable/LootTable.h"
#include "ElementLoot/Loot.h"

namespace Dropt {
	namespace Interface {
		using namespace Dropt::impl;

		class MemoryHandler
		{

		};

		namespace WeightedInit {

			template<typename LootType, Obtainabilities Obtainability>
			LootTable<LootType, Variance::Chance, Obtainability>* Create_LootTable() {
				return nullptr;
			}
		}
	}
}