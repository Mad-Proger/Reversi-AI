#include "Evaluator.h"

Evaluator::Evaluator(const Model& model) : model(model) {
}

float Evaluator::getPositionValue(const Desk& d) const {
    Matrix input(65, 1);
    for (size_t x = 0; x < 8; ++x) {
        for (size_t y = 0; y < 8; ++y) {
            input(8 * x + y, 0) = float(d(x, y));
        }
    }
    input(64, 0) = float(d.getCurrentColor());
    return model(input)(0, 0);
}