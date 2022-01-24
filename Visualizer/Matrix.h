#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <stdexcept>

class Matrix {
public:
    Matrix();
    Matrix(size_t n, size_t m);
    Matrix(const Matrix& x);
    ~Matrix();

    Matrix operator=(const Matrix& x);

    Matrix operator*(const Matrix& x) const;

    void apply(float(*func)(float));
    std::pair<size_t, size_t> getSize() const;

    float& operator()(size_t i, size_t j);
    const float& operator()(size_t i, size_t j) const;

private:
    size_t n;
    size_t m;
    float* data;
};
