#pragma once
#include "Types.h"
#include <cstdint>

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

		bool operator <(const LootDispatchVariance<Variance::Interval>& Other) const {
			return this->GetInterval() < Other.GetInterval();
		}
		bool operator >(const LootDispatchVariance<Variance::Interval>& Other) const {
			return !(*this < Other);
		}
	protected:
		uint32_t Interval = 0;
	};

}