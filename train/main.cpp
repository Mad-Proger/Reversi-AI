#include "ModelTrainer.h"

#include <torch/serialize.h>

#include <filesystem>

int main(int argc, char** argv) {
    std::filesystem::path executable = std::filesystem::absolute(argv[0]);
    std::filesystem::path resourceDir = executable.parent_path() / "res";

    auto blackModelFile = argc > 1 ? std::filesystem::absolute(argv[1]) : resourceDir / "black.model";
    auto whiteModelFile = argc > 2 ? std::filesystem::absolute(argv[2]) : resourceDir / "white.model";

    ModelTrainer trainer(blackModelFile, whiteModelFile);

    std::cout << "Enter number of games: ";
    int cntGames;
    std::cin >> cntGames;

    std::cout << "Enter maximum number of threads: ";
    int jobs;
    std::cin >> jobs;

    std::cout << "Enter player random factor: ";
    float epsilon;
    std::cin >> epsilon;

    std::cout << "Enter number of epochs: ";
    int epochs;
    std::cin >> epochs;

    std::cout << "Enter learning rate: ";
    float learningRate;
    std::cin >> learningRate;

    trainer.playGames(cntGames, jobs, epsilon);
    trainer.fitModels(epochs, learningRate);
    trainer.saveModels(blackModelFile, whiteModelFile);

    return 0;
}
