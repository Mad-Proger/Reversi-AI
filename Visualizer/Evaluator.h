#pragma once

#include "Desk.h"

#include <torch/torch.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

class Evaluator {
public:
    Evaluator(const std::filesystem::path& modelFile);

    float getPositionValue(const Desk& d) const;
private:
    mutable torch::nn::Sequential m_model;

    static float getActualScore(const Desk& d);
    float getModelPrediction(const Desk& d) const;
};
