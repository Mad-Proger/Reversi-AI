#include "ModelTrainer.h"

ModelTrainer::ModelTrainer(const std::filesystem::path& blackModelFilepath,
                           const std::filesystem::path& whiteModelFilepath):
    blackModel(), whiteModel(),
    positionValues() {
    loadModelFromFile(blackModelFilepath, blackModel);
    loadModelFromFile(whiteModelFilepath, whiteModel);
}

void ModelTrainer::playGames(int cntGames, int jobs, float epsilon) {
    auto stateTree = generateStateTree(cntGames, jobs, epsilon);
    std::cout << "Games played\n";
    generatePositionValues(stateTree);
    std::cout << "Positions evaluated" << std::endl;
}

void ModelTrainer::fitModels(int epochs, float learningRate) {
    auto [blackDataset, whiteDataset] = generateDatasets();
    std::cout << "Datasets generated\n";
    std::cout << "Black positions count: " << blackDataset.size().value() << '\n';
    std::cout << "White positions count: " << whiteDataset.size().value() << std::endl;
    fitModel(blackModel, std::move(blackDataset), epochs, learningRate);
    fitModel(whiteModel, std::move(whiteDataset), epochs, learningRate);
}

void ModelTrainer::saveModels(const std::filesystem::path& blackModelFilepath,
                              const std::filesystem::path& whiteModelFilepath) {
    saveModelToFile(blackModelFilepath, blackModel);
    saveModelToFile(whiteModelFilepath, whiteModel);
}

void ModelTrainer::loadModelFromFile(const std::filesystem::path& filepath, NeuralNetwork& model) {
    std::ifstream modelFile(filepath, std::ios::binary);
    if (!modelFile.is_open()) {
        throw std::runtime_error("Couldn't open model file");
    }

    torch::serialize::InputArchive inp;
    inp.load_from(modelFile);
    model->load(inp);
    modelFile.close();
}

void ModelTrainer::saveModelToFile(const std::filesystem::path& filepath, NeuralNetwork& model) {
    std::ofstream modelFile(filepath, std::ios::binary);
    if (!modelFile.is_open()) {
        throw std::runtime_error("Couldn't open model file");
    }

    torch::serialize::OutputArchive out;
    model->save(out);
    out.save_to(modelFile);
    modelFile.close();
}

std::map<CompressedDesk, std::set<CompressedDesk>> ModelTrainer::generateStateTree(
    int cntGames, int jobs, float epsilon) {
    std::vector<std::thread> threads(jobs);
    std::map<CompressedDesk, std::set<CompressedDesk>> stateTree;
    std::mutex mutex;

    for (int i = 0; i < jobs; ++i) {
        std::thread buf(&ModelTrainer::playGamesThread, this,
                        std::ref(stateTree), std::ref(mutex),
                        (cntGames + jobs - 1) / jobs, epsilon);
        std::swap(threads[i], buf);
    }
    for (int i = 0; i < jobs; ++i) {
        threads[i].join();
    }

    return stateTree;
}

void ModelTrainer::generatePositionValues(const std::map<CompressedDesk, std::set<CompressedDesk>>& stateTree) {
    positionValues.clear();
    for (const auto& [st, nextStates] : stateTree) {
        if (nextStates.empty()) {
            positionValues[st] = *st.toTensor().mean().data_ptr<float>();
            continue;
        }

        positionValues[st] = st.getCurrentColor() == 1 ? -1.f : 1.f;
        for (const auto& nxt : nextStates) {
            if (st.getCurrentColor() == 1) {
                positionValues[st] = std::max(positionValues[st], positionValues[nxt]);
            } else {
                positionValues[st] = std::min(positionValues[st], positionValues[nxt]);
            }
        }
    }
}

void ModelTrainer::playGamesThread(std::map<CompressedDesk, std::set<CompressedDesk>>& stateTree,
                                   std::mutex& mutex, int cntGames, float epsilon) {
    std::vector<std::pair<CompressedDesk, CompressedDesk>> moves;
    EpsilonGreedy player(blackModel, whiteModel, epsilon);

    for (int i = 0; i < cntGames; ++i) {
        Desk st, prev;

        while (st.checkAnyMove()) {
            prev = st;
            auto [x, y] = player.findMove(st);
            st.makeMove(x, y);

            moves.emplace_back(prev, st);
        }
    }

    mutex.lock();
    for (const auto& [prev, st] : moves) {
        stateTree[prev].insert(st);
        stateTree[st];
    }
    mutex.unlock();
}

void ModelTrainer::fitModel(NeuralNetwork& model, PositionDataset&& dataset, int epochs, float learningRate) {
    torch::optim::SGD optimizer(model->parameters(), torch::optim::SGDOptions(learningRate));
    int batchSize = static_cast<int>(std::sqrt(dataset.size().value()));
    auto dataLoader = torch::data::make_data_loader(std::move(dataset),
                                                    torch::data::DataLoaderOptions(batchSize).workers(20));

    for (int epoch = 1; epoch <= epochs; ++epoch) {
        for (const auto& batch : *dataLoader) {
            auto [data, target] = stackBatch(batch);
            optimizer.zero_grad();
            torch::Tensor results = model->forward(data);
            torch::Tensor loss = torch::nn::functional::mse_loss(results, target);
            loss.backward();
            optimizer.step();
        }
        std::cout << "Epoch " << epoch << " done" << std::endl;
    }
}

std::pair<PositionDataset, PositionDataset> ModelTrainer::generateDatasets() const {
    PositionDataset blackDataset, whiteDataset;

    for (const auto& [position, value] : positionValues) {
        if (position.getCurrentColor() == 1) {
            blackDataset.add(position, value);
        } else {
            whiteDataset.add(position, value);
        }
    }

    return { blackDataset, whiteDataset };
}

std::pair<torch::Tensor, torch::Tensor> ModelTrainer::stackBatch(const std::vector<torch::data::Example<>>& batch) {
    std::vector<torch::Tensor> data, target;
    for (const auto& [d, t] : batch) {
        data.push_back(d);
        target.push_back(t);
    }
    return { torch::stack(data), torch::stack(target) };
}
