#pragma once
#include "../../Core/CoreLoot.h"
#include "../LootBag/LootBag.h"


namespace Dropt {
	namespace impl
	{

		///*************************************************************************************///
		/// <summary>																			///
		/// Core class for Loot Tables															///
		/// </summary>																			///
		///*************************************************************************************///
		
		class AbstractLootTable : public AbstractHandler
		{
		public:
			virtual ~AbstractLootTable() {};
			bool CanLootBeObtained() const override { return bCanLootBeObtained; }
			bool IsLootTableFinalized() const { return bIsFinalized; }
			AbstractLootDispatcher* GetSister() { return Sister; }
			bool FinalizeLoot() {
				return GetSister()->FinalizeLoot();
			}

		protected:
			//virtual bool PrepareLootToBeObtained() = 0;
			AbstractLootTable(AbstractLootDispatcher* _Sister) :
				AbstractHandler(_Sister)
			{

			}

			bool bIsFinalized = false;
			bool bCanLootBeObtained = false;
		};


		///*************************************************************************************///
		/// <summary>																			///
		/// Provides basic implementation for Loot Tables										///
		/// </summary>																			///
		/// <typeparam name="LootType">: Type of loot being stored</typeparam>					///
		///*************************************************************************************///

		template<typename LootType>
		class BaseLootTable : public AbstractLootTable
		{
		protected:
			BaseLootTable(AbstractLootDispatcher* _Sister) :
				AbstractLootTable(_Sister)
			{};
		public:
			virtual ~BaseLootTable() {};

			template<Variance Variant>
			bool AddLoot(BaseLootBag<LootType, Variant>* Loot) {
				return AddLoot(Loot->GetSister());
			}

			template<Variance Variant>
			bool AddLoot(CoreLootTable<LootType, Variant>* Loot) {
				return AddLoot(Loot->GetSister());
			}

			template<Variance Variant>
			bool AddLoot(CoreElementLoot<LootType, Variant>* Loot) {
				return AddLoot(Loot->GetSister());
			}

			virtual bool AddLoot(CoreLootContainer<LootType, Variance::Chance>* Loot) =	0;
			virtual bool AddLoot(CoreLootContainer<LootType, Variance::Constant>* Loot) = 0;
			virtual bool AddLoot(CoreLootContainer<LootType, Variance::Interval>* Loot) = 0;


			uint64_t GetTotalLootCount() {
				return	(uint64_t)(WeightedLootBag.GetNumOfLoot()) +
						(uint64_t)(IntervalLootBag.GetNumOfLoot()) +
						(uint64_t)(ConstantLootBag.GetNumOfLoot());
			}

			// Updates OutLoot and returns true if loot is obtained
			bool GetLoot(std::list<LootType*>& OutLoot) {
				return GetSister()->GetLoot(OutLoot);
			}

			AbstractCoreLoot<LootType>* GetSister() {
				return static_cast<AbstractCoreLoot<LootType>*>(Sister);
			}

		protected:

			bool RollForLoot(std::list<LootType*>& OutLoot) {
				if (WeightedLootBag.GetNumOfLoot() > 0)
					WeightedLootBag.GetLoot(OutLoot);

				if (IntervalLootBag.GetNumOfLoot() > 0)
					IntervalLootBag.GetLoot(OutLoot);

				if (ConstantLootBag.GetNumOfLoot() > 0)
					ConstantLootBag.GetLoot(OutLoot);

				return (OutLoot.size() > 0);
			}

			bool PrepareLootToBeObtained() {
				bool bAtLeastOneBagIsFunctional = false;
				if (WeightedLootBag.FinalizeLoot())
					bAtLeastOneBagIsFunctional = true;

				if (IntervalLootBag.FinalizeLoot())
					bAtLeastOneBagIsFunctional = true;

				if (ConstantLootBag.FinalizeLoot())
					bAtLeastOneBagIsFunctional = true;

				return bAtLeastOneBagIsFunctional;
			}

			bool CanAddLoot() const
			{
				return !this->IsLootTableFinalized();
			}



			// LootBag with Weighted Drops
			LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Chance>
				WeightedLootBag;
			// LootBag with Interval Drops
			LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Interval>
				IntervalLootBag;
			// LootBag with Constant Drops
			LootBag<LootType, Variance::Constant, Obtainabilities::Common, Variance::Constant>
				ConstantLootBag;


		};

		///*************************************************************************************///
		/// <summary>																			///
		/// Provides a way to abstract Loot Tables without the need for obtainability			///
		/// </summary>																			///
		/// <typeparam name="LootType"></typeparam>												///
		///*************************************************************************************///
		template<typename LootType, Variance Variant>
		class CoreLootTable : public BaseLootTable<LootType>
		{
		public:
			CoreLootContainer<LootType, Variant>* GetSister() {
				return static_cast<CoreLootContainer<LootType, Variant>*>(this->Sister);
			}

		protected:
			CoreLootTable(AbstractLootDispatcher* _Sister) :
				BaseLootTable<LootType>(_Sister) {}
		};





		/// <summary>
		/// The final Loot Table with all control paths predetermined
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		class LootTable : public CoreLootTable<LootType, Variant>, public CoreLoot<LootType, Variant, Obtainability>
		{
		public:
			LootTable() :
				CoreLoot<LootType, Variant, Obtainability>(this),
				CoreLootTable<LootType, Variant>(this){}

			using CoreLoot<LootType, Variant, Obtainability>::GetLoot;	// Ambiguous solution
			bool FinalizeLoot() override final { 
				if (this->PrepareLootToBeObtained())
					this->bIsFinalized = true;
				return this->bIsFinalized; 
			}


			bool AddLoot(CoreLootContainer<LootType, Variance::Chance>* Loot) override final;
			bool AddLoot(CoreLootContainer<LootType, Variance::Interval>* Loot) override final;
			bool AddLoot(CoreLootContainer<LootType, Variance::Constant>* Loot) override final;

		protected:
			bool GetLoot_Impl(std::list<LootType*>& OutLoot) override {
				return this->RollForLoot(OutLoot);
			}

			virtual bool IsFinalized() const override { return this->IsLootTableFinalized(); }
		};



		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		inline bool Dropt::impl::LootTable<LootType, Variant, Obtainability>::
			AddLoot(CoreLootContainer<LootType, Variance::Chance>* Loot)
		{
			if (this->CanAddLoot() && Loot->IsFinalized()) {
				return this->WeightedLootBag.AddLoot(Loot);
			}
			return false;
		}
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		inline bool Dropt::impl::LootTable<LootType, Variant, Obtainability>::
			AddLoot(CoreLootContainer<LootType, Variance::Interval>* Loot)
		{
			if (this->CanAddLoot() && Loot->IsFinalized()) {
				return this->IntervalLootBag.AddLoot(Loot);
			}
			return false;
		}
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		inline bool Dropt::impl::LootTable<LootType, Variant, Obtainability>::
			AddLoot(CoreLootContainer<LootType, Variance::Constant>* Loot)
		{
			if (this->CanAddLoot() && Loot->IsFinalized()) {
				Node<LootType, Variance::Constant>* NewNode = new Node<LootType, Variance::Constant>(*Loot, this);
				if (this->ConstantLootBag.AddLoot(NewNode)) {
					
				}
				return this->ConstantLootBag.AddLoot(Loot);
			}
			return false;
		}
	}
}