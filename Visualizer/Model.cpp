#include "Model.h"

Model::Model() : inputNeurons(), n(), weights() {
}

Model::Model(const Model& model) : inputNeurons(model.inputNeurons), n(model.n) {
    weights = new Matrix[n];
    for (size_t i = 0; i < n; ++i) {
        weights[i] = model.weights[i];
    }
}

Model::~Model() {
    delete[] weights;
}

Model Model::operator=(const Model& model) {
    if (n != model.n) {
        delete[] weights;
        weights = new Matrix[model.n];
    }

    inputNeurons = model.inputNeurons;
    n = model.n;

    for (size_t i = 0; i < n; ++i) {
        weights[i] = model.weights[i];
    }

    return *this;
}

void Model::load(const std::string& filename) {
    std::ifstream modelData(filename);
    if (!modelData.is_open()) {
        throw std::invalid_argument("Couldn't open model file");
    }

    this->~Model();

    modelData >> inputNeurons >> n;
    std::vector<size_t> hiddenNeurons(n);
    for (size_t i = 0; i < n; ++i) {
        modelData >> hiddenNeurons[i];
    }

    weights = new Matrix[n];
    size_t prevNeurons = inputNeurons;
    for (size_t i = 0; i < n; ++i) {
        weights[i] = Matrix(hiddenNeurons[i], prevNeurons);

        for (size_t x = 0; x < hiddenNeurons[i]; ++x) {
            for (size_t y = 0; y < prevNeurons; ++y) {
                modelData >> weights[i](x, y);
            }
        }

        prevNeurons = hiddenNeurons[i];
    }
}

Matrix Model::operator()(Matrix data) const {
    try {
        for (size_t i = 0; i < n; ++i) {
            data = weights[i] * data;
            data.apply(std::tanh);
        }
    } catch (std::invalid_argument) {
        throw std::invalid_argument("Invalid input shape");
    }

    return data;
}
