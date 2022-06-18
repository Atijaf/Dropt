#pragma once
#include "LootBag/LootBag.h"
#include "LootTable/LootTable.h"
#include "ElementLoot/Loot.h"
#include <unordered_map>
#include <type_traits>

namespace Dropt {
	using namespace impl;

	enum class Types {
		LootTable,	// Stores arrays of instances of user specified type
		LootBag,	// Stores array of interchangable Types of user specified type
		Loot,		// Stores instance of user specified type
	};

	enum class Rarity {
		Common,		// Element may be obtained indefintely
		Variable,	// Element may be obtained x amount of times
		Unique,		// Element may be obtained once
	};

	enum class ObtainMethod {
		Chance,		// Elements are obtained by chance
		Interval,	// Elements are obtained at an interval (every x times)
		Constant,	// Elements are obtained every time
	};

	class Interface
	{
	public:	
		

		template<typename LootType>
		CoreVariantLootTable<LootType, Variance::Chance>* CreateLootTable_Weighted(const char* TableName, uint32_t Weight, uint32_t MaxNumObtainable = -1)
		{
			CoreVariantLootTable<LootType, Variance::Chance>* OutTable;
			if (MaxNumObtainable == (uint32_t)-1) {
				OutTable = new LootTable<LootType, Variance::Chance, Obtainabilities::Common>();
			}
			else if (MaxNumObtainable == 1) {
				OutTable = new LootTable<LootType, Variance::Chance, Obtainabilities::Unique>();
			}
			else {
				auto TmpTable = new LootTable<LootType, Variance::Chance, Obtainabilities::Variable>();
				TmpTable->SetMaxNumOfTimesLootCanBeObtained(MaxNumObtainable);
				OutTable = TmpTable;
			}

			((CoreLootContainer<LootType, Variance::Chance>*)(OutTable))->SetWeight(Weight);

			AddToMemoryContainer(TableName, (AbstractLootDispatcher*)(OutTable));
			return OutTable;
		}


	private:
		bool AddToMemoryContainer(const char* DataName, AbstractLootDispatcher* Data) {
			MemoryContainer.insert(std::pair<const char*, AbstractLootDispatcher*>(DataName, Data));
			return true;
		}
		std::unordered_multimap<std::string, AbstractLootDispatcher*> MemoryContainer;


		template<typename LootType>
		class LootTypeFactory
		{
		private:
			Interface& InterfaceRef;

			template<Variance Variant, Obtainabilities Obtainability>
			LootTable<LootType, Variant, Obtainability>* InitBaseTable(const char* TableName) {
				auto Out = new LootTable<LootType, Variant, Obtainability>();
				InterfaceRef.AddToMemoryContainer(TableName, Out);
				return Out;
			}
			template<Obtainabilities Obtainability>
			LootTable<LootType, Variance::Chance, Obtainability>* InitWeightedTable(const char* TableName, uint32_t Weight) {
				auto Out = InitBaseTable<LootTable, Variance::Chance, Obtainability>(TableName);
				Out->SetWeight(Weight);
				return Out;
			}
		public:
			LootTypeFactory(Interface& _InterfaceRef) :
				InterfaceRef(_InterfaceRef) {};

			template<Obtainabilities Obtainability>
			LootTable<LootType, Variance::Chance, Obtainability>*
				CreateLootTable_Weighted(const char* TableName, uint32_t Weight, uint32_t MaxNumObtainable = -1) {
				auto OutLootTable = InitWeightedTable<Obtainability>(TableName, Weight);
				return OutLootTable;
			}

			template<>
			LootTable<LootType, Variance::Chance, Obtainabilities::Variable>*
				CreateLootTable_Weighted<Obtainabilities::Variable>(const char* TableName, uint32_t Weight, uint32_t MaxNumObtainable) {

				auto OutLootTable = InitWeightedTable<Obtainability>(TableName, Weight);
				OutLootTable->SetMaxNumOfTimesLootCanBeObtained(MaxNumObtainable);
				return OutLootTable;
			}
		};

	};
}