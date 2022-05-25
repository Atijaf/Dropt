#pragma once
#include "AbstractLootBag.h"
#include <functional>

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

		// Performs final actions, required to allow this bag to be grabbed from
		virtual bool FinalizeLootBag_impl() override final;

	private:
		// Returns the sum of all of this bag's contents.  Note that this is not the weight of this bag, IF this bag is a weighted bag itself
		uint64_t GetSumWeightOfContents() const { return SumOfWeights; }
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

		// Sum of all content's weights
		uint64_t SumOfWeights;
	};


	template<typename LootType>
	inline bool impl::CoreLootBag<LootType, Variance::Chance>::FinalizeLootBag_impl()
	{
		///	This doesn't check if the contents are finalized.  It doesn't have to, as any elements added to this
		/// Had to confirm that they are finalized prior to being added
		if (IsLootBagFinalized()) return true;
		// Find GCF of all weights in LootArray and reduce the weights based off that;
		ReduceToGCF();
		// Sort from greatest to least
		LootArray.Sort();
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

		uint32_t Index = LootArray.GetNumOfElements() - 1;
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
		BagIntDistrib = std::uniform_int_distribution<uint64_t>(1, SumOfWeights);
	}

	template<typename LootType>
	inline bool CoreLootBag<LootType, Variance::Chance>::GrabLoot(std::list<LootType*>& OutLoot) {
		uint64_t RandomRoll = BagIntDistrib(Dropt::Helper::RandomEngine);
		uint32_t LootIndex = 0;
		auto Loot = FindLootFromRandomNumber(RandomRoll, LootIndex);
		Loot->GetLoot(OutLoot);

		if (AbstractLootDispatcher::ShouldRemoveFromContainer(Loot)) {

		}

		return false;
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