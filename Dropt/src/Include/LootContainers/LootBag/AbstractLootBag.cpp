#include "LootBag.h"

bool Dropt::impl::AbstractLootBag::IsLootBagFinalized() const
{
	return bIsFinalized;
}

bool Dropt::impl::AbstractLootBag::CanLootBeObtained() const
{
	return bCanLootBeObtained;
}

bool Dropt::impl::AbstractLootBag::FinalizeLoot()
{
	if (this->PrepareLootToBeObtained())
		this->bIsFinalized = true;
	return this->bIsFinalized;
}

bool Dropt::impl::AbstractLootBag::PrepareLootToBeObtained()
{
	if (GetNumOfLoot() == 0)
		return false;
	this->TrimArray();
	return true;
}


