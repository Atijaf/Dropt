#pragma once
#include "../ElementLoot/Loot.h"

namespace impl
{
	/// <summary>
	/// Very Base of a Loot Bag.  Provides implementation for adding loot, and forces GrabLoot to be defined in any children of this class
	/// </summary>
	/// <typeparam name="LootType"></typeparam>
	template<typename LootType, Variance ContentVariant>
	class BaseLootBag
	{
	public:
		void AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot);
		void Resize(uint32_t NewSize);
	protected:
		BaseLootBag() {};
		BaseLootBag(uint32_t InitialSize);
		virtual ~BaseLootBag();
		virtual bool GrabLoot(std::list<LootType*>& OutLoot) { return false; }

		CoreLootContainer<LootType, ContentVariant>** LootArray = nullptr;
		uint32_t Size = 0;
		uint32_t NumOfElements = 0;
	};

	template<typename LootType, Variance ContentVariant>
	class CoreLootBag : public BaseLootBag<LootType, ContentVariant>
	{
		template<typename>
		friend class CoreLootTable;
	protected:
		CoreLootBag(uint32_t InitialSize) : BaseLootBag(InitialSize) {}
		bool GrabLoot(std::list<LootType*>& OutLoot) override { return false; }
	};

	template<typename LootType, Variance ContentVariant>
	inline impl::BaseLootBag<LootType, ContentVariant>::BaseLootBag(uint32_t InitialSize) :
		Size(InitialSize),
		LootArray(new CoreLootContainer<LootType, ContentVariant>* [InitialSize])
	{
	}

	template<typename LootType, Variance ContentVariant>
	inline impl::BaseLootBag<LootType, ContentVariant>::~BaseLootBag()
	{
		if (Size > 0)
			delete[] LootArray;
	}

	template<typename LootType, Variance ContentVariant>
	inline void impl::BaseLootBag<LootType, ContentVariant>::Resize(uint32_t NewSize)
	{
		if (NewSize == NumOfElements) return;

		CoreLootContainer<LootType, ContentVariant>** NewContainer = nullptr;

		NewContainer = new CoreLootContainer<LootType, ContentVariant>*[NewSize];
		for (uint32_t i = 0; i < (NewSize > NumOfElements)? NumOfElements: NewSize; ++i)
			NewContainer[i] = LootArray[i];

		delete[] LootArray;
		LootArray = NewContainer;
	}

	template<typename LootType, Variance ContentVariant>
	inline void impl::BaseLootBag<LootType, ContentVariant>::AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot)
	{
		if (NumOfElements == Size) {
			uint8_t MaxIncrement = 100;
			uint8_t IncrementAmount = 10;
			if (NumOfElements / 2 > MaxIncrement)
				Increment = MaxIncrement;

			// If 32 bit unsigned limit is reached, end;
			if ((uint64_t)(NumOfElements + IncrementAmount) > 4294967295)
				return;
			Resize(Size + Increment);
		}

		LootArray[NumOfElements++] = Loot;
	}

}