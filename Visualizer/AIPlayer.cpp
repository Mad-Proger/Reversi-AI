#include "AIPlayer.h"

AIPlayer::AIPlayer(const Evaluator& evaluator, size_t recursionDepth) : evaluator(evaluator), maxDepth(recursionDepth) {
}

void AIPlayer::findMove(const Desk& d, int& xOpt, int& yOpt) {
    xOpt = 8;
    yOpt = 8;

    Desk d1;
    std::vector<std::pair<int, int>> possibleMoves = d.getPossibleMoves();
    int color = d.getCurrentColor();
    float optValue = color == 1 ? -1.f : 1.f;

    for (auto [x, y] : possibleMoves) {
        d1 = d;
        d1.makeMove(x, y);

        float value = dfs(-1.f, 1.f, d1, 1);
        if ((color == 1 && value >= optValue) ||
            (color == -1 && value <= optValue)) {
            optValue = value;
            xOpt = x;
            yOpt = y;
        }
    }
}

float AIPlayer::dfs(float alpha, float beta, const Desk& d, size_t depth) const {
    if (!d.checkAnyMove(-1) && !d.checkAnyMove(1)) {
        float value = 0.f;

        for (size_t x = 0; x < 8; ++x) {
            for (size_t y = 0; y < 8; ++y) {
                value += d(x, y);
            }
        }

        return value / 64.f;
    }
    if (depth >= maxDepth) {
        return evaluator.getPositionValue(d);
    }

    int color = d.getCurrentColor();
    float value = color == 1 ? -1.f : 1.f;
    std::vector<std::pair<int, int>> possibleMoves = d.getPossibleMoves();
    Desk d1;

    for (auto [x, y] : possibleMoves) {
        d1 = d;
        d1.makeMove(x, y);

        float bufValue = dfs(alpha, beta, d1, depth + 1);
        if (color == 1) {
            value = std::max(value, bufValue);
            if (value > beta) {
                break;
            }
            alpha = std::max(alpha, value);
        } else {
            value = std::min(value, bufValue);
            if (value < alpha) {
                break;
            }
            beta = std::min(beta, value);
        }
    }

    return value;
}
