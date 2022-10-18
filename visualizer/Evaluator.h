#pragma once

#include "Desk.h"

#include <torch/torch.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

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

class Evaluator {
public:
    Evaluator(const std::filesystem::path& blackModelFile,
              const std::filesystem::path& whiteModelFile);

    float getPositionValue(const Desk& d) const;
private:
    mutable NeuralNetwork blackModel;
    mutable NeuralNetwork whiteModel;

    static float getActualScore(const Desk& d);
    float getModelPrediction(const Desk& d) const;
    
    static void loadModel(NeuralNetwork& model,
                          const std::filesystem::path& modelFile);
};
