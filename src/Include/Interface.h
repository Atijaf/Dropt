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
/*		
		template<typename LootType, Variance Variant, Obtainabilities Obtainability>
		LootTable<LootType, Variant, Obtainability>* CreateLootTable(const char* TableName) {
			LootTable<LootType, Variant, Obtainability>* Out = new LootTable<LootType, Variant, Obtainability>();
			AddToMemoryContainer(TableName, Out);
			return Out;
		}
*/

		template<Obtainabilities Obtain>
		static typename std::enable_if<Obtain == Obtainabilities::Unique, int>::type
		check2() {
			return 1;
		}

		/// <summary>
		/// Creates a user friendly interface for creating Loot of a specific type
		/// </summary>
		/// <typeparam name="LootType">:	Type of loot that will be stored </typeparam>
		/// 
		/// 
		template<typename LootType>
		class LootTypeFactory
		{
		public:
			LootTypeFactory() {};

		public:

			template<Obtainabilities Obtainability>
			LootTable<LootType, Variance::Chance, Obtainability>*
				CreateLootTable_Weighted(uint32_t Weight) {
				return nullptr;	//SFINAE
			}

			// Creates a weighted Loot Table that can be obtained x amount of times
			// CAN ONLY be used when Obtainability is set to Variable
			template<Obtainabilities Obtainability>
				typename std::enable_if<Obtainability == Obtainabilities::Variable,
				LootTable<LootType, Variance::Chance, Obtainabilities::Variable>*>::type
				CreateLootTable_Weighted(uint32_t Weight, uint32_t MaxNumObtainable) {
				return nullptr;	//SFINAE
			}

			

		public:
			template<>
			LootTable<LootType, Variance::Chance, Obtainabilities::Common>*
				CreateLootTable_Weighted<Obtainabilities::Common>(uint32_t Weight) {
				auto Out = new LootTable<LootType, Variance::Chance, Obtainabilities::Common>();
				return Out;
			}
			template<>
			LootTable<LootType, Variance::Chance, Obtainabilities::Unique>*
				CreateLootTable_Weighted<Obtainabilities::Unique>(uint32_t Weight) {
				auto Out = new LootTable<LootType, Variance::Chance, Obtainabilities::Unique>();
				return Out;
			}
			template<>
			LootTable<LootType, Variance::Chance, Obtainabilities::Variable>*
				CreateLootTable_Weighted<Obtainabilities::Variable>(uint32_t Weight, uint32_t MaxNumObtainable) {
				auto Out = new LootTable<LootType, Variance::Chance, Obtainabilities::Variable>();
				return Out;
			}

		};



	protected:


		bool AddToMemoryContainer(const char* DataName, AbstractLootDispatcher* Data) {
			MemoryContainer.insert(std::pair<const char*, AbstractLootDispatcher*>(DataName, Data));
			return true;
		}

		/*
		template<typename LootType, Obtainabilities Obtain>
		const LootTable<LootType, Variance::Chance, Obtain>* 
			CreateLootTable<Variance::Chance>(uint32_t Weight) {
			return nullptr;
		}
		*/




	private:
		std::unordered_multimap<std::string, AbstractLootDispatcher*> MemoryContainer;

	};
}