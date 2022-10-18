#include "EpsilonGreedy.h"

EpsilonGreedy::EpsilonGreedy(NeuralNetwork  blackModel,
                             NeuralNetwork  whiteModel,
                             float epsilon):
        blackModel(std::move(blackModel)),
        whiteModel(std::move(whiteModel)),
        epsilon(epsilon),
        rnd(std::random_device()()),
        probabilityDistribution(),
        coordinateDistribution(0, 7){}

std::pair<int, int> EpsilonGreedy::findMove(const Desk& d) const {
    if (probabilityDistribution(rnd) < epsilon) {
        return findRandomMove(d);
    }
    return findOptimalMove(d);
}

std::pair<int, int> EpsilonGreedy::findRandomMove(const Desk& d) const {
    int color = d.getCurrentColor();
    if (!d.checkAnyMove(color)) {
        return { -1, -1 };
    }

    int x, y;
    do {
        x = coordinateDistribution(rnd);
        y = coordinateDistribution(rnd);
    } while (!d.checkMove(x, y, color));

    return { x, y };
}

std::pair<int, int> EpsilonGreedy::findOptimalMove(const Desk& d) const {
    int color = d.getCurrentColor();
    if (!d.checkAnyMove(color)) {
        return { -1, -1 };
    }

    std::pair<int, int> optMove{ -1, -1 };
    float value = color == 1 ? -1.f : 1.f;
    Desk dCopy;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (!d.checkMove(x, y, color)) {
                continue;
            }

            dCopy = d;
            dCopy.makeMove(x, y);
            float curValue = evaluatePosition(dCopy);
            if ((color == 1 && curValue >= value) ||
                (color == -1 && curValue <= value)) {
                optMove = { x, y };
                value = curValue;
            }
        }
    }

    return optMove;
}

float EpsilonGreedy::evaluatePosition(const Desk& d) const {
    int color = d.getCurrentColor();
    auto& model = color == 1 ? blackModel : whiteModel;

    torch::Tensor inp = torch::zeros({ 8, 8 });
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            inp[x][y] = d(x, y);
        }
    }

    return *model->forward(inp.reshape({ -1 })).data_ptr<float>();
}
