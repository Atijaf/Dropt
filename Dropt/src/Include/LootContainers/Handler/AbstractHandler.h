#pragma once
#include "../Node.h"
#include "../../Core/Dispatch.h"


namespace Dropt
{
	namespace impl
	{
		/// <summary>
		/// Stores commonalities between Loot Bag, Loot Table, and Element Loot
		/// A base to those three
		/// </summary>
		class AbstractHandler {
		protected:
			AbstractHandler(AbstractLootDispatcher* _Sister) :
				Sister(_Sister){};
		public:
			virtual ~AbstractHandler() {};

			AbstractLootDispatcher* GetSister() const { return Sister; }
			void AddReference(AbstractNode* Reference);
			void RemoveReference(AbstractNode* Reference);
			virtual bool CanLootBeObtained() const = 0;

		protected:
			virtual bool PrepareLootToBeObtained() = 0;

			AbstractLootDispatcher* Sister;
			// List of all references made of this
			std::list<AbstractNode*> References;
		};


	}
}