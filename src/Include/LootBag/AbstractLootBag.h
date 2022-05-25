#pragma once
#include "../ElementLoot/Loot.h"
#include "../Helper/MArray.h"
#include "../Helper/Globals.h"
#include <random>

namespace impl
{

	class AbstractLootBag {
	public:
		bool IsLootBagFinalized() const { return bIsFinalized; }
	protected:
		bool FinalizeLootBag() {
			if (FinalizeLootBag_impl())
				bIsFinalized = true;
			return bIsFinalized;
		}

	private:
		virtual bool FinalizeLootBag_impl() = 0;
		bool bIsFinalized = false;
	};
	/// <summary>
	/// Very Base of a Loot Bag.  Provides implementation for adding loot, and forces GrabLoot to be defined in any children of this class
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType, Variance ContentVariant>
	class BaseLootBag : public AbstractLootBag
	{
	public:
		template<Obtainabilities Obtainability>
		// Catch all add loot function
		bool AddLoot(CoreLoot<LootType, ContentVariant, Obtainability>* Loot);
		uint32_t GetNumOfLoot() const { return LootArray.GetNumOfElements(); }

	protected:
		/// <summary>
		/// Initializes a LootBag
		/// </summary>
		/// <param name="FuncSort">:	Sorting Function that returns bools and accepts two CoreLootContainer Pointers</param>
		/// <param name="InitialSize">:	Starting size of the array (Default 10)</param>
		BaseLootBag(std::function<bool( CoreLootContainer<LootType, ContentVariant>* A,
										CoreLootContainer<LootType, ContentVariant>* B)> FuncSort,
					uint32_t InitialSize = 10) :
			LootArray(InitialSize, FuncSort) {}

		virtual ~BaseLootBag();

		virtual bool GrabLoot(std::list<LootType*>& OutLoot) { return false; }
		void RemoveIndexFromArray(const uint32_t Index);

		std::uniform_int_distribution<uint64_t> BagIntDistrib;
		Dropt::Helper::MArray<CoreLootContainer<LootType, ContentVariant>*> LootArray;
		
	private:
	};

	template<typename LootType, Variance ContentVariant>
	class CoreLootBag : public BaseLootBag<LootType, ContentVariant>
	{
	protected:
		CoreLootBag(uint32_t InitialSize) : BaseLootBag(InitialSize) {}
		bool GrabLoot(std::list<LootType*>& OutLoot) override { return false; }
	};


	template<typename LootType, Variance ContentVariant>
	inline impl::BaseLootBag<LootType, ContentVariant>::~BaseLootBag()
	{
	}


	template<typename LootType, Variance ContentVariant>
	template<Obtainabilities Obtainability>
	inline bool BaseLootBag<LootType, ContentVariant>::AddLoot(CoreLoot<LootType, ContentVariant, Obtainability>* Loot)
	{
		return LootArray.AddElement(Loot);
	}

	template<typename LootType, Variance ContentVariant>
	inline void BaseLootBag<LootType, ContentVariant>::RemoveIndexFromArray(const uint32_t Index)
	{
	}

}