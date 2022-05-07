#pragma once
#include "Types.h"
#include <cstdint>

namespace impl
{
	class AbstractLootObtainabilityController
	{
	public:
		virtual bool ShouldRemoveFromContainer() const = 0;
	};

	template<Obtainabilities Obtainability>
	class LootObtainabilityController : public AbstractLootObtainabilityController
	{
	};

	template<>
	class LootObtainabilityController<Obtainabilities::Common> : public AbstractLootObtainabilityController
	{
	public:
		bool ShouldRemoveFromContainer() const override { return false; }
	protected:
	};

	template<>
	class LootObtainabilityController<Obtainabilities::Variable> : public AbstractLootObtainabilityController
	{
	public:
		bool ShouldRemoveFromContainer() const override
		{
			if (NumOfTimesThisHasBeenObtained < MaxNumOfTimesThisCanBeObtained)
				return false;
			return true;
		}
		void SetMaxNumOfTimesLootCanBeObtained(uint32_t _Num) { MaxNumOfTimesThisCanBeObtained = _Num; }
	protected:
		uint32_t MaxNumOfTimesThisCanBeObtained = 1;
		uint32_t NumOfTimesThisHasBeenObtained = 0;
	};

	template<>
	class LootObtainabilityController<Obtainabilities::Unique> : public AbstractLootObtainabilityController
	{
	public:
		bool ShouldRemoveFromContainer() const override { return true; }
	protected:
	};
}