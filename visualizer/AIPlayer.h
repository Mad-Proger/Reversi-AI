#pragma once

#include "Player.h"
#include "common/Desk.h"
#include "Evaluator.h"

#include <algorithm>
#include <utility>
#include <vector>

class AIPlayer: public Player {
public:
    AIPlayer(const Evaluator& evaluator, size_t recursionDepth);

    void findMove(const Desk& d, int& xOpt, int& yOpt) override;

private:
    const size_t maxDepth;
    const Evaluator& evaluator;

    float dfs(float alpha, float beta, const Desk& d, size_t depth) const;
};
