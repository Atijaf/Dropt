#pragma once
#include "Types.h"
#include <cstdint>

namespace Dropt {
	namespace impl
	{
		class AbstractLootObtainabilityController
		{
		protected:
			AbstractLootObtainabilityController() {};
		public:
			virtual bool ShouldRemoveFromContainer() const = 0;
		protected:
			virtual void Observe_GetLoot() {};
		};

		template<Obtainabilities Obtainability>
		class LootObtainabilityController : public AbstractLootObtainabilityController
		{
		protected:
			LootObtainabilityController() {};
		};

		template<>
		class LootObtainabilityController<Obtainabilities::Common> : public AbstractLootObtainabilityController
		{
		protected:
			LootObtainabilityController() {};
		public:
			bool ShouldRemoveFromContainer() const override { return false; }

		};

		template<>
		class LootObtainabilityController<Obtainabilities::Variable> : public AbstractLootObtainabilityController
		{
		protected:
			LootObtainabilityController() {};
		public:
			bool ShouldRemoveFromContainer() const override
			{
				if (NumOfTimesThisHasBeenObtained >= MaxNumOfTimesThisCanBeObtained)
					return true;
				return false;
			}
			void SetMaxNumOfTimesLootCanBeObtained(uint32_t _Num) { MaxNumOfTimesThisCanBeObtained = _Num; }
		protected:
			uint32_t MaxNumOfTimesThisCanBeObtained = 1;
			uint32_t NumOfTimesThisHasBeenObtained = 0;
			void Observe_GetLoot() override {
				++NumOfTimesThisHasBeenObtained;
			};
		};

		template<>
		class LootObtainabilityController<Obtainabilities::Unique> : public AbstractLootObtainabilityController
		{
		protected:
			LootObtainabilityController() {};
		public:
			bool ShouldRemoveFromContainer() const override { return bHasLootBeenObtainedOnce; }
			void Observe_GetLoot() override {
				bHasLootBeenObtainedOnce = true;
			}
		protected:
			bool bHasLootBeenObtainedOnce = false;
		};
	}
}