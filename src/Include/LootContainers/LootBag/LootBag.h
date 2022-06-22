#pragma once
#include "WeightedLootBag.h"
#include "IntervalLootBag.h"
#include "ConstantLootBag.h"
namespace Dropt {
	namespace impl
	{

		/// <summary>
		/// </summary>
		/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
		/// <param name="Variant">:			Is this LootBag obtained by Chance, Interval, or constantly </param>
		/// <param name="Obtainability">:	Defines if this bag is Unique, Variable, or Common</param>
		/// <param name="ContentVariant">:	Defines if the contents of this bag are obtained by Chance, Interval, or Constantly</param>
		template<	typename		LootType,
					Variance		BagVariant,
					Obtainabilities Obtainability,
					Variance		ContentVariant>
		class LootBag : public CoreLootBagImpl<LootType, BagVariant, ContentVariant>, public CoreLoot<LootType, BagVariant, Obtainability>
		{
		public:

			using CoreLoot<LootType, BagVariant, Obtainability>::GetLoot;

			LootBag(uint32_t InitialSize = 10) :
				CoreLootBagImpl<LootType,BagVariant, ContentVariant>(InitialSize,(this)) {};
			// Performs final actions on a Loot Table to prepare it for grabs
			bool FinalizeLoot() override {
				return this->FinalizeLootBag();
			}

			bool IsFinalized() const override { return this->IsLootBagFinalized(); }
		protected:
			// Implementation of GetLoot for LootBag, defined in AbstractLootBag
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				return this->GrabLoot(OutLoot);
			}

		private:
			// Returns true if the Loot Bag has no contents OR if it was set to be removed after x amount of grabs (Unique/variable LootBag)
			// This means that it will be removed from it's owner's container
			bool ShouldRemoveFromContainer() const override {
				return (LootObtainabilityController<Obtainability>::ShouldRemoveFromContainer() ||
					this->GetNumOfLoot() == 0);
			}

		};

	}
}