#pragma once

#include "common/Desk.h"
#include "common/NeuralNetwork.h"

#include <torch/torch.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

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
