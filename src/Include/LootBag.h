#pragma once
#include "LootDispatcherDefinition.h"
#include "LootDispatcherIdentity.h"
#include "Loot.h"
#include "LootContainer.h"

namespace impl
{
	/// <summary>
	/// </summary>
	/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
	/// <param name="Bag Controller">:	Defines if this Loot Bag is obtained by Chance, Schedule, Interval, or constantly </param>
	/// <param name="Bag Identity">:	Defines if this bag is Unique, Variable, or Common</param>
	template<	typename LootType,
				Variance BagController, 
				Identifiers BagIdentity>
	class LootBag : public LootContainer<LootType, BagController, BagIdentity>
	{
	public:

		void AddLoot(ElementLoot<LootType,LootCommonality>* _Loot) {
			this->LootArray[0] = _Loot;
		}

		void AddNestedLoot(CoreLootContainer<LootType, LootCommonality>* NestedLoot) {
			this->LootArray[0] = NestedLoot;
		}

	protected:
		
		bool GetLoot(std::list<void*>& OutLoot) override {
			LootArray[0]->GetLoot(OutLoot);
			return true;
		};


		CoreLoot<LootType, ContentController>** LootArray = new CoreLoot<LootType, ContentController>*[1];// nullptr;
	};


}