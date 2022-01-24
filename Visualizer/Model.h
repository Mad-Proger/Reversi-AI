#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "Matrix.h"

class Model {
public:
    Model();
    Model(const Model& model);
    ~Model();

    Model operator=(const Model& model);

    void load(const std::string& filename);

    Matrix operator()(Matrix data) const;

private:
    size_t inputNeurons;
    size_t n;
    Matrix* weights;
};
