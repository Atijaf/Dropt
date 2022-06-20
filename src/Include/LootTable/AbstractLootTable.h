#pragma once
#include "../LootBag/LootBag.h"
#include <list>


namespace Dropt {
	
	class Interface;

	namespace impl
	{

		class AbstractLootTable
		{
		public:
			bool IsLootTableFinalized() const { return bLootTableFinalized; }
			virtual bool FinalizeLootTable() = 0;
			AbstractLootDispatcher* GetSibling() { return Sibling; }

		protected:
			AbstractLootTable(AbstractLootDispatcher* _Sibling) :
				Sibling(_Sibling)
			{

			}

			bool bLootTableFinalized = false;
			AbstractLootDispatcher* Sibling;
		};

		/// <summary>
		/// Forces Variant to remain the same amongst an array of these objects
		/// </summary>
		/// <typeparam name="Variant"> How the Loot Table is obtained (If stored within another Loot Table or Loot Bag)</typeparam>
		template<typename LootType>
		class BaseLootTable : public AbstractLootTable
		{
		public:

			template<Variance Variant>
			void AddLoot(BaseLootBag<LootType, Variant>* Loot) {
				AddLoot(Loot->GetSibling());
			}

			template<Variance Variant>
			void AddLoot(CoreLootContainer<LootType, Variant>* Loot) {}

			template<>
			void AddLoot(CoreLootContainer<LootType, Variance::Chance>* Loot) {
				if (CanAddLoot(Loot))
					WeightedLootBag.AddLoot(Loot);
			}


			template<>
			void AddLoot(CoreLootContainer<LootType, Variance::Constant>* Loot) {
				if (CanAddLoot(Loot))
					ConstantLootBag.AddLoot(Loot);
			}

			template<>
			void AddLoot(CoreLootContainer<LootType, Variance::Interval>* Loot) {
				if (CanAddLoot(Loot))
					IntervalLootBag.AddLoot(Loot);
			}

			uint64_t GetTotalLoot() {
				return	(uint64_t)(WeightedLootBag.GetNumOfLoot()) +
						(uint64_t)(IntervalLootBag.GetNumOfLoot()) +
						(uint64_t)(ConstantLootBag.GetNumOfLoot());
				return 0;
			}

			// Updates OutLoot and returns true if loot is obtained
			bool GetLoot(std::list<LootType*>& OutLoot) {
				return GetSibling()->GetLoot(OutLoot);
			}

			AbstractCoreLoot<LootType>* GetSibling() { 
				return static_cast<AbstractCoreLoot<LootType>*>(Sibling); 
			}

		protected:
			BaseLootTable(AbstractLootDispatcher* _Sibling) :
				AbstractLootTable(_Sibling)
			{

			}
			bool RollForLoot(std::list<LootType*>& OutLoot) {
				if (WeightedLootBag.GetNumOfLoot() > 0)
					WeightedLootBag.GetLoot(OutLoot);

				if (IntervalLootBag.GetNumOfLoot() > 0)
					IntervalLootBag.GetLoot(OutLoot);

				if (ConstantLootBag.GetNumOfLoot() > 0)
					ConstantLootBag.GetLoot(OutLoot);

				return (OutLoot.size() > 0);
			}

			bool FinalizeLootTable() {
				bLootTableFinalized = (WeightedLootBag.FinalizeLoot() &&
					IntervalLootBag.FinalizeLoot() &&
					ConstantLootBag.FinalizeLoot());
				return bLootTableFinalized;
			}

			template<Variance Variant>
			bool CanAddLoot(CoreLootContainer<LootType, Variant>* Loot) const 
			{ 
				return !this->IsLootTableFinalized(); 
			}
			
			template<> bool CanAddLoot<Variance::Chance>(CoreLootContainer<LootType, Variance::Chance>* Loot) const {
				return(!this->IsLootTableFinalized() &&	// If this table is finalized, nothing else can be added 
						Loot->GetWeight() != 0);		// Elements with 0 weight cannot be added
			}
			template<> bool CanAddLoot<Variance::Interval>(CoreLootContainer<LootType, Variance::Interval>* Loot) const {
				return(!this->IsLootTableFinalized() &&	// If this table is finalized, nothing else can be added 
						Loot->GetInterval() != 0);		// Elements with 0 Interval cannot be added
			}



			// LootBag with Weighted Drops
			LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Chance> WeightedLootBag;
			// LootBag with Interval Drops
			LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Interval> IntervalLootBag;
			// LootBag with Constant Drops
			LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Constant> ConstantLootBag;


		};

		template<typename LootType, Variance Variant>
		class CoreLootTable : public BaseLootTable<LootType>
		{
		public:
			CoreLootContainer<LootType, Variant>* GetSibling() { 
				return static_cast<CoreLootContainer<LootType,Variant>*>(this->Sibling); }
		protected:
			CoreLootTable(AbstractLootDispatcher* _Sibling) :
				BaseLootTable<LootType>(_Sibling) 
			{

			}


		};
	}
}