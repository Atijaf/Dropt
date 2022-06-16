#pragma once
#include "../ElementLoot/Loot.h"
#include "../Helper/MArray.h"
#include "../Helper/Globals.h"
#include <random>

namespace Dropt {
	namespace impl
	{

		// Forward Declaration
		//template<typename LootType, Variance ContentVariant>
		//class CoreLootBag;

		class AbstractLootBag {
		public:
			bool IsLootBagFinalized() const { return bIsFinalized; }
		protected:
			bool FinalizeLootBag() {
				if (FinalizeLootBag_impl())
					bIsFinalized = true;
				return bIsFinalized;
			}

			// Threshhold for the LootArrayIndexOffset before we should reallocate LootArray
			static const uint16_t ResizeArrayThreshold = 1;
			// Offset Size by this amount
			uint16_t LootArrayIndexOffset = 0;

			bool bIsFinalized = false;
		private:
			virtual bool FinalizeLootBag_impl() = 0;
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
			uint32_t GetNumOfLoot() const { return LootArray.GetNumOfElements() - LootArrayIndexOffset; }

		protected:
			/// <summary>
			/// Initializes a LootBag
			/// </summary>
			/// <param name="FuncSort">:	Sorting Function that returns bools and accepts two CoreLootContainer Pointers</param>
			/// <param name="InitialSize">:	Starting size of the array (Default 10)</param>
			BaseLootBag(std::function<bool(CoreLootContainer<LootType, ContentVariant>* A,
				CoreLootContainer<LootType, ContentVariant>* B)> FuncSort,
				uint32_t InitialSize = 10) :
				LootArray(InitialSize, FuncSort) {}

			virtual ~BaseLootBag();

			virtual bool GrabLoot(std::list<LootType*>& OutLoot) { return false; }
			virtual void RemoveIndexFromArray(const uint32_t Index);
			virtual void TrimArray();

			std::uniform_int_distribution<uint64_t> BagIntDistrib;
			Dropt::Helper::MArray<CoreLootContainer<LootType, ContentVariant>*> LootArray;
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
			// Any Loot being added to a Loot Bag must be finalized..  i.e. Confirmed to be able to return loot
			// This solves circular nested Loot.
			if (!Loot->IsFinalized()) {
				// Try to finalize the Loot
				if (!Loot->FinalizeLoot())
					return false;
			}
			return LootArray.AddElement(Loot);
		}

		template<typename LootType, Variance ContentVariant>
		inline void BaseLootBag<LootType, ContentVariant>::RemoveIndexFromArray(const uint32_t Index)
		{
			// Remove loot by shifting all elements from the right to Index
			// i.e. Move Element at Index to the last index

			// 0,1,2,3,4
			// Index 2
			// 2=3
			// 3<5
			// 3=4
			// 5<5 no
			uint32_t i = Index;
			while (i + 1 < GetNumOfLoot()) {
				LootArray[i] = LootArray[i + 1];
				++i;
			}
			LootArray[i] = nullptr;

			// Increment Counter, check if we reached threshold to resize array
			++LootArrayIndexOffset;
			if (LootArrayIndexOffset >= ResizeArrayThreshold) {
				TrimArray();
			}
		}

		template<typename LootType, Variance ContentVariant>
		inline void impl::BaseLootBag<LootType, ContentVariant>::TrimArray()
		{
			bIsFinalized = false;
			LootArray.Resize(GetNumOfLoot());
			LootArrayIndexOffset = 0;
			FinalizeLootBag();
		}
	}
}