#pragma once
#include "Dispatch.h"

namespace impl
{
	class AbstractLootDispatcherIdentity
	{
	public:
		virtual bool ShouldRemoveFromContainer() const = 0;
	protected:
	};


	template<Identifiers ContainerIdentity>
	class LootDispatcherIdentity : public AbstractLootDispatcherIdentity
	{
	public:
		virtual bool ShouldRemoveFromContainer() const = 0;
	protected:
	};

	template<>
	class LootDispatcherIdentity<Identifiers::Common> : public AbstractLootDispatcherIdentity
	{
	public:
		bool ShouldRemoveFromContainer() const override { return false; }
	protected:
	};

	template<>
	class LootDispatcherIdentity<Identifiers::Variable> : public AbstractLootDispatcherIdentity
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
	class LootDispatcherIdentity<Identifiers::Unique> : public AbstractLootDispatcherIdentity
	{
	public:
		bool ShouldRemoveFromContainer() const override { return true; }
	protected:
	};
}