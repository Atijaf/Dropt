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
		CoreLootBag() :
			BaseLootBag([](CoreLootContainer<LootType, Variance::Chance>* A, CoreLootContainer<LootType, Variance::Chance>* B)
				{ return(*A > *B); }
			)
		{}
	protected:
		bool GrabLoot(std::list<LootType*>& OutLoot) override;

		virtual bool FinalizeLoot_impl() override final;

	private:
		uint32_t FindGCFOfWeights() const;
		void ReduceToGCF();
	};


	template<typename LootType>
	inline bool impl::CoreLootBag<LootType, Variance::Chance>::FinalizeLoot_impl()
	{
		// Find GCF of all weights in LootArray and reduce the weights based off that;
		ReduceToGCF();
		// Sort from greatest to least
		LootArray.Sort();

		return false;
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

		uint32_t Result = LootArray[0]->GetWeight();
		for (uint32_t i = 1; i < LootArray.GetNumOfElements(); ++i) {
			Result = GCD(LootArray[i]->GetWeight(), Result);

			// No need to further check for GCDs
			if (Result == 1)
				break;
		}
		return Result;
	}

	template<typename LootType>
	inline bool CoreLootBag<LootType, Variance::Chance>::GrabLoot(std::list<LootType*>& OutLoot) {
		return false;
	}
}