#include "uuid-gen.hpp"
#include <random>
#include <algorithm>
#include <array>
#include "uuid.h"


uuids::uuid generateUuid() {
    static std::mt19937 generator = []() {
        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate(seed_data.begin(), seed_data.end(), std::ref(rd));
        std::seed_seq seq(seed_data.begin(), seed_data.end());
        return std::mt19937(seq);
    }();

    static uuids::uuid_random_generator gen(&generator);

    return gen();
}
