#include "Desk.h"

Desk::Desk():
    currentColor(1),
    blackMask(), whiteMask() {
    setColor(3, 3, -1);
    setColor(4, 4, -1);
    setColor(3, 4, 1);
    setColor(4, 3, 1);
}

bool Desk::checkMove(int x, int y, int color) const {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return false;
    }
    if (color != -1 && color != 1) {
        return false;
    }
    if (operator()(x, y) != 0) {
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

bool Desk::checkAnyMove() const {
    return checkAnyMove(-1) || checkAnyMove(1);
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
                setColor(x1, y1, currentColor);
            }
        }
    }

    setColor(x, y, currentColor);
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
            if (operator()(x, y) == 1) {
                ++black;
            } else if (operator()(x, y) == -1) {
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

        if (operator()(x, y) == -color) {
            foundOpponent = true;
        } else if (operator()(x, y) == color) {
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

int Desk::operator()(int x, int y) const {
    if ((blackMask >> (8 * x + y)) & 1) {
        return 1;
    }
    if ((whiteMask >> (8 * x + y)) & 1) {
        return -1;
    }
    return 0;
}

void Desk::setColor(int x, int y, int color) {
    uint64_t bit = 1;
    bit <<= 8 * x + y;
    if (color == 1) {
        blackMask |= bit;
        whiteMask &= ~bit;
    } else {
        whiteMask |= bit;
        blackMask &= ~bit;
    }
}
