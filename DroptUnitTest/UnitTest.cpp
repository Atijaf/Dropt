#include "../Dropt/src/Include/Interface.h"
#include "../Dropt/src/Include/Helper/MArray.h"
#include <string>
#include <charconv>

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

namespace Dropt
{
	namespace UnitTest {
		using namespace Helper;
		using namespace impl;

		struct Armor
		{
			Armor(const char* _Name) :Name(_Name) {};
			const char* Name;

			bool operator == (const Armor& Other)
			{
				return (std::strcmp(Name, Other.Name) == 0); // strings are equal
			}
		};

		static Interface Inter;

		template<Variance ContentVariant>
		static CoreLootBag<Armor, Variance::Constant, ContentVariant>* CreateAndFillBag(MArray<CoreElementLoot<Armor, ContentVariant>*> LootArray)
		{
			auto OutBag = Inter.CreateBag_Constant<Armor, ContentVariant>("Constant Bag");
			for (uint32_t i = 0; i < LootArray.GetSize(); ++i)
				OutBag->AddLoot(LootArray[i]);			
			OutBag->FinalizeLoot();
			return OutBag;
		}

		static MArray<CoreElementLoot<Armor,Variance::Chance>*> CreateWeightedArmor()
		{
			auto HeadArmorTmp = new Armor("Head Armor");
			auto TorsoArmorTmp = new Armor("Torso Armor");
			auto LegArmorTmp = new Armor("Leg Armor");
			auto FeetArmorTmp = new Armor("Feet Armor");

			MArray<CoreElementLoot<Armor, Variance::Chance>*> Out(4);

			Out[0] = Inter.CreateElementLoot_Weighted(HeadArmorTmp->Name, HeadArmorTmp, 2198889, 11);
			Out[1] = Inter.CreateElementLoot_Weighted(TorsoArmorTmp->Name, TorsoArmorTmp, 3490300, 31);
			Out[2] = Inter.CreateElementLoot_Weighted(LegArmorTmp->Name, LegArmorTmp, 438900, 17);
			Out[3] = Inter.CreateElementLoot_Weighted(FeetArmorTmp->Name, FeetArmorTmp, 1052100, 25);
			
			return Out;

			/*
			* Above is the same as this, which is what it should turn into once it finds the GCF
			auto HeadArmor = Inter.CreateElementLoot_Weighted(HeadArmorTmp->Name, HeadArmorTmp, 100, 11);
			auto TorsoArmorTmp = Inter.CreateElementLoot_Weighted(TorsoArmorTmp->Name, TorsoArmorTmp, 63, 11);
			auto LegArmorTmp = Inter.CreateElementLoot_Weighted(LegArmorTmp->Name, LegArmorTmp, 501, 11);
			auto FeetArmorTmp = Inter.CreateElementLoot_Weighted(FeetArmorTmp->Name, FeetArmorTmp, 209, 11);
			*/
		}

		static MArray<CoreElementLoot<Armor, Variance::Interval>*> CreateIntervalArmor()
		{
			auto HeadArmorTmp = new Armor("Head Armor");
			auto TorsoArmorTmp = new Armor("Torso Armor");
			auto LegArmorTmp = new Armor("Leg Armor");
			auto FeetArmorTmp = new Armor("Feet Armor");

			MArray<CoreElementLoot<Armor, Variance::Interval>*> Out(4);

			Out[0] = Inter.CreateElementLoot_Interval(HeadArmorTmp->Name, HeadArmorTmp, 5, 5);
			Out[1] = Inter.CreateElementLoot_Interval(TorsoArmorTmp->Name, TorsoArmorTmp, 3, 12);
			Out[2] = Inter.CreateElementLoot_Interval(LegArmorTmp->Name, LegArmorTmp, 2, 137);
			Out[3] = Inter.CreateElementLoot_Interval(FeetArmorTmp->Name, FeetArmorTmp, 11, 25);
			return Out;
		}

		template<typename LootType, Variance BagVariant>
		static bool IsBagSortedCorrectly(CoreLootBag<LootType, BagVariant, Variance::Chance>* Bag)
		{
			if (Bag->GetNumOfLoot() <= 1) return true;
			
			uint32_t PrevWeight = Bag->GetElementAtIndex(0)->GetWeight();
			for (uint32_t i = 1; i < Bag->GetNumOfLoot(); ++i)
			{
				uint32_t CurrentWeight = Bag->GetElementAtIndex(i)->GetWeight();
				if(PrevWeight < CurrentWeight)
					return false;
				PrevWeight = CurrentWeight;
			}
			return true;
		}

		template<typename LootType, Variance BagVariant>
		static bool IsBagSortedCorrectly(CoreLootBag<LootType, BagVariant, Variance::Interval>* Bag)
		{
			if (Bag->GetNumOfLoot() <= 1) return true;

			uint32_t PrevInterval = Bag->GetElementAtIndex(0)->GetInterval();
			for (uint32_t i = 1; i < Bag->GetNumOfLoot(); ++i)
			{
				uint32_t CurrentInterval = Bag->GetElementAtIndex(i)->GetInterval();
				if (PrevInterval > CurrentInterval)
					return false;
				PrevInterval = CurrentInterval;
			}
			return true;
		}



		//Confirm Weights are in order and a LCF was found
		static void TestWeightedBag()
		{
			auto ConstantBagWithWeightedArmor = CreateAndFillBag(CreateWeightedArmor());
			ASSERT(IsBagSortedCorrectly(ConstantBagWithWeightedArmor), "Weighted Bag was sorted incorrectly");
		}

		static void TestIntervalBag()
		{
			auto ConstantBagWithIntervalArmor = CreateAndFillBag(CreateIntervalArmor());
			ASSERT(IsBagSortedCorrectly(ConstantBagWithIntervalArmor), "Interval Bag is sorted incorrectly");

			//Interval of 2 Loot
			Armor* i2l =	new Armor("Leg Armor");		// Interval: 2  | 137
			Armor* i3l =	new Armor("Torso Armor");	// Interval: 3  | 12 times
			Armor* i5l =	new Armor("Head Armor");	// Interval: 5  | 5 times
			Armor* i11l =	new Armor("Feet Armor");	// Interval: 11 | 25 times
			Armor* LootArray[4] = { i2l,i3l,i5l,i11l };
			uint32_t Intervals[4] = { 2,3,5,11 };
			uint32_t MaxNumOfTimes[4] = { 138, 12,5,25 };
		
			uint32_t RollCounter = 1;
			MArray<MArray<Armor*>> ExpectedLoot(276);
			for (uint32_t i = 0; i < 275; ++i) {
				uint32_t Index2 = 0;
				for (uint8_t k = 0; k < 4; ++k) {
					if (RollCounter % Intervals[k] == 0 && MaxNumOfTimes[k] > 0) {
						ExpectedLoot[RollCounter].AddElement(LootArray[k]);
						++Index2;
						MaxNumOfTimes[k]--;
					}
				}
				++RollCounter;
			}

			RollCounter = 1;
			while (ConstantBagWithIntervalArmor->IsLootBagFinalized()) {
				std::list<Armor*> LootObtained;
				ConstantBagWithIntervalArmor->GetLoot(LootObtained);

				if (LootObtained.size() > 0) {
					uint32_t LootObtainedIndex = 0;
					ASSERT(LootObtained.size() == ExpectedLoot[RollCounter].GetNumOfElements(),
						("Expected " + std::to_string(ExpectedLoot[RollCounter].GetNumOfElements()) + " Elements, but got "
						+ std::to_string(LootObtained.size()))
						+ "\nRollerCounter: " + std::to_string(RollCounter));



					for (auto it = LootObtained.begin(); it != LootObtained.end(); ++it) {
						ASSERT((**it == *ExpectedLoot[RollCounter][LootObtainedIndex]),
							"Expected element " + std::string(ExpectedLoot[RollCounter][LootObtainedIndex]->Name) + " But got " + std::string((*it)->Name) +
							"\nRollCounter: " + std::to_string(RollCounter) + "\nLootObtainedIndex: " + std::to_string(LootObtainedIndex));
						++LootObtainedIndex;
					}
				}

				++RollCounter;
			}


			delete i2l; delete i3l; delete i5l; delete i11l;

		}

		static void TortureTest()
		{
			std::uniform_int_distribution<uint32_t> WeightRandIntDistrib(1,1593153210);
			std::uniform_int_distribution<uint32_t> IntervalRandIntDistrib(10,1000);
			std::uniform_int_distribution<uint32_t> RandLootIndexDistrib(0, 10000-1);

			std::mt19937 RandomEngine(0);

			CoreLootTable<Armor, Variance::Chance>* WeightedVariableTables[50];
			CoreLootTable<Armor, Variance::Interval>* IntervalVariableTables[50];
			CoreLootTable<Armor, Variance::Constant>* ConstantTable = Inter.CreateLootTable_Constant<Armor>("Constant Table");
			for (uint32_t i = 0; i < 50; ++i) {
				// Random Weight between 1 and 4,294,967,295
				uint32_t RandWeight = WeightRandIntDistrib(RandomEngine);
				const char* WeightedTableName = ("Weighted Table ");
				WeightedVariableTables[i] = Inter.CreateLootTable_Weighted<Armor>(WeightedTableName, RandWeight);

				// Random Interval between 10 and 1,000,000
				uint32_t RandInterval = IntervalRandIntDistrib(RandomEngine);
				const char* IntervalTableName = ("Interval Table ");
				IntervalVariableTables[i] = Inter.CreateLootTable_Interval<Armor>(IntervalTableName, RandInterval);
			}

			CoreElementLoot<Armor, Variance::Chance>* WeightedLoots[10000];
			Armor* WeightedLoot[10000];
			CoreElementLoot<Armor, Variance::Interval>* IntervalLoots[10000];
			Armor* IntervalLoot[10000];
			for (uint32_t i = 0; i < 10000; ++i) {
				uint32_t RandWeight = WeightRandIntDistrib(RandomEngine);
				const char* WeightedLootName = ("Weighted Loot ");
				WeightedLoot[i] = new Armor(WeightedLootName);
				WeightedLoots[i] = Inter.CreateElementLoot_Weighted<Armor>(WeightedLootName, WeightedLoot[i], RandWeight);

				uint32_t RandInterval = IntervalRandIntDistrib(RandomEngine);
				const char* IntervalLootName = ("Interval Loot ");
				IntervalLoot[i] = new Armor(IntervalLootName);
				IntervalLoots[i] = Inter.CreateElementLoot_Interval<Armor>(IntervalLootName, IntervalLoot[i], RandInterval);
			}

			for (uint32_t i = 0; i < 1000000; ++i)
			{
				uint32_t RandLootIndex = RandLootIndexDistrib(RandomEngine);
				if (!WeightedVariableTables[i % 50]->AddLoot(WeightedLoots[RandLootIndex]))
					return;
				if(!WeightedVariableTables[i % 50]->AddLoot(IntervalLoots[RandLootIndex]))
					return;
				if(!IntervalVariableTables[i % 50]->AddLoot(WeightedLoots[RandLootIndex]))
					return;
				if (!IntervalVariableTables[i % 50]->AddLoot(IntervalLoots[RandLootIndex]))
					return;
			}
			for (uint32_t i = 0; i < 50; ++i) {
				WeightedVariableTables[i]->FinalizeLoot();
				IntervalVariableTables[i]->FinalizeLoot();
				ConstantTable->AddLoot(WeightedVariableTables[i]);
				//ConstantTable->AddLoot(IntervalVariableTables[i]);
			}
			ConstantTable->FinalizeLoot();

			std::list<Armor*> ObtainedLoot;
			for (uint32_t i = 0; i < 100000; ++i) {
				ConstantTable->GetLoot(ObtainedLoot);
			}

			std::cout << ObtainedLoot.size();

			for (uint32_t i = 0; i < 10000; ++i)
			{
				delete WeightedLoot[i];
				delete IntervalLoot[i];
			}

		}
	}
}

/*
* Completed Tests
* - Weighted Bag, 
*	- Sorted
*	- GCF Found
* 
* - Interval Bag
*	- Sorted
*	- Offsets calculated
*	- Obtained correctly
*	* Need largely separted intervals tested
*
* - Variable Obtainability
*	- Tested in Interval Bag
* - Unique Obtainability
*	- Tested in Interval Bag
* 
* - Empty Bag
*	- Tested in Interval Bag
*/

int main() {
	using namespace Dropt::UnitTest;
	TestWeightedBag();
	TestIntervalBag();
	TortureTest();


	return 0;
}