#pragma once

#include "common/NeuralNetwork.h"
#include "CompressedDesk.h"
#include "EpsilonGreedy.h"
#include "PositionDataset.h"

#include <torch/nn/functional.h>
#include <torch/data/datasets.h>
#include <torch/data/dataloader.h>
#include <torch/optim.h>
#include <torch/serialize.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class ModelTrainer {
public:
    ModelTrainer(const std::filesystem::path& blackModelFilepath,
                 const std::filesystem::path& whiteModelFilepath);

    void playGames(int cntGames, int jobs, float epsilon);

    void fitModels(int epochs, float learningRate);

    void saveModels(const std::filesystem::path& blackModelFilepath,
                    const std::filesystem::path& whiteModelFilepath);

private:
    NeuralNetwork blackModel;
    NeuralNetwork whiteModel;
    std::map<CompressedDesk, float> positionValues;

    static void loadModelFromFile(const std::filesystem::path& filepath, NeuralNetwork& model);

    static void saveModelToFile(const std::filesystem::path& filepath, NeuralNetwork& model);

    std::map<CompressedDesk, std::set<CompressedDesk>> generateStateTree(int cntGames, int jobs, float epsilon);

    void generatePositionValues(const std::map<CompressedDesk, std::set<CompressedDesk>>& stateTree);

    std::pair<PositionDataset, PositionDataset> generateDatasets() const;

    void playGamesThread(std::map<CompressedDesk, std::set<CompressedDesk>>& stateTree,
                         std::mutex& mutex, int cntGames, float epsilon);

    static void fitModel(NeuralNetwork& model, PositionDataset&& dataset, int epochs, float learningRate);

    static std::pair<torch::Tensor, torch::Tensor> stackBatch(const std::vector<torch::data::Example<>>& batch);
};
