#pragma once
#include "LootContainers/LootBag/LootBag.h"
#include "LootContainers/LootTable/LootTable.h"
#include "ElementLoot/Loot.h"
#include <unordered_map>
#include <type_traits>


namespace Dropt {
	using namespace impl;


	class Interface
	{
	public:	

		/// <summary>
		/// Creates a Loot Table that contains a weight
		/// </summary>
		/// <typeparam name="LootType"></typeparam>
		/// <param name="TableName">:			Name of this Table</param>
		/// <param name="Weight">:				Weight of this Table</param>
		/// <param name="MaxNumObtainable">:	How many times this Loot Table can be rolled</param>
		/// <returns> A CoreLootTable that defines the type of loot and variant of the Loot Table</returns>
		template<typename LootType>
		CoreLootTable<LootType, Variance::Chance>* CreateLootTable_Weighted(const char* TableName, uint32_t Weight, uint32_t MaxNumObtainable = -1)
		{
			auto OutTable = CreateLootTable<LootType, Variance::Chance>(TableName, MaxNumObtainable);
			OutTable->GetSibling()->SetWeight(Weight);
			return OutTable;
		}

		template<typename LootType, Variance ContentVariant>
		CoreLootBag<LootType, Variance::Chance, ContentVariant>* CreateBag_Weighted(const char* BagName, uint32_t BagWeight, uint32_t MaxNumObtainable = -1)
		{
			auto OutBag = CreateLootBag<LootType, Variance::Chance, ContentVariant>(BagName, MaxNumObtainable);
			OutBag->GetSibling()->SetWeight(BagWeight);
			return OutBag;
		}

		template<typename LootType>
		CoreElementLoot<LootType, Variance::Chance>* CreateElementLoot_Weighted(const char* LootName, LootType* Loot, uint32_t LootWeight, uint32_t MaxNumObtainable = -1)
		{
			auto OutLoot = CreateElementLoot<LootType, Variance::Chance>(LootName, Loot, MaxNumObtainable);
			OutLoot->GetSibling()->SetWeight(LootWeight);
			return OutLoot;
		}


		/// <summary>
		/// Creates a Loot Table that contains an Interval
		/// </summary>
		template<typename LootType>
		CoreLootTable<LootType, Variance::Interval>* CreateLootTable_Interval(const char* TableName, uint32_t Interval, uint32_t MaxNumObtainable = -1)
		{
			auto OutTable = CreateLootTable<LootType, Variance::Interval>(TableName, MaxNumObtainable);
			OutTable->GetSibling()->SetInterval(Interval);
			return OutTable;
		}

		template<typename LootType, Variance ContentVariant>
		CoreLootBag<LootType, Variance::Interval, ContentVariant>* CreateBag_Interval(const char* BagName, uint32_t BagInterval, uint32_t MaxNumObtainable = -1)
		{
			auto OutBag = CreateLootBag<LootType, Variance::Interval, ContentVariant>(BagName, MaxNumObtainable);
			OutBag->GetSibling()->SetInterval(BagInterval);
			return OutBag;
		}

		template<typename LootType>
		CoreElementLoot<LootType, Variance::Interval>* CreateElementLoot_Interval(const char* LootName, LootType* Loot, uint32_t LootInterval, uint32_t MaxNumObtainable = -1)
		{
			auto OutLoot = CreateElementLoot<LootType, Variance::Interval>(LootName, Loot, MaxNumObtainable);
			OutLoot->GetSibling()->SetInterval(LootInterval);
			return OutLoot;
		}

		/// <summary>
		/// Creates a Loot Table that is constant
		/// </summary>
		template<typename LootType>
		CoreLootTable<LootType, Variance::Constant>* CreateLootTable_Constant(const char* TableName, uint32_t MaxNumObtainable = -1)
		{
			return CreateLootTable<LootType, Variance::Constant>(TableName, MaxNumObtainable);
		}

		template<typename LootType, Variance ContentVariant>
		CoreLootBag<LootType, Variance::Constant, ContentVariant>* CreateBag_Constant(const char* BagName, uint32_t MaxNumObtainable = -1)
		{
			return CreateLootBag<LootType, Variance::Constant, ContentVariant>(BagName, MaxNumObtainable);
		}

		template<typename LootType>
		CoreElementLoot<LootType, Variance::Constant>* CreateElementLoot_Constant(const char* LootName, LootType* Loot, uint32_t MaxNumObtainable = -1)
		{
			return CreateElementLoot<LootType, Variance::Constant>(LootName, Loot, MaxNumObtainable);
		}


	private:



		template<typename LootType, Variance Variant>
		CoreLootTable<LootType, Variant>* CreateLootTable(const char* TableName, uint32_t MaxNumObtainable) {
			CoreLootTable<LootType, Variance::Chance>* OutTable;
			if (MaxNumObtainable == (uint32_t)-1) {
				OutTable = new LootTable<LootType, Variant, Obtainabilities::Common>();
			}
			else if (MaxNumObtainable == 1) {
				OutTable = new LootTable<LootType, Variant, Obtainabilities::Unique>();
			}
			else {
				auto TmpTable = new LootTable<LootType, Variant, Obtainabilities::Variable>();
				TmpTable->SetMaxNumOfTimesLootCanBeObtained(MaxNumObtainable);
				OutTable = TmpTable;
			}
			AddToMemoryContainer(TableName, OutTable);
			return OutTable;
		}

		template<typename LootType, Variance BagVariant, Variance ContentVariant>
		CoreLootBag<LootType, BagVariant, ContentVariant>* CreateLootBag(const char* BagName, uint32_t MaxNumObtainable) {
			CoreLootBag<LootType,BagVariant,ContentVariant>* OutBag;
			if (MaxNumObtainable == (uint32_t)-1) {
				OutBag = new LootBag<LootType, BagVariant, Obtainabilities::Common, ContentVariant>();
			}
			else if (MaxNumObtainable == 1) {
				OutBag = new LootBag<LootType, BagVariant, Obtainabilities::Unique, ContentVariant>();
			}
			else {
				auto TmpTable = new LootBag<LootType, BagVariant, Obtainabilities::Variable, ContentVariant>();
				TmpTable->SetMaxNumOfTimesLootCanBeObtained(MaxNumObtainable);
				OutBag = TmpTable;
			}
			AddToMemoryContainer(BagName, OutBag);
			return OutBag;
		}

		template<typename LootType, Variance Variant>
		CoreElementLoot<LootType, Variant>* CreateElementLoot(const char* LootName, LootType* _Loot, uint32_t MaxNumObtainable) {
			CoreElementLoot<LootType, Variance::Chance>* OutElementLoot;
			if (MaxNumObtainable == (uint32_t)-1) {
				OutElementLoot = new ElementLoot<LootType, Variant, Obtainabilities::Common>(_Loot);
			}
			else if (MaxNumObtainable == 1) {
				OutElementLoot = new ElementLoot<LootType, Variant, Obtainabilities::Unique>(_Loot);
			}
			else {
				auto TmpLoot = new ElementLoot<LootType, Variant, Obtainabilities::Variable>(_Loot);
				TmpLoot->SetMaxNumOfTimesLootCanBeObtained(MaxNumObtainable);
				OutElementLoot = TmpLoot;
			}
			AddToMemoryContainer(LootName, OutElementLoot);
			return OutElementLoot;
		}

		bool AddToMemoryContainer(const char* DataName, AbstractLootTable* Data) {
			TableMemoryContainer.insert(std::pair<const char*, AbstractLootTable*>(DataName, Data));
			return true;
		}
		bool AddToMemoryContainer(const char* DataName, AbstractLootBag* Data) {
			BagMemoryContainer.insert(std::pair<const char*, AbstractLootBag*>(DataName, Data));
			return true;
		}
		bool AddToMemoryContainer(const char* DataName, AbstractElementLoot* Data) {
			LootMemoryContainer.insert(std::pair<const char*, AbstractElementLoot*>(DataName, Data));
			return true;
		}
		std::unordered_multimap<std::string, AbstractLootTable*> TableMemoryContainer;
		std::unordered_multimap<std::string, AbstractLootBag*> BagMemoryContainer;
		std::unordered_multimap<std::string, AbstractElementLoot*> LootMemoryContainer;

	};
}