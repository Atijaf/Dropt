#pragma once
#include "AbstractLootBag.h"

namespace impl
{

	template<>
	class LootBagImplementation<Variance::Chance> : public AbstractLootBag<Variance::Chance>
	{
	protected:
		bool GrabLoot(std::list<void*>& OutLoot) override { return false; };
	};

}