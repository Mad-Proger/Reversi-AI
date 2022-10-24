#include "CompressedDesk.h"

CompressedDesk::CompressedDesk(const Desk& d):
    cntPieces(0), currentColor(d.getCurrentColor()),
    blackPiecesMask(0), whitePiecesMask(0) {
    uint64_t bit = 1;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y, bit <<= 1) {
            if (d(x, y) == 0) {
                continue;
            }
            ++cntPieces;
            (d(x, y) == 1 ? blackPiecesMask : whitePiecesMask) |= bit;
        }
    }
}

bool CompressedDesk::operator<(const CompressedDesk& other) const {
    if (cntPieces != other.cntPieces) {
        return cntPieces > other.cntPieces;
    }
    if (currentColor != other.currentColor) {
        return currentColor < other.currentColor;
    }
    if (blackPiecesMask != other.blackPiecesMask) {
        return blackPiecesMask < other.blackPiecesMask;
    }
    return whitePiecesMask < other.whitePiecesMask;
}

torch::Tensor CompressedDesk::toTensor() const {
    torch::Tensor res = torch::zeros({ 64 });
    uint64_t bit = 1;
    for (int i = 0; i < 64; ++i, bit <<= 1) {
        if (blackPiecesMask & bit) {
            res[i] = 1.f;
        } else if (whitePiecesMask & bit) {
            res[i] = -1.f;
        }
    }
    return res;
}

int CompressedDesk::getCurrentColor() const {
    return currentColor;
}
