#pragma once

#include "conversion.h"
#include <string>

namespace otbv {
void save(std::ostream &stream, const std::vector<bool> &data,
          const size_t resolution);
// void load();
} // namespace otbv
