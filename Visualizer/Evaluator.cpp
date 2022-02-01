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
    int color = d.getCurrentColor();
    input(idx++) = float(color);

    keras2cpp::Tensor output = model(input);
    output.resize(8, 8);
    float value = float(-color);
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (d.checkMove(x, y, d.getCurrentColor())) {
                value = std::min(value, -color * output(x, y));
            }
        }
    }
    return -color * value;
}
