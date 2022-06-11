#pragma once
#include "Types.h"
#include <cstdint>

namespace Dropt {
	namespace impl
	{
		/// <summary>
		/// All LootDispatchVariance objects shall be inherited from classes that want to be of that Type
		/// ie. Loot that has one of these traits
		/// Chance		: The object is weighted amongst other objects
		/// Interval	: The object is obtained via an Interval
		/// Constant	: The object is always obtained
		/// </summary>


		class AbstractLootDispatchVariance
		{
		};

		template<Variance Variant>
		class LootDispatchVariance : public AbstractLootDispatchVariance
		{
		public:
			LootDispatchVariance() {};

			bool operator <(const LootDispatchVariance<Variant>& Other) const {
				return false;
			}
			bool operator >(const LootDispatchVariance<Variant>& Other) const {
				return false;
			}
		};

		template<>
		class LootDispatchVariance<Variance::Chance> : public AbstractLootDispatchVariance
		{
		public:
			LootDispatchVariance() {};

			uint64_t GetRelativeWeight() const { return RelativeWeight; }
			uint32_t GetWeight() const { return Weight; }

			void SetRelativeWeight(uint64_t _RelativeWeight) { RelativeWeight = _RelativeWeight; }
			void SetWeight(uint32_t _Weight) { Weight = _Weight; }

			bool operator <(const LootDispatchVariance<Variance::Chance>& Other) const {
				return this->GetWeight() < Other.GetWeight();
			}
			bool operator >(const LootDispatchVariance<Variance::Chance>& Other) const {
				return !(*this < Other);
			}
		protected:
			uint64_t RelativeWeight = 0;
			uint32_t Weight = 0;
		};

		template<>
		class LootDispatchVariance<Variance::Interval> : public AbstractLootDispatchVariance
		{
		public:

			uint32_t GetInterval() const { return Interval; }

			void SetInterval(uint32_t _Interval) { Interval = _Interval; }
			uint32_t GetOffset() const { return Offset; }
			void CalculateOffset(const uint32_t MaxInterval);

			bool operator <(const LootDispatchVariance<Variance::Interval>& Other) const {
				return this->GetInterval() < Other.GetInterval();
			}
			bool operator >(const LootDispatchVariance<Variance::Interval>& Other) const {
				return !(*this < Other);
			}

		protected:
			// Per x amount of rolls, if roll number % Interval is 0, loot is obtained
			uint32_t Interval = 0;
			/// Assume you have an array of loot containing these Intervals: 5,7.
			/// We want to reset the roll counter to 0 after hitting 7, but
			/// then the intervals would be messed up, as interval number 5 wouldn't
			/// happen until roll 5, when it should have happened earlier since 
			/// we reset the roll counter to 0 on roll number 7.
			/// Offset would be set to (MaxInterval % Interval + offset) % Interval
			///						   (7 % 5 + 0) % 5 == 2
			uint32_t Offset = 0;
		};

		inline void LootDispatchVariance<Variance::Interval>::CalculateOffset(const uint32_t MaxInterval)
		{
			Offset = (MaxInterval % Interval + Offset) % Interval;
		}


	}
}