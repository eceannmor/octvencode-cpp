#pragma once

#include "conversion.h"
#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

namespace otbv {
void stream_data_as_file_bytes(
    std::ostream &stream, const std::vector<bool> &data,
    const std::tuple<size_t, size_t, size_t> resolution, const bool padded);

void save(std::string filename, std::vector<bool> data);
void save(std::string filename,
          std::vector<std::vector<std::vector<bool>>> data);

// void save(std::string filename, vector3<bool> data);

// void load();

} // namespace otbv
