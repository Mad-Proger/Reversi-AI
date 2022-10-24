#pragma once

#include "common/Desk.h"
#include "common/NeuralNetwork.h"

#include <utility>
#include <random>

class EpsilonGreedy {
public:
    EpsilonGreedy(NeuralNetwork blackModel,
                  NeuralNetwork whiteModel,
                  float epsilon);

    std::pair<int, int> findMove(const Desk& d) const;

private:
    mutable NeuralNetwork blackModel;
    mutable NeuralNetwork whiteModel;
    const float epsilon;
    mutable std::mt19937 rnd;
    mutable std::uniform_real_distribution<float> probabilityDistribution;
    mutable std::uniform_int_distribution<int> coordinateDistribution;

    std::pair<int, int> findRandomMove(const Desk& d) const;

    std::pair<int, int> findOptimalMove(const Desk& d) const;

    float evaluatePosition(const Desk& d) const;
};
