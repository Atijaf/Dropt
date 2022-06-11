#pragma once
#include "AbstractLootBag.h"
#include <functional>

namespace Dropt {
	namespace impl
	{
		/// <summary>
		/// Specialized class for Weighted Loot Bag
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType>
		class CoreLootBag<LootType, Variance::Chance> : public BaseLootBag<LootType, Variance::Chance>
		{
		public:
			CoreLootBag(uint32_t InitialSize) :	// Initialization list
				BaseLootBag([](CoreLootContainer<LootType, Variance::Chance>* A, CoreLootContainer<LootType, Variance::Chance>* B)
					{ return(*A > *B); },	// Comparator
					InitialSize				// Initial Size of array
				)
			{}

		protected:
			// Obtains loot from the Bag of Weighted Elements
			bool GrabLoot(std::list<LootType*>& OutLoot) override;

			virtual void RemoveIndexFromArray(const uint32_t Index) override;

			// Performs final actions, required to allow this bag to be grabbed from
			virtual bool FinalizeLootBag_impl() override final;
			// Finds GCF amongs all elements weights (5,10,15: GCF is 5)
			uint32_t FindGCFOfWeights() const;
			// Reduces all elements weights based on GCF(5,10,15: GCF is 5 : 1,2,3)
			void ReduceToGCF();
			// Relative weights is the sum of all weights less than, and including, an element's weight
			// ie. (Weights: 20,5,1,1 : Relative Weights: 27,8,2,1)
			void DefineRelativeWeights();
			// Defines a "Dice" that is used when grabbing random elements
			void DefineDice();
			// Finds Loot from a Random Number.
			// Returns Loot, along with it's index, 
			CoreLootContainer<LootType, Variance::Chance>* FindLootFromRandomNumber(uint64_t RandomNumber, uint32_t& OutLootIndex);
			bool bIsSorted = false;
			// Offset Size by this amount
			uint16_t LootArrayIndexOffset = 0;
		};


		template<typename LootType>
		inline bool impl::CoreLootBag<LootType, Variance::Chance>::FinalizeLootBag_impl()
		{
			///	This doesn't check if the contents are finalized.  It doesn't have to, as any elements added to this
			/// Had to confirm that they are finalized prior to being added

			// If no Loot, then it can't be finalized
			if (GetNumOfLoot() == 0)
				return false;
			if (IsLootBagFinalized()) return true;
			// Find GCF of all weights in LootArray and reduce the weights based off that;
			ReduceToGCF();

			// Sort from greatest to least if not already sorted
			// This only needs to occur once, as we never rearrange the array in a way that unsorts it
			// See @RemoveIndexFromArray(uint32_t Index)
			if (!bIsSorted) {
				LootArray.Sort();
				bIsSorted = true;
			}

			// Loop through array, setting their relative weights
			DefineRelativeWeights();
			// Defines a "Dice" that is used when grabbing random elements
			DefineDice();

			return true;
		}

		template<typename LootType>
		inline void impl::CoreLootBag<LootType, Variance::Chance>::ReduceToGCF()
		{
			const uint32_t GCF = FindGCFOfWeights();
			if (GCF > 1)
				for (uint32_t i = 0; i < GetNumOfLoot(); ++i)
					LootArray[i]->SetWeight(LootArray[i]->GetWeight() / GCF);
		}

		template<typename LootType>
		inline void CoreLootBag<LootType, Variance::Chance>::RemoveIndexFromArray(const uint32_t Index)
		{
			// Call Parent Function (Like super)
			BaseLootBag::RemoveIndexFromArray(Index);

			// If the Offset is 0, then the Parent call has finished all work that needed to be done
			if (LootArrayIndexOffset == 0)
				return;
			// Otherwise, we NEED to perform these actions
			DefineRelativeWeights();
			DefineDice();
		}

		template<typename LootType>
		inline uint32_t CoreLootBag<LootType, Variance::Chance>::FindGCFOfWeights() const
		{

			if (LootArray.GetNumOfElements() == 0)
				return 0;

			// Recursive LAMBDA to find GCD of pair
			std::function<uint32_t(uint32_t, uint32_t)> GCD = [&GCD](uint32_t a, uint32_t b) {
				if (a == 0)
					return b;
				return GCD(b % a, a);
			};

			uint32_t Index = this->GetNumOfLoot() - 1;
			uint32_t Result = LootArray[Index]->GetWeight();
			--Index;
			for (uint32_t i = 1; i < LootArray.GetNumOfElements(); ++i, --Index) {
				Result = GCD(LootArray[Index]->GetWeight(), Result);

				// No need to further check for GCDs
				if (Result == 1)
					break;
			}
			return Result;
		}

		template<typename LootType>
		inline void impl::CoreLootBag<LootType, Variance::Chance>::DefineRelativeWeights()
		{
			uint32_t Index = GetNumOfLoot() - 1;
			uint64_t WeightSum = 0;
			for (uint32_t i = 0; i < GetNumOfLoot(); ++i, --Index) {
				WeightSum += LootArray[Index]->GetWeight();
				LootArray[Index]->SetRelativeWeight(WeightSum);
			}

		}

		template<typename LootType>
		inline void CoreLootBag<LootType, Variance::Chance>::DefineDice()
		{
			BagIntDistrib = std::uniform_int_distribution<uint64_t>(0, LootArray[0]->GetRelativeWeight() - 1);
		}

		template<typename LootType>
		inline bool CoreLootBag<LootType, Variance::Chance>::GrabLoot(std::list<LootType*>& OutLoot) {
			bool bReturnFlag = true;
			CoreLootContainer<LootType, Variance::Chance>* Loot = nullptr;
			uint32_t LootIndex = 0;
			if (GetNumOfLoot() > 1) {
				uint64_t RandomRoll = BagIntDistrib(Dropt::Helper::RandomEngine);
				Loot = FindLootFromRandomNumber(RandomRoll, LootIndex);
			}
			else {
				Loot = LootArray[0];
			}
			if (!Loot->GetLoot(OutLoot))
				bReturnFlag = false;// Debug.  This shouldn't ever happen, but if it does, investigate
									// Might happen if this function is called on a loot bag that has no contents
			if (AbstractLootDispatcher::ShouldRemoveFromContainer(Loot))
				RemoveIndexFromArray(LootIndex);

			return bReturnFlag;
		}

		template<typename LootType>
		inline CoreLootContainer<LootType, Variance::Chance>* impl::CoreLootBag<LootType, Variance::Chance>::
			FindLootFromRandomNumber(uint64_t RandomNumber, uint32_t& OutLootIndex)
		{
			//Indexes:	 0,  1,  2, 3, 4
			//			65, 35, 20, 5, 1
			// Random Number: 2
			OutLootIndex = 0; // Default
			uint32_t i = 1;
			for (i; i < GetNumOfLoot(); i++) {
				if (LootArray[i]->GetRelativeWeight() < RandomNumber) {
					OutLootIndex = i - 1;
					return LootArray[OutLootIndex];
				}
			}
			return LootArray[i];
		}
	}
}