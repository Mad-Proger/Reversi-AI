#pragma once

#include <vector>
#include <utility>
#include <algorithm>

#include "common/Desk.h"
#include "Player.h"
#include "Evaluator.h"

class AIPlayer :
    public Player {
public:
    AIPlayer(const Evaluator& evaluator, size_t recursionDepth);

    void findMove(const Desk& d, int& xOpt, int& yOpt);

private:
    const size_t maxDepth;
    const Evaluator& evaluator;

    float dfs(float alpha, float beta, const Desk& d, size_t depth) const;
};
