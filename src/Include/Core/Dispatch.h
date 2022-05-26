#pragma once

#include "LootObtainabilityController.h"
#include <cstdint>
#include <list>
#include "Types.h"

namespace impl
{

	// Core class of all objects using the magic of OOP
	class AbstractLootDispatcher
	{
	public:

		virtual bool FinalizeLoot() = 0;
		virtual bool IsFinalized() const = 0;
		
		// Should we remove the passed object from the object containing it?
		static bool ShouldRemoveFromContainer(const AbstractLootDispatcher* ObtainabilityController) {
			return ObtainabilityController->ShouldRemoveFromContainer();
		}

	private:

		// Returns true if we should remove this object from the container it is stored in.
		// Is called form a static parent method
		virtual bool ShouldRemoveFromContainer() const = 0;
	};
}