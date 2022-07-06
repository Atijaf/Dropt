#pragma once
#include "../../Core/CoreLoot.h"
#include "../../Helper/Globals.h"
#include "../../Helper/MArray.h"
#include "../../ElementLoot/Loot.h"
#include "../Node.h"
#include "../Handler/AbstractHandler.h"


namespace Dropt {
	namespace impl
	{

		//TIHI Forward declared Loot Table
		template<typename LootTypeTmp, Variance Variant>
		class CoreLootTable;

		///*************************************************************************///
		/// <summary>																///
		/// Base class for all Loot Bag derivitives									///
		/// </summary>																///
		///*************************************************************************///
		
		class AbstractLootBag : public AbstractHandler {
		protected:
			AbstractLootBag(AbstractLootDispatcher* _Sister) :
				AbstractHandler(_Sister){}

		public:
			virtual ~AbstractLootBag() {};


			bool IsLootBagFinalized() const;
			bool CanLootBeObtained() const override;
			bool FinalizeLoot();

		protected:
			bool PrepareLootToBeObtained() override;
			virtual void RemoveIndexFromArray(const uint32_t Index) = 0;
			virtual void TrimArray() = 0;
			virtual uint32_t GetNumOfLoot() const = 0;


			// Threshhold for the LootArrayIndexOffset before we should reallocate LootArray
			static const uint16_t ResizeArrayThreshold = 1;
			// Offset Size by this amount
			uint16_t LootArrayIndexOffset = 0;
			// Can this bag be set as a drop?
			bool bIsFinalized = false;
			// Can loot from this bag be obtained?
			bool bCanLootBeObtained = false;
		};

		///*************************************************************************///
		/// <summary>																///
		/// Provides a little more implementation for the loot bags					///
		/// </summary>																///
		/// <typeparam name="LootType">The type of loot being stored</typeparam>	///
		///*************************************************************************///
		
		template<typename LootType>													
		class AbstractBaseLootBag : public AbstractLootBag
		{
		protected:
			AbstractBaseLootBag(AbstractLootDispatcher* _Sister) :
				AbstractLootBag(_Sister) {};
		public:
			virtual ~AbstractBaseLootBag() {};
			// Updates OutLoot and returns true if Loot is Obtained
			bool GetLoot(std::list<LootType*>& OutLoot);
		protected:

			virtual bool GrabLoot(std::list<LootType*>& OutLoot) = 0;
			
			AbstractCoreLoot<LootType>* GetSister();
		};

		template<typename LootType, Variance Variant>
		class BaseLootBag : public AbstractBaseLootBag<LootType>
		{
		public:
			virtual ~BaseLootBag() {};
			CoreLootContainer<LootType, Variant>* GetSister() {
				return static_cast<CoreLootContainer<LootType, Variant>*>(this->Sister);
			}
		protected:
			BaseLootBag(AbstractLootDispatcher* _Sister) :
				AbstractBaseLootBag<LootType>(_Sister) {}
		};

		///*************************************************************************************///
		/// <summary>																			///
		/// Provides more controll over the bag													///
		/// </summary>																			///
		/// <typeparam name="LootType">:		Type of loot stored </typeparam>				///
		/// <typeparam name="BagVariant">:		How bag is compared to other loot</typeparam>	///
		/// <typeparam name="ContentVariant">:	How Contents of bag is obtained</typeparam>		///
		///*************************************************************************************///
		
		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		class CoreLootBag : public BaseLootBag<LootType, BagVariant>
		{
		public:
			virtual ~CoreLootBag() {
				for (uint32_t i = 0; i < LootArray.GetNumOfElements(); ++i)
					delete LootArray[i];
			}

			CoreLootContainer<LootType, ContentVariant>* GetElementAtIndex(uint32_t Index);
			virtual bool AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot) = 0;
			bool AddLoot(BaseLootBag<LootType, ContentVariant>* Loot);
			bool AddLoot(CoreLootTable<LootType, ContentVariant>* Loot);
			bool AddLoot(CoreElementLoot<LootType, ContentVariant>* Loot);


			CoreLootContainer<LootType, BagVariant>* GetSister();

			uint32_t GetNumOfLoot() const override final;
		protected:
			/// <summary>
			/// Initializes a LootBag
			/// </summary>
			/// <param name="FuncSort">:	Sorting Function that returns bools and accepts two CoreLootContainer Pointers</param>
			/// <param name="InitialSize">:	Starting size of the array (Default 10)</param>
			CoreLootBag<LootType, BagVariant, ContentVariant>(uint32_t InitialSize, AbstractLootDispatcher* _Sister) :
				BaseLootBag<LootType, BagVariant>(_Sister),
				LootArray(InitialSize)
			{};

			void RemoveIndexFromArray(const uint32_t Index) override;
			void TrimArray() override;
			bool GrabLoot(std::list<LootType*>& OutLoot) override { return false; }

			Dropt::Helper::MArray<Node<LootType, ContentVariant>*> LootArray;
		};

		///*************************************************************************************///
		/// <summary>																			///
		/// Class from which Weighted, Interval, and Constant Loot Bag specializations			///
		/// derive from																			///
		/// </summary>																			///
		/// <typeparam name="LootType">:		Type of loot stored </typeparam>				///
		/// <typeparam name="BagVariant">:		How bag is compared to other loot</typeparam>	///
		/// <typeparam name="ContentVariant">:	How Contents of bag is obtained</typeparam>		///
		///*************************************************************************************///
		
		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		class CoreLootBagImpl : public CoreLootBag<LootType, BagVariant, ContentVariant> {
		protected:
			CoreLootBagImpl<LootType, BagVariant, ContentVariant>(uint32_t InitialSize, AbstractLootDispatcher* _Sister) :
				CoreLootBag<LootType, BagVariant, ContentVariant>(InitialSize, _Sister)
			{};
		};


		/// <summary>
		/// The final LootBag with all control paths predetermined
		/// </summary>
		/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
		/// <param name="Variant">:			Is this LootBag obtained by Chance, Interval, or constantly </param>
		/// <param name="Obtainability">:	Defines if this bag is Unique, Variable, or Common</param>
		/// <param name="ContentVariant">:	Defines if the contents of this bag are obtained by Chance, Interval, or Constantly</param>
		template<	typename		LootType,
					Variance		BagVariant,
					Obtainabilities Obtainability,
					Variance		ContentVariant>
		class LootBag : public CoreLootBagImpl<LootType, BagVariant, ContentVariant>, public CoreLoot<LootType, BagVariant, Obtainability>
		{
		public:
			using CoreLoot<LootType, BagVariant, Obtainability>::GetLoot;
			LootBag(uint32_t InitialSize = 10) :
					CoreLoot<LootType, BagVariant, Obtainability>(this),
					CoreLootBagImpl<LootType, BagVariant, ContentVariant>(InitialSize, this) {};


			// Performs final actions on a Loot Table to prepare it for grabs
			bool FinalizeLoot() override final;
			bool IsFinalized() const override final;

			bool AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot) override final;
		protected:
			// Implementation of GetLoot for LootBag, defined in AbstractLootBag
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override final{
				return this->GrabLoot(OutLoot);
			}

		private:
			// Returns true if the Loot Bag has no contents OR if it was set to be removed after x amount of grabs (Unique/variable LootBag)
			// This means that it will be removed from it's owner's container
			bool ShouldRemoveFromContainer() const override {
				return (LootObtainabilityController<Obtainability>::ShouldRemoveFromContainer() ||
					this->GetNumOfLoot() == 0);
			}

		};




		///*********************************************************************************///
		///																					///
		///								ALL FUNCTION DEFINITIONS							///
		///																					///
		///*********************************************************************************///

		template<typename LootType, Variance BagVariant, Obtainabilities Obtainability, Variance ContentVariant>
		inline bool Dropt::impl::LootBag<LootType, BagVariant, Obtainability, ContentVariant>::FinalizeLoot()
		{
			return AbstractLootBag::FinalizeLoot();
		}

		template<typename LootType, Variance BagVariant, Obtainabilities Obtainability, Variance ContentVariant>
		inline bool Dropt::impl::LootBag<LootType, BagVariant, Obtainability, ContentVariant>::IsFinalized() const
		{
			return AbstractLootBag::IsLootBagFinalized();
		}


		template<typename	LootType, 
							Variance		BagVariant, 
							Obtainabilities Obtainability, 
							Variance		ContentVariant>
		inline bool Dropt::impl::LootBag<LootType, BagVariant, Obtainability, ContentVariant>::
			AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot)
		{
			// Any Loot being added to a Loot Bag must be finalized..  i.e. Confirmed to be able to return loot
			// This solves circular nested Loot.
			if (!Loot->IsFinalized()) {
				// Try to finalize the Loot
				if (!Loot->FinalizeLoot())
					return false;
			}
			Node<LootType, ContentVariant>* NewNode = new Node<LootType, ContentVariant>(*Loot, this->GetSister());
			if (this->LootArray.AddElement(NewNode)) {
				return true;
			}

			delete NewNode;
			return false;
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(BaseLootBag<LootType, ContentVariant>* Loot)
		{
			return AddLoot(Loot->GetSister());
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(CoreLootTable<LootType, ContentVariant>* Loot)
		{
			return AddLoot(Loot->GetSister()); // This works due to forward declaration of CoreLootTable
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline bool CoreLootBag<LootType, BagVariant, ContentVariant>::AddLoot(CoreElementLoot<LootType, ContentVariant>* Loot)
		{
			return AddLoot(Loot->GetSister());
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline uint32_t CoreLootBag<LootType, BagVariant, ContentVariant>::GetNumOfLoot() const
		{
			return LootArray.GetNumOfElements() - this->LootArrayIndexOffset;
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline CoreLootContainer<LootType, ContentVariant>* CoreLootBag<LootType, BagVariant, ContentVariant>::GetElementAtIndex(uint32_t Index)
		{
			if (Index < LootArray.GetNumOfElements())
				return LootArray[Index];
			return nullptr;
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		inline CoreLootContainer<LootType, BagVariant>* CoreLootBag<LootType, BagVariant, ContentVariant>::GetSister()
		{
			return static_cast<CoreLootContainer<LootType, BagVariant>*>(this->Sister);
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
			// Before trimming the array, we must delete unallocated memory
			for (uint32_t i = GetNumOfLoot(); i < this->LootArray.GetNumOfElements(); ++i)
				delete this->LootArray[i];

			LootArray.Resize(GetNumOfLoot());
			this->LootArrayIndexOffset = 0;
		}

		template<typename LootType>
		inline bool AbstractBaseLootBag<LootType>::GetLoot(std::list<LootType*>& OutLoot)
		{
			return GetSister()->GetLoot(OutLoot);
		}

		template<typename LootType>
		inline AbstractCoreLoot<LootType>* AbstractBaseLootBag<LootType>::GetSister()
		{
			return static_cast<AbstractCoreLoot<LootType>*>(this->Sister);
		}

}
}