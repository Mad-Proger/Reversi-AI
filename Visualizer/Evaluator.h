#pragma once

#include "Desk.h"

#include <torch/torch.h>

class Evaluator {
public:
    Evaluator(const torch::nn::Sequential& model);

    float getPositionValue(const Desk& d) const;
private:
    mutable torch::nn::Sequential m_model;

    static float getActualScore(const Desk& d);
    float getModelPrediction(const Desk& d) const;
};
