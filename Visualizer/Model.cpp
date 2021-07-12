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
	std::ifstream input(filename);
	if (!input.is_open()) {
		throw std::invalid_argument("Could not open model file");
	}

	input >> inputNeurons >> n;
	std::vector<size_t> hiddenNeurons(n);
	for (size_t i = 0; i < n; ++i) {
		input >> hiddenNeurons[i];
	}

	size_t prevNeurons = inputNeurons;
	for (size_t i = 0; i < n; ++i) {
		weights[i] = Matrix(hiddenNeurons[i], prevNeurons);
		size_t weightsCount = hiddenNeurons[i] * prevNeurons;
		for (size_t j = 0; j < weightsCount; ++j) {
			input >> *(weights[i][0] + j);
		}
	}
}

void Model::load(const std::string& filename) {
	std::ifstream modelData(filename);
	if (!modelData.is_open()) {
		throw std::invalid_argument("Couldn't open model file");
	}

	modelData >> inputNeurons >> n;
	std::vector<size_t> hiddenNeurons(n);
	for (size_t i = 0; i < n; ++i) {
		modelData >> hiddenNeurons[i];
	}

	weights = new Matrix[n];
	size_t prevNeurons = inputNeurons;
	for (size_t i = 0; i < n; ++i) {
		weights[i] = Matrix(hiddenNeurons[i], prevNeurons);
		size_t weightsCount = hiddenNeurons[i] * prevNeurons;

		std::vector<float> weightsValues(weightsCount);
		for (size_t j = 0; j < weightsCount; ++j) {
			modelData >> weightsValues[j];
		}
		weights[i].setData(weightsValues);

		prevNeurons = hiddenNeurons[i];
	}
}

Matrix Model::operator()(Matrix data) const {
	auto [n, m] = data.getSize();
	if (n != inputNeurons || m != 1) {
		throw std::invalid_argument("Invalid input shape");
	}

	for (size_t i = 0; i < n; ++i) {
		data = weights[i] * data;
		data.apply(std::tanhf);
	}

	return data;
}
