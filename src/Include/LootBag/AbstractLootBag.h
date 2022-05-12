#pragma once
#include "../ElementLoot/Loot.h"

namespace impl
{

	template<Variance ContentVariant>
	class AbstractLootBag
	{
	public:
	protected:
		VariantLoot<ContentVariant>** LootArray = new VariantLoot<ContentVariant>* [1];
		virtual bool GrabLoot(std::list<void*>& OutLoot) = 0;
	};

	template<Variance ContentVariant>
	class LootBagImplementation : public AbstractLootBag<ContentVariant>
	{};

	

	template<>
	class LootBagImplementation<Variance::Constant> : public AbstractLootBag<Variance::Constant>
	{
	protected:
		bool GrabLoot(std::list<void*>& OutLoot) override { return false; };
	};


	template<typename LootType, Variance ContentVariant>
	class CoreLootBag : public LootBagImplementation<ContentVariant>
	{
	public:
		void AddLoot(CoreLootContainer<LootType, ContentVariant>* Loot) {
			this->LootArray[0] = Loot;
		}
	};

	
	/// <summary>
	/// </summary>
	/// <param name="Loot Type">:		The type defined by user (ie. Money, Weapon, Armor..) </param>
	/// <param name="Variant">:			Is this LootBag obtained by Chance, Interval, or constantly </param>
	/// <param name="Obtainability">:	Defines if this bag is Unique, Variable, or Common</param>
	/// <param name="ContentVariant">:	Defines if the contents of this bag are obtained by Chance, Interval, or Constantly</param>
	template<	typename LootType,
				Variance Variant,
				Obtainabilities Obtainability,
				Variance ContentVariant>

	class LootBag : public CoreLootBag<LootType, ContentVariant>, public CoreLoot<LootType, Variant, Obtainability>
	{
	public:
		LootBag(){
			
		}
	protected:
		bool GetLoot_Impl(std::list<void*>& OutLoot) override {
			return GrabLoot(OutLoot);
		}
	};
}