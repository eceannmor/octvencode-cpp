#pragma once

#include "conversion.h"
#include <string>

namespace otbv {
void save(std::string path, const std::vector<bool> &data,
          const std::size_t resolution);
// void load();
} // namespace otbv
