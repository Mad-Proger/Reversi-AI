#pragma once

#include <torch/torch.h>

class NeuralNetwork: public torch::nn::Sequential {
public:
    NeuralNetwork(): torch::nn::Sequential(
        torch::nn::Linear(64, 200),
        torch::nn::Tanh(),
        torch::nn::Linear(200, 70),
        torch::nn::Tanh(),
        torch::nn::Linear(70, 1),
        torch::nn::Tanh()
    ) {};
};