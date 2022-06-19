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
		template<typename LootType, Variance BagVariant>
		class CoreLootBagImpl<LootType, BagVariant, Variance::Chance> : public CoreLootBagInterface<LootType, BagVariant, Variance::Chance>
		{
		public:
			CoreLootBagImpl(uint32_t InitialSize, AbstractLootDispatcher* _Sibling) :	// Initialization list
				CoreLootBagInterface(InitialSize, _Sibling)
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


		template<typename LootType, Variance BagVariant>
		inline bool impl::CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::FinalizeLootBag_impl()
		{
			// Find GCF of all weights in LootArray and reduce the weights based off that;
			ReduceToGCF();

			// Sort from greatest to least if not already sorted
			// This only needs to occur once, as we never rearrange the array in a way that unsorts it
			// See @RemoveIndexFromArray(uint32_t Index)
			if (!bIsSorted) {
				LootArray.Sort([](CoreLootContainer<LootType, Variance::Chance>* A, CoreLootContainer<LootType, Variance::Chance>* B)
					{
						return(*A > *B);
					});
				bIsSorted = true;
			}

			// Loop through array, setting their relative weights
			DefineRelativeWeights();
			// Defines a "Dice" that is used when grabbing random elements
			DefineDice();

			return true;
		}

		template<typename LootType, Variance BagVariant>
		inline void impl::CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::ReduceToGCF()
		{
			const uint32_t GCF = FindGCFOfWeights();
			if (GCF > 1)
				for (uint32_t i = 0; i < GetNumOfLoot(); ++i)
					LootArray[i]->SetWeight(LootArray[i]->GetWeight() / GCF);
		}

		template<typename LootType, Variance BagVariant>
		inline void CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::RemoveIndexFromArray(const uint32_t Index)
		{
			// Call Parent Function (Like super)
			CoreLootBagInterface::RemoveIndexFromArray(Index);

			// If the Offset is 0, then the Parent call has finished all work that needed to be done
			if (LootArrayIndexOffset == 0)
				return;
			// Otherwise, we NEED to perform these actions
			DefineRelativeWeights();
			DefineDice();
		}

		template<typename LootType, Variance BagVariant>
		inline uint32_t CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::FindGCFOfWeights() const
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

		template<typename LootType, Variance BagVariant>
		inline void impl::CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::DefineRelativeWeights()
		{
			uint32_t Index = GetNumOfLoot() - 1;
			uint64_t WeightSum = 0;
			for (uint32_t i = 0; i < GetNumOfLoot(); ++i, --Index) {
				WeightSum += LootArray[Index]->GetWeight();
				LootArray[Index]->SetRelativeWeight(WeightSum);
			}

		}

		template<typename LootType, Variance BagVariant>
		inline void CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::DefineDice()
		{
			BagIntDistrib = std::uniform_int_distribution<uint64_t>(0, LootArray[0]->GetRelativeWeight() - 1);
		}

		template<typename LootType, Variance BagVariant>
		inline bool CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::GrabLoot(std::list<LootType*>& OutLoot) {
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

		template<typename LootType, Variance BagVariant>
		inline CoreLootContainer<LootType, Variance::Chance>* impl::CoreLootBagImpl<LootType, BagVariant, Variance::Chance>::
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