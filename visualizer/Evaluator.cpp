#include "Evaluator.h"

Evaluator::Evaluator(const std::filesystem::path& blackModelFile,
                     const std::filesystem::path& whiteModelFile):
                     blackModel(), whiteModel() {
    loadModel(blackModel, blackModelFile);
    loadModel(whiteModel, whiteModelFile);    
}

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
            res += static_cast<float>(d(i, j));
        }
    }
    return res / 64;
}

float Evaluator::getModelPrediction(const Desk& d) const {
    torch::Tensor inp = torch::zeros({ 8, 8 });
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            inp[i][j] = d(i, j);
        }
    }

    auto& model = d.getCurrentColor() == 1 ? blackModel : whiteModel;
    auto res = model->forward(inp.reshape({ -1 }));
    return *res.data_ptr<float>();
}

void Evaluator::loadModel(NeuralNetwork& model,
                          const std::filesystem::path& modelFilepath) {
    std::ifstream modelFile(modelFilepath, std::ios::binary);
    if (!modelFile.is_open()) {
        throw std::runtime_error("Couldn't open model file");
    }

    torch::serialize::InputArchive in;
    in.load_from(modelFile);
    model->load(in);

    modelFile.close();
}
