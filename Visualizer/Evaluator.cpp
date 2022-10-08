#include "Evaluator.h"

Evaluator::Evaluator() {}

float Evaluator::getPositionValue(const Desk& d) const {
    float res = 0.f;
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            res += d(i, j);
        }
    }
    return res / 64;
}
