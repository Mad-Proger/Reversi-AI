#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include "Desk.h"
#include "Model.h"
#include "Matrix.h"
#include "Player.h"

class AIPlayer :
    public Player {
public:
    AIPlayer(const Model& evaluator, size_t recursionDepth);

    void findMove(const Desk& d, int& xOpt, int& yOpt);

private:
    const size_t maxDepth;
    Model model;

    float evaluateBoard(const Desk& d) const;
    float dfs(float alpha, float beta, const Desk& d, size_t depth) const;
};
