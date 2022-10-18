#include "common/NeuralNetwork.h"

#include <torch/serialize.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>

NeuralNetwork loadModelFromFile(const std::filesystem::path& filepath) {
    std::ifstream modelFile(filepath, std::ios::binary);
    if (!modelFile.is_open()) {
        throw std::runtime_error("Couldn't open model file");
    }

    torch::serialize::InputArchive inp;
    inp.load_from(modelFile);
    NeuralNetwork model;
    model->load(inp);
    modelFile.close();

    return model;
}

int main(int argc, char** argv) {
    std::filesystem::path executable = std::filesystem::absolute(argv[0]);
    std::filesystem::path resourceDir = executable.parent_path() / "res";

    auto blackModelFile = argc > 1 ? std::filesystem::absolute(argv[1]) : resourceDir / "black.model";
    auto whiteModelFile = argc > 2 ? std::filesystem::absolute(argv[2]) : resourceDir / "white.model";
    NeuralNetwork blackModel = loadModelFromFile(blackModelFile);
    NeuralNetwork whiteModel = loadModelFromFile(whiteModelFile);



    return 0;
}
