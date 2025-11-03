#pragma once

#include "conversion.h"
#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

namespace otbv {
void data_to_file_bytes(std::ostream &stream, const std::vector<bool> &data,
                        const std::tuple<size_t, size_t, size_t> resolution);

// void save(std::string filename, volume<bool> data);

// void load();

} // namespace otbv
