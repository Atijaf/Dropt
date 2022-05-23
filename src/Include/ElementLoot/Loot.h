#pragma once
#include "../Core/Dispatch.h"
#include "../Core/CoreLoot.h"
namespace impl
{

	template<typename LootType>
	class CoreElementLoot {
	public:
		LootType* GetLoot() const { return Loot; }
	protected:
		LootType* Loot;
	};

	/// <summary>
	/// Loot Class Wrapper for creating obtainable loot (The Final Product)
	/// </summary>
	/// <typeparam name="LootType">:		The Type of Loot being stored</typeparam>
	/// <typeparam name="Variant">:			If loot is obtained via Chance, Interval, or is constant</typeparam>
	/// <typeparam name="Obtainability">:	If the loot is Unique, Variable, or Common</typeparam>
	template<typename LootType, Variance Variant, Obtainabilities Obtainability>
	class ElementLoot : public CoreElementLoot<LootType>, public CoreLoot<LootType, Variant, Obtainability>
	{
	public:
		ElementLoot(LootType* _Loot) : Loot(_Loot) {};
		bool FinalizeLoot() override final { return true; }

	protected:
		bool GetLoot_Impl(std::list<LootType*>& OutLoot) override{
			OutLoot.push_back(CoreElementLoot::GetLoot());
			return true;
		}

		LootType* Loot;
	};
}