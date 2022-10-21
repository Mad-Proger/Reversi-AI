#include "PositionDataset.h"

torch::data::Example<> PositionDataset::get(size_t index) {
    return { data[index], target[index]};
}

void PositionDataset::add(const CompressedDesk& d, float value) {
    data.push_back(d.toTensor());
    target.push_back(torch::full({ 1 }, value));
}

void PositionDataset::clear() {
    data.clear();
    target.clear();
}

torch::optional<size_t> PositionDataset::size() const {
    return data.size();
}
