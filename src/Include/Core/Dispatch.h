#pragma once
#include <cstdint>
#include <list>
#include "Types.h"

namespace impl
{

	// Core class of all objects using the magic of OOP
	class AbstractLootDispatcher
	{
	public:

	protected:
		virtual bool ShouldRemoveFromContainer() const = 0;
	};
}