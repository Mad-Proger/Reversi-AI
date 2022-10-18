#pragma once

#include <vector>
#include <cstring>
#include <utility>

class Desk {
public:
    Desk();

    Desk(const Desk& d);

    ~Desk();

    Desk operator=(const Desk& d);

    bool checkMove(int x, int y, int color) const;

    bool checkAnyMove(int color) const;

    bool makeMove(int x, int y);

    int getCurrentColor() const;

    std::pair<int, int> getScore() const;

    std::vector<std::pair<int, int>> getPossibleMoves() const;

    int operator()(size_t x, size_t y) const;

private:
    int* fieldData;
    int** field;
    int currentColor;

    int distanceNearest(int x, int y, int color, int dx, int dy) const;
};
