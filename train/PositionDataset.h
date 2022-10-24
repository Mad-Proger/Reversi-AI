#pragma once

#include "CompressedDesk.h"

#include <torch/data/datasets.h>
#include <torch/torch.h>

#include <cstdint>
#include <vector>

class PositionDataset:
    public torch::data::datasets::Dataset<PositionDataset> {
public:
    PositionDataset() = default;

    ExampleType get(size_t index) override;

    torch::optional<size_t> size() const override;

    void add(const CompressedDesk& d, float value);

    void clear();

private:
    std::vector<torch::Tensor> data;
    std::vector<torch::Tensor> target;
};
