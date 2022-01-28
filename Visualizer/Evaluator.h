#pragma once

#include "Desk.h"
#include "Model.h"

class Evaluator {
public:
    Evaluator(const Model& model);

    float getPositionValue(const Desk& d) const;
private:
    Model model;
};