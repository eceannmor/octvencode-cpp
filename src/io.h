#pragma once

#include "conversion.h"
#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

namespace otbv {
void stream_bytes(std::ostream &stream, const std::vector<bool> &data,
          const size_t resolution);

void encoded_to_file(std::string filename, const std::vector<bool> &data, std::tuple<size_t, size_t, size_t> resolution);

void save(std::string filename, volume<bool> data);

          // void load();
} // namespace otbv
