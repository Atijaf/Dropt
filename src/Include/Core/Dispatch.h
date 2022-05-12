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
		/// <summary>
		/// Obtains loot from a drop table
		/// </summary>
		/// <param name="OutLoot">The Loot</param>
		/// <returns>True if OutLoot size is greater than 0</returns>
		template<typename LootType>
		bool ObtainLoot(std::list<LootType*>& OutLoot){ 
			std::list<void*> ObtainedLoot;
			if (GetLoot(ObtainedLoot)) {
				for (auto it = ObtainedLoot.begin(); it != ObtainedLoot.end(); ++it) {
					OutLoot.push_back(static_cast<LootType*>(*it));
					return true;
				}
			}

			return false;
			// Convert void ptrs to LootType
		}

	protected:

		virtual bool GetLoot(std::list<void*>& OutLoot) = 0;
		virtual bool ShouldRemoveFromContainer() const = 0;
	};
}