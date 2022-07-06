#include "../Include/LootContainers/Handler/AbstractHandler.h"

namespace Dropt
{
	namespace impl
	{
		void AbstractHandler::AddReference(AbstractNode* Reference)
		{
			this->References.push_back(Reference);
		}
		void AbstractHandler::RemoveReference(AbstractNode* Reference)
		{
			this->References.remove(Reference);
		}
	}
}