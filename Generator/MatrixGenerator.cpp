#include "MatrixGenerator.h"

#include <random>

bool MatrixGenerator::GenerateNext(){
    static std::random_device device;
    static std::mt19937 rng(device());
    static std::uniform_int_distribution<std::mt19937::result_type> distr(0, 100);

    return distr(rng) >= 37;
}
