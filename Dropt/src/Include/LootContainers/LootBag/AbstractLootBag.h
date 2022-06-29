#pragma once
#include "../../ElementLoot/Loot.h"
#include "../../Helper/MArray.h"
#include "../../Helper/Globals.h"
#include "../../Core/CoreLoot.h"
#include "../Node.h"
#include <random>

namespace Dropt {
	namespace impl
	{
		//TIHI Forward declared Loot Table
		template<typename LootTypeTmp, Variance Variant>
		class CoreLootTable;

		// Forward Declaration
		//template<typename LootType, Variance ContentVariant>
		//class CoreLootBag;

		class AbstractLootBag {
		public:

			virtual ~AbstractLootBag() {};

			bool IsLootBagFinalized() const { return bIsFinalized; }
			AbstractLootDispatcher* GetSibling() { return Sibling; }

			bool FinalizeLoot() {
				return GetSibling()->FinalizeLoot();
			}

		protected:

			virtual bool FinalizeLootBag()
			{
				if (GetNumOfLoot() == 0)
					return false;
				return true;
			}

			AbstractLootBag(AbstractLootDispatcher* _Sibling):
				Sibling(_Sibling)
			{
			}

			virtual uint32_t GetNumOfLoot() const = 0;


			AbstractLootDispatcher* Sibling;

			// Threshhold for the LootArrayIndexOffset before we should reallocate LootArray
			static const uint16_t ResizeArrayThreshold = 1;
			// Offset Size by this amount
			uint16_t LootArrayIndexOffset = 0;

			bool bIsFinalized = false;
		};

		/// <summary>
		/// A Core class that can be used to get loot from the Bag
		/// </summary>
		/// <typeparam name="LootType"> Type of Loot being stored in the Bag</typeparam>
		template<typename LootType>
		class AbstractBaseLootBag : public AbstractLootBag
		{
		public:
			virtual ~AbstractBaseLootBag() {};
			// Updates OutLoot and returns true if Loot is Obtained
			bool GetLoot(std::list<LootType*>& OutLoot) { return GetSibling()->GetLoot(OutLoot); }
		protected:
			AbstractBaseLootBag(AbstractLootDispatcher* _Sibling) :
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
		class BaseLootBag : public AbstractBaseLootBag<LootType>
		{
		public:
			virtual ~BaseLootBag() {};
			CoreLootContainer<LootType, Variant>* GetSibling() {
				return static_cast<CoreLootContainer<LootType, Variant>*>(this->Sibling);
			}
		protected:
			BaseLootBag(AbstractLootDispatcher* _Sibling):
				AbstractBaseLootBag<LootType>(_Sibling){}
		};

		/// <summary>
		/// A sort of Interface class that allows adding loot, 
		/// and provides pure virtual functions for derived classes to define
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		class CoreLootBag : public BaseLootBag<LootType, BagVariant>
		{
		public:

			virtual ~CoreLootBag() {
				for (uint32_t i = 0; i < LootArray.GetNumOfElements(); ++i)
					delete LootArray[i];
			}

			bool AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot);
			bool AddLoot(BaseLootBag<LootType, ContentVariant>* Loot);
			bool AddLoot(CoreLootTable<LootType, ContentVariant>* Loot);
			bool AddLoot(CoreElementLoot<LootType, ContentVariant>* Loot);

			CoreLootContainer<LootType, ContentVariant>* GetElementAtIndex(uint32_t Index) {
				if (Index < LootArray.GetNumOfElements())
					return LootArray[Index];
				return nullptr;
			}
			

			uint32_t GetNumOfLoot() const override { return LootArray.GetNumOfElements() - this->LootArrayIndexOffset; }

			CoreLootContainer<LootType, BagVariant>* GetSibling() {
				return static_cast<CoreLootContainer<LootType, BagVariant>*>(this->Sibling);
			}
		
		protected:
			/// <summary>
			/// Initializes a LootBag
			/// </summary>
			/// <param name="FuncSort">:	Sorting Function that returns bools and accepts two CoreLootContainer Pointers</param>
			/// <param name="InitialSize">:	Starting size of the array (Default 10)</param>
			CoreLootBag<LootType, BagVariant, ContentVariant>(	
					uint32_t					InitialSize,
					AbstractLootDispatcher*		_Sibling):
				
				// Initializers		
				BaseLootBag<LootType, BagVariant>(_Sibling),
				LootArray(InitialSize) 
			{
			};

			virtual void RemoveIndexFromArray(const uint32_t Index) override;
			virtual void TrimArray() override;

			Dropt::Helper::MArray<Node<LootType, ContentVariant>*> LootArray;
		};


		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		class CoreLootBagImpl : public CoreLootBag<LootType, BagVariant, ContentVariant> {
		protected:
			CoreLootBagImpl<LootType, BagVariant, ContentVariant>(	
					uint32_t				InitialSize,
					AbstractLootDispatcher* _Sibling):
				CoreLootBag(InitialSize, _Sibling) 
			{

			};
		};
		




		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot)
		{
			// Any Loot being added to a Loot Bag must be finalized..  i.e. Confirmed to be able to return loot
			// This solves circular nested Loot.
			if (!Loot->IsFinalized()) {
				// Try to finalize the Loot
				if (!Loot->FinalizeLoot())
					return false;
			}

			Node<LootType, ContentVariant>* NewNode = new Node<LootType, ContentVariant>(*Loot);
			if (LootArray.AddElement(NewNode))
				return true;
			delete NewNode;
			return false;
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(BaseLootBag<LootType, ContentVariant>* Loot)
		{
			return AddLoot(Loot->GetSibling());
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(CoreLootTable<LootType, ContentVariant>* Loot)
		{
			return AddLoot(Loot->GetSibling()); // This works due to forward declaration of CoreLootTable
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(CoreElementLoot<LootType, ContentVariant>* Loot)
		{
			return AddLoot(Loot->GetSibling());
		}



		

		
		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline void CoreLootBag<LootType, BagVariant, ContentVariant>::RemoveIndexFromArray(const uint32_t Index)
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
			++this->LootArrayIndexOffset;
			if (this->LootArrayIndexOffset >= this->ResizeArrayThreshold) {
				TrimArray();
			}
		}
		

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline void impl::CoreLootBag<LootType, BagVariant, ContentVariant>::TrimArray()
		{
			this->bIsFinalized = false;
			LootArray.Resize(GetNumOfLoot());
			this->LootArrayIndexOffset = 0;
			this->FinalizeLoot();
		}
	}
}