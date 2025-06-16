#include "MatrixGenerator.h"

#include <random>
#include <algorithm>
#include <execution>

void MatrixGenerator::GenerateNext(quint8 width, quint8 height, std::vector<std::vector<bool>>& matrix){
    static std::random_device device;
    static std::mt19937 rng(device());
    static std::uniform_int_distribution<std::mt19937::result_type> distr(0, 100);

    matrix.resize(0);
    /*std::for_each(std::execution::par_unseq, matrix.begin(), matrix.end(), [&](std::vector<bool>& vec){
        vec.resize(height);
    });*/

    for(quint8 h = 0; h < height; h++){
        std::vector<bool> row(0);
        for(quint8 w = 0; w < width; w++){
            row.push_back(distr(rng) >= 45);
        }
        matrix.push_back(row);
    }
}
