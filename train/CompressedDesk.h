#pragma once

#include "common/Desk.h"

#include <torch/torch.h>

#include <cstdint>

class CompressedDesk {
public:
    CompressedDesk() = default;

    explicit CompressedDesk(const Desk& d);

    bool operator<(const CompressedDesk& other) const;

    torch::Tensor toTensor() const;

    int getCurrentColor() const;

private:
    int cntPieces;
    int currentColor;
    uint64_t whitePiecesMask;
    uint64_t blackPiecesMask;
};
