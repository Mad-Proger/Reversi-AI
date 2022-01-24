#include "Desk.h"

Desk::Desk() {
    fieldData = new int[8 * 8];
    memset(fieldData, 0, 8 * 8 * sizeof(int));

    field = new int* [8];
    for (size_t i = 0; i < 8; ++i) {
        field[i] = fieldData + 8 * i;
    }

    field[3][3] = field[4][4] = -1;
    field[3][4] = field[4][3] = 1;
    currentColor = 1;
}

Desk::Desk(const Desk& d) {
    fieldData = new int[8 * 8];
    memcpy(fieldData, d.fieldData, 8 * 8 * sizeof(int));

    field = new int* [8];
    for (size_t i = 0; i < 8; ++i) {
        field[i] = fieldData + 8 * i;
    }

    currentColor = d.currentColor;
}

Desk::~Desk() {
    delete[] fieldData;
    delete[] field;
}

Desk Desk::operator=(const Desk& d) {
    memcpy(fieldData, d.fieldData, 8 * 8 * sizeof(int));
    currentColor = d.currentColor;
    return *this;
}

bool Desk::checkMove(int x, int y, int color) const {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return false;
    }
    if (color != -1 && color != 1) {
        return false;
    }
    if (field[x][y] != 0) {
        return false;
    }

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            if (distanceNearest(x, y, color, dx, dy) != -1) {
                return true;
            }
        }
    }

    return false;
}

bool Desk::checkAnyMove(int color) const {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (checkMove(x, y, color)) {
                return true;
            }
        }
    }

    return false;
}

bool Desk::makeMove(int x, int y) {
    if (!checkMove(x, y, currentColor)) {
        return false;
    }

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int x1 = x;
            int y1 = y;
            int d = distanceNearest(x, y, currentColor, dx, dy);
            for (int i = 0; i < d; ++i) {
                x1 += dx;
                y1 += dy;
                field[x1][y1] = currentColor;
            }
        }
    }

    field[x][y] = currentColor;
    if (checkAnyMove(-currentColor)) {
        currentColor = -currentColor;
    }

    return true;
}

int Desk::getCurrentColor() const {
    return currentColor;
}

std::pair<int, int> Desk::getScore() const {
    int black = 0;
    int white = 0;

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (field[x][y] == 1) {
                ++black;
            } else if (field[x][y] == -1) {
                ++white;
            }
        }
    }

    return std::pair{ black, white };
}

std::vector<std::pair<int, int>> Desk::getPossibleMoves() const {
    std::vector<std::pair<int, int>> res;

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (checkMove(x, y, currentColor)) {
                res.emplace_back(x, y);
            }
        }
    }

    return res;
}

int Desk::distanceNearest(int x, int y, int color, int dx, int dy) const {
    bool foundOpponent = false;
    for (int d = 0; d < 8; ++d) {
        x += dx;
        y += dy;

        if (x < 0 || x >= 8 || y < 0 || y >= 8) {
            break;
        }

        if (field[x][y] == -color) {
            foundOpponent = true;
        } else if (field[x][y] == color) {
            if (foundOpponent) {
                return d;
            } else {
                return -1;
            }
        } else {
            return -1;
        }
    }

    return -1;
}

int Desk::operator()(size_t x, size_t y) const {
    if (x >= 8 || y >= 8) {
        return 0;
    }
    return field[x][y];
}
