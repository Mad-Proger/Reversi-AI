#include "Matrix.h"

Matrix::Matrix() : n(), m(), data() {
}

Matrix::Matrix(size_t n, size_t m) : n(n), m(m) {
	data = new float[n * m];
	memset(data, 0, n * m * sizeof(float));
}

Matrix::Matrix(const Matrix& x) {
	n = x.n;
	m = x.m;

	data = new float[n * m];
	memcpy(data, x.data, n * m * sizeof(float));
}

Matrix::~Matrix() {
	delete[] data;
}

Matrix Matrix::operator=(const Matrix& x) {
	if (n * m != x.n * x.m) {
		delete[] data;
		data = new float[x.n * x.m];
	}
	
	n = x.n;
	m = x.m;
	memcpy(data, x.data, n * m * sizeof(float));

	return *this;
}

Matrix Matrix::operator*(const Matrix& x) const {
	if (m != x.n) {
		throw std::invalid_argument("Matrix sizes don't match");
	}

	Matrix res(n, x.m);
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			for (size_t k = 0; k < x.m; ++k) {
				res(i, k) += operator()(i, j) * x(j, k);
			}
		}
	}
}

void Matrix::apply(float(*func)(float)) {
	if (func == nullptr) {
		throw std::invalid_argument("Null function pointer");
	}
	
	for (size_t i = 0; i < n * m; ++i) {
		data[i] = func(data[i]);
	}
}

std::pair<size_t, size_t> Matrix::getSize() const {
	return std::pair{ n, m };
}

float& Matrix::operator()(size_t i, size_t j) {
	if (i >= n || j >= m) {
		throw std::out_of_range("Matrix index out of range");
	}
	return data[i * m + j];
}

const float& Matrix::operator()(size_t i, size_t j) const {
	if (i >= n || j >= m) {
		throw std::out_of_range("Matrix index out of range");
	}
	return data[i * m + j];
}