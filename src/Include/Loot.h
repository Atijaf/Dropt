#pragma once
#include "Dispatch.h"
#include "LootDispatcherDefinition.h"
#include "LootDispatcherIdentity.h"
#include "CoreLoot.h"
namespace impl
{


	/// <summary>
	/// Loot Class Wrapper for creating obtainable loot (The Final Product)
	/// </summary>
	/// <typeparam name="LootType">:	The Type of Loot being stored</typeparam>
	/// <typeparam name="Control">:		If loot is obtained via Chance, Interval, or is constant</typeparam>
	/// <typeparam name="Ident">:		If the loot is Unique, Variable, or Common</typeparam>
	template<typename LootType, Variance Control, Identifiers Ident>
	class ElementLoot : public CoreLoot<LootType, Control>, public LootDispatcherIdentity<Ident>
	{
	public:
		ElementLoot(LootType* _Loot) : Loot(_Loot) {};

		bool GetLoot(std::list<void*>& OutLoot) override{
			OutLoot.push_back(Loot);
			return true;
		}
	protected:
		LootType* Loot;
	};
}