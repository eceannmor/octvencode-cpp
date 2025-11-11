#pragma once

#include <string>
#include <vector>

namespace otbv {
    void save(std::string filename, std::vector<bool> data);
    void save(std::string filename, std::vector<std::vector<std::vector<bool>>> data);
}