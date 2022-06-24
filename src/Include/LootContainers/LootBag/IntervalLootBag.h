#pragma once
#include "AbstractLootBag.h"

namespace Dropt {
	namespace impl
	{

		/// <summary>
		/// Specialized class for Interval Loot Bag
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType, Variance BagVariant>
		class CoreLootBagImpl<LootType, BagVariant, Variance::Interval> : public CoreLootBag<LootType, BagVariant, Variance::Interval>
		{
		public:
			CoreLootBagImpl(uint32_t InitialSize, AbstractLootDispatcher* _Sibling) :
				CoreLootBag<LootType, BagVariant,Variance::Interval>(InitialSize,_Sibling)
			{
			
			};

		protected:
			virtual bool GrabLoot(std::list<LootType*>& OutLoot) override final;
			virtual bool FinalizeLootBag() override final;
			void ResetCounter();

			bool bIsSorted = false;
			uint32_t GrabCounter = 0;
		};

		template<typename LootType, Variance BagVariant>
		inline bool impl::CoreLootBagImpl<LootType, BagVariant, Variance::Interval>::FinalizeLootBag()
		{
			if (!AbstractLootBag::FinalizeLootBag()) return false;

			if (!bIsSorted) {
				this->LootArray.Sort([](CoreLootContainer<LootType, Variance::Interval>* A, CoreLootContainer<LootType, Variance::Interval>* B)
					{
						return(*A > *B);
					});
				bIsSorted = true;
			}
			return true;
		}

		template<typename LootType, Variance BagVariant>
		inline bool CoreLootBagImpl<LootType, BagVariant, Variance::Interval>::GrabLoot(std::list<LootType*>& OutLoot) {

			uint32_t NumOfLootObtained = 0;
			++GrabCounter;
			uint32_t i = 0;
			while (i < this->GetNumOfLoot()) {

				// In order for GrabCounter to evenly go into Interval, GrabCounter MUST be
				// at least equal to or larger than the Interval
				if (GrabCounter + this->LootArray[i]->GetOffset() < this->LootArray[i]->GetInterval())
					break;

				// If GrabCounter does not evently go into the Loot's Interval,
				// at Index I in LootArray, then increment i and loop
				if ((GrabCounter + this->LootArray[i]->GetOffset()) % this->LootArray[i]->GetInterval() != 0)
					++i;

				// Else, GrabCounter evenly goes into the Loot's Interval, 
				// at Index I in LootArray, get that loot
				else {
					this->LootArray[i]->GetLoot(OutLoot);
					// If we remove the index from the array, then we don't need to increment i
					if (AbstractLootDispatcher::ShouldRemoveFromContainer(this->LootArray[i]))
						this->RemoveIndexFromArray(i);
					// Increment I IF we did not remove it from the array
					else ++i;
				}
			}

			// Lastly, if we obtained the last loot in the array, we should reset the counter
			if (i == this->GetNumOfLoot() && this->GetNumOfLoot() > 0 &&
				GrabCounter % this->LootArray[this->GetNumOfLoot() - 1]->GetInterval() == 0)
				ResetCounter();

			return (NumOfLootObtained > 0);
		}



		template<typename LootType, Variance BagVariant>
		inline void impl::CoreLootBagImpl<LootType, BagVariant, Variance::Interval>::ResetCounter()
		{
			// Calculate the offsets for all but last indexed loot
			for (uint32_t i = 0; i < this->GetNumOfLoot() - 1; ++i)
				this->LootArray[i]->CalculateOffset(GrabCounter);

			// Reset counter
			GrabCounter = 0;

		}

	}
}