#pragma once
#include "AbstractLootBag.h"

namespace impl
{
	template<>
	class LootBagImplementation<Variance::Interval> : public AbstractLootBag<Variance::Interval>
	{
	protected:
		bool GrabLoot(std::list<void*>& OutLoot) override;
	};
}
