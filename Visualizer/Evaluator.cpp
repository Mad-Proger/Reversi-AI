#include "Evaluator.h"

Evaluator::Evaluator(const torch::nn::Sequential& model) : m_model(model) {}

float Evaluator::getPositionValue(const Desk& d) const {
    if (d.checkAnyMove(1) && d.checkAnyMove(-1)) {
        return getActualScore(d);
    }
    return getModelPrediction(d);
}

float Evaluator::getActualScore(const Desk& d) {
    float res = 0.f;
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            res += d(i, j);
        }
    }
    return res / 64;
}

float Evaluator::getModelPrediction(const Desk& d) const {
    torch::Tensor inp = torch::zeros({ 8, 8 });
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            inp[i][j] = d(i, j);
        }
    }

    auto res = m_model->forward(inp.reshape({ -1 }));
    return *res.data_ptr<float>();
}
