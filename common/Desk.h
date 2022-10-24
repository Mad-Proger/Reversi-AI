#pragma once

#include <cstdint>
#include <utility>
#include <vector>

class Desk {
public:
    Desk();

    bool checkMove(int x, int y, int color) const;

    bool checkAnyMove(int color) const;

    bool checkAnyMove() const;

    bool makeMove(int x, int y);

    int getCurrentColor() const;

    std::pair<int, int> getScore() const;

    std::vector<std::pair<int, int>> getPossibleMoves() const;

    int operator()(int x, int y) const;

private:
    int currentColor;
    uint64_t blackMask;
    uint64_t whiteMask;

    int distanceNearest(int x, int y, int color, int dx, int dy) const;
    void setColor(int x, int y, int color);
};
