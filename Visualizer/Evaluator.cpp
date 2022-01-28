#include "Evaluator.h"

Evaluator::Evaluator(const keras2cpp::Model& model) : model(model) {
}

float Evaluator::getPositionValue(const Desk& d) const {
    keras2cpp::Tensor input{ 65 };
    size_t idx = 0;
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            input(idx++) = float(d(i, j));
        }
    }
    input(idx++) = float(d.getCurrentColor());
    return model(input)(0);
}
