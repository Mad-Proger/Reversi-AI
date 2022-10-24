#pragma once

#include "common/Desk.h"

#include <torch/torch.h>

#include <cstdint>

class DeskState: public Desk {
public:
    DeskState();

    bool operator<(const DeskState& other) const;

    torch::Tensor toTensor() const;

private:
    int cntPieces;
};
