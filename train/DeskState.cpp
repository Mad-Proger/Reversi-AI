#include "DeskState.h"

DeskState::DeskState(): Desk(), cntPieces(4) {}

bool DeskState::operator<(const DeskState& other) const {
    if (cntPieces != other.cntPieces) {
        return cntPieces > other.cntPieces;
    }
    if (currentColor != other.currentColor) {
        return currentColor < other.currentColor;
    }
    if (blackMask != other.blackMask) {
        return blackMask < other.blackMask;
    }
    return whiteMask < other.whiteMask;
}

torch::Tensor DeskState::toTensor() const {
    torch::Tensor res = torch::zeros({ 64 });
    uint64_t bit = 1;
    for (int i = 0; i < 64; ++i, bit <<= 1) {
        if (blackMask & bit) {
            res[i] = 1.f;
        } else if (whiteMask & bit) {
            res[i] = -1.f;
        }
    }
    return res;
}
