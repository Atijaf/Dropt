#pragma once

#include "AbstractLootBag.h"
namespace Dropt {
	namespace impl
	{
		/// <summary>
		/// Specialized class for Constant Loot Bag
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType>
		class CoreLootBag<LootType, Variance::Constant> : public BaseLootBag<LootType, Variance::Constant>
		{
		public:
			CoreLootBag(uint32_t InitialSize) :
				BaseLootBag([](CoreLootContainer<LootType, Variance::Constant>* A, CoreLootContainer<LootType, Variance::Constant>* B) {
				return(*A > *B); },
					InitialSize
					)
			{};
		protected:
			virtual bool GrabLoot(std::list<LootType*>& OutLoot) override final;
			virtual bool FinalizeLootBag_impl() override final;
			// Removes all elements that should be removed
			void EliminateRemovableIndexes();
			uint32_t* GetRangesToRemove(uint32_t& OutSize);
			void RemoveRangeOfElements(const uint32_t Min, const uint32_t Max);
		};

		template<typename LootType>
		inline bool impl::CoreLootBag<LootType, Variance::Constant>::FinalizeLootBag_impl()
		{
			// Return true if contains 1 or more loot
			return (GetNumOfLoot() != 0);
		}

		template<typename LootType>
		inline bool CoreLootBag<LootType, Variance::Constant>::GrabLoot(std::list<LootType*>& OutLoot) {

			bool bReturnFlag = true;
			if (this->GetNumOfLoot() > 0) {
				CoreLootContainer<LootType, Variance::Constant>* Loot = nullptr;
				for (uint32_t i = 0; i < this->GetNumOfLoot(); ++i) {
					Loot = LootArray[i];
					// Debug.  This shouldn't ever happen, but if it does, investigate
					// Might happen if this function is called on a loot bag that has no contents
					if (!Loot->GetLoot(OutLoot))
						bReturnFlag = false;
				}
				// Remove Elements from LootArray that should be removed
				EliminateRemovableIndexes();
				return true;
			}
			return false;
		}

		template<typename LootType>
		inline uint32_t* impl::CoreLootBag<LootType, Variance::Constant>::GetRangesToRemove(uint32_t& OutSize)
		{
			OutSize = 0;

			// Get list of indexes to remove
			std::list<uint32_t> ListIndexesToRemove;
			for (uint32_t i = 0; i < this->GetNumOfLoot(); ++i)
				if (AbstractLootDispatcher::ShouldRemoveFromContainer(LootArray[i]))
					ListIndexesToRemove.push_back(i);
			uint32_t ArraySize = ListIndexesToRemove.size();

			if (ArraySize > 0) {
				// Convert ListIndexesToRemove into array
				uint32_t* IndexesToRemove = new uint32_t[ArraySize];
				{
					uint32_t i = 0;
					for (auto it = ListIndexesToRemove.begin(); i < ArraySize; ++it)
						IndexesToRemove[i++] = *it;
				}

				// Worst case is twice the size of ArraySize (0,2,5,9) == ([9,9][5,5][2,2][0,0])
				uint32_t* OutIndexRangesToRemove = new uint32_t[ArraySize * 2];

				// Get a range of indexes to remove to optimize this process
				uint32_t MaxRange = 0;
				uint32_t MinRange = 0;

				while (ArraySize > 0) {
					uint32_t ArrayIndex = ArraySize - 1;
					MaxRange = MinRange = IndexesToRemove[ArrayIndex];
					for (uint32_t i = 1; i < ArraySize; ++i) {
						// Check if the next index is Sequential (-x)
						if (IndexesToRemove[ArrayIndex - 1] == MinRange - 1) {
							ArrayIndex--;
							MinRange = IndexesToRemove[ArrayIndex];
						}
						else
							break;
					}
					OutIndexRangesToRemove[OutSize++] = MaxRange;
					OutIndexRangesToRemove[OutSize++] = MinRange;

					// We have effectively eliminated some contents of @RemovingIndexes.  Decrement ArraySize
					const uint32_t GapSize = (MaxRange - MinRange + 1);
					ArraySize -= GapSize;
				}
				return OutIndexRangesToRemove;
			}
			// Nothing to remove
			OutSize = 0;
			return nullptr;
		}

		template<typename LootType>
		inline void impl::CoreLootBag<LootType, Variance::Constant>::EliminateRemovableIndexes()
		{
			// Loop through entire array to get all the indexes that should be removed
			uint32_t ArraySize = 0;
			uint32_t* RemovingIndexes = GetRangesToRemove(ArraySize);

			if (ArraySize > 0) {
				for (uint32_t i = 0; i < ArraySize; i += 2) {
					const uint32_t MaxRange = RemovingIndexes[i];
					const uint32_t MinRange = RemovingIndexes[i + 1];
					RemoveRangeOfElements(MinRange, MaxRange);
				}
				delete[] RemovingIndexes;

				TrimArray();
			}
		}

		template<typename LootType>
		inline void impl::CoreLootBag<LootType, Variance::Constant>::RemoveRangeOfElements(const uint32_t Min, const uint32_t Max)
		{
			// Remove loot by shifiting all elements from right to Index;
			uint32_t ShiftIndex = Min;
			uint32_t i = Max + 1;
			while (i < GetNumOfLoot()) {
				LootArray[ShiftIndex++] = LootArray[i++];
			}
			const uint32_t PrevNumOfLoot = GetNumOfLoot();
			LootArrayIndexOffset += (Max - Min + 1);

			// Nullify end of LootArray
			for (uint32_t i = GetNumOfLoot(); i < PrevNumOfLoot; ++i)
				LootArray[i] = nullptr;
		}


	}
}