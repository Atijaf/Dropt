#pragma once
#include <random>

namespace Dropt {
	namespace Helper
	{
		// Initialize seed by default
		static int Seed = 0;
		// Engine, using Seed, to generate random numbers
		static std::mt19937_64 RandomEngine(Seed);
		// Allows initialization of RandomEngine with specific Seed
		static void InitSeed(int _Seed) { Seed = _Seed; RandomEngine = std::mt19937_64(Seed); }
	}
}