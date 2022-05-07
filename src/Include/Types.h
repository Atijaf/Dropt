#pragma once

namespace impl
{
	// These can be used to define a LootBag and/or Loot, 
	// and they define the loot they are attached to (Ie. a unique LootBag can only be grabbed from once)
	enum class Identifiers
	{
		Unique,		// Can be obtained once
		Variable,	// Can be obtained x times
		Common,		// Can be obtained indefinitely
	};

	enum class Variance
	{
		Chance,		// Contents of Container uses Chance
		Interval,	// Contents of Container are intervaled
		Constant,	// All Contents of Container are obtained
	};
}