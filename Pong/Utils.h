#pragma once
#include <random>

// Utils.h - Shared stateless utility functions
namespace Utils {
    inline float randomFloat(std::mt19937& rng, float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
}
