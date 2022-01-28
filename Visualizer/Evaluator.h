#pragma once

#include "Desk.h"
#include "src/model.h"
#include "src/tensor.h"

class Evaluator {
public:
    Evaluator(const keras2cpp::Model& model);

    float getPositionValue(const Desk& d) const;
private:
    const keras2cpp::Model& model;
};