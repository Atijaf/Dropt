#pragma once
#include "AbstractLootBag.h"


namespace impl
{

	/// <summary>
	/// Specialized class for Interval Loot Bag
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType>
	class CoreLootBag<LootType, Variance::Interval> : public BaseLootBag<LootType, Variance::Interval>
	{
	public:
		CoreLootBag(uint32_t InitialSize) :
			BaseLootBag([](CoreLootContainer<LootType, Variance::Interval>* A, CoreLootContainer<LootType, Variance::Interval>* B) {
				return(*A > *B); },
				InitialSize	
			)
			
		{};

	protected:
		virtual bool GrabLoot(std::list<LootType*>& OutLoot) override final;
		virtual bool FinalizeLootBag_impl() override final;
		virtual void RemoveIndexFromArray(const uint32_t Index) override;
		void ResetCounter();

		bool bIsSorted = false;
		uint32_t GrabCounter = 0;
	};

	template<typename LootType>
	inline bool impl::CoreLootBag<LootType, Variance::Interval>::FinalizeLootBag_impl()
	{
		if (GetNumOfLoot() == 0) return false;
		if (IsLootBagFinalized()) return true;
		
		if (!bIsSorted) {
			LootArray.Sort();
			bIsSorted = true;
		}
		return true;
	}

	template<typename LootType>
	inline bool CoreLootBag<LootType, Variance::Interval>::GrabLoot(std::list<LootType*>& OutLoot) {

		uint32_t NumOfLootObtained = 0;
		++GrabCounter;
		uint32_t i = 0;
		while (i < GetNumOfLoot()) {

			// In order for GrabCounter to evenly go into Interval, GrabCounter MUST be
			// at least equal to or larger than the Interval
			if (GrabCounter + LootArray[i]->GetOffset() < LootArray[i]->GetInterval())
				break;
			
			// If GrabCounter does not evently go into the Loot's Interval,
			// at Index I in LootArray, then increment i and loop
			if ((GrabCounter + LootArray[i]->GetOffset()) % LootArray[i]->GetInterval() != 0)
				++i;

			// Else, GrabCounter evenly goes into the Loot's Interval, 
			// at Index I in LootArray, get that loot
			else{
				LootArray[i]->GetLoot(OutLoot);
				// If we remove the index from the array, then we don't need to increment i
				if (AbstractLootDispatcher::ShouldRemoveFromContainer(LootArray[i]))
					RemoveIndexFromArray(i);
				// Increment I IF we did not remove it from the array
				else ++i;
			}
		}

		// Lastly, if we obtained the last loot in the array, we should reset the counter
		if (i == GetNumOfLoot() && GetNumOfLoot() > 0 &&
			GrabCounter % LootArray[GetNumOfLoot() - 1]->GetInterval() == 0)
				ResetCounter();

		return (NumOfLootObtained > 0);
	}

	template<typename LootType>
	inline void CoreLootBag<LootType, Variance::Interval>::RemoveIndexFromArray(const uint32_t Index) {
		// Call Parent Function (Like super)
		BaseLootBag::RemoveIndexFromArray(Index);
	}

	template<typename LootType>
	inline void impl::CoreLootBag<LootType, Variance::Interval>::ResetCounter()
	{
		// Calculate the offsets for all but last indexed loot
		for (uint32_t i = 0; i < GetNumOfLoot() - 1; ++i)
			LootArray[i]->CalculateOffset(GrabCounter);

		// Reset counter
		GrabCounter = 0;

	}

}