#pragma once

#include "LootBag.h"

namespace Dropt {
	namespace impl
	{
		/// <summary>
		/// Specialized class for Constant Loot Bag
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType, Variance BagVariant>
		class CoreLootBagImpl<LootType, BagVariant, Variance::Constant> : public CoreLootBag<LootType, BagVariant, Variance::Constant>
		{
		public:
			CoreLootBagImpl(uint32_t InitialSize, AbstractLootDispatcher* _Sister) :
				CoreLootBag<LootType, BagVariant,Variance::Constant>(InitialSize,_Sister)
			{

			};
		protected:
			virtual bool GrabLoot(std::list<LootType*>& OutLoot) override final;
			// Removes all elements that should be removed
			void EliminateRemovableIndexes();
			uint32_t* GetRangesToRemove(uint32_t& OutSize);
			void RemoveRangeOfElements(const uint32_t Min, const uint32_t Max);
		};



		template<typename LootType, Variance BagVariant>
		inline bool CoreLootBagImpl<LootType, BagVariant, Variance::Constant>::GrabLoot(std::list<LootType*>& OutLoot) 
		{
			bool bFoundLoot = false;
			if (this->GetNumOfLoot() > 0) {
				CoreLootContainer<LootType, Variance::Constant>* Loot = nullptr;
				for (uint32_t i = 0; i < this->GetNumOfLoot(); ++i) {
					if (this->LootArray[i]->GetLoot(OutLoot))
						bFoundLoot = true;
				}
				// Remove Elements from LootArray that should be removed
				EliminateRemovableIndexes();
			}
			return bFoundLoot;
		}

		template<typename LootType, Variance BagVariant>
		inline uint32_t* impl::CoreLootBagImpl<LootType, BagVariant, Variance::Constant>::GetRangesToRemove(uint32_t& OutSize)
		{
			OutSize = 0;

			// Get list of indexes to remove
			std::list<uint32_t> ListIndexesToRemove;
			for (uint32_t i = 0; i < this->GetNumOfLoot(); ++i)
				if (AbstractLootDispatcher::ShouldRemoveFromContainer(this->LootArray[i]))
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
					OutIndexRangesToRemove[OutSize] = MaxRange;
					OutIndexRangesToRemove[OutSize+1] = MinRange;
					OutSize += 2;

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

		template<typename LootType, Variance BagVariant>
		inline void impl::CoreLootBagImpl<LootType, BagVariant, Variance::Constant>::EliminateRemovableIndexes()
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
				this->TrimArray();
			}
		}

		template<typename LootType, Variance BagVariant>
		inline void impl::CoreLootBagImpl<LootType, BagVariant, Variance::Constant>::RemoveRangeOfElements(const uint32_t Min, const uint32_t Max)
		{
			// Remove loot by shifiting all elements from right to Index;
			uint32_t ShiftIndex = Min;
			uint32_t i = Max + 1;
			while (i < this->GetNumOfLoot()) {
				this->LootArray[ShiftIndex++] = this->LootArray[i++];
			}
			const uint32_t PrevNumOfLoot = this->GetNumOfLoot();
			this->LootArrayIndexOffset += (Max - Min + 1);

			// Nullify end of LootArray
			for (uint32_t i = this->GetNumOfLoot(); i < PrevNumOfLoot; ++i)
				this->LootArray[i] = nullptr;
		}


	}
}