#pragma once

#include <string>
#include <vector>

namespace otbv {
void save(const std::string &filename, const std::vector<bool> &data,
          const std::tuple<size_t, size_t, size_t> &resolution);
void save(const std::string &filename,
          const std::vector<std::vector<std::vector<bool>>> &data);
std::vector<std::vector<std::vector<bool>>> load(const std::string &filename);
} // namespace otbv