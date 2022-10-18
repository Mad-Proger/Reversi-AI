#pragma once

#include "common/Desk.h"

#include <cstdint>

class CompressedDesk {
public:
    CompressedDesk() = default;
    CompressedDesk(const Desk& d);

    bool operator<(const CompressedDesk& other) const;

private:
    int cntPieces;
    int currentColor;
    uint64_t whitePiecesMask;
    uint64_t blackPiecesMask;
};
