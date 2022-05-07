#pragma once
#include "Loot.h"

namespace impl
{
	/// <summary>
	/// </summary>
	/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
	/// <param name="Variant">:			Is this LootBag obtained by Chance, Interval, or constantly </param>
	/// <param name="Obtainability">:	Defines if this bag is Unique, Variable, or Common</param>
	/// <param name="ContentVariant">:	Defines if the contents of this bag are obtained by Chance, Interval, or Constantly</param>
	template<	typename LootType,
				Variance Variant, 
				Obtainabilities Obtainability,
				Variance ContentVariant>
	class LootBag : public CoreLoot<LootType, Variant, Obtainability>
	{
	public:

		/// <summary>
		/// Adds any loot that is derived from CoreLoot
		/// </summary>
		/// <param name="ObjectObtainability">: Template Parameter, Unique, Variable, or Common</param>
		/// <param name="Loot">: The Loot Being added </param>
		template<Obtainabilities ObjectObtainability>
		void AddLoot(CoreLoot<LootType,ContentVariant, ObjectObtainability>* Loot) {
			this->LootArray[0] = Loot;
		}

	protected:
		
		bool GetLoot(std::list<void*>& OutLoot) override {
			LootArray[0]->GetLoot(OutLoot);
			return true;
		};

		// Generic array that contains CoreLoot Objects with varying Obtainability definitions
		CoreLootContainer<LootType, ContentVariant>** LootArray = new CoreLootContainer<LootType, ContentVariant>*[1];// nullptr;
	};


}