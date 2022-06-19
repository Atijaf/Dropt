#pragma once
#include "../ElementLoot/Loot.h"
#include "../Helper/MArray.h"
#include "../Helper/Globals.h"
#include "../Core/CoreLoot.h"
#include <random>

namespace Dropt {
	namespace impl
	{

		// Forward Declaration
		//template<typename LootType, Variance ContentVariant>
		//class CoreLootBagInterface;

		class AbstractLootBag {
		public:
			bool IsLootBagFinalized() const { return bIsFinalized; }

		protected:
			AbstractLootBag(AbstractLootDispatcher* _Sibling):
				Sibling(_Sibling)
			{
			}

			bool FinalizeLootBag() {
				if (GetNumOfLoot() == 0)
					return false;
				if (IsLootBagFinalized())
					return true;

				if (FinalizeLootBag_impl())
					bIsFinalized = true;
				return bIsFinalized;
			}

			virtual uint32_t GetNumOfLoot() const = 0;

			AbstractLootDispatcher* GetSibling() { return Sibling; }

			AbstractLootDispatcher* Sibling;

			// Threshhold for the LootArrayIndexOffset before we should reallocate LootArray
			static const uint16_t ResizeArrayThreshold = 1;
			// Offset Size by this amount
			uint16_t LootArrayIndexOffset = 0;

			bool bIsFinalized = false;
		private:
			virtual bool FinalizeLootBag_impl() { return true; }
		};

		/// <summary>
		/// A Core class that can be used to get loot from the Bag
		/// </summary>
		/// <typeparam name="LootType"> Type of Loot being stored in the Bag</typeparam>
		template<typename LootType>
		class BaseLootBag : public AbstractLootBag
		{
		public:
			// Updates OutLoot and returns true if Loot is Obtained
			bool GetLoot(std::list<LootType*>& OutLoot) { return GetSibling()->GetLoot(OutLoot); }
		protected:
			BaseLootBag(AbstractLootDispatcher* _Sibling) :
				AbstractLootBag(_Sibling) {};

			virtual bool GrabLoot(std::list<LootType*>& OutLoot) = 0;
			virtual void RemoveIndexFromArray(const uint32_t Index) = 0;
			virtual void TrimArray() = 0;
			AbstractCoreLoot<LootType>* GetSibling() {
				return static_cast<AbstractCoreLoot<LootType>*>(Sibling);
			}

			std::uniform_int_distribution<uint64_t> BagIntDistrib;
		};

		template<typename LootType, Variance Variant>
		class CoreLootBag : public BaseLootBag<LootType>
		{
		public:
		protected:
			CoreLootBag(AbstractLootDispatcher* _Sibling):
				BaseLootBag(_Sibling){}

			CoreLootContainer<LootType, Variant>* GetSibling() {
				return static_cast<CoreLootContainer<LootType, Variant>*(Sibling);
			}
		};



		/// <summary>
		/// A sort of Interface class that allows adding loot, 
		/// and provides pure virtual functions for derived classes to define
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		class CoreLootBagInterface : public CoreLootBag<LootType, BagVariant>
		{
		public:
			// Catch all add loot function
			template<Obtainabilities Obtainability>
			bool AddLoot(CoreLoot<LootType, ContentVariant, Obtainability>* Loot);
			uint32_t GetNumOfLoot() const override { return LootArray.GetNumOfElements() - LootArrayIndexOffset; }

			
		protected:
			/// <summary>
			/// Initializes a LootBag
			/// </summary>
			/// <param name="FuncSort">:	Sorting Function that returns bools and accepts two CoreLootContainer Pointers</param>
			/// <param name="InitialSize">:	Starting size of the array (Default 10)</param>
			CoreLootBagInterface<LootType, BagVariant, ContentVariant>(	
					uint32_t					InitialSize,
					AbstractLootDispatcher*		_Sibling):
				
				// Initializers		
				CoreLootBag(_Sibling),
				LootArray(InitialSize) 
			{
			};

			virtual ~CoreLootBagInterface() {};

			virtual void RemoveIndexFromArray(const uint32_t Index) override {};
			virtual void TrimArray() override;
			CoreLootContainer<LootType, BagVariant>* GetSibling() {
				return static_cast<CoreLootContainer<LootType, BagVariant>*>(Sibling);
			}

			Dropt::Helper::MArray<CoreLootContainer<LootType, ContentVariant>*> LootArray;
		};


		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		class CoreLootBagImpl : public CoreLootBagInterface<LootType, BagVariant, ContentVariant> {
		protected:
			CoreLootBagImpl<LootType, BagVariant, ContentVariant>(	
					uint32_t				InitialSize,
					AbstractLootDispatcher* _Sibling):
				CoreLootBagInterface(InitialSize, _Sibling) 
			{

			};
		};
		




		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		template<Obtainabilities Obtainability>
		inline bool CoreLootBagInterface<LootType, BagVariant, ContentVariant>::AddLoot(CoreLoot<LootType, ContentVariant, Obtainability>* Loot)
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

		/*
		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline void CoreLootBagInterface<LootType, BagVariant, ContentVariant>::RemoveIndexFromArray(const uint32_t Index)
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
		*/

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline void impl::CoreLootBagInterface<LootType, BagVariant, ContentVariant>::TrimArray()
		{
			bIsFinalized = false;
			LootArray.Resize(GetNumOfLoot());
			LootArrayIndexOffset = 0;
			FinalizeLootBag();
		}
	}
}