#pragma once
#include <random>

namespace Dropt {
	namespace Helper
	{
		// Initialize seed by default
		static std::mt19937_64 DroptSeed(time(nullptr));
		// Allows initialization of DroptSeed with specific Seed
		static void InitSeed(int Seed) { DroptSeed = std::mt19937_64(Seed); }
	}
}